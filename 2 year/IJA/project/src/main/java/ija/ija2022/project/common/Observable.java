/**
 * Observable interface for Observer pattern.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.common;

public interface Observable {
    /**
    * Adds an observer to the list of observers.
    * 
    * @param observer - the observer to be
    */
    void addObserver(Observer observer);

    /**
    * Removes an observer from the list of observers.
    * 
    * @param observer - the observer to remove
    */
    void removeObserver(Observer observer);

    /**
    * Notify observers of changes. This is called from #onCreate
    */
    void notifyObservers();

    interface Observer {
        /**
        * Called when the state of the component changes.
        * 
        * @param observable - The observable that notified
        */
        void update(Observable observable);
    }
}
