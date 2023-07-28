package ija.ija2022.homework1.game;

import ija.ija2022.homework1.common.*;

import java.util.ArrayList;
import java.util.List;

public class MazeConfigure {
    public boolean canRead = false;
    public int rows;
    public int cols;
    public List<StringBuilder> lines = new ArrayList<>();
    public void startReading(int rows, int cols) {
        this.rows = rows;
        this.cols = cols;
        canRead = true;
    }

    public boolean processLine(String line) {
        return lines.add(new StringBuilder(line));
    }

    public boolean stopReading() {
        if (canRead) {
            canRead = false;
            return true;
        }
        return false;
    }

    public Maze createMaze() {
        boolean canCreate = true;
        if (!canRead && lines.size() == rows) {
            for (StringBuilder line: lines) {
                if (!(line.toString().matches("[TXGK.S]*") && line.length() == cols)) {
                    canCreate = false;
                    break;
                }
            }
        } else {
            canCreate = false;
        }
        if (!canCreate) {
            return null;
        } else {
            MyMaze maze = new  MyMaze(rows, cols, lines);
            maze.setPacmanObject(new PacmanObject());
            return maze;
        }
    }

}
