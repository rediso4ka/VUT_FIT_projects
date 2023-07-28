package ija.ija2022.homework2.game;

import ija.ija2022.homework2.tool.common.*;

import javax.swing.*;
import java.lang.reflect.InvocationTargetException;
import java.util.HashSet;
import java.util.Set;

public class GhostObject implements IMazeObject {
    private int row;

    private int col;

    private final IMaze commonMaze;

    private final Set<Observer> observers = new HashSet();

    public GhostObject(int row, int col, IMaze commonMaze) {
        this.row = row;
        this.col = col;
        this.commonMaze = commonMaze;
    }

    @Override
    public boolean canMove(IField.Direction dir) {
        CommonField nextIField = this.commonMaze.getField(this.row + dir.deltaRow(), this.col + dir.deltaCol());

        return nextIField != null && nextIField.canMove();
    }

    @Override
    public boolean move(IField.Direction dir) {
        boolean canMove = this.canMove(dir);

        if (!canMove) return false;

        int nextRow = this.row + dir.deltaRow();
        int nextCol = this.col + dir.deltaCol();

        IField currentField = (IField) this.commonMaze.getField(this.row, this.col);
        IField nextField = (IField) this.commonMaze.getField(nextRow, nextCol);

        nextField.put(this);
        currentField.remove(this);

        this.row = nextRow;
        this.col = nextCol;

        PacmanObject pacman = this.commonMaze.getPacman();
        if (pacman != null) {
            if (pacman.getRow() == this.row && pacman.getCol() == this.col) {
                pacman.decrLives();
            }
        }

        return true;
    }

    @Override
    public int getRow() {
        return this.row;
    }

    @Override
    public int getCol() {
        return this.col;
    }

    @Override
    public CommonField getField() {
        return this.commonMaze.getField(this.row, this.col);
    }

    @Override
    public int getLives() {
        return 0;
    }

    @Override
    public void addObserver(Observer observer) {
        this.observers.add(observer);
    }

    @Override
    public void removeObserver(Observer observer) {
        this.observers.remove(observer);
    }

    @Override
    public void notifyObservers() {
        this.observers.forEach(observer -> observer.update(this));
    }
}
