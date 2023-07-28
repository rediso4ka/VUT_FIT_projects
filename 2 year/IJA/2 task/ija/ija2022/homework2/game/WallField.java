package ija.ija2022.homework2.game;

import ija.ija2022.homework2.tool.common.IField;
import ija.ija2022.homework2.tool.common.IMazeObject;

public class WallField extends BaseField implements IField {
    public WallField(int row, int col) {
        super(row, col);
    }

    @Override
    public boolean isEmpty() {
        return true;
    }

    @Override
    public boolean put(IMazeObject object) {
        throw new UnsupportedOperationException();
    }

    @Override
    public boolean remove(IMazeObject object) {
        throw new UnsupportedOperationException();
    }
}
