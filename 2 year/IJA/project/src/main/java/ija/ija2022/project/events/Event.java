/**
 * Basic event class for project. It is used as a base class for all other events.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.events;

public abstract class Event {
    private boolean isCancelled;

    /**
    * Sets the cancelled flag.
    * 
    * @param isCancelled - true if the task is cancelled
    */
    public void setCancelled(boolean isCancelled) {
        this.isCancelled = isCancelled;
    }

    /**
    * Returns true if the task has been cancelled
    */
    public boolean isCancelled() {
        return isCancelled;
    }
}
