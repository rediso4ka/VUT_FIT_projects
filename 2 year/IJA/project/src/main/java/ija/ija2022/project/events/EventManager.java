/**
 * Project event manager.
 * It is used to manage events and their listeners.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.events;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.*;

public class EventManager implements IEventManager {
    private final Map<Class<? extends Event>, List<EventHandle>> eventHandleMap = new HashMap<>();

    private static EventManager instance;

    /**
    * Returns the singleton instance of EventManager.
    * 
    * 
    * @return the singleton instance of Event
    */
    public static EventManager getInstance() {
        // Create a new instance of the EventManager.
        if (instance == null) {
            instance = new EventManager();
        }
        return instance;
    }

    /**
    * Adds event listeners to the given object.
    * 
    * @param object - the object to add
    */
    public void addEventListener(Object object) {
        for (Method method : object.getClass().getDeclaredMethods()) {
            // Process the method if the method annotation is present and has a parameter type.
            if (method.isAnnotationPresent(EventHandler.class) && method.getParameterTypes().length > 0) {
                processMethod(object, method);
            }
        }
    }

    /**
    * Adds an event listener that will be called when events occur.
    * 
    * @param object - the object on which to listen for events
    * @param eventClass - the class of the
    */
    public void addSpecificEventListener(Object object, Class<? extends Event> eventClass) {
        for (Method method : object.getClass().getDeclaredMethods()) {
            // This method is called by the event handler.
            if (method.isAnnotationPresent(EventHandler.class) && method.getParameterTypes().length > 0 && method.getParameterTypes()[0].equals(eventClass)) {
                processMethod(object, method);
            }
        }
    }

    /**
    * Processes a method. This method is called by #processMethod ( Object Method ) to add the method to the event handle map
    * 
    * @param object - the object that contains the method
    * @param method - the method that is
    */
    private void processMethod(Object object, Method method) {
        EventHandler eventHandler = (EventHandler) method.getDeclaredAnnotations()[0];
        // Add a new event handle to the eventHandleMap.
        if (eventHandleMap.get(method.getParameterTypes()[0]) != null) {
            eventHandleMap.get(method.getParameterTypes()[0]).add(new EventHandle(method, object, eventHandler.priority()));
            sortArrayList(eventHandleMap.get(method.getParameterTypes()[0]));
        } else {
            List<EventHandle> eventHandles = new ArrayList<>();
            eventHandles.add(new EventHandle(method, object, eventHandler.priority()));
            eventHandleMap.put((Class<? extends Event>) method.getParameterTypes()[0], eventHandles);
        }
        method.setAccessible(true);
    }

    /**
    * Sorts the array list by priority
    * 
    * @param arrayList - the array list to
    */
    private void sortArrayList(List<EventHandle> arrayList) {
        arrayList.sort((objOne, objTwo) -> objTwo.getPriority().ordinal() - objOne.getPriority().ordinal());
    }

    /**
    * Removes all event handles associated with the given object.
    * 
    * @param object - the object to remove
    */
    public void removeEventListener(Object object) {
        Iterator<Map.Entry<Class<? extends Event>, List<EventHandle>>> iterator = eventHandleMap.entrySet().iterator();
        // Removes all events from the iterator.
        while (iterator.hasNext()) {
            Map.Entry<Class<? extends Event>, List<EventHandle>> entry = iterator.next();
            Iterator<EventHandle> eventHandleIterator = entry.getValue().iterator();
            // Removes all events from the iterator.
            while (eventHandleIterator.hasNext()) {
                // Remove the next event handle iterator.
                if (eventHandleIterator.next().getMethodClass().equals(object))
                    eventHandleIterator.remove();
            }
        }
    }

    /**
    * Fires an event. This is used to notify listeners of events
    * 
    * @param event - The event to fire
    */
    public void fireEvent(Event event) {
        List<EventHandle> eventHandles;
        // Calls all event handles for the given event.
        if ((eventHandles = eventHandleMap.get(event.getClass())) != null) {
            // Calls all event handlers for all events that have been cancelled.
            for (int i = 0; i < eventHandles.size(); i++) {
                EventHandle eventHandle = eventHandles.get(i);
                // Invoke the method of the event.
                if (!event.isCancelled() && eventHandle.getPriority() != EventPriority.MONITOR) {
                    try {
                        eventHandle.getMethod().invoke(eventHandle.getMethodClass(), event);
                    } catch (InvocationTargetException | IllegalAccessException e) {
                        e.printStackTrace();
                    }
                }
            }
        }
    }
}
