/**
 * Collision controller class is used to detect collisions between objects.
 * This class is used to detect and handle collisions between pacman and other objects.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.collisions;

import ija.ija2022.project.events.EventManager;
import ija.ija2022.project.events.events.WinEvent;
import ija.ija2022.project.maze.CommonMaze;
import ija.ija2022.project.objects.*;

import java.util.ArrayList;
import java.util.List;

public class CollisionController {
    private final CommonMaze maze;

    private final List<Collision> collisions = new ArrayList<>();

    public CollisionController(CommonMaze maze) {
        this.maze = maze;
    }

    /**
    * Detect collisions and add them to
    */
    public void detectCollisions() {
        PacmanObject pacman = this.maze.getPacman();

        for (GhostObject ghost : this.maze.ghosts()) {
            // Add a collision to the collision list.
            if (collides(pacman, ghost))
                this.collisions.add(new Collision(pacman, ghost, (pair) -> {
                    PacmanObject p = (PacmanObject) pair.getKey();
                    GhostObject g = (GhostObject) pair.getValue();

                    if (!g.isFrozen())
                        p.decrLives();
                    g.die();
                }));
        }

        for (KeyObject key : this.maze.keys()) {
            // Add a collision to the collision list.
            if (this.collidesBasic(pacman, key))
                this.collisions.add(new Collision(pacman, key, (pair) -> {
                    KeyObject k = (KeyObject) pair.getValue();

                    k.collect();
                }));
        }

        for (ClockObject clock : this.maze.clocks()) {
            // Add a collision to the list of collision objects.
            if (this.collidesBasic(pacman, clock))
                this.collisions.add(new Collision(pacman, clock, (pair) -> {
                    ClockObject c = (ClockObject) pair.getValue();

                    this.maze.freezeGhosts();
                    c.collect();
                }));
        }

        for (HeartObject heart : this.maze.hearts()) {
            // Add a collision to the collision list.
            if (this.collidesBasic(pacman, heart))
                this.collisions.add(new Collision(pacman, heart, (pair) -> {
                    PacmanObject p = (PacmanObject) pair.getKey();
                    HeartObject h = (HeartObject) pair.getValue();

                    p.incrLives();
                    h.collect();
                }));
        }

        if (this.collidesBasic(pacman, this.maze.target())) {
            EventManager.getInstance().fireEvent(new WinEvent());
        }
    }

    /**
    * Checks if pacman collides with object
    * 
    * @param pacman - The pacman to check.
    * @param object - The object to check
    */
    private boolean collidesBasic(PacmanObject pacman, CommonMazeObject object) {
        // Returns true if the object is null or if the pacman is null.
        if (object == null || pacman == null) return false;

        return pacman.getRow() == object.getRow() && pacman.getCol() == object.getCol();
    }

    /**
    * Checks if there is a collision between pacman and ghost.
    * 
    * @param pacman - The object to check for colliding with
    * @param ghost - The object to check
    */
    private boolean collides(PacmanObject pacman, GhostObject ghost) {
        int pacmanPrevRow = pacman.getRow() - pacman.getDirection().deltaRow();
        int pacmanPrevCol = pacman.getCol() - pacman.getDirection().deltaCol();
        int ghostPrevRow = ghost.getRow() - ghost.getDirection().deltaRow();
        int ghostPrevCol = ghost.getCol() - ghost.getDirection().deltaCol();
        boolean collidesByPrevPosition = pacmanPrevRow == ghostPrevRow && pacmanPrevCol == ghostPrevCol;
        boolean collidesByCrossPositions = (pacman.getRow() == ghostPrevRow && pacman.getCol() == ghostPrevCol)
                || (pacmanPrevRow == ghost.getRow() && pacmanPrevCol == ghost.getCol());


        return this.collidesBasic(pacman, ghost) || collidesByPrevPosition || collidesByCrossPositions;
    }

    /**
    * Handles all collisions and clears
    */
    public void handleCollisions() {
        for (Collision collision : this.collisions) collision.handle();

        this.collisions.clear();
    }
}
