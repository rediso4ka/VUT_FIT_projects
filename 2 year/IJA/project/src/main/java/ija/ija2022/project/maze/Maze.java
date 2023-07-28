/**
 * Maze class represents a maze with fields and objects.
 * It implements CommonMaze interface.
 * It contains methods for initializing and manipulating with maze and its objects.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.maze;

import ija.ija2022.project.fields.CommonField;
import ija.ija2022.project.fields.WallField;
import ija.ija2022.project.objects.*;
import ija.ija2022.project.utils.Pair;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

public class Maze implements CommonMaze {
    private final int rows;
    private final int cols;
    private final CommonField[][] fields;
    private final ArrayList<CommonMazeObject>[][] objects;
    private final List<Pair<Integer, Integer>> updatesFields = new ArrayList<>();
    private List<int[]> pacmanPath = Collections.emptyList();

    public Maze(int rows, int cols) {
        this.rows = rows + 2;
        this.cols = cols + 2;
        this.fields = new CommonField[this.rows][this.cols];
        this.objects = new ArrayList[this.rows][this.cols];

        this.initWalls();
        this.initObjects();
    }

    /**
    * Initializes wall fields.
    */
    private void initWalls() {
        // Creates a new WallField for each row in the list.
        for (int i = 0; i < this.rows; i++) {
            // Creates a new WallField for each row in the list.
            for (int j = 0; j < this.cols; j++) {
                // Creates a new field in the list.
                if (i == 0 || i == this.rows - 1 || j == 0 || j == this.cols - 1) {
                    this.fields[i][j] = new WallField(i, j);
                }
            }
        }
    }

    /**
    * Initializes the objects array.
    */
    private void initObjects() {
        for (int i = 0; i < this.rows; i++) {
            for (int j = 0; j < this.cols; ++j) {
                this.objects[i][j] = new ArrayList<>();
            }
        }
    }

    /**
    * Returns the field at the specified position.
    * 
    * @param row - The row to look at.
    * @param col - The column to look at.
    * 
    * @return The field at the specified position
    */
    @Override
    public CommonField getField(int row, int col) {
        if (row < 0 || row >= this.rows)
            return null;

        if (col < 0 || col >= this.cols)
            return null;

        return this.fields[row][col];
    }

    /**
    * Returns the number of columns
    */
    @Override
    public int numCols() {
        return this.cols;
    }

    /**
    * Returns all ghost objects
    */
    @Override
    public GhostObject[] ghosts() {
        return Arrays.stream(this.objects).flatMap(Arrays::stream)
                .flatMap(List::stream)
                .filter(object -> object instanceof GhostObject)
                .toArray(GhostObject[]::new);
    }

    /**
    * Returns all KeyObjects 
    */
    public KeyObject[] keys() {
        return Arrays.stream(this.objects).flatMap(Arrays::stream)
                .flatMap(List::stream)
                .filter(object -> object instanceof KeyObject)
                .toArray(KeyObject[]::new);
    }

    /**
    * Returns all clock objects 
    */
    public ClockObject[] clocks() {
        return Arrays.stream(this.objects).flatMap(Arrays::stream)
                .flatMap(List::stream)
                .filter(object -> object instanceof ClockObject)
                .toArray(ClockObject[]::new);
    }

    /**
    * Returns all heart objects
    */
    public HeartObject[] hearts() {
        return Arrays.stream(this.objects).flatMap(Arrays::stream)
                .flatMap(List::stream)
                .filter(object -> object instanceof HeartObject)
                .toArray(HeartObject[]::new);
    }

    /**
    * Returns the target object if any.
    * 
    * 
    * @return the target object or null if there is no target object
    */
    public TargetObject target() {
        return Arrays.stream(this.objects).flatMap(Arrays::stream)
                .flatMap(List::stream)
                .filter(object -> object instanceof TargetObject)
                .map(object -> (TargetObject) object)
                .findFirst()
                .orElse(null);
    }

    /**
    * Returns Pacman object
    * 
    * 
    * @return Pacman 
    */
    public PacmanObject getPacman() {
        return Arrays.stream(this.objects).flatMap(Arrays::stream)
                .flatMap(List::stream)
                .filter(object -> object instanceof PacmanObject)
                .map(object -> (PacmanObject) object)
                .findFirst()
                .orElse(null);
    }

    /**
    * Returns the number of rows
    */
    @Override
    public int numRows() {
        return this.rows;
    }

    /**
    * Sets the field at the specified position
    * 
    * @param row - The row to set the field at
    * @param col - The column to set the field at
    * @param iField - The field to set
    */
    @Override
    public void setField(int row, int col, CommonField iField) {
        this.fields[row][col] = iField;
    }

    /**
    * Returns the fields in the maze
    */
    @Override
    public CommonField[][] getFields() {
        return this.fields;
    }

    /**
    * Returns the objects in the maze
    */
    @Override
    public ArrayList<CommonMazeObject>[][] getObjects() {
        return this.objects;
    }

    /**
    * Puts the object at the specified row and column.
    * 
    * @param object - The object to put.
    * @param row - The row of the object.
    * @param col - The column of the object
    */
    @Override
    public void putObject(CommonMazeObject object, int row, int col) {
        if (object == null)
            return;

        this.objects[row][col].add(object);
        this.updatesFields.add(new Pair<>(row, col));
    }

    /**
    * Moves an object to a new location
    * 
    * @param object - The object to move.
    * @param row - The row of the object
    * @param col - The column of the
    */
    public void moveObject(CommonMazeObject object, int row, int col) {
        if (object == null) return;

        for (int i = 0; i < this.objects.length; i++) {
            for (int j = 0; j < this.objects[i].length; j++) {
                if (this.objects[i][j].contains(object)) {
                    this.objects[i][j].remove(object);
                    this.updatesFields.add(new Pair<>(i, j));
                }
            }
        }

        this.objects[row][col].add(object);
        this.updatesFields.add(new Pair<>(row, col));
    }

    /**
    * Removes the object from the specified row and column.
    * 
    * @param object - The object to remove.
    * @param row - The row of the object.
    * @param col - The column of the object
    */
    @Override
    public void removeObject(CommonMazeObject object, int row, int col) {
        if (object == null) return;

        this.objects[row][col].remove(object);
        this.updatesFields.add(new Pair<>(row, col));

        if (this.isAllKeysCollected())
            this.updatesFields.add(new Pair<>(this.target().getRow(), this.target().getCol()));
    }

    /**
    * Freeze ghosts 
    */
    @Override
    public void freezeGhosts() {
        for (GhostObject ghost : this.ghosts()) {
            ghost.setFrozen(true);
            this.updatesFields.add(new Pair<>(ghost.getRow(), ghost.getCol()));
        }
    }

    /**
    * Unfreeze ghosts
    */
    @Override
    public void unfreezeGhosts() {
        for (GhostObject ghost : this.ghosts()) {
            ghost.setFrozen(false);
            this.updatesFields.add(new Pair<>(ghost.getRow(), ghost.getCol()));
        }
    }

    /**
    * Notify all observers that this field has been updated.
    */
    public void notifyUpdates() {
        this.updatesFields.forEach(pair -> this.fields[pair.getKey()][pair.getValue()].notifyObservers());
        this.updatesFields.clear();
    }

    /**
    * Returns the Pacman path
    */
    public List<int[]> getPacmanPath() {
        return pacmanPath;
    }

    /**
    * Sets the Pacman path.
    * 
    * @param pacmanPath - a list of integers representing the
    */
    public void setPacmanPath(List<int[]> pacmanPath) {
        this.pacmanPath = pacmanPath;

        pacmanPath.forEach(p -> this.updatesFields.add(new Pair<>(p[0], p[1])));
    }

    /**
    * Returns true if all keys have been collected
    */
    public boolean isAllKeysCollected() {
        return keys().length == 0;
    }
}
