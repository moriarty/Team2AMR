# Desc: Pioneer robot
# Author: Sergey Alexandrov
# Date: 07 Nov 2011

# ....................... Robot configuration ........................ #

define pioneer position
(
  color "red"          # Default color.
  drive "diff"         # Differential steering model.
  gui_nose 1           # Draw a nose on the robot so we can see which way it points
  obstacle_return 1    # Can hit things.
  ranger_return 0.5    # Reflects sonar beams
  blob_return 1        # Seen by blobfinders
  fiducial_return 1    # Seen as "1" fiducial finders

  localization "gps"
  localization_origin [0 0 0 0] # Start odometry at (0, 0, 0).

  # actual size
  size [0.44 0.38 0.22]

  # the pioneer's center of rotation is offset from its center of area
  origin [-0.04 0 0 0]

  # estimated mass in KG
  mass 23.0

  # differential steering model
  drive "diff"

  # simplified body shape:
  block(
    points 8
    point[0] [-0.2 0.12]
    point[1] [-0.2 -0.12]
    point[2] [-0.12 -0.2555]
    point[3] [0.12 -0.2555]
    point[4] [0.2 -0.12]
    point[5] [0.2 0.12]
    point[6] [0.12 0.2555]
    point[7] [-0.12 0.2555]
    z [0 0.22]
  )
)
