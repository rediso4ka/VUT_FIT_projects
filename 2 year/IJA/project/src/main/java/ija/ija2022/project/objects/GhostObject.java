/**
 * Represents a ghost object in the maze.
 * Ghost object is an object that can move through the maze.
 * It is controlled by the computer.
 * When the player collides with the ghost, the player loses a life and the ghost is removed from the maze.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.objects;

import ija.ija2022.project.fields.CommonField;
import ija.ija2022.project.maze.CommonMaze;

/**
 * Represents a ghost object in the maze game. Inherits from the BaseObject class.
 */
public class GhostObject extends BaseObject {
    private boolean frozen = false;

    public GhostObject(int row, int col, CommonMaze commonMaze) {
        super(row, col, commonMaze);
    }

    /**
     * Removes the current object from the maze at its current position.
     */
    public void die() {
        this.maze.removeObject(this, this.row, this.col);
    }

    /**
     * Returns whether the object is currently frozen or not.
     *
     * @return true if the object is frozen, false otherwise
     */
    public boolean isFrozen() {
        return frozen;
    }

    /**
     * Sets the frozen state of an object and resets its direction to North.
     *
     * @param frozen The new frozen state of the object.
     */
    public void setFrozen(boolean frozen) {
        this.frozen = frozen;
        this.direction = CommonField.Direction.N;
    }
}
