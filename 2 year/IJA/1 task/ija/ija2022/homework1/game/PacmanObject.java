package ija.ija2022.homework1.game;

import ija.ija2022.homework1.common.Field;
import ija.ija2022.homework1.common.MazeObject;

public class PacmanObject implements MazeObject {
    public Field field;
    @Override
    public boolean canMove(Field.Direction dir) {
        return field.nextField(dir).canMove();
    }

    @Override
    public boolean move(Field.Direction dir) {
        if (!canMove(dir)) {
            return false;
        }
        field.nextField(dir).put(this);
        field.remove(this);
        return true;
    }

    public void setField(Field field) {
        this.field = field;
    }

    public Field getField() {
        return field;
    }
}
