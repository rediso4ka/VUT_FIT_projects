/**
 * CommonField interface for observable fields.
 * It is used as a common interface for fields.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.fields;

import ija.ija2022.project.common.Observable;
import ija.ija2022.project.maze.CommonMaze;
import ija.ija2022.project.objects.CommonMazeObject;

import java.util.ArrayList;

public interface CommonField extends Observable {
    /**
    * Returns the next field in the direction.
    * 
    * @param var1 - the direction to go to
    * 
    * @return the next field in the
    */
    CommonField nextField(Direction var1);

    /**
    * Returns true if this field is empty
    */
    boolean isEmpty();

    /**
    * Returns true if this is pacman's path
    */
    default boolean isPacmanPath() {
        return false;
    }

    /**
    * Returns the list of common maze objects on this field
    */
    ArrayList<CommonMazeObject> get();

    /**
    * Returns true if the player can move to this field
    */
    boolean canMove();

    /**
    * Sets the maze to use.
    * 
    * @param commonMaze - the common maze to
    */
    void setMaze(CommonMaze commonMaze);

    /**
    * Returns the row of the field
    */
    int getRow();

    /**
    * Returns the column of the field
    */
    int getCol();

    enum Direction {
        L(0, -1),
        U(-1, 0),
        R(0, 1),
        D(1, 0),
        N(0, 0);

        private final int r;
        private final int c;

        Direction(int dr, int dc) {
            this.r = dr;
            this.c = dc;
        }

        /**
        * Returns the row delta of the direction.
        */
        public int deltaRow() {
            return this.r;
        }

        /**
        * Returns the column delta of the direction.
        */
        public int deltaCol() {
            return this.c;
        }

        /**
        * Returns the opposite direction.
        * 
        * 
        * @return the opposite direction or N if there is
        */
        public Direction opposite() {
            // Returns the value of this node.
            return switch (this) {
                case L -> R;
                case U -> D;
                case R -> L;
                case D -> U;
                default -> N;
            };
        }

        /**
        * Returns the character represented by this direction.
        * 
        * 
        * @return the character represented by this direction
        */
        public String getChar() {
            // Returns the string representation of this direction.
            return switch (this) {
                case L -> "l";
                case U -> "u";
                case R -> "r";
                case D -> "d";
                case N -> "n";
            };
        }

        /**
        * Returns the Direction corresponding to the row and column delta.
        * 
        * @param dr - the row delta of the Direction
        * @param dc - the column delta of the Direction
        * 
        * @return the Direction or null if not
        */
        public static Direction from(int dr, int dc) {
            for (Direction direction : Direction.values()) {
                // Returns the direction of the direction.
                if (direction.deltaRow() == dr && direction.deltaCol() == dc) {
                    return direction;
                }
            }

            return null;
        }
    }
}
