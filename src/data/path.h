/** @file       src/data/path.h
    @ingroup    Data
    @brief      Path struct.
    @author     Alex Moriarty <alexander@dal.ca>
    @author     Jacob Perron <perronj@yorku.ca>
*/

#ifndef __DATA_PATH_H_
#define __DATA_PATH_H_

#include "position.h"
#include "move.h"
#include <vector>

/** Represents a sequence of positions along with a sequence of moves.
 *
 *  Intended for the use of path planning, path executing, and
 *  tracking/logging of a robots movement. Contains a two dimensional list
 *  where the first dimension represents a list of positions in the world
 *  and the second a list of moves.
 *
 *  An ordered list of positions are stored each with a corresponding ordered
 *  list of moves. The list of moves are intended to represent moves
 *  taken, or to be taken, by a robot from the corresponding position.
 *  Therefore, in appropriate practice, the last position would have no
 *  corresponding moves.
 *
 *  @ref checkpoint allows the client to add a new Position to the end of the
 *  list based on the moves taken from the previous Position.
 */
class Path {
    public:

        /** Constructor.
         *
         *  The first node in the list is initialized with a single Position
         *  and no moves in order to cover the scenerio where the client
         *  only wishes to work with a path consisting of moves and no
         *  positions. If a Position is added after construction the place-
         *  holder is removed.
         */
        Path();

        /** Constructor.
         *
         *  In this constructor the first Position in the Path is given.
         *  Because of this there is no need to create a place-holder inside
         *  and so the internal variable @c overwritten is set to signify this.
         *
         *  @param pos : The first Position in the Path.
         */
        Path(Position pos);

        /** Destructor. */
        ~Path();

        /** Adds a Position to the end of the path.
         *
         *  @note Client should take care to realize that added moves
         *      from previous Position may not lead to added Position
         *      depending on this Path objects use.
         *
         *  @param pos : The new Position to be added to the path.
         */
        void addPosition(Position pos);

        /** Adds a Move to the end of a list of moves.
         *
         *  The Move is added to the list of moves occuring after
         *  the provided position.
         *
         *  @param move : Move struct to be added to the end.
         *
         *  @param positionIndex : The index of the Position in the path.
         */
        void addMove(Move move, int positionIndex);

        /** Adds Move to the very end of the path.
         *
         *  Adds a Move struct to the end of the list of moves associated
         *  with the last Position in the Path.
         *
         *  @param move : The Move struct to be added.
         */
        void addMove(Move move);

        /** Returns the number of positions in the path.
         *
         *  @return The current number of positions in the path.
         */
        int size();

        /** Returns the number of moves taken after a Position.
         *
         *  The number of moves are counted up until the next Position
         *  or end of the path.
         *
         *  @param i : The index of the Position in the path.
         *
         *  @return The number of moves.
         */
        int numOfMoves(int i);

        /** Adds a new Position to the end of the path.
         *
         *  The new Position is based on the past moves recorded since
         *  the last added Position. If no moves have been associated
         *  with the previous Position then no new Position is added.
         */
        void checkpoint();

        /** Accessor for Position.
         *
         *  @param i : The index of the Position in the Path.
         *
         *  @return A Position in the Path.
         */
        Position getPosition(int i);

        /** Accessor for Move.
         *
         *  @param positionIndex : The index of the Position in the Path.
         *
         *  @param moveIndex : The index of the Move for the given
         *      Position index.
         *
         *  @return A Move in the Path.
         */
        Move getMove(int positionIndex, int moveIndex);

        /** Accessor for first element in first list of moves.
         *
         *  @return A Move in the Path.
         */
        Move getMove();

        /** Removes a Position from the Path.
         *
         *  The position and it's corresponding Moves are removed.
         *
         *  @param i : The index of the Position in the Path.
         *
         *  @return The Position removed from the Path.
         */
        Position removePosition(int i);

        /** Removes a Move from the Path.
         *
         *  The Move is removed from the specified Position.
         *
         *  @param positionIndex : Index of the Position in the Path.
         *
         *  @param moveIndex : Index of the Move.
         *
         *  @return The Move removed from the Path
         */
        Move removeMove(int positionIndex, int moveIndex);

        /** Removes the first Move from the first list of moves in the Path.
         *
         *  The Move at the beginning of the move list associated with the
         *  first Position in the Path is removed.
         *
         *  @return The removed Move struct.
         */
        Move removeMove();

    private:

        struct Node {
            Position position;
            std::vector<Move> move;
        };

        /** List of nodes each containing a Position and list of Moves. */
        std::vector<Node> node;

        /** Used to determine when placeholder Position should be removed. */
        bool overwritten;
};
#endif
