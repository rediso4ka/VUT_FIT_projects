package ija.ija2022.homework2.game;

import ija.ija2022.homework2.tool.common.*;

import java.util.ArrayList;
import java.util.List;

public class Maze implements IMaze {
    private final int rows;
    private final int cols;
    private final IField[][] fields;

    private PacmanObject pacman;


//    private final Set<Observer> observers = new HashSet<>();

    public Maze(int rows, int cols) {
        this.rows = rows + 2;
        this.cols = cols + 2;
        this.fields = new IField[this.rows][this.cols];

        this.generateWalls();
    }

    private void generateWalls() {
        for (int i = 0; i < this.rows; i++) {
            for (int j = 0; j < this.cols; j++) {
                if (i == 0 || i == this.rows - 1 || j == 0 || j == this.cols - 1) {
                    this.fields[i][j] = new WallField(i, j);
                }
            }
        }
    }

    @Override
    public IField getField(int row, int col) {
        if (row < 0 || row >= this.rows)
            return null;

        if (col < 0 || col >= this.cols)
            return null;

        return this.fields[row][col];
    }

    @Override
    public int numCols() {
        return this.cols;
    }

    @Override
    public List<CommonMazeObject> ghosts() {
        List<CommonMazeObject> ghosts = new ArrayList<>();
        for (IField[] fields: this.fields) {
            for (IField field: fields) {
                if (!(field instanceof WallField)){
                    if (field.get() instanceof GhostObject) {
                        ghosts.add(field.get());
                    }
                }
            }
        }
        return ghosts;
    }


    @Override
    public int numRows() {
        return this.rows;
    }

    @Override
    public void setField(int row, int col, IField iField) {
        this.fields[row][col] = iField;
    }

    @Override
    public IField[][] getFields() {
        return this.fields;
    }

    @Override
    public void putObject(IMazeObject object, int row, int col) {
        if (object == null)
            return;

        if (object instanceof PacmanObject) {
            this.pacman = (PacmanObject) object;
        } else if (object instanceof GhostObject) {
            IField field = getField(row, col);
            if (field instanceof WallField) {
                return;
            }
            field.put(object);
        }
    }

    public void moveObject(IMazeObject object, int row, int col) {
        if (object == null)
            return;

        if (object instanceof PacmanObject) {
            this.pacman = (PacmanObject) object;
        } else if (object instanceof GhostObject) {
            IField field = this.getField(row, col);
            field.remove((IMazeObject) field.get());
            field.put(object);
        }

//        this.notifyObservers();
    }

    public PacmanObject getPacman() {
        return this.pacman;
    }

    @Override
    public void removeObject(int row, int col) {
//        this.fields[row][col].remove(
    }

    @Override
    public void update(Observable observable) {
        if (observable instanceof IMazeObject object) {
            this.moveObject(object, object.getRow(), object.getCol());
        }
    }

//    @Override
//    public void addObserver(Observer observer) {
//        this.observers.add(observer);
//    }
//
//    @Override
//    public void removeObserver(Observer observer) {
//        this.observers.remove(observer);
//    }
//
//    @Override
//    public void notifyObservers() {
//        this.observers.forEach(o -> o.update(this));
//    }
}
