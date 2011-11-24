#
# Simplified makefile for Mobile Robots programming exercises.
#

# To use, addjust for your system type, if you are *NOT* using the
# suppliend xubuntu image.

#OSTYPE := MACOS
OSTYPE := LINUX

# ------------- Settings for Doxygen --------------------
#
## set to YES if you have the dot tool (graphviz) installed and
## want doxygen to make nice class diagramms from your source files.
HAS_DOT := YES

# -------------- YOUR PROGRAM SETUP ---------------------

# Put here the names of all your exe files
# do not use any suffix, even not ".exe"
ALL_EXE := robot

# Put here the source files (*only* the ".cc" or ".cpp" files, not the
# ".h" files!)
#
# Create one variable for each of your exe files and list the sources
# for that exe only.  The variable name *MUST* match the name of the
# exe file in ALL_EXE exactly, with the suffix "_CC" added!
# The suffix of the variable is always _CC, even if you use ".cpp"
# with your files.
#
# If your program requires special libraries for linking, then create
# a variable <your_exe_name>_LIBS and list all required libraries,
# *including* their suffix.
robot_CC := src/main.cpp				\
			src/actr/motor.cpp			\
			src/snsr/ranger.cpp			\
			src/ctrl/robot.cpp			\
			src/ctrl/controller.cpp		\
			src/ctrl/motioncommand.cpp	\
            src/ctrl/wallfollower.cpp   \
            src/ctrl/bug.cpp            \
            src/ctrl/braitenberg.cpp    \
			src/plan/navigation.cpp		\
			src/plan/pathexecuter.cpp	\
			src/plan/pathplanner.cpp	\
			src/plan/local.cpp			\
			src/objt/objectavoider.cpp	\
			src/objt/objectdetector.cpp	\
			src/data/path.cpp			\
			src/hrio/console.cpp        \
            src/util/logger.cpp

robot_LIBS := lib/libpstermiosimple.a                               \

robot_INC := src
robot_SRCDIRS := src  

# you may force compiler to automatically include specific header in
# all of your files during compilation
# EXT_CXXFLAGS := --include someheader.h 

include Makefile.Rules

cleanALL:
	rm -rf  build/*

cleanLOG:
	rm log/*.log

