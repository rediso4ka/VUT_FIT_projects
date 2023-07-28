/**
 * Project settings model
 * It is a part of MVC design pattern.
 * It is used to store the settings.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.settings;

import ija.ija2022.project.theming.THEME_NAMES;

public class SettingsModel {
    private GAME_MODE gameMode;
    private int maxLives;
    private int freezeSteps;
    private THEME_NAMES theme;

    /**
     * Returns the current game mode.
     *
     * @return The current game mode.
     */
    public GAME_MODE getGameMode() {
        return gameMode;
    }

    /**
     * Sets the game mode of the player.
     *
     * @param gameMode The game mode to set for the player.
     */
    public void setGameMode(GAME_MODE gameMode) {
        this.gameMode = gameMode;
    }

    /**
     * Sets the maximum number of lives for a player.
     *
     * @param lives The maximum number of lives to set.
     */
    public void setMaxLives(int lives) {
        this.maxLives = lives;
    }

    /**
     * Returns the maximum number of lives allowed for a player.
     *
     * @return The maximum number of lives allowed.
     */
    public int getMaxLives() {
        return this.maxLives;
    }

    /**
     * Returns the number of steps to freeze
     *
     * @return The number of steps to freeze.
     */
    public int getFreezeSteps() {
        return freezeSteps;
    }

    /**
     * Sets the number of freeze steps for the current object.
     *
     * @param freezeSteps The number of freeze steps to set.
     */
    public void setFreezeSteps(int freezeSteps) {
        this.freezeSteps = freezeSteps;
    }

    /**
     * Sets the theme of the application to the specified theme.
     *
     * @param theme The theme to set the application to.
     */
    public void setTheme(THEME_NAMES theme) {
        this.theme = theme;
    }

    /**
     * Returns the current theme of the application.
     *
     * @return The current theme.
     */
    public THEME_NAMES getTheme() {
        return this.theme;
    }
}
