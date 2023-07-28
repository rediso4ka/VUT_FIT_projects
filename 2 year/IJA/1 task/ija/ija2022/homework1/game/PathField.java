package ija.ija2022.homework1.game;

import ija.ija2022.homework1.common.*;

import java.util.Objects;

public class PathField implements Field {
    public int row;
    public int col;
    public char object;
    public MyMaze maze;
    public PathField(int row, int col) {
        this.row = row;
        this.col = col;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) return true;
        if (!(obj instanceof PathField)) return false;
        PathField pathField = (PathField) obj;
        return row == pathField.row && col == pathField.col;
    }

    @Override
    public int hashCode() {
        return Objects.hash(row, col);
    }

    @Override
    public boolean canMove() {
        return object != 'X';
    }

    @Override
    public MazeObject get() {
        switch (object) {
            case 'S':
                maze.getPacmanObject().setField(this);
                return maze.getPacmanObject();
            default:
                return null;
        }
    }

    @Override
    public boolean isEmpty() {
        return object == '.';
    }

    @Override
    public Field nextField(Direction dirs) {
        int r = row;
        int c = col;
        switch (dirs) {
            case D:
                r += 1;
                break;
            case L:
                c += -1;
                break;
            case R:
                c += 1;
                break;
            case U:
                r += -1;
                break;
        }
        return maze.getField(r, c);
    }

    @Override
    public boolean put(MazeObject object) {
        if (!(object instanceof PacmanObject)) {
            return false;
        }
        this.object = 'S';
        maze.getLines().get(row-1).setCharAt(col-1, 'S');
        return true;
    }

    @Override
    public boolean remove(MazeObject object) {
        if (this.isEmpty() || this.get().getClass() != object.getClass()) {
            return false;
        }
        this.object = '.';
        maze.getLines().get(row-1).setCharAt(col-1, '.');
        return true;
    }

    @Override
    public void setMaze(Maze maze) {
        this.maze = (MyMaze) maze;
    }

    public void setObject(char object) {
        this.object = object;
    }
}
