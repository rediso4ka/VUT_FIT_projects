/**
 * Enum for theme object names patterns.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.theming;

public enum THEME_OBJECT_NAMES_PATTERNS {
    WALL("wall"),
    FLOOR("floor"),
    GHOST("ghost"),
    PACMAN("pacman"),
    HEART("heart"),
    HEART_EMPTY("heart_empty"),
    CLOCK("clock"),
    KEY("key"),
    TARGET("target"),
    BACKGROUND("background");

    private final String name;

    THEME_OBJECT_NAMES_PATTERNS(String name) {
        this.name = name;
    }
}
