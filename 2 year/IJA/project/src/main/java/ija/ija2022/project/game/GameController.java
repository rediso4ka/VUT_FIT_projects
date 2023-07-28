/**
 * GameController class is responsible for controlling the game.
 * It is a part of the MVC pattern.
 * The whole logic of the game update and render processes is implemented here
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.game;

import ija.ija2022.project.astar.AStarPathFinder;
import ija.ija2022.project.collisions.CollisionController;
import ija.ija2022.project.common.BaseGameViewController;
import ija.ija2022.project.events.EventHandler;
import ija.ija2022.project.events.EventManager;
import ija.ija2022.project.events.events.KeyDownEvent;
import ija.ija2022.project.events.events.LivesChangeEvent;
import ija.ija2022.project.events.events.PathFieldMouseClickEvent;
import ija.ija2022.project.events.events.WinEvent;
import ija.ija2022.project.fields.CommonField;
import ija.ija2022.project.logger.LOGGER_MODE;
import ija.ija2022.project.logger.LoggerController;
import ija.ija2022.project.maze.CommonMaze;
import ija.ija2022.project.maze.MazePresenter;
import ija.ija2022.project.maze.configure.MazeConfigure;
import ija.ija2022.project.objects.GhostObject;
import ija.ija2022.project.objects.PacmanObject;
import ija.ija2022.project.settings.GAME_MODE;
import ija.ija2022.project.ui.controllers.KeyboardController;

import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyEvent;
import java.io.File;
import java.util.Arrays;
import java.util.List;
import java.util.Map;

public class GameController extends BaseGameViewController {
    private MazeConfigure mazeConfigure;
    private CommonMaze maze;
    private int unfreezeTicks;
    private MazePresenter presenter;
    private GameView view;
    private CollisionController collisionController;
    private LoggerController loggerController;

    public GameController(GAME_MODE mode, String filePath) {
        super(mode);

        this.loggerController = new LoggerController(LOGGER_MODE.WRITE);

        this.mazeConfigure = new MazeConfigure(filePath, true);
        this.maze = this.mazeConfigure.createMaze();
        this.unfreezeTicks = this.maze.ghosts().length * this.settingsController.getFreezeSteps();

        this.collisionController = new CollisionController(this.maze);

        this.presenter = new MazePresenter(this.maze);
        this.view = new GameView(this);

        this.view.setVisible(true);

        // Starts the game if the game is in a CONTINUOUS mode.
        if (this.mode == GAME_MODE.CONTINUOUS)
            this.start();

        EventManager.getInstance().fireEvent(new LivesChangeEvent(this.maze.getPacman().getLives()));
    }

    /**
     * Handles key down events.
     *
     * @param event - The event to handle
     */
    @EventHandler
    private void handleKeyDownEvent(KeyDownEvent event) {
        // Returns true if the job has finished.
        if (this.isFinished.get()) return;

        // If the game mode is step by step then tick the game.
        if (this.mode == GAME_MODE.STEP_BY_STEP)
            this.tick();
            // Start the timer if it is running.
        else if (!this.isRunning.get())
            this.start();
    }

    /**
     * Handles a pacman path click.
     *
     * @param event - The event to handle
     */
    @EventHandler
    private void handlePathMouseClickEvent(PathFieldMouseClickEvent event) {
        CommonField field = event.getField();

        // Returns the field that this field is associated with.
        if (field == null) return;

        AStarPathFinder pathFinder = new AStarPathFinder(this.maze);
        List<int[]> path = pathFinder.findPath(
                this.maze.getPacman().getRow(),
                this.maze.getPacman().getCol(),
                field.getRow(),
                field.getCol()
        );

        // Returns the path for the pacman.
        if (path == null || path.isEmpty()) return;

        this.maze.setPacmanPath(path);
    }

    /**
     * Invoked when a WinEvent is received.
     *
     * @param e - The WinEvent that was
     */
    @EventHandler
    private void handleWinEvent(WinEvent e) {
        this.isWon.set(true);
    }

    @EventHandler
    private void handleLivesChangeEvent(LivesChangeEvent e) {
        if (e.getLives() == 0) {
            this.isFinished.set(true);
            this.isWon.set(false);

            JOptionPane.showMessageDialog(
                    this.view,
                    "You lost!",
                    "Game over",
                    JOptionPane.INFORMATION_MESSAGE
            );

            this.view.dispose();
        }
    }

    /**
     * Updates the maze. Called every tick
     */
    protected void update() {
        for (GhostObject ghost : this.maze.ghosts()) {
            if (!ghost.isFrozen()) {
                ghost.generateDirection();
                ghost.move();
            } else {
                this.unfreezeTicks--;
            }

            this.loggerController.addItem(ghost);
        }

        // Unfreeze all ghosts that are frozen.
        if (this.unfreezeTicks <= 0 && Arrays.stream(this.maze.ghosts()).anyMatch(GhostObject::isFrozen))
            this.maze.unfreezeGhosts();

        PacmanObject pacman = this.maze.getPacman();

        // Move the pacman path to the current path.
        if (this.maze.getPacmanPath().size() == 0) {
            Map<Integer, Boolean> keys = KeyboardController.getInstance().getKeys();

            // Set the direction of the pacman.
            if (keys.getOrDefault(KeyEvent.VK_W, false))
                pacman.setDirection(CommonField.Direction.U);
            else if (keys.getOrDefault(KeyEvent.VK_S, false))
                pacman.setDirection(CommonField.Direction.D);
            else if (keys.getOrDefault(KeyEvent.VK_A, false))
                pacman.setDirection(CommonField.Direction.L);
            else if (keys.getOrDefault(KeyEvent.VK_D, false))
                pacman.setDirection(CommonField.Direction.R);

            // Move the pacman to the next row and column
            if (!this.maze.getField(pacman.getRow() + pacman.getDirection().deltaRow(), pacman.getCol() + pacman.getDirection().deltaCol()).canMove())
                pacman.setDirection(CommonField.Direction.N);

            keys.clear();

            pacman.move();
        } else {
            pacman.move(this.maze.getPacmanPath().get(0)[0], this.maze.getPacmanPath().get(0)[1]);
            this.maze.getPacmanPath().remove(0);

            // Set direction to N if the path is empty
            if (this.maze.getPacmanPath().isEmpty())
                pacman.setDirection(CommonField.Direction.N);
        }

        this.loggerController.addItem(pacman);
        this.loggerController.nextEntry();

        this.collisionController.detectCollisions();
        this.collisionController.handleCollisions();
    }

    /**
     * Called when the maze is rendered
     */
    protected void render() {
        this.maze.notifyUpdates();
    }

    /**
     * Finishes the game
     */
    @Override
    public void finish() {
        super.finish();

        JOptionPane.showMessageDialog(
                this.view,
                "Congratulations! You won the game!",
                "You won!",
                JOptionPane.INFORMATION_MESSAGE
        );

        this.view.dispose();
    }

    /**
     * Handles the window closing.
     *
     * @param window - The window that was closed
     */
    public void handleWindowClose(Window window) {
        this.stop();

        this.showSaveModal(window, "Save Game", "Do you want to save the game?", true);
    }

    /**
     * Shows a modal to save maze
     *
     * @param window        - the window to display the modal
     * @param title         - the title of the modal
     * @param message       - the message of the modal
     * @param includeCancel - whether to include the cancel button
     */
    private void showSaveModal(Window window, String title, String message, boolean includeCancel) {
        String[] options = {"Yes! Please.", "No! Not now."};
        if (includeCancel) options = new String[]{"Yes! Please.", "No! Not now.", "Cancel"};

        int result = JOptionPane.showOptionDialog(
                window,
                message,
                title,
                JOptionPane.YES_NO_OPTION,
                JOptionPane.QUESTION_MESSAGE,
                null,
                options,
                options[0]
        );

        // Shows a dialog to save a file to save.
        if (result == JOptionPane.YES_OPTION) {
            JFileChooser fileChooser = new JFileChooser("data/");
            fileChooser.setDialogTitle("Specify a file to save");

            int userSelection = fileChooser.showSaveDialog(window);

            // Closes the file chooser and closes the logger controller.
            if (userSelection == JFileChooser.APPROVE_OPTION) {
                File fileToSave = fileChooser.getSelectedFile();
                this.loggerController.close(fileToSave.getAbsolutePath(), this.mazeConfigure.getMazeText());
                this.destroy();
            }

            // Show the save modal dialog if the user cancels the file chooser.
            if (userSelection == JFileChooser.CANCEL_OPTION) {
                this.showSaveModal(window, title, message, includeCancel);
            }
        }

        // Destroys the game if the user does not want to save the game.
        if (result == JOptionPane.NO_OPTION)
            this.destroy();

        // If the result is JOptionPane. CANCEL_OPTION then start the game again.
        if (result == JOptionPane.CANCEL_OPTION && !this.isFinished.get())
            this.start();
    }

    /**
     * Called when the view is destroyed
     */
    @Override
    public void destroy() {
        super.destroy();

        EventManager.getInstance().removeEventListener(this);

        this.loggerController = null;
        this.collisionController = null;
        this.maze = null;
        this.mazeConfigure = null;
        this.presenter = null;
        this.view = null;
    }

    /**
     * Returns the maze view.
     *
     * @return JPanel the maze view
     */
    @Override
    public JPanel getMazeView() {
        return this.presenter;
    }
}