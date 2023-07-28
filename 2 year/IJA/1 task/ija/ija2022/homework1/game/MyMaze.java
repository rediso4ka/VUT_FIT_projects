package ija.ija2022.homework1.game;

import ija.ija2022.homework1.common.Field;
import ija.ija2022.homework1.common.Maze;

import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;

public class MyMaze implements Maze {
    public int rows;
    public int cols;
    public List<StringBuilder> lines = new ArrayList<>();
    public PacmanObject pacmanObject;
    public MyMaze(int rows, int cols, List<StringBuilder> lines) {
        this.rows = rows + 2;
        this.cols = cols + 2;
        this.lines = lines;
    }
    @Override
    public Field getField(int row, int col) {
        if (row < 0 || row >= rows || col < 0 || col >= cols) {
            return null;
        } else if (row == 0 || row == rows - 1 || col == 0 || col == cols - 1 || lines.get(row-1).charAt(col-1) == 'X') {
            return new WallField(row, col);
        } else {
            PathField pathField = new PathField(row, col);
            pathField.setObject(lines.get(row-1).charAt(col-1));
            pathField.setMaze(this);
            return pathField;
        }
    }

    @Override
    public int numCols() {
        return cols;
    }

    @Override
    public int numRows() {
        return rows;
    }

    @Override
    public List<StringBuilder> getLines() {
        return lines;
    }

    public void setPacmanObject(PacmanObject pacmanObject) {
        this.pacmanObject = pacmanObject;
    }

    public PacmanObject getPacmanObject() {
        return pacmanObject;
    }
}
