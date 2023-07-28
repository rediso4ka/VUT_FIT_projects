/**
 * Represents a theme of the game.
 * It is used to get address of the images to be used in the game.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.theming;

import ija.ija2022.project.fields.CommonField;
import org.json.JSONObject;
import org.json.JSONTokener;

import java.io.InputStream;

public class Theme {
    private final String directoryName;
    private String baseColor;

    public Theme(String directoryName) {
        this.directoryName = "/themes/" + directoryName;

        this.processConfig();
    }

    /**
     * Processes the configuration file for the theme.
     * The configuration file is a JSON file that contains the base color for the theme.
     * If the configuration file is not found, a RuntimeException is thrown.
     * @throws RuntimeException if the configuration file is not found
     */
    private void processConfig() {
        InputStream is = ThemeManager.class.getResourceAsStream(this.directoryName + "/config.json");

        if (is == null) {
            throw new RuntimeException("Theme config file not found");
        }

        JSONObject object = new JSONObject(new JSONTokener(is));

        this.baseColor = object.getString("baseColor");
    }

    /**
     * Returns the name of the wall sprite for this object.
     *
     * @return The name of the wall sprite.
     */
    public String getWallSpriteName() {
        return this.directoryName + "/" + THEME_OBJECT_NAMES_PATTERNS.WALL.name().toLowerCase() + ".png";
    }

    /**
     * Returns the name of the sprite for the floor object in the current directory.
     *
     * @return The name of the sprite for the floor object
     */
    public String getFloorSpriteName() {
        return this.directoryName + "/" + THEME_OBJECT_NAMES_PATTERNS.FLOOR.name().toLowerCase() + ".png";
    }

    /**
     * Returns the name of the sprite image file for Pac-Man in the given direction.
     *
     * @param direction The direction in which Pac-Man is facing.
     * @return The name of the sprite image file.
     */
    public String getPacmanSpriteName(CommonField.Direction direction) {
        return this.directoryName + "/" + THEME_OBJECT_NAMES_PATTERNS.PACMAN.name().toLowerCase() + "_" + direction.getChar() + ".png";
    }

    /**
     * Returns the name of the ghost sprite image file based on the given direction.
     *
     * @param direction The direction the ghost is facing.
     * @return The name of the ghost sprite image file.
     */
    public String getGhostSpriteName(CommonField.Direction direction) {
        return this.directoryName + "/" + THEME_OBJECT_NAMES_PATTERNS.GHOST.name().toLowerCase() + "_" + direction.getChar() + ".png";
    }

    /**
     * Returns the name of the ghost sprite image file based on whether it is frozen or not.
     *
     * @param frozen Whether the ghost is frozen or not.
     * @return The name of the ghost sprite image file.
     */
    public String getGhostSpriteName(boolean frozen) {
        return this.directoryName + "/" + THEME_OBJECT_NAMES_PATTERNS.GHOST.name().toLowerCase() + "_" + (frozen ? "f" : "n") + ".png";
    }

    /**
     * Returns the file name of the empty heart sprite for this directory.
     *
     * @return The file name of the empty heart sprite.
     */
    public String getEmptyHeartSpriteName() {
        return this.directoryName + "/" + THEME_OBJECT_NAMES_PATTERNS.HEART_EMPTY.name().toLowerCase() + ".png";
    }

    /**
     * Returns the name of the sprite image file for the heart object in the current directory.
     *
     * @return The name of the sprite image file for the heart object
     */
    public String getHeartSpriteName() {
        return this.directoryName + "/" + THEME_OBJECT_NAMES_PATTERNS.HEART.name().toLowerCase() + ".png";
    }

    /**
     * Returns the name of the sprite image file for the clock object in the current directory.
     *
     * @return The name of the sprite image file for the clock object
     */
    public String getClockSpriteName() {
        return this.directoryName + "/" + THEME_OBJECT_NAMES_PATTERNS.CLOCK.name().toLowerCase() + ".png";
    }

    /**
     * Returns the name of the sprite image file for the key object in the current directory.
     *
     * @return The name of the sprite image file for the key object
     */
    public String getKeySpriteName() {
        return this.directoryName + "/" + THEME_OBJECT_NAMES_PATTERNS.KEY.name().toLowerCase() + ".png";
    }

    /**
     * Returns the name of the sprite image file for the target object in the current directory.
     *
     * @param opened Whether the target object is open or closed.
     * @return The name of the sprite image file for the target object.
     */
    public String getTargetSpriteName(boolean opened) {
        return this.directoryName + "/" + THEME_OBJECT_NAMES_PATTERNS.TARGET.name().toLowerCase() + "_" + (opened ? "o" : "c") + ".png";
    }

    /**
     * Returns the name of the background image file for this theme.
     *
     * @return The name of the background image file.
     */
    public String getBackgroundImageName() {
        return this.directoryName + "/" + THEME_OBJECT_NAMES_PATTERNS.BACKGROUND.name().toLowerCase() + ".jpg";
    }

    /**
     * Returns the base color of this object.
     *
     * @return The base color of this object.
     */
    public String getBaseColor() {
        return baseColor;
    }
}
