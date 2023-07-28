package ija.ija2022.homework2.game;

import ija.ija2022.homework2.tool.common.CommonField;
import ija.ija2022.homework2.tool.common.IField;
import ija.ija2022.homework2.tool.common.IMaze;
import ija.ija2022.homework2.tool.common.IMazeObject;

import java.util.HashMap;
import java.util.Map;

public class MazeConfigure {
    private static final Map<Character, Class<?>> FIELDS_MAP = new HashMap<>() {{
        put('.', PathField.class);
        put('X', WallField.class);
        put('K', WallField.class);
        put('T', WallField.class);
//        put('G', PathField.class);
    }};

    private static final Map<Character, Class<?>> OBJECTS_MAP = new HashMap<>() {{
        put('S', PacmanObject.class);
        put('G', GhostObject.class);
    }};

    private boolean reading;

    private int rowCounter;

    private IMaze commonMaze;

    public MazeConfigure() {
        this.reading = false;
        this.rowCounter = 1;
        this.commonMaze = null;
    }

    private boolean checkLine(String line) {
        if (line == null) return false;

        if (line.length() != this.commonMaze.numCols() - 2) return false;

        return line.matches("^[.XSGKT]+$");
    }

    public IMaze createMaze() {
        if (this.reading) {
            return null;
        }

        if (this.commonMaze.getFields() == null) {
            return null;
        }

        if (this.rowCounter != this.commonMaze.numRows()) {
            return null;
        }

        return this.commonMaze;
    }

    public boolean processLine(String line) {
        if (!this.reading) {
            return false;
        }

        if (!this.checkLine(line)) {
            return false;
        }

        if (this.rowCounter >= this.commonMaze.numRows()) {
            this.rowCounter++;
            return false;
        }

        for (int i = 0; i < line.length(); i++) {
            char ch = line.charAt(i);

            boolean hasField = FIELDS_MAP.containsKey(ch);
            boolean hasObject = OBJECTS_MAP.containsKey(ch);

            if (hasField) {
                try {
                    IField field = (IField) FIELDS_MAP.get(ch).getConstructor(int.class, int.class).newInstance(this.rowCounter, i + 1);
                    this.commonMaze.setField(this.rowCounter, i + 1, field);
                    field.setMaze(this.commonMaze);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }

            if (hasObject) {
                try {
                    PathField field = new PathField(this.rowCounter, i + 1);
                    this.commonMaze.setField(this.rowCounter, i + 1, field);
                    field.setMaze(this.commonMaze);
                    IMazeObject object = (IMazeObject) OBJECTS_MAP.get(ch).getConstructor(int.class, int.class, IMaze.class).newInstance(this.rowCounter, i + 1, this.commonMaze);
                    object.addObserver(this.commonMaze);
                    this.commonMaze.putObject(object, this.rowCounter, i + 1);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }

        this.rowCounter++;

        return true;
    }

    public void startReading(int rows, int cols) {
        if (this.reading) {
            return;
        }

        if (this.commonMaze != null && this.commonMaze.getFields() != null) {
            return;
        }

        if (rows <= 0 || cols <= 0) {
            return;
        }

        this.reading = true;
        this.rowCounter = 1;
        this.commonMaze = new Maze(rows, cols);
    }

    public boolean stopReading() {
        if (this.reading) {
            this.rowCounter++;
            this.reading = false;
            return true;
        }

        return false;
    }
}
