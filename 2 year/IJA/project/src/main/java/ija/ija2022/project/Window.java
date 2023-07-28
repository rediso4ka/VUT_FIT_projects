/**
 * Main menu of the game.
 * It is used to start the game, replay the game or change settings.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project;

import ija.ija2022.project.game.GameController;
import ija.ija2022.project.replay.ReplayController;
import ija.ija2022.project.settings.GAME_MODE;
import ija.ija2022.project.settings.SettingsView;
import ija.ija2022.project.theming.ThemeManager;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

public class Window extends JFrame {
    public Window() {
        super("Pacman game");
        this.setFocusable(true);
        this.setFocusTraversalKeysEnabled(true);
        this.setFocusableWindowState(true);
        this.setLayout(new GridBagLayout());
        this.setSize(500, 500);
        this.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        JPanel panel = new JPanel() {
            @Override
            /**
             * Paints the background image.
             */
            protected void paintComponent(Graphics g) {
                super.paintComponent(g);

                Graphics2D g2 = (Graphics2D) g;
                g2.setClip(0, 0, getWidth(), getHeight());

                String bgImageName = ThemeManager.getInstance().getTheme().getBackgroundImageName();

                BufferedImage backgroundImage = null;
                try {
                    backgroundImage = ImageIO.read(getClass().getResource(bgImageName));
                } catch (IOException e) {
                    throw new RuntimeException(e);
                }

                Rectangle bounds = g2.getClipBounds();
                int width = bounds.width;
                int height = bounds.height;

                g2.drawImage(backgroundImage, 0, 0, width, height, null);
            }
        };
        panel.setLayout(new GridBagLayout());

        createButtons(panel);

        GridBagConstraints c = new GridBagConstraints();
        c.fill = GridBagConstraints.BOTH;
        c.weighty = 1;
        c.weightx = 1;
        c.gridx = 0;
        c.gridy = 0;
        c.anchor = GridBagConstraints.CENTER;
        this.add(panel, c);
    }

    /**
     * Creates buttons for the game menu panel and sets their action listeners.
     *
     * @param panel The panel to add the buttons to
     */
    private void createButtons(JPanel panel) {
        GridBagConstraints c = new GridBagConstraints();
        c.fill = GridBagConstraints.HORIZONTAL;
        c.gridx = 0;
        JButton startButton = new JButton("start game");
        startButton.setFont(new Font("sans-serif", Font.BOLD, 20));
        panel.add(startButton, c);
        JButton replayButton = new JButton("replay game");
        replayButton.setFont(new Font("sans-serif", Font.BOLD, 20));
        c.gridy = 1;
        panel.add(replayButton, c);
        JButton settingsButton = new JButton("settings");
        settingsButton.setFont(new Font("sans-serif", Font.BOLD, 20));
        c.gridy = 2;
        panel.add(settingsButton, c);
        JButton endButton = new JButton("exit game");
        endButton.setFont(new Font("sans-serif", Font.BOLD, 20));
        c.gridy = 3;
        panel.add(endButton, c);

        replayButton.addActionListener(e -> {
            JFileChooser fileChooser = new JFileChooser("data/");

            int result = fileChooser.showOpenDialog(Window.this);
            if (result == JFileChooser.APPROVE_OPTION) {
                File selectedFile = fileChooser.getSelectedFile();
                new ReplayController(GAME_MODE.STEP_BY_STEP, selectedFile.getAbsolutePath());
            }
        });

        startButton.addActionListener(e -> {
            int index = JOptionPane.showOptionDialog(
                    Window.this,
                    "Select a mode of play:",
                    "Game mode",
                    JOptionPane.DEFAULT_OPTION,
                    JOptionPane.QUESTION_MESSAGE,
                    null,
                    GAME_MODE.values(),
                    GAME_MODE.STEP_BY_STEP
            );
            GAME_MODE selectedMode = GAME_MODE.values()[index];

            JFileChooser fileChooser = new JFileChooser("data/maps/");

            int result = fileChooser.showOpenDialog(Window.this);
            if (result == JFileChooser.APPROVE_OPTION) {
                File selectedFile = fileChooser.getSelectedFile();
                new GameController(selectedMode, selectedFile.getAbsolutePath());
            }
        });

        settingsButton.addActionListener(e -> new SettingsView().setVisible(true));

        endButton.addActionListener(e -> System.exit(0));
    }
}