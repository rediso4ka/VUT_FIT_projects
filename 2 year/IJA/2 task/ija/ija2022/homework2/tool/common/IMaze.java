package ija.ija2022.homework2.tool.common;

import ija.ija2022.homework2.game.PacmanObject;

public interface IMaze extends CommonMaze, Observable.Observer {
    void setField(int row, int col, IField iField);

    IField[][] getFields();

    void putObject(IMazeObject object, int row, int col);

    void moveObject(IMazeObject object, int row, int col);

    PacmanObject getPacman();

    void removeObject(int row, int col);
}
