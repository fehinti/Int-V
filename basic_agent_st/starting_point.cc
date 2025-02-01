//
// Created by Gastone Pietro Rosati Papini on 10/08/22.
//

// --- MATLAB PRIMITIVES INCLUDE ---
#include "primitives.h"
// --- MATLAB PRIMITIVES INCLUDE ---

// --- CUSTOM INCLUDES ---
#include "HLPlanner.h"
#include "functions.h"
// --- CUSTOM INCLUDES ---

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <numeric>

extern "C" {
#include "screen_print_c.h"
}
#include "screen_print.h"
#include "server_lib.h"
#include "logvars.h"

#define DEFAULT_SERVER_IP    "127.0.0.1"
#define SERVER_PORT               30000  // Server port
#define DT 0.05

// Handler for CTRL-C
#include <signal.h>
static uint32_t server_run = 1;
void intHandler(int signal) {
    server_run = 0;
}


int main(int argc, const char * argv[]) {
    logger.enable(true);

    // Messages variables
    scenario_msg_t scenario_msg;
    manoeuvre_msg_t manoeuvre_msg;
    size_t scenario_msg_size = sizeof(scenario_msg.data_buffer);
    size_t manoeuvre_msg_size = sizeof(manoeuvre_msg.data_buffer);
    uint32_t message_id = 0;

    // RRT* High Level Planner
    HLPlanner hl_planner;
    deque<Point> route;
    vector < Point > nodes;

#if not defined( _MSC_VER ) and not defined( _WIN32 )
    // More portable way of supporting signals on UNIX
    struct sigaction act;
    act.sa_handler = intHandler;
    sigaction(SIGINT, &act, NULL);
#else
    signal(SIGINT, intHandler);
#endif

    server_agent_init(DEFAULT_SERVER_IP, SERVER_PORT);

    // Start server of the Agent
    printLine();
    printTable("Waiting for scenario message...", 0);
    printLine();
    while (server_run == 1) {

        // Clean the buffer
        memset(scenario_msg.data_buffer, '\0', scenario_msg_size);

        // Receive scenario message from the environment
        if (server_receive_from_client(&server_run, &message_id, &scenario_msg.data_struct) == 0) {
            // Init time
            static auto start = std::chrono::system_clock::now();
            auto time = std::chrono::system_clock::now()-start;
            double num_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(time).count()/1000.0;
            printLogTitle(message_id, "received message");


            // Vehicle state message  from simulator
            input_data_str *in = &scenario_msg.data_struct;
            manoeuvre_msg.data_struct.CycleNumber = in->CycleNumber;
            manoeuvre_msg.data_struct.Status = in->Status;


            // ADD AGENT CODE HERE

            // Calculation of distance traveled by car
            static double init_dist = in->TrfLightDist;
            double dist_travelled = init_dist - in->TrfLightDist;

            // Vehicle state for Autonomous AGENT
            const double max_acc = 5;
            const double min_acc = -5;
            double a0 = fmin( fmax( in->ALgtFild, min_acc ), max_acc );
            double v0 = in->VLgtFild;
            double v_min = 3, v_max = 15, t_min, t_max;
            double sim_time = in->ECUupTime;

            double v_cruise = in->RequestedCruisingSpeed;
            double sf = in->TrfLightDist;
            double min_T, max_T;

            // LATERAL CONTROLLER [First part of the controller] - BEGIN
            //
            // 1. Approximate vehicle position from ORIGIN [0,0]
            static double init_lat_offsL = in->LatOffsLineL; // initial lateral position of the vehicle
            static double last_lat_offsL = in->LatOffsLineL;
            static double last_dist_travelled = dist_travelled;
            double curr_lat_offsL = in->LatOffsLineL;
            Point d_veh_pt = { dist_travelled-last_dist_travelled, last_lat_offsL-curr_lat_offsL };
            Point veh_pt = { dist_travelled, -in->LatOffsLineR+in->LaneWidth/2 };

            static int i = 0;
            if ( i % 10 == 0 )
            {
                last_lat_offsL = curr_lat_offsL;
                last_dist_travelled = dist_travelled;
            }
            i++;

            // 1. Calculate the yaw angle of the vehicle
            double yaw = atan2(d_veh_pt.y, d_veh_pt.x);

            double L_h = 1.5; // preview point look ahead distance
            Point P_h = { veh_pt.x + L_h*cos(yaw), veh_pt.y + L_h*sin(yaw) }; // preview point
            //
            // LATERAL CONTROLLER [First part of the controller] - END

            
            // RRT* High Level Planner - BEGIN
            //
            double rrt_lkahd = fmax( 50, v0*5 );
            static bool rrt_params_flag = true;

            if ( rrt_params_flag )
            {
                hl_planner.setLaneWidth(in->LaneWidth);
                hl_planner.setStart({in->VehicleLen,0});
                hl_planner.setTarget({in->TrfLightDist,-in->LaneWidth/4});
                hl_planner.setSMax(5);
                hl_planner.setMaxIter(10000);

                hl_planner.setVehicle({ in->VehicleWidth, in->VehicleLen });

                // Add obstacles to the HL planner
                for ( int i = 0; i < in->NrObjs; i++ )
                {
                    hl_planner.addObstacle( { in->ObjX[i], in->ObjY[i],
                                             in->ObjWidth[i]*2, in->ObjLen[i]*2 } );
                }

                hl_planner.planRoute( nodes );
                hl_planner.getRoute( route );
                route = MovingAverage( route, 3 );
                deque<Point> route_smoothe = MovingAverage( route, 3 );
                Point target = hl_planner.getTarget();
                cout << "Total nodes: " << nodes.size() << endl;
                

                    rrt_params_flag = false;
            }

            // linear regression sample size
            int sample_size = 3, sample_cnt = 0;
            // for (int j = 0; j < min(n, i + sample_size / 2 + 1); ++j)
            deque<Point> sample_points;
            for (int i = 0; i < route.size(); i++)
            {
                if ( veh_pt.x > route[i].x )
                {
                    route.pop_front();
                    continue;
                }
                
                if ( sample_cnt < sample_size )
                {
                    sample_cnt++;
                    sample_points.push_back( route[i] );
                    cout << "Sample point: " << route[i].x << " " << route[i].y << endl;
                }
                else
                {
                    break;
                }
            }
            pair<double, double> p_line = ComputePath(sample_points, sample_size, 0.3);
            cout << "Slope: " << p_line.first << " Intercept: " << p_line.second << endl;

            if ( route.size() > 0 )
            {
                manoeuvre_msg.data_struct.NTrajectoryPoints = 20;
                for ( int i = 0; i < 20; i++ )
                {
                    // int n_index = route.size()-i-1;
                    // int n_index = nodes.size()-i-1;
                    int n_index = i;
                    // cout << "Node " << n_index << ": " << nodes[n_index].x << " " << nodes[n_index].y << endl;
                    // cout << "Node " << i << ": " << nodes[i].x << " " << nodes[i].y << endl;
                    manoeuvre_msg.data_struct.TrajectoryPointIX[i] = route[n_index].x;
                    manoeuvre_msg.data_struct.TrajectoryPointIY[i] = route[n_index].y;
                }
            }
            // Refer to LATERAL CONTROLLER [second part of the controller] below
            //
            // RRT* High Level Planner - END


            /* LONGITUDINAL CONTROLLER */
            //
            // Return coeff
            double coeffsT2[COEFFS_DIM];
            double v2;
            double T2;
            double coeffsT1[COEFFS_DIM];
            double v1;
            double T1;

            // Best coeff
            double *bestCoeff;
            double best_v;
            double best_s;
            double bestT;


            /* MAIN TRAFFIC LIGHT LOGIC */
            // MAIN Logic - Begin

            // Traffic light safety parameters
            double x_lkahd = fmax( 50, v0*5 );
            double x_s = 5;                                    // traffic light safety distance
            double T_s = x_s/v_min;                            // traffic light safety time
            double x_in = 10;                                  // length of intersection
            double T_in = x_in / v_min;                        // time to cross intersection
            double x_tr;                                       // traffic light distance
            double x_stop;                                     // stop distance according to traffic light
            double t_grn, t_red;                                  // time of traffic light at GREEN and RED respectively

            int32_T curr_state = in->TrfLightCurrState;

            if ( in->NrTrfLights != 0 )
            {
                x_tr = in->TrfLightDist;
                x_stop = x_tr - x_s/2;
            }
            if ( in->NrTrfLights == 0 || x_tr >= x_lkahd )
            {                
                pass_primitive ( v0, a0, x_lkahd, v_cruise, v_cruise, 0, 0, coeffsT2, &v2, &T2, coeffsT1, &v1, &T1);
                bestCoeff = coeffsT1;
            }
            else
            {
                switch ( curr_state )
                {
                    case 1:
                        t_grn = 0;
                        t_red = in->TrfLightFirstTimeToChange - T_in;
                        break;
                    case 2:
                        t_grn = in->TrfLightSecondTimeToChange + T_s;
                        t_red = in->TrfLightThirdTimeToChange - T_in;
                        break;
                    case 3:
                        t_grn = in->TrfLightFirstTimeToChange + T_s;
                        t_red = in->TrfLightSecondTimeToChange - T_in;
                        break;                
                }

                if ( in->TrfLightCurrState==1 && x_tr<=x_s )
                {
                    pass_primitive ( v0, a0, x_lkahd, v_cruise, v_cruise, 0, 0, coeffsT2, &v2, &T2, coeffsT1, &v1, &T1);
                    bestCoeff = coeffsT1;
                }
                else
                {
                    pass_primitive ( v0, a0, x_tr, v_min, v_max, t_grn, t_red, coeffsT2, &v2, &T2, coeffsT1, &v1, &T1);
                    
                    if ( IsCoeffZero( coeffsT1 )==1 && IsCoeffZero( coeffsT2 )==1 )
                    {                        
                        stop_primitive ( v0, a0, x_stop, coeffsT1, &sf, &T1 );
                        bestCoeff = coeffsT1;
                    }
                    else
                    {
                        if ( ( coeffsT1[3]<0 && coeffsT2[3]>0 ) || ( coeffsT1[3]>0 && coeffsT2[3]<0 ) )
                        {
                            pass_primitive_j0( v0, a0, x_tr, v_min, v_max, coeffsT1, &v1, &T1 );
                            bestCoeff = coeffsT1;
                        }
                        else
                        {
                            if ( fabs( coeffsT1[3] ) < fabs( coeffsT1[3] ) )
                            {
                                bestCoeff = coeffsT1;
                            }
                            else
                            {
                                bestCoeff = coeffsT2;
                            }
                        }
                    }
                }
            }
            
            // MAIN Logic - End

            // Logged values for MATLAB plot
            // bestCoeff = coeffsT1;
            best_v = v1;
            best_s = sf;
            bestT = T1;

            // Compute the req_vel and req_acc based on Perception Action Maps (PAM)
            static double last_req_acc = 0;
            double long_gain = 20;
            double j0 = bestCoeff[3];
            double s0 = bestCoeff[4];
            double cr0 = bestCoeff[5];
            double t_offs = 0;
            double jT0 = j0 + t_offs*s0 + 0.5*t_offs*t_offs*cr0;
            double jT1 = j0 + (DT+t_offs)*s0 + 0.5*(DT+t_offs)*(DT+t_offs)*cr0;
            double req_acc = fmin( fmax( ( last_req_acc + long_gain*( DT*(jT1+jT0)*0.5) ), min_acc ) , max_acc );
            // double req_acc = a_opt( DT, v0, a0, best_s, best_v, 0, bestT);
            last_req_acc = req_acc;
            double req_vel = v_opt( DT, v0, a0, best_s, best_v, 0, bestT);

            // PID control
            double P_gain = 0.01;
            double I_gain = 1;
            double threshold = -0.02;
            static double integral = 0;
            double error = req_acc - a0;
            integral = integral + error * DT;
            
            // Clear integral term accumulated after the vehicle has stopped
            if ( req_vel < threshold )
            {
                integral = 0;
            }
            
            double req_ped = P_gain * error + I_gain * integral;
            //
            /* LONGITUDINAL     CONTROLLER */


            /* LATERAL CONTROLLER - BEGIN */
            //

            // 2. Calculate the distance to the lane

            // double ep = init_lat_offsL - curr_lat_offsL;
            Point p_line_A = {veh_pt.x,ComputeY(veh_pt.x, p_line.first,p_line.second)};
            Point p_line_B = {P_h.x,ComputeY(P_h.x, p_line.first,p_line.second)};
            double ep = minimum_distance(p_line_A, p_line_B, veh_pt);
            double theta_p = atan2(p_line_B.y - p_line_A.y, p_line_B.x - p_line_A.x);
            double theta_e = yaw - theta_p;

            // tunable distance and angular error coefficients
            double k_e = -0.075;
            double k_theta = -2;

            double req_delta = k_e*ep + k_theta*theta_e;
            //
            /* LATERAL CONTROLLER - END*/

            // LOGGING - Values from the Autonomous Agent logged for Data Analysis
            logger.log_var("Example", "cycle", in->CycleNumber);
            logger.log_var("Example", "time", sim_time);
            logger.log_var("Example", "dist_travelled", dist_travelled);
            logger.log_var("Example", "sf", sf);
            logger.log_var("Example", "vel", v0);
            logger.log_var("Example", "acc", a0);
            logger.log_var("Example", "req_ped", req_ped);
            logger.log_var("Example", "req_acc", req_acc);
            logger.log_var("Example", "req_vel",req_vel);
            logger.log_var("Example", "C0", bestCoeff[0]);
            logger.log_var("Example", "C1", bestCoeff[1]);
            logger.log_var("Example", "C2", bestCoeff[2]);
            logger.log_var("Example", "C3", bestCoeff[3]);
            logger.log_var("Example", "C4", bestCoeff[4]);
            logger.log_var("Example", "C5", bestCoeff[5]);
            logger.log_var("Example", "T", bestT);

            // ADD LOW LEVEL CONTROL CODE HERE
            manoeuvre_msg.data_struct.RequestedAcc = req_ped; /* Naming convention NOT ACCURATE, 
            this is more the gas pedal value [1 => MAXIMUM ACCELERATION & -1 => MAXIMUM BRAKING] */
            manoeuvre_msg.data_struct.RequestedSteerWhlAg = req_delta;

            // Write log
            logger.write_line("Example");

            // Screen print
            printLogVar(message_id, "Time", num_seconds);
            printLogVar(message_id, "Status", in->Status);
            printLogVar(message_id, "CycleNumber", in->CycleNumber);
            printLogVar(message_id, "dX", d_veh_pt.x);
            printLogVar(message_id, "dY", d_veh_pt.y);
            printLogVar(message_id, "X", veh_pt.x);
            printLogVar(message_id, "Y", veh_pt.y);
            printLogVar(message_id, "Yaw", yaw);
            printLogVar(message_id, "req_vel", req_vel);
            printLogVar(message_id, "vel", v0);
            printLogVar(message_id, "ep", ep);

            // Send manoeuvre message to the environment
            if (server_send_to_client(server_run, message_id, &manoeuvre_msg.data_struct) == -1) {
                perror("error send_message()");
                exit(EXIT_FAILURE);
            } else {
                printLogTitle(message_id, "sent message");
            }
        }
    }

    // Close the server of the agent
    server_agent_close();
    return 0;
}