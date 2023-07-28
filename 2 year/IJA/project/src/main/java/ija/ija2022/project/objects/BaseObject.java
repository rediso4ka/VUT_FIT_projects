/**
 * Base object class.
 * It is implemented by all objects in the maze.
 * It contains basic information about the object, methods for moving and working with observers.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.objects;

import ija.ija2022.project.common.Observable;
import ija.ija2022.project.fields.CommonField;
import ija.ija2022.project.maze.CommonMaze;

import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;

public class BaseObject implements CommonMazeObject {
    protected int row;

    protected int col;

    protected final CommonMaze maze;

    protected CommonField.Direction direction = CommonField.Direction.N;

    private final Set<Observer> observers = new HashSet<>();

    BaseObject(int row, int col, CommonMaze maze) {
        this.row = row;
        this.col = col;
        this.maze = maze;
    }

    /**
    * Returns true if this object can be moved to the given direction
    * 
    * @param direction - the direction to check
    */
    @Override
    public boolean canMove(CommonField.Direction direction) {
        CommonField nextField = this.maze.getField(
                this.row + direction.deltaRow(),
                this.col + direction.deltaCol()
        );

        boolean canGhostMoveToGhost = this instanceof GhostObject &&
                this.maze.getObjects()[this.row + direction.deltaRow()][this.col + direction.deltaCol()]
                        .stream().anyMatch(obj -> obj instanceof GhostObject);

        return nextField != null && nextField.canMove() && !canGhostMoveToGhost;
    }

    /**
    * Returns true if the object can be moved to the given field
    * 
    * @param row - The row of the field
    * @param col - The column of the
    */
    @Override
    public boolean canMove(int row, int col) {
        CommonField nextField = this.maze.getField(row, col);

        return nextField != null && nextField.canMove();
    }

    /**
    * Generates a random direction for the object to move to
    */
    public void generateDirection() {
        CommonField.Direction[] directions = Arrays.stream(CommonField.Direction.values())
                .filter(this::canMove)
                .toArray(CommonField.Direction[]::new);

        if (directions.length == 0) return;

        int randomIndex = (int) (Math.random() * directions.length);
        this.direction = directions[randomIndex];
    }

    /**
    * Moves the character in the direction specified
    */
    @Override
    public void move() {
        // Generate the direction if it is not set
        if (this.direction == null) {
            this.generateDirection();
            return;
        }

        this.move(this.direction);
    }

    /**
    * Moves the object to the specified direction.
    * 
    * @param direction - The direction to move
    */
    @Override
    public void move(CommonField.Direction direction) {
        boolean canMove = this.canMove(direction);

        if (!canMove) return;

        int nextRow = this.row + direction.deltaRow();
        int nextCol = this.col + direction.deltaCol();

        this.row = nextRow;
        this.col = nextCol;

        this.maze.moveObject(this, nextRow, nextCol);
    }

    /**
    * Moves the object to the specified row and column.
    * 
    * @param row - The row to move to.
    * @param col - The column to move to
    */
    @Override
    public void move(int row, int col) {
        boolean canMove = this.canMove(row, col);

        if (!canMove) return;

        this.row = row;
        this.col = col;

        this.maze.moveObject(this, row, col);
    }

    /**
    * Sets the direction of the object
    * 
    * @param direction - the direction of the
    */
    public void setDirection(CommonField.Direction direction) {
        this.direction = direction;
    }

    /**
    * Returns the direction of this object
    */
    public CommonField.Direction getDirection() {
        return direction;
    }

    /**
    * Returns the row that this object is on
    */
    @Override
    public int getRow() {
        return this.row;
    }

    /**
    * Returns the column that this object is on
    */
    @Override
    public int getCol() {
        return this.col;
    }

    /**
    * Adds an observer to the list of observers.
    * 
    * @param observer - The observer to add
    */
    @Override
    public void addObserver(Observable.Observer observer) {
        this.observers.add(observer);
    }

    /**
    * Removes an observer from the list of observers.
    * 
    * @param observer - The observer to remove
    */
    @Override
    public void removeObserver(Observable.Observer observer) {
        this.observers.remove(observer);
    }

    /**
    * Notify all observers that the object has changed.
    */
    @Override
    public void notifyObservers() {
        this.observers.forEach(observer -> observer.update(this));
    }
}
