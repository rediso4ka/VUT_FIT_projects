/**
 * WallField class represents a wall field in the maze.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.fields;

public class WallField extends BaseField implements CommonField {
    public WallField(int row, int col) {
        super(row, col);
    }

    /**
    * Returns always false because pacman cannot move to the wall field
    */
    @Override
    public boolean isEmpty() {
        return false;
    }
}
