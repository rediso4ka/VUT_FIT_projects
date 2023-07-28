/**
 * Represents Pacman object in the game.
 * Pacman object is an object that can move through the maze.
 * It is controlled by the player.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.objects;

import ija.ija2022.project.events.EventManager;
import ija.ija2022.project.events.events.LivesChangeEvent;
import ija.ija2022.project.maze.CommonMaze;
import ija.ija2022.project.settings.SettingsController;

import java.util.Objects;

public class PacmanObject extends BaseObject {
    private int lives;

    public PacmanObject(int row, int col, CommonMaze commonMaze) {
        super(row, col, commonMaze);

        SettingsController settings = new SettingsController();

        this.lives = settings.getMaxLives();
    }

    /**
     * Returns whether this entity is a Pacman or not.
     *
     * @return true, indicating that this entity is a Pacman
     */
    @Override
    public boolean isPacman() {
        return true;
    }

    /**
     * Returns the number of lives the player currently has.
     *
     * @return The number of lives.
     */
    public int getLives() {
        return this.lives;
    }

    /**
     * Decrements the number of lives by 1 and fires a LivesChangeEvent.
     */
    public void decrLives() {
        this.lives -= 1;
        EventManager.getInstance().fireEvent(new LivesChangeEvent(this.lives));
    }

    /**
     * Increases the number of lives by 1 and fires a LivesChangeEvent.
     */
    public void incrLives() {
        this.lives += 1;
        EventManager.getInstance().fireEvent(new LivesChangeEvent(this.lives));
    }

    /**
     * Compares this PacmanObject to another object to determine if they are equal.
     *
     * @param o The object to compare to this PacmanObject
     * @return true if the objects are equal, false otherwise
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        PacmanObject that = (PacmanObject) o;

        if (getRow() != that.getRow()) return false;
        if (getCol() != that.getCol()) return false;
        if (lives != that.lives) return false;
        return Objects.equals(maze, that.maze);
    }

    /**
     * Computes the hash code for this MazeState object.
     *
     * @return The hash code for this object.
     */
    @Override
    public int hashCode() {
        int result = getRow();
        result = 31 * result + getCol();
        result = 31 * result + lives;
        result = 31 * result + (maze != null ? maze.hashCode() : 0);
        return result;
    }
}
