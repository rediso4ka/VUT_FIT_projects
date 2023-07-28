package ija.ija2022.homework2.tool.common;

public interface IField extends Observable, CommonField {
    boolean put(IMazeObject object);

    boolean remove(IMazeObject object);

    void setMaze(IMaze commonMaze);
}
