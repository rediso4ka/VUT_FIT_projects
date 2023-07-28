package ija.ija2022.homework1.common;

public interface MazeObject {
    boolean canMove(Field.Direction dir);
    boolean move(Field.Direction dir);
}
