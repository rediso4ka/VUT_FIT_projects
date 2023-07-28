/**
 * Basic keyboard controller for the game.
 * It fires KeyDownEvent when a key is pressed.
 * Event can be handled by anywhere in the application.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.ui.controllers;

import ija.ija2022.project.events.EventManager;
import ija.ija2022.project.events.events.KeyDownEvent;

import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.util.HashMap;
import java.util.Map;

public class KeyboardController implements KeyListener {
    private static KeyboardController instance = null;
    private final Map<Integer, Boolean> keys;

    private KeyboardController() {
        this.keys = new HashMap<>();
    }

    /**
     * Returns the singleton instance of the KeyboardController class.
     *
     * @return The singleton instance of the KeyboardController class.
     */
    public static KeyboardController getInstance() {
        if (KeyboardController.instance == null) {
            KeyboardController.instance = new KeyboardController();
        }

        return KeyboardController.instance;
    }

    @Override
    public void keyTyped(KeyEvent e) {

    }

    /**
     * Invoked when a key has been pressed. This method updates the keys map with the pressed key and
     * fires a KeyDownEvent to the EventManager.
     *
     * @param e The KeyEvent object representing the key press event.
     */
    @Override
    public void keyPressed(KeyEvent e) {
        keys.put(e.getKeyCode(), true);
        EventManager.getInstance().fireEvent(new KeyDownEvent(e.getKeyCode()));
    }

    @Override
    public void keyReleased(KeyEvent e) {

    }

    public Map<Integer, Boolean> getKeys() {
        return keys;
    }
}
