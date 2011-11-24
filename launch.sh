#!/bin/bash

# default config
config="simple"

# if argument is given
if [ "$1" ]
then
  config=$1
fi

# start player/stage in separate terminal
echo Starting player/stage
xfce4-terminal -x robot-player ./stage/$config.cfg

# wait some time until stage is fully loaded 
sleep 3

# start robot
echo Starting robot
./robot

echo Robot task exited, killing simulation
kill -9 `ps auxw | grep robot-player | awk '{print $2}'` 2> /dev/null

echo Done
