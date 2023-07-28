package ija.ija2022.homework2.game;

import ija.ija2022.homework2.tool.common.*;

import java.util.HashSet;
import java.util.Objects;
import java.util.Set;

public class BaseField extends AbstractObservableField implements IField {
    protected int row;

    protected int col;

    protected IMazeObject object;

    protected IMaze commonMaze;

    public BaseField(int row, int col) {
        this.row = row;
        this.col = col;
    }

    @Override
    public boolean canMove() {
        return false;
    }

    @Override
    public boolean contains(CommonMazeObject commonMazeObject) {
        return this.object == commonMazeObject;
    }

    @Override
    public IMazeObject get() {
        return this.object;
    }

    @Override
    public boolean isEmpty() {
        return this.object == null;
    }

    @Override
    public CommonField nextField(Direction dirs) {
        return this.commonMaze.getField(this.row + dirs.deltaRow(), this.col + dirs.deltaCol());
    }

    @Override
    public boolean put(IMazeObject object) {
        if (object == null) return false;

        this.object = object;
        return true;
    }

    @Override
    public boolean remove(IMazeObject object) {
        if (object == null || this.object != object) return false;

        this.object = null;
        return true;
    }

    @Override
    public void setMaze(IMaze commonMaze) {
        this.commonMaze = commonMaze;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        BaseField baseField = (BaseField) o;

        if (row != baseField.row) return false;
        return col == baseField.col;
    }

    @Override
    public int hashCode() {
        return Objects.hash(row, col, commonMaze, object);
    }
}
