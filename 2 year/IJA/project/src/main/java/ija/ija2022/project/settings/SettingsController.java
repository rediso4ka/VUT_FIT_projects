/**
 * Project settings controller
 * It is a part of MVC design pattern.
 * It is used by the settings view to change the settings.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.settings;

import ija.ija2022.project.theming.THEME_NAMES;
import ija.ija2022.project.theming.ThemeManager;
import org.json.JSONObject;
import org.json.JSONTokener;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

public class SettingsController {
    private final SettingsModel model;

    public SettingsController() {
        this.model = new SettingsModel();
        this.loadSettings();
    }

    /**
     * Loads the game settings from a JSON file and sets the corresponding values in the model.
     * The JSON file must be located in the "data" directory and named "settings.json".
     *
     * @throws RuntimeException if the JSON file is not found or cannot be read
     */
    private void loadSettings() {
        FileInputStream is;
        try {
            is = new FileInputStream("data/settings.json");
        } catch (FileNotFoundException e) {
            throw new RuntimeException(e);
        }

        JSONTokener tokener = new JSONTokener(is);
        JSONObject object = new JSONObject(tokener);
        String gameMode = object.getString("mode");
        int maxLives = object.getInt("maxLives");
        int freezeSteps = object.getInt("freezeSteps");
        String themeName = object.getString("theme");

        this.model.setGameMode(GAME_MODE.fromValue(gameMode));
        this.model.setMaxLives(maxLives);
        this.model.setFreezeSteps(freezeSteps);
        this.model.setTheme(THEME_NAMES.fromValue(themeName));
    }

    /**
     * Sets the game mode to continuous mode.
     */
    public void setContinuousMode() {
        model.setGameMode(GAME_MODE.CONTINUOUS);
    }

    /**
     * Sets the game mode to step-by-step mode.
     */
    public void setStepByStepMode() {
        model.setGameMode(GAME_MODE.STEP_BY_STEP);
    }

    /**
     * Saves the current game settings to a JSON file.
     * The settings include the game mode, maximum lives, freeze steps, and theme.
     * The file is saved to the "data" directory with the name "settings.json".
     * If there is an error while saving the file, an error message will be printed to the console.
     */
    public void saveSettings() {
        JSONObject object = new JSONObject();
        object.put("mode", model.getGameMode().toString());
        object.put("maxLives", model.getMaxLives());
        object.put("freezeSteps", model.getFreezeSteps());
        object.put("theme", model.getTheme().getName());

        FileOutputStream os = null;
        try {
            os = new FileOutputStream("data/settings.json");
        } catch (FileNotFoundException e) {
            System.out.println("Settings file not found");
        }

        try {
            assert os != null;
            os.write(object.toString().getBytes());
            os.close();
        } catch (IOException e) {
            System.out.println("Error while saving settings");
        }
    }

    /**
     * Returns the current game mode of the model.
     *
     * @return The current game mode.
     */
    public GAME_MODE getGameMode() {
        return model.getGameMode();
    }

    /**
     * Sets the maximum number of lives for the game.
     *
     * @param lives The maximum number of lives to set.
     */
    public void setMaxLives(int lives) {
        model.setMaxLives(lives);
    }

    /**
     * Returns the maximum number of lives allowed in the game.
     *
     * @return The maximum number of lives allowed.
     */
    public int getMaxLives() {
        return model.getMaxLives();
    }

    /**
     * Sets the number of freeze steps in the model.
     *
     * @param freezeSteps The number of freeze steps to set.
     */
    public void setFreezeSteps(int freezeSteps) {
        model.setFreezeSteps(freezeSteps);
    }

    /**
     * Returns the number of freeze steps in the model.
     *
     * @return The number of freeze steps.
     */
    public int getFreezeSteps() {
        return model.getFreezeSteps();
    }

    /**
     * Sets the theme of the application to the specified theme.
     *
     * @param theme The theme to set the application to.
     */
    public void setTheme(THEME_NAMES theme) {
        model.setTheme(theme);
        ThemeManager.getInstance().setTheme(theme);
    }

    /**
     * Returns the current theme of the model.
     *
     * @return The current theme of the model.
     */
    public THEME_NAMES getTheme() {
        return model.getTheme();
    }
}
