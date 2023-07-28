/**
 * BaseField class represents a field in the maze.
 * It is used as a base class for all other types of fields.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.fields;

import ija.ija2022.project.maze.CommonMaze;
import ija.ija2022.project.objects.CommonMazeObject;

import java.util.ArrayList;
import java.util.Objects;

public class BaseField extends AbstractObservableField {
    protected final int row;

    protected final int col;

    protected CommonMaze commonMaze;

    public BaseField(int row, int col) {
        this.row = row;
        this.col = col;
    }

    /**
    * Return true if the object can be moved to this field
    */
    @Override
    public boolean canMove() {
        return false;
    }

    /**
    * Returns the objects in the maze
    */
    @Override
    public ArrayList<CommonMazeObject> get() {
        return this.commonMaze.getObjects()[this.row][this.col];
    }

    /**
    * Returns the row that this cell is on
    */
    public int getRow() {
        return row;
    }

    /**
    * Returns the column that this row occupies
    */
    public int getCol() {
        return col;
    }

    /**
    * Returns true if there are no objects
    */
    @Override
    public boolean isEmpty() {
        return this.commonMaze.getObjects()[this.row][this.col].isEmpty();
    }

    /**
    * Returns the field that follows the maze.
    * 
    * @param dirs - Direction to move in.
    * 
    * @return CommonField that follows the maze
    */
    @Override
    public CommonField nextField(Direction dirs) {
        return this.commonMaze.getField(this.row + dirs.deltaRow(), this.col + dirs.deltaCol());
    }

    /**
    * Sets the maze to use.
    * 
    * @param commonMaze - the common maze to
    */
    @Override
    public void setMaze(CommonMaze commonMaze) {
        this.commonMaze = commonMaze;
    }

    /**
    * Returns true if this field is equal to the specified object.
    * 
    * @param o - The object to compare
    */
    @Override
    public boolean equals(Object o) {
        // Returns true if this object is the same as the receiver.
        if (this == o) return true;
        // Returns true if this object is a subclass of the same class.
        if (o == null || getClass() != o.getClass()) return false;

        BaseField baseField = (BaseField) o;

        // Returns true if the row is equal to the base field s row.
        if (row != baseField.row) return false;
        return col == baseField.col;
    }

    /**
    * Returns a hash code for this maze
    */
    @Override
    public int hashCode() {
        return Objects.hash(row, col, commonMaze);
    }
}
