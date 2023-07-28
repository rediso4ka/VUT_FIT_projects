/**
 * MazeConfigure class is used to configure maze from file.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.maze.configure;

import ija.ija2022.project.fields.CommonField;
import ija.ija2022.project.fields.PathField;
import ija.ija2022.project.fields.WallField;
import ija.ija2022.project.maze.CommonMaze;
import ija.ija2022.project.maze.Maze;
import ija.ija2022.project.objects.*;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

public class MazeConfigure {
    private static final Map<Character, Class<?>> FIELDS_MAP = new HashMap<>() {{
        put(CHARACTER_MAP.PATH.getCharacter(), PathField.class);
        put(CHARACTER_MAP.WALL.getCharacter(), WallField.class);
    }};

    private static final Map<Character, Class<?>> OBJECTS_MAP = new HashMap<>() {{
        put(CHARACTER_MAP.PACMAN.getCharacter(), PacmanObject.class);
        put(CHARACTER_MAP.GHOST.getCharacter(), GhostObject.class);
        put(CHARACTER_MAP.KEY.getCharacter(), KeyObject.class);
        put(CHARACTER_MAP.CLOCK.getCharacter(), ClockObject.class);
        put(CHARACTER_MAP.HEART.getCharacter(), HeartObject.class);
        put(CHARACTER_MAP.TARGET.getCharacter(), TargetObject.class);
    }};

    private boolean reading;

    private int rowCounter;

    private CommonMaze commonMaze;

    private String mazeText;

    public MazeConfigure(String text) {
        this(text, false);
    }

    public MazeConfigure(String text, boolean isFilePath) {
        this.reading = false;
        this.rowCounter = 1;
        this.commonMaze = null;
        this.mazeText = null;

        // Set the text of the maze.
        if (isFilePath)
            try {
                this.mazeText = Files.readString(Paths.get(text));
            } catch (IOException e) {
                System.out.println("Cannot read file");
                System.exit(1);
                return;
            }
        else
            this.mazeText = text;

        String[] lines = this.mazeText.split("\n");
        // Removes all whitespace from the lines.
        for (int i = 0; i < lines.length; i++) {
            lines[i] = lines[i].trim();
        }

        // Check if the file is too short.
        if (lines.length < 2) {
            System.out.println("File is too short");
            System.exit(1);
            return;
        }

        Integer[] dimensions = this.parseDimensions(lines[0]);

        // Check if the dimensions are valid.
        if (dimensions.length != 2) {
            System.out.println("Invalid dimensions");
            System.exit(1);
            return;
        }

        this.startReading(dimensions[0], dimensions[1]);
        lines = Arrays.copyOfRange(lines, 1, lines.length);
        Arrays.stream(lines).forEach(this::processLine);
        this.stopReading();
    }

    /**
    * Parses the dimensions from the line
    * 
    * @param line - the line to parse
    */
    private Integer[] parseDimensions(String line) {
        try {
            return Arrays.stream(line.split(" ")).map(Integer::parseInt).toArray(Integer[]::new);
        } catch (NumberFormatException e) {
            System.out.println("Invalid dimensions");
            System.exit(1);
            return null;
        }
    }

    /**
    * Checks if line is valid
    * 
    * @param line - line to check for
    */
    private boolean checkLine(String line) {
        // Returns true if the line is null.
        if (line == null) return false;

        // Check if the line length is equal to the commonMaze. numCols
        if (line.length() != this.commonMaze.numCols() - 2) return false;

        String allowedChars = Arrays.stream(CHARACTER_MAP.values())
                .map(CHARACTER_MAP::getCharacter)
                .map(String::valueOf)
                .reduce((a, b) -> a + b).orElse("");

        return line.matches("^[ " + allowedChars + "]+$");
    }

    /**
    * Creates a maze for use.
    * 
    * 
    * @return the common maze or null if there is
    */
    public CommonMaze createMaze() {
        if (this.reading) {
            return null;
        }

        if (this.commonMaze.getFields() == null) {
            return null;
        }

        // Returns null if the row counter is not equal to the commonMaze. numRows
        if (this.rowCounter != this.commonMaze.numRows()) {
            return null;
        }

        return this.commonMaze;
    }

    /**
    * Processes a line of text
    * 
    * @param line - The line to be
    */
    public void processLine(String line) {
        line = line.trim();
        if (!this.reading) return;

        // Check if the current line is valid.
        if (!this.checkLine(line)) return;

        // Increment the row counter by one.
        if (this.rowCounter >= this.commonMaze.numRows()) {
            this.rowCounter++;
            return;
        }

        // This method is used to create a new line of data.
        for (int i = 0; i < line.length(); i++) {
            char ch = line.charAt(i);

            boolean hasField = FIELDS_MAP.containsKey(ch);
            boolean hasObject = OBJECTS_MAP.containsKey(ch);

            if (hasField) {
                try {
                    CommonField field = (CommonField) FIELDS_MAP.get(ch).getConstructor(int.class, int.class).newInstance(this.rowCounter, i + 1);
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
                    CommonMazeObject object = (CommonMazeObject) OBJECTS_MAP.get(ch).getConstructor(int.class, int.class, CommonMaze.class).newInstance(this.rowCounter, i + 1, this.commonMaze);
                    this.commonMaze.putObject(object, this.rowCounter, i + 1);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }

        this.rowCounter++;
    }

    /**
    * Starts reading maze. 
    * 
    * @param rows - number of rows in the maze
    * @param cols - number of columns in the
    */
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

    /**
    * Stop reading the data.
    */
    public void stopReading() {
        // This method increments the row counter and resets the reading flag.
        if (this.reading) {
            this.rowCounter++;
            this.reading = false;
        }
    }

    /**
    * Returns the maze text.
    * 
    * 
    * @return the maze text or null if there is
    */
    public String getMazeText() {
        return mazeText;
    }
}
