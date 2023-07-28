/**
 * GameView class represents the view of the game.
 * It is responsible for displaying the game.
 * It is a part of the MVC pattern.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.game;

import ija.ija2022.project.events.EventHandler;
import ija.ija2022.project.events.EventManager;
import ija.ija2022.project.events.events.LivesChangeEvent;
import ija.ija2022.project.theming.ThemeManager;
import ija.ija2022.project.ui.controllers.KeyboardController;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.io.IOException;

public class GameView extends JFrame {
    private GameController controller;
    private final JPanel heartsPanel;

    public GameView(GameController controller) {
        super("Play Game");
        this.controller = controller;
        this.setLayout(new GridBagLayout());
        this.setSize(500, 500);
        this.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        this.addKeyListener(KeyboardController.getInstance());
        this.setFocusable(true);
        this.setFocusTraversalKeysEnabled(true);
        this.setFocusableWindowState(true);

        Color themeColor = Color.decode(ThemeManager.getInstance().getTheme().getBaseColor());

        this.setBackground(themeColor);
        this.getContentPane().setBackground(themeColor);

        JPanel mazePanel = new JPanel();
        mazePanel.setLayout(new FlowLayout(FlowLayout.CENTER));
        mazePanel.setBackground(themeColor);

        GridBagConstraints c = new GridBagConstraints();
        c.fill = GridBagConstraints.BOTH;
        c.weighty = 1;
        c.weightx = 1;
        c.gridx = 0;
        c.gridy = 1;
        c.anchor = GridBagConstraints.PAGE_START;
        mazePanel.add(controller.getMazeView());
        this.add(mazePanel, c);

        heartsPanel = new JPanel();
        heartsPanel.setBackground(themeColor);
        heartsPanel.setLayout(new FlowLayout(FlowLayout.LEADING));

        c.fill = GridBagConstraints.HORIZONTAL;
        c.anchor = GridBagConstraints.CENTER;
        c.weighty = 0.02;
        c.weightx = 1;
        c.gridx = 0;
        c.gridy = 0;
        c.ipady = 0;
        this.add(heartsPanel, c);

        EventManager.getInstance().addEventListener(this);
    }

    /**
     * Handles lives change.
     *
     * @param event - The event to handle
     */
    @EventHandler
    private void handleLivesChange(LivesChangeEvent event) {
        this.drawLives(event.getLives());
    }

    /**
     * Draws lives on the hearts panel
     *
     * @param count - number of lives to
     */
    private void drawLives(int count) {
        this.heartsPanel.removeAll();

        int max = Math.max(this.controller.getMaxLives(), count);

        int heartSize = 20;

        ImageIcon heartIcon = null;
        try {
            heartIcon = new ImageIcon(ImageIO.read(getClass().getResource(ThemeManager.getInstance().getTheme().getHeartSpriteName())));
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        Image image = heartIcon.getImage();
        Image newImage = image.getScaledInstance(heartSize, heartSize, java.awt.Image.SCALE_SMOOTH);
        heartIcon = new ImageIcon(newImage);
        ImageIcon heartEmptyIcon = null;
        try {
            heartEmptyIcon = new ImageIcon(ImageIO.read(getClass().getResource(ThemeManager.getInstance().getTheme().getEmptyHeartSpriteName())));
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        image = heartEmptyIcon.getImage();
        newImage = image.getScaledInstance(heartSize, heartSize, java.awt.Image.SCALE_SMOOTH);
        heartEmptyIcon = new ImageIcon(newImage);

        // Add all hearts to the hearts panel.
        for (int i = 0; i < max; i++)
            heartsPanel.add(new JLabel(i < count ? heartIcon : heartEmptyIcon));

        this.revalidate();
        this.repaint();
    }

    /**
     * Called when a window is closing.
     *
     * @param window - The window that was
     */
    public void handleWindowClosing(Window window) {
        this.controller.handleWindowClose(window);
    }

    /**
     * Removes this component from the game.
     */
    @Override
    public void dispose() {
        super.dispose();

        this.handleWindowClosing(this);

        this.heartsPanel.removeAll();

        this.removeKeyListener(KeyboardController.getInstance());
        EventManager.getInstance().removeEventListener(this);

        this.controller = null;
    }
}
