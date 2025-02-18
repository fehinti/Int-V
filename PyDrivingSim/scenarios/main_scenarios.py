
from pydrivingsim import TrafficLight, Target, TrafficCone, SuggestedSpeedSignal, GraphicObject, Vehicle, Agent
from pydrivingsim import Point
import time

startlog = time.time()

class OnlyVehicle():
    def __init__(self):
        #Initialize the vehicle
        self.vehicle = Vehicle()
        self.vehicle.set_screen_here()
        self.vehicle.set_pos_ang((0, -1, 0))

        #Initialize target
        target = Target()
        target.set_pos((182, -1))
        target.set_object(self.vehicle)

    def update(self):
        self.vehicle.set_screen_here()
        self.vehicle.control([0.5, 0.0])

    def terminate(self):
        pass

class AutonomousVehicle():
    def __init__(self):
        # Initialize the vehicle
        self.vehicle = Vehicle()
        self.vehicle.set_screen_here()
        self.vehicle.set_pos_ang((0, 0, 0))

        #Initialize the agent
        self.agent = Agent(self.vehicle)

        #Initialize target
        target = Target()
        target.set_pos((182, -1))
        target.set_object(self.vehicle)

        # Custom implementation
        self.trajectory = []
        # Custom implementation

    def update(self):
        self.agent.compute()
        action = self.agent.get_action()

        # Custom implementation
        self.trajectory = self.agent.get_trajectory()
        # Custom implementation

        self.vehicle.set_screen_here()
        self.vehicle.control([action[0], action[1]])

    def terminate(self):
        self.agent.terminate()


class BasicTrafficLight():
    def __init__(self):
        # cone = TrafficCone()
        # cone.set_pos((1.0,0))
        cone = TrafficCone()
        cone.set_pos((1.0,2))
        cone = TrafficCone()
        cone.set_pos((1.0,-2))

        cone = TrafficCone()
        cone.set_pos((12.0,2))
        cone = TrafficCone()
        cone.set_pos((12.0,1))
        cone = TrafficCone()
        cone.set_pos((12.0,0))
        cone = TrafficCone()
        cone.set_pos((12.0,-2))

        cone = TrafficCone()
        cone.set_pos((20.0,2))
        cone = TrafficCone()
        cone.set_pos((20.0,-1))
        cone = TrafficCone()
        cone.set_pos((20.0,0))
        cone = TrafficCone()
        cone.set_pos((20.0,-2))

        # OBSTACLES result in => NO PATH
        # cone = TrafficCone()
        # cone.set_pos((35.0,2))
        # cone = TrafficCone()
        # cone.set_pos((35.0,-1))
        # cone = TrafficCone()
        # cone.set_pos((35.0,0))
        # cone = TrafficCone()
        # cone.set_pos((35.0,1))

        trafficlight = TrafficLight()
        trafficlight.set_pos((160,-3))


class BasicSpeedLimit():
    def __init__(self):
        signal = SuggestedSpeedSignal(10)
        signal.set_pos((50, 4))
        bologna = GraphicObject("imgs/pictures/bologna.png", 35)
        bologna.set_pos((67,12))
        signal = SuggestedSpeedSignal(90)
        signal.set_pos((96, 4))
        super = GraphicObject("imgs/pictures/superstrada.png", 5)
        super.set_pos((100,6))

# CUSTOM FUNCTION
# Function to test implementation of lateral controller with steering
# cones along the path
class BasicLateralControllerTest():
    def __init__( self, number_of_cones ):
        cones = []
        for i in range( 0, number_of_cones ):
            cones.append(TrafficCone())
            cones[i].set_pos(( 25+i*100/number_of_cones, pow( -1, i )))

class AddPoint():
    def __init__(self, av:AutonomousVehicle):
        self.points = []
        self.first_point = None
        self.av = av

    def update(self):
        if self.points.__len__() == 0:
            for point in self.av.trajectory:
                self.points.append(Point(point[0],point[1]))
        elif self.av.trajectory[0] != self.first_point:
            for point in self.points:
                point.reset()
            self.points.clear()
            for point in self.av.trajectory:
                self.points.append(Point(point[0],point[1]))

        if self.av.trajectory.__len__() > 0:
            self.first_point = self.av.trajectory[0]

