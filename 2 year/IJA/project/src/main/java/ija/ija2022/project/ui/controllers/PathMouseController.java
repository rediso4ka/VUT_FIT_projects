/**
 * Represents an event of mouse click on a path field.
 * It fires PathFieldMouseClickEvent when a path field is clicked.
 * Event can be handled by anywhere in the application.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.ui.controllers;

import ija.ija2022.project.events.EventManager;
import ija.ija2022.project.events.events.PathFieldMouseClickEvent;
import ija.ija2022.project.fields.CommonField;
import ija.ija2022.project.fields.FieldView;

import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

public class PathMouseController implements MouseListener {
    private static PathMouseController instance;

    /**
     * Returns the singleton instance of the PathMouseController class.
     *
     * @return The singleton instance of the PathMouseController class.
     */
    public static PathMouseController getInstance() {
        if (instance == null) {
            instance = new PathMouseController();
        }

        return instance;
    }

    /**
     * Invoked when the mouse button has been clicked (pressed and released) on a component.
     * This method retrieves the field that was clicked and creates a PathFieldMouseClickEvent
     * to notify any listeners of the click event.
     *
     * @param e The MouseEvent that occurred
     */
    @Override
    public void mouseClicked(MouseEvent e) {
        CommonField field = ((FieldView) e.getSource()).getField();
        EventManager.getInstance().fireEvent(new PathFieldMouseClickEvent(field));
    }

    @Override
    public void mousePressed(MouseEvent e) {

    }

    @Override
    public void mouseReleased(MouseEvent e) {

    }

    @Override
    public void mouseEntered(MouseEvent e) {

    }

    @Override
    public void mouseExited(MouseEvent e) {

    }
}
