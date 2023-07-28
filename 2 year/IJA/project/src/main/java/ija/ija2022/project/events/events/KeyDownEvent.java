/**
 * Project event for key down event.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.events.events;

import ija.ija2022.project.events.Event;

public class KeyDownEvent extends Event {
    private final Integer keyCode;

    public KeyDownEvent(Integer _keyCode) {
        keyCode = _keyCode;
    }

    /**
    * Returns the key code associated with this Key.
    * 
    * 
    * @return the key code associated with this
    */
    public Integer getKeyCode() {
        return keyCode;
    }
}
