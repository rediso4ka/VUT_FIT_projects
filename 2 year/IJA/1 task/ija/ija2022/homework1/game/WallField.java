package ija.ija2022.homework1.game;

import ija.ija2022.homework1.common.*;

import java.util.Objects;

public class WallField implements Field {
    public int row;
    public int col;
    public WallField(int row, int col) {
        this.row = row;
        this.col = col;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) return true;
        if (!(obj instanceof WallField)) return false;
        WallField wallField = (WallField) obj;
        return row == wallField.row && col == wallField.col;
    }

    @Override
    public int hashCode() {
        return Objects.hash(row, col);
    }

    @Override
    public boolean canMove() {
        return false;
    }

    @Override
    public MazeObject get() {
        return null;
    }

    @Override
    public boolean isEmpty() {
        return true;
    }

    @Override
    public Field nextField(Direction dirs) {
        return null;
    }

    @Override
    public boolean put(MazeObject object) {
        throw new UnsupportedOperationException();
    }

    @Override
    public boolean remove(MazeObject object) {
        return false;
    }

    @Override
    public void setMaze(Maze maze) {

    }
}
