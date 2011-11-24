#include "path.h"
#include <math.h>
#include "util/logger.h"

CREATE_LOGGER("Path");

Path::Path(Position start) {
    Node n;
    n.position = start;
    node.push_back(n);
    overwritten = true; //no need for place-holder.
    LOG_CTOR << "Constructed." << std::endl;
}

Path::Path() {
    //initial position needed for scenerio when working with moves only.
    Position pos;
    Node n;
    n.position = pos;
    node.push_back(n);
    overwritten = false;
    LOG_CTOR << "Constructed." << std::endl;
}

Path::~Path() {
    LOG_DTOR << "Destructed." << std::endl;
}

void Path::addPosition(Position pos) {
    if (!overwritten) { //first position being added
        node.pop_back();//remove place holder
        overwritten = true;
    }
    Node n;
    n.position = pos;
    node.push_back(n);
}

void Path::addMove(Move m, int i) {
    node[i].move.push_back(m);
}

void Path::addMove(Move m) {
    addMove(m, node.size()-1);
}

int Path::size() {
    return node.size();
}

int Path::numOfMoves(int i) {
    return node[i].move.size();
}

Position Path::getPosition(int i) {
    return node[i].position;
}

Move Path::getMove(int i, int j) {
    return node[i].move[j];
}

Move Path::getMove() {
    // Could result in segfault if move or node doesn't exist.
    return node[0].move[0];
}

Position Path::removePosition(int i) {
    // Could result in segfault if move or node doesn't exist?
    //copy Position struct
    Position p = node[i].position;

    //remove original and it's associated Move structs
    for (int k = 0; k < node.size(); k++)
        node[i].move.erase(node[i].move.begin()+k);

    node.erase(node.begin()+i);

    return p;
}

Move Path::removeMove(int i, int j) {
    //Could result in segfaults if move or node doesn't exist?
    //copy Move struct
    Move m = node[i].move[j];
    //remove original
    node[i].move.erase(node[i].move.begin()+j-1);
    return m;
}

Move Path::removeMove() {
    //Could result in segfaults if move or node doesn't exist?
    //copy Move struct
    Move m = node[0].move[0];

    //remove original
    node[0].move.erase(node[0].move.begin());

    return m;
}

/* 
NEEDS A REWRITE
*/
/*
void Path::checkpoint() {
    int moveNum = numOfMoves(size()-1);
    if (moveNum > 0) {
        Node prev = node[node.size()-1];

        double x = prev.position.x;
        double y = prev.position.y;
        double yaw = prev.position.yaw;

        for (int i = 0; i < moveNum; i++) {
            double distance = prev.move[i].value;
            double rotation = prev.move[i].yaw;

            //apply move
            x += distance*cos(rotation);
            y += distance*sin(rotation);
            yaw = rotation;
        }

        //add new position
        Position position(x, y, yaw);
        addPosition(position);
    } //end if
} //end checkpoint
*/
