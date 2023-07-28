/**
 * Represents target object in the maze.
 * Target object is a finish object.
 * When the player collides with the target, the player wins the game.
 * If the player has not collected all the keys, the player will not be able to collide with the target.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.objects;

import ija.ija2022.project.maze.CommonMaze;

public class TargetObject extends BaseObject {
    public TargetObject(int row, int col, CommonMaze commonMaze) {
        super(row, col, commonMaze);
    }

    public void finish() {
        //
    }
}
