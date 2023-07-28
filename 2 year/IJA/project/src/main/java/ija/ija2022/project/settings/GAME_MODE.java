/**
 * Enum for game mode.
 * Step-by-step mode means that the game will be played step by step.
 * Each step will be executed after pressing the key.
 * Continuous mode means that the game will be played continuously.
 * The game will be played until the player wins or loses.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.settings;

public enum GAME_MODE {
    STEP_BY_STEP("step-by-step"),
    CONTINUOUS("continuous");

    private final String mode;


    GAME_MODE(String mode) {
        this.mode = mode;
    }

    @Override
    public String toString() {
        return this.mode;
    }

    public static GAME_MODE fromValue(String mode) {
        for (GAME_MODE m : GAME_MODE.values()) {
            if (m.mode.equals(mode)) {
                return m;
            }
        }
        return null;
    }
}
