/**
 * Represents a heart object in the maze.
 * Heart object is a collectible object that can be collected by the player.
 * When collected, the player gains a life.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.objects;

import ija.ija2022.project.maze.CommonMaze;

public class HeartObject extends BaseObject {
    public HeartObject(int row, int col, CommonMaze commonMaze) {
        super(row, col, commonMaze);
    }

    /**
     * Removes the current object from the maze at its current position.
     */
    public void collect() {
        this.maze.removeObject(this, this.row, this.col);
    }
}
