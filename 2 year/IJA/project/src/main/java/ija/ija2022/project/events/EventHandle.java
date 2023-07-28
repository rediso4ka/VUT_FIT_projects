/**
 * Class representing event handle.
 * It contains method, method class and priority.
 * It is used for event handling.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.events;

import java.lang.reflect.Method;

public class EventHandle {
    private final Method method;
    private final Object methodClass;
    private final EventPriority priority;

    public EventHandle(Method method, Object methodClass, EventPriority priority) {
        this.method = method;
        this.methodClass = methodClass;
        this.priority = priority;
    }

    /**
    * Returns the priority of this event.
    * 
    * 
    * @return EventPriority#PRIOR for the event
    */
    public EventPriority getPriority() {
        return priority;
    }

    /**
    * Returns the method associated with this request.
    * 
    * 
    * @return the method associated with this
    */
    public Method getMethod() {
        return method;
    }

    /**
    * Returns the class of the method represented by this MethodDescriptor.
    * 
    * 
    * @return the class of the method represented by this MethodDescriptor
    */
    public Object getMethodClass() {
        return methodClass;
    }

    /**
    * Returns true if the given object is an EventHandle and has the same method and method class
    * 
    * @param obj - the object to compare
    */
    @Override
    public boolean equals(Object obj) {
        return obj instanceof EventHandle && ((EventHandle) obj).getMethod().equals(method) && ((EventHandle) obj).getMethodClass().equals(methodClass);
    }
}