# Desc: Braitenberg vehicle
# Author: Timo Henne, Sergey Alexandrov
# Date: 07 Nov 2011

include "sonars.inc"

define braitenberg_vehicle position
(
  color "green"
  drive "diff"
  gui_nose 1
  obstacle_return 1
  ranger_return 0.5
  blob_return 1
  fiducial_return 1

  localization "gps"
  localization_origin [0 0 0 0]

  # actual size
  size [0.5 0.3 0.22]

  # the robot's center of rotation
  origin [0.0 0.0 0 0]

  # estimated mass in KG
  mass 15.0

  # this polygon defines the robot shape
  block(
    points 4
    point[0] [  0.25  0.15 ]
    point[1] [  0.25 -0.15 ]
    point[2] [ -0.25 -0.15 ]
    point[3] [ -0.25  0.15 ]
    z [0 0.22]
  )

  # use the sonar array defined in sonars.inc
  braitenberg_sonar( pose [0.0 0 0 0] )
)
