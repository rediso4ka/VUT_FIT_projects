/**
 * Represents a controller for replay mode.
 * It is a part of MVC design pattern.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.replay;

import ija.ija2022.project.collisions.CollisionController;
import ija.ija2022.project.commands.CommandProcessor;
import ija.ija2022.project.common.BaseGameViewController;
import ija.ija2022.project.events.EventHandler;
import ija.ija2022.project.events.EventManager;
import ija.ija2022.project.events.events.KeyDownEvent;
import ija.ija2022.project.events.events.LivesChangeEvent;
import ija.ija2022.project.logger.LOGGER_MODE;
import ija.ija2022.project.logger.LogEntry;
import ija.ija2022.project.logger.LoggerController;
import ija.ija2022.project.maze.CommonMaze;
import ija.ija2022.project.maze.MazePresenter;
import ija.ija2022.project.maze.configure.MazeConfigure;
import ija.ija2022.project.settings.GAME_MODE;

import javax.swing.*;
import java.util.List;

import static java.awt.event.KeyEvent.VK_LEFT;
import static java.awt.event.KeyEvent.VK_RIGHT;

public class ReplayController extends BaseGameViewController {
    private final CommonMaze maze;
    private REPLAY_DIRECTION replayDirection = REPLAY_DIRECTION.FORWARD;
    private final MazePresenter presenter;
    private final ReplayView view;
    private final CollisionController collisionController;
    private final LoggerController logger;
    private final CommandProcessor commandProcessor;

    public ReplayController(GAME_MODE mode, String filePath) {
        super(mode);

        this.logger = new LoggerController(LOGGER_MODE.READ, filePath);

        MazeConfigure mazeConfigure = new MazeConfigure(this.logger.getMapText());

        this.maze = mazeConfigure.createMaze();
        this.presenter = new MazePresenter(this.maze);
        this.view = new ReplayView(this);

        this.collisionController = new CollisionController(this.maze);
        this.commandProcessor = new CommandProcessor(this.maze, this.logger);

        this.view.setVisible(true);

        EventManager.getInstance().fireEvent(new LivesChangeEvent(this.maze.getPacman().getLives()));
    }

    /**
     * Handles the KeyDownEvent for the left and right arrow keys.
     * If the mode is set to STEP_BY_STEP, it prepares the previous or next step
     * depending on the key pressed, and then ticks the game.
     *
     * @param e The KeyDownEvent object
     */
    @EventHandler
    private void handleKeyDownEvent(KeyDownEvent e) {
        if (e.getKeyCode() != VK_LEFT && e.getKeyCode() != VK_RIGHT)
            return;

        if (this.mode == GAME_MODE.STEP_BY_STEP) {
            if (e.getKeyCode() == VK_LEFT) this.preparePreviousStep();
            else if (e.getKeyCode() == VK_RIGHT) this.prepareNextStep();

            this.tick();
        }
    }

    /**
     * Prepares the player to move to the previous step in the replay.
     * If the replay direction was previously forward, the logger will move to the previous entry.
     * The replay direction is then set to backward.
     */
    private void preparePreviousStep() {
        if (this.replayDirection == REPLAY_DIRECTION.FORWARD)
            this.logger.previousEntry();
        this.replayDirection = REPLAY_DIRECTION.BACKWARD;
    }

    /**
     * Prepares the next step in the replay process by setting the replay direction to forward
     * and moving the logger to the next entry if the replay direction was previously backward.
     */
    private void prepareNextStep() {
        if (this.replayDirection == REPLAY_DIRECTION.BACKWARD)
            this.logger.nextEntry();
        this.replayDirection = REPLAY_DIRECTION.FORWARD;
    }

    /**
     * Goes back to the previous step in the process.
     * This method prepares the previous step and then ticks the process.
     */
    public void previousStep() {
        this.preparePreviousStep();
        this.tick();
    }

    /**
     * Proceeds to the next step of the process by preparing the next step and ticking.
     */
    public void nextStep() {
        this.prepareNextStep();
        this.tick();
    }

    /**
     * Jumps to a specific step in the logger and processes the entries up to that point.
     * Also detects and handles collisions and renders the current state.
     *
     * @param step The step to jump to in the logger
     */
    public void jumpToStep(int step) {
        boolean reverse = this.logger.getIndex() > step;

        List<LogEntry> entries = this.logger.getEntries(reverse ? step : this.logger.getIndex(), reverse ? this.logger.getIndex() : step);
        this.commandProcessor.processEntries(entries, reverse);
        this.logger.setIndex(step);

        this.collisionController.detectCollisions();
        this.collisionController.handleCollisions();

        this.render();
    }

    /**
     * Updates the state of the game by processing the next or previous entry in the logger,
     * depending on the replay direction. It then detects and handles any collisions that may have occurred.
     */
    protected void update() {
        this.commandProcessor.processEntry(
                this.logger.getEntry(this.logger.getIndex()),
                replayDirection == REPLAY_DIRECTION.BACKWARD
        );

        if (replayDirection == REPLAY_DIRECTION.FORWARD)
            this.logger.nextEntry();
        else
            this.logger.previousEntry();

        this.collisionController.detectCollisions();
        this.collisionController.handleCollisions();
    }

    /**
     * Renders the maze by notifying any updates to the maze object.
     */
    protected void render() {
        this.maze.notifyUpdates();
    }

    /**
     * Runs a check to see if the logger has a current entry. If there is no current entry,
     * the method stops the logger.
     */
    protected void runCheck() {
        if (this.logger.currentEntry() == null)
            this.stop();
    }

    /**
     * Destroys the current instance of the object, removing all event listeners and disposing of the view and logger.
     */
    @Override
    public void destroy() {
        super.destroy();

        EventManager.getInstance().removeEventListener(this);

        this.view.dispose();
        this.logger.close();
    }

    /**
     * Returns the maze view panel.
     *
     * @return The maze view panel.
     */
    public JPanel getMazeView() {
        return this.presenter;
    }
}
