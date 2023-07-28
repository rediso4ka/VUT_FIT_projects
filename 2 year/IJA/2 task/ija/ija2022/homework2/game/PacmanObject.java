package ija.ija2022.homework2.game;

import ija.ija2022.homework2.tool.common.*;

import java.util.HashSet;
import java.util.List;
import java.util.Objects;
import java.util.Set;

public class PacmanObject implements IMazeObject {
    private int row;

    private int col;

    public int lives;

    private final IMaze commonMaze;

    private final Set<Observer> observers = new HashSet<>();

    public PacmanObject(int row, int col, IMaze commonMaze) {
        this.row = row;
        this.col = col;
        this.commonMaze = commonMaze;
        this.lives = 3;
    }

    @Override
    public boolean canMove(IField.Direction dir) {
        CommonField nextField = this.commonMaze.getField(this.row + dir.deltaRow(), this.col + dir.deltaCol());

        return nextField != null && nextField.canMove();
    }

    @Override
    public boolean move(IField.Direction dir) {
        boolean canMove = this.canMove(dir);

        if (!canMove) return false;

        this.commonMaze.moveObject(this, this.row + dir.deltaRow(), this.col + dir.deltaCol());

        this.row += dir.deltaRow();
        this.col += dir.deltaCol();

        List<CommonMazeObject> ghosts = this.commonMaze.ghosts();
        for (CommonMazeObject ghost : ghosts) {
            IMazeObject ghostObject = (IMazeObject) ghost;

            if (ghostObject.getRow() == this.row && ghostObject.getCol() == this.col) {
                this.lives -= 1;
                break;
            }
        }

        this.notifyObservers();

        return true;
    }

    public int getRow() {
        return this.row;
    }

    public int getCol() {
        return this.col;
    }

    @Override
    public boolean isPacman() {
        return true;
    }

    @Override
    public CommonField getField() {
        return this.commonMaze.getField(this.row, this.col);
    }

    public int getLives() {
        return this.lives;
    }

    public void decrLives() {
        this.lives -= 1;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        PacmanObject that = (PacmanObject) o;

        if (getRow() != that.getRow()) return false;
        if (getCol() != that.getCol()) return false;
        if (lives != that.lives) return false;
        return Objects.equals(commonMaze, that.commonMaze);
    }

    @Override
    public int hashCode() {
        int result = getRow();
        result = 31 * result + getCol();
        result = 31 * result + lives;
        result = 31 * result + (commonMaze != null ? commonMaze.hashCode() : 0);
        return result;
    }


    @Override
    public void addObserver(Observer observer) {
        this.observers.add(observer);
    }

    @Override
    public void removeObserver(Observer observer) {
        this.observers.remove(observer);
    }

    @Override
    public void notifyObservers() {
        this.observers.forEach(observer -> observer.update(this));
    }
}
