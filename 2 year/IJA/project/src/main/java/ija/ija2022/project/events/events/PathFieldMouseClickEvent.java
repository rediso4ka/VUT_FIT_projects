/**
 * Project event for path field mouse click event.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.events.events;

import ija.ija2022.project.events.Event;
import ija.ija2022.project.fields.CommonField;

public class PathFieldMouseClickEvent extends Event {
    private CommonField field;

    public PathFieldMouseClickEvent(CommonField field) {
        this.field = field;
    }

    /**
    * Returns the CommonField that this event is associated with.
    * 
    * @return the CommonField that this event is associated with
    */
    public CommonField getField() {
        return this.field;
    }
}
