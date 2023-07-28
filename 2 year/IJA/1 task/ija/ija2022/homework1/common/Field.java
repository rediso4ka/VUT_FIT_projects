package ija.ija2022.homework1.common;

import java.sql.Array;

public interface Field {
    public static enum Direction {
        D, L, R, U;
        public int deltaCol() {
            return 0;
        }

        int deltaRow() {
            return 0;
        }
    }
    boolean canMove();
    MazeObject get();
    boolean isEmpty();
    Field nextField(Field.Direction dirs);
    boolean put(MazeObject object);
    boolean remove(MazeObject object);
    void setMaze(Maze maze);
}
