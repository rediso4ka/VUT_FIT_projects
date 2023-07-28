package ija.ija2022.homework2.game;

import ija.ija2022.homework2.tool.common.CommonMazeObject;
import ija.ija2022.homework2.tool.common.IField;
import ija.ija2022.homework2.tool.common.IMazeObject;

import java.util.List;

public class PathField extends BaseField implements IField {
    public PathField(int row, int col) {
        super(row, col);
        this.object = null;
    }

    @Override
    public boolean canMove() {
        return true;
    }

    @Override
    public boolean isEmpty() {
        if (this.commonMaze.getPacman() != null) {
            if (this.commonMaze.getPacman().getCol() == this.col && this.commonMaze.getPacman().getRow() == this.row) {
                return false;
            }
        }
        return this.object == null;
    }

    @Override
    public IMazeObject get() {
        if (this.commonMaze.getPacman() != null) {
            if (this.commonMaze.getPacman().getCol() == this.col && this.commonMaze.getPacman().getRow() == this.row) {
                return this.commonMaze.getPacman();
            }
        }
        return this.object;
    }

    @Override
    public boolean put(IMazeObject object) {
        if (object == null) return false;
        if (!this.isEmpty()) return false;
        this.object = object;

        this.notifyObservers();

        return true;
    }

    @Override
    public boolean remove(IMazeObject object) {
        if (object == null) return false;

        if (this.isEmpty()) return false;

        this.object = null;

        this.notifyObservers();

        return true;
    }
}
