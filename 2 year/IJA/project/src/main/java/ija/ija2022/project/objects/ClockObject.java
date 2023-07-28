/**
 * Represents a clock object in the maze.
 * Clock object is a collectible object that can be collected by the player.
 * When collected, all the ghosts in the maze are getting frozen for a period of time.
 * During this period of time, the player can safely move through the ghosts and eat them without losing a life.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.objects;

import ija.ija2022.project.maze.CommonMaze;

public class ClockObject extends BaseObject {
    public ClockObject(int row, int col, CommonMaze commonMaze) {
        super(row, col, commonMaze);
    }

    /**
    * Collects the clock object from the maze.
    */
    public void collect() {
        this.maze.removeObject(this, this.row, this.col);
    }
}
