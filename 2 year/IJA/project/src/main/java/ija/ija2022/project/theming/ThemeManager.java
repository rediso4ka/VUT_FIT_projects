/**
 * ThemeManager class is used to get the current theme.
 * It is a singleton class.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.theming;

import ija.ija2022.project.settings.SettingsController;

public class ThemeManager {
    private static ThemeManager instance = null;

    private final SettingsController settingsController;
    private Theme currentTheme;

    private ThemeManager() {
        this.settingsController = new SettingsController();
        this.currentTheme = new Theme(this.settingsController.getTheme().getName());
    }

    /**
     * Returns the singleton instance of the ThemeManager class.
     *
     * @return The singleton instance of the ThemeManager class.
     */
    public static ThemeManager getInstance() {
        if (instance == null) {
            instance = new ThemeManager();
        }

        return instance;
    }

    /**
     * Sets the current theme to the specified theme name.
     *
     * @param name The name of the theme to set
     */
    public void setTheme(THEME_NAMES name) {
        this.currentTheme = new Theme(name.getName());
    }

    /**
     * Returns the current theme.
     *
     * @return The current theme.
     */
    public Theme getTheme() {
        return this.currentTheme;
    }
}
