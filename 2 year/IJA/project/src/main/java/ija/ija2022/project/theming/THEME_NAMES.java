/**
 * Enum for theme names.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.theming;

public enum THEME_NAMES {
    CASTLE("castle"),
    FOREST("forest");

    private final String name;

    THEME_NAMES(String name) {
        this.name = name;
    }

    public String getName() {
        return this.name;
    }

    public static THEME_NAMES fromValue(String name) {
        for (THEME_NAMES n : THEME_NAMES.values()) {
            if (n.name.equals(name)) {
                return n;
            }
        }
        return null;
    }
}
