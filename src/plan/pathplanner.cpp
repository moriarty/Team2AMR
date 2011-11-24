#include "pathplanner.h"

CREATE_LOGGER("PathPlanner");

Path* PathPlanner::calcPath(Position p1 , Position p2){
    double dist = p1.calcDistTo(p2);
    double yaw = p1.calcAngleTo(p2);
    MAKE_LOG << "Dist: " << dist << "Yaw: " << yaw << std::endl;
    Path* path = new Path(p1);
    path->addMove(Move(yaw,false));
    path->addMove(Move(dist,true));
    return path;
}

Position PathPlanner::calcPosition(Position p1, double dist) {
    Position end;
    end.x = (cos(p1.yaw) * dist) + p1.x;
    end.y = (sin(p1.yaw) * dist) + p1.y;
    end.yaw = p1.yaw;
    return end;
}

Path* PathPlanner::calcPath(Position p1, double dist) {
    return calcPath(p1, calcPosition(p1, dist));
}
