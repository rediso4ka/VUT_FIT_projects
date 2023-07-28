/**
 * Project event for lives change event.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.events.events;

import ija.ija2022.project.events.Event;

public class LivesChangeEvent extends Event {
    private final int lives;

    public LivesChangeEvent(int lives) {
        this.lives = lives;
    }

    /**
    * Returns the number of lives
    */
    public int getLives() {
        return this.lives;
    }
}
