/**
 * Interface for event manager.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.events;

public interface IEventManager {

    /**
    * Adds an event listener to this component.
    * 
    * @param object - the object that will be
    */
    void addEventListener(Object object);

    /**
    * Adds an event listener that will be called when events occur.
    * 
    * @param object - the object that the event is associated with
    * @param eventClass - the class of the
    */
    void addSpecificEventListener(Object object, Class<? extends Event> eventClass);

    /**
    * Removes an event listener.
    * 
    * @param object - the event listener to
    */
    void removeEventListener(Object object);

    /**
    * Fires an event to all listeners.
    * 
    * @param event - the event to fire
    */
    void fireEvent(Event event);

}
