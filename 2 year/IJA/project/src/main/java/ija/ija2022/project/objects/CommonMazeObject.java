/**
 * CommonMazeObject interface represents an object in the maze.
 * It is implemented by all objects in the maze.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.objects;

import ija.ija2022.project.common.Observable;
import ija.ija2022.project.fields.CommonField;

public interface CommonMazeObject extends Observable {
    /**
    * Returns true if the object can move in the specified direction.
    * 
    * @param direction - The direction to check
    */
    boolean canMove(CommonField.Direction direction);

    /**
    * Returns true if the object can move to the specified row and column.
    * 
    * @param row - The row of the cell to check.
    * @param col - The column of the cell to check
    */
    boolean canMove(int row, int col);

    /**
    * Moves the object to the position specified by the direction.
    */
    void move();

    /**
    * Moves the object in the specified direction.
    * 
    * @param direction - The direction to move
    */
    void move(CommonField.Direction direction);

    /**
    * Moves the object to the specified row and column.
    * 
    * @param row - The row to move to.
    * @param col - The column to move to
    */
    void move(int row, int col);

    /**
    * Returns true if the object is pacman.
    */
    default boolean isPacman() {
        return false;
    }

    /**
    * Returns the number of lives of the object.
    */
    default int getLives() {
        return 0;
    }

    /**
    * Sets the direction of the object.
    * 
    * @param direction - The direction to set
    */
    void setDirection(CommonField.Direction direction);

    /**
    * Returns the direction of this object.
    */
    CommonField.Direction getDirection();

    /**
    * Returns the row of the object.
    */
    int getRow();

    /**
    * Returns the column that the object.
    */
    int getCol();
}
