/**
 * CommonMaze interface represents a maze.
 * It is implemented by Maze class.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.maze;

import ija.ija2022.project.fields.CommonField;
import ija.ija2022.project.objects.*;

import java.util.ArrayList;
import java.util.List;

public interface CommonMaze {
    /**
    * Returns the field at the specified row and column.
    * 
    * @param row - The row to read from.
    * @param col - The column to read from.
    * 
    * @return The field at the specified row and column
    */
    CommonField getField(int row, int col);

    /**
    * Returns the number of rows
    */
    int numRows();

    /**
    * Returns the number of columns
    */
    int numCols();

    /**
    * Returns an array of ghost objects
    */
    GhostObject[] ghosts();

    /**
    * Returns an array of key objects
    */
    KeyObject[] keys();

    /**
    * Returns an array of clock objects
    */
    ClockObject[] clocks();

    /**
    * Returns an array of heart objects
    */
    HeartObject[] hearts();

    /**
    * Returns the target object
    * 
    * 
    * @return The target object
    */
    TargetObject target();

    /**
    * Returns Pacman object
    * 
    * 
    * @return PacmanObject
    */
    PacmanObject getPacman();

    /**
    * Sets the field at the specified row and column.
    * 
    * @param row - The row to set the field at.
    * @param col - The column to set the field at.
    * @param iField - The field to set
    */
    void setField(int row, int col, CommonField iField);

    /**
    * Returns an array of fields
    */
    CommonField[][] getFields();

    /**
    * Returns an array of objects
    */
    ArrayList<CommonMazeObject>[][] getObjects();

    /**
    * Puts the CommonMazeObject at the specified row and column.
    * 
    * @param object - The CommonMazeObject to put.
    * @param row - The row in the maze where the object is to be placed.
    * @param col - The column in the maze where the object is to be placed
    */
    void putObject(CommonMazeObject object, int row, int col);

    /**
    * Moves the object to the row and column specified.
    * 
    * @param object - The object to move.
    * @param row - The row to move to.
    * @param col - The column to move to
    */
    void moveObject(CommonMazeObject object, int row, int col);

    /**
    * Removes the object from the maze.
    * 
    * @param object - The object to remove.
    * @param row - The row of the object.
    * @param col - The column of the object
    */
    void removeObject(CommonMazeObject object, int row, int col);

    /**
    * Freeze ghosts. 
    */
    void freezeGhosts();

    /**
    * Called when ghosts are no longer frozen
    */
    void unfreezeGhosts();

    /**
    * Notify updates. It is used to render the maze.
    */
    void notifyUpdates();

    /**
    * Returns path of the Pacman
    */
    List<int[]> getPacmanPath();

    /**
    * This method is used to set the path of the Pacman
    * 
    * @param pacmanPath - A list of int
    */
    void setPacmanPath(List<int[]> pacmanPath);

    /**
    * Returns true if all keys have been collected
    */
    boolean isAllKeysCollected();
}
