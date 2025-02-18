# Authors : Gastone Pietro Rosati Papini
# Date    : 09/08/2022
# License : MIT
import math
import signal

from pydrivingsim import World
from scenarios import BasicSpeedLimit, BasicTrafficLight, OnlyVehicle, AutonomousVehicle

# custom imports
from scenarios import BasicLateralControllerTest, AddPoint

class GracefulKiller:
  kill_now = False
  def __init__(self):
    signal.signal(signal.SIGINT, self.exit_gracefully)
    signal.signal(signal.SIGTERM, self.exit_gracefully)

  def exit_gracefully(self, *args):
    self.kill_now = True

def main():
    #av = OnlyVehicle()
    av = AutonomousVehicle()
    BasicTrafficLight()
    BasicSpeedLimit()
    # BasicLateralControllerTest(7) # Cones added to test lateral controller
    ap = AddPoint(av)                     # Add points to the path

    killer = GracefulKiller()
    while not killer.kill_now and World().loop:
        ap.update()
        av.update()
        World().update()

    av.terminate()
    World().exit()

main()