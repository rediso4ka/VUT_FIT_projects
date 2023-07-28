/**
 * BaseGameViewController class is used in GameController and ReplayController.
 * This class mainly works with the game loop and the game tick thread.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.common;

import ija.ija2022.project.events.EventManager;
import ija.ija2022.project.settings.GAME_MODE;
import ija.ija2022.project.settings.SettingsController;

import javax.swing.*;
import java.util.concurrent.atomic.AtomicBoolean;

public abstract class BaseGameViewController implements Runnable {
    protected GAME_MODE mode;
    protected Thread tickThread;
    protected int tickTime = 250;
    protected AtomicBoolean isRunning = new AtomicBoolean(false);
    protected AtomicBoolean isFinished = new AtomicBoolean(false);
    protected AtomicBoolean isWon = new AtomicBoolean(false);
    protected SettingsController settingsController;

    public BaseGameViewController(GAME_MODE mode) {
        this.mode = mode;
        this.tickThread = new Thread(this);
        this.settingsController = new SettingsController();

        EventManager.getInstance().addEventListener(this);
    }

    /**
     * Runs the game. This is the main loop
     */
    @Override
    public void run() {
        isRunning.set(true);
        // This method is thread safe.
        while (isRunning.get()) {
            try {
                this.tick();

                if (this.isWon.get()) {
                    this.finish();
                    break;
                }

                // Sleeps for tickTime tickTime.
                if (this.mode == GAME_MODE.CONTINUOUS) Thread.sleep(tickTime);

                runCheck();
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
    }

    protected void runCheck() {
    }

    /**
     * Updates the state of the maze. This is called by the tick
     */
    abstract protected void update();

    /**
     * Renders the view. This is called by the tick
     */
    abstract protected void render();

    /**
     * Updates the game. Called every tick
     */
    protected void tick() {
        this.update();
        this.render();
    }

    /**
     * Starts the tick thread.
     */
    public void start() {
        this.tickThread = new Thread(this);
        this.tickThread.start();
    }

    /**
     * Stops the thread. Does not wait for it to finish
     */
    public void stop() {
        this.isRunning.set(false);
    }

    /**
     * Stops and sets isFinished
     */
    public void finish() {
        this.stop();
        this.isFinished.set(true);
    }

    /**
     * Stops the timer and destroys the controller
     */
    public void destroy() {
        this.stop();
        this.tickThread.interrupt();
        this.tickThread = null;
        this.settingsController = null;

        EventManager.getInstance().removeEventListener(this);
    }

    /**
     * Increases the tick time by 50
     */
    public void increaseTickTime() {
        tickTime += 50;
    }

    /**
     * Decreases the tick time by 50
     */
    public void decreaseTickTime() {
        tickTime -= 50;
        // Set the tick time to 0.
        if (tickTime < 0) {
            tickTime = 0;
        }
    }

    /**
     * Sets the game mode.
     *
     * @param mode - The game mode to
     */
    public void setMode(GAME_MODE mode) {
        this.mode = mode;
    }

    /**
     * Returns the game mode.
     *
     * @return GAME_MODE. COMPACT or GAME_MODE.
     */
    public GAME_MODE getMode() {
        return mode;
    }

    /**
     * Returns the maze view.
     *
     * @return JPanel The maze view
     */
    abstract public JPanel getMazeView();

    /**
     * Returns the maximum number of lives that can be created
     */
    public int getMaxLives() {
        return this.settingsController.getMaxLives();
    }
}
