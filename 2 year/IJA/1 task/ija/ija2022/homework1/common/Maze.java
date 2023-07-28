package ija.ija2022.homework1.common;

import java.util.List;

public interface Maze {
    Field getField(int row, int col);
    int numCols();
    int numRows();
    List<StringBuilder> getLines();
}
