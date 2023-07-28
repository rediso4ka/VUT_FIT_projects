/**
 * ReplayView class is responsible for displaying the replay game window.
 * It is a part of MVC design pattern.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.replay;

import ija.ija2022.project.events.EventHandler;
import ija.ija2022.project.events.EventManager;
import ija.ija2022.project.events.events.LivesChangeEvent;
import ija.ija2022.project.events.events.UpdateReplayStepEvent;
import ija.ija2022.project.settings.GAME_MODE;
import ija.ija2022.project.theming.ThemeManager;
import ija.ija2022.project.ui.controllers.KeyboardController;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.IOException;

public class ReplayView extends JFrame {
    private ReplayController controller;
    private final JToggleButton pauseResumeButton;
    private final JTextField stepTextField;
    private final JButton stepButton;
    private final JPanel heartsPanel;

    public ReplayView(ReplayController controller) {
        super("Replay Game");
        this.controller = controller;
        this.setLayout(new GridBagLayout());
        this.setSize(500, 500);
        this.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        this.addKeyListener(KeyboardController.getInstance());
        this.setFocusable(true);
        this.setFocusTraversalKeysEnabled(true);
        this.setFocusableWindowState(true);

        Color themeColor = Color.decode(ThemeManager.getInstance().getTheme().getBaseColor());

        this.setBackground(themeColor);
        this.getContentPane().setBackground(themeColor);
        GridBagConstraints c = new GridBagConstraints();

        JPanel buttonsPanel = new JPanel();
        buttonsPanel.setLayout(new FlowLayout(FlowLayout.CENTER));
        buttonsPanel.setBackground(themeColor);

        ImageIcon increaseContinuousSpeedIcon = new ImageIcon("src/main/resources/icons/ff.png");
        JButton increaseContinuousSpeed = new JButton(increaseContinuousSpeedIcon);
        increaseContinuousSpeed.setFocusable(false);
        increaseContinuousSpeed.setToolTipText("Increase speed");
        ImageIcon decreaseContinuousSpeedIcon = new ImageIcon("src/main/resources/icons/fr.png");
        JButton decreaseContinuousSpeed = new JButton(decreaseContinuousSpeedIcon);
        decreaseContinuousSpeed.setFocusable(false);
        decreaseContinuousSpeed.setToolTipText("Decrease speed");
        ImageIcon stepForwardIcon = new ImageIcon("src/main/resources/icons/sn.png");
        JButton stepForward = new JButton(stepForwardIcon);
        stepForward.setFocusable(false);
        stepForward.setToolTipText("Next step");
        ImageIcon stepBackwardIcon = new ImageIcon("src/main/resources/icons/sp.png");
        JButton stepBackward = new JButton(stepBackwardIcon);
        stepBackward.setFocusable(false);
        stepBackward.setToolTipText("Previous step");
        ImageIcon pauseIcon = new ImageIcon("src/main/resources/icons/pa.png");
        pauseResumeButton = new JToggleButton(pauseIcon);
        pauseResumeButton.setFocusable(false);
        pauseResumeButton.setToolTipText("Play/Pause");

        decreaseContinuousSpeed.addActionListener(e -> controller.increaseTickTime());
        stepBackward.addActionListener(e -> controller.previousStep());
        pauseResumeButton.addActionListener(this::pauseButtonClickHandler);
        stepForward.addActionListener(e -> controller.nextStep());
        increaseContinuousSpeed.addActionListener(e -> controller.decreaseTickTime());

        stepTextField = new JTextField("0");
        stepButton = new JButton("Go to step");
        stepButton.setFocusable(false);
        stepButton.addActionListener(this::stepButtonClickHandler);

        buttonsPanel.add(decreaseContinuousSpeed);
        buttonsPanel.add(stepBackward);
        buttonsPanel.add(pauseResumeButton);
        buttonsPanel.add(stepForward);
        buttonsPanel.add(increaseContinuousSpeed);

        c.fill = GridBagConstraints.HORIZONTAL;
        c.weightx = 1;
        c.gridwidth = 2;
        this.add(buttonsPanel, c);

        c.gridx = 0;
        c.gridwidth = 1;
        c.gridy = 1;
        c.fill = GridBagConstraints.HORIZONTAL;
        this.add(stepTextField, c);
        c.gridx = 1;
        this.add(stepButton, c);

        heartsPanel = new JPanel();
        heartsPanel.setBackground(themeColor);
        heartsPanel.setLayout(new FlowLayout(FlowLayout.LEADING));

        c.fill = GridBagConstraints.HORIZONTAL;
        c.anchor = GridBagConstraints.CENTER;
        c.weightx = 1;
        c.gridx = 0;
        c.gridy = 2;
        this.add(heartsPanel, c);

        JPanel mazePanel = new JPanel();
        mazePanel.setLayout(new FlowLayout(FlowLayout.CENTER));
        mazePanel.setBackground(themeColor);

        c.fill = GridBagConstraints.BOTH;
        c.weighty = 1;
        c.weightx = 1;
        c.gridx = 0;
        c.gridwidth = 5;
        c.gridy = 3;
        mazePanel.add(controller.getMazeView());
        this.add(mazePanel, c);

        addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                super.windowClosing(e);
                handleWindowClosing();
            }
        });

        EventManager.getInstance().addEventListener(this);
    }

    /**
     * Draws the lives of the player on the screen.
     *
     * @param count The number of lives the player has.
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

        for (int i = 0; i < max; i++)
            heartsPanel.add(new JLabel(i < count ? heartIcon : heartEmptyIcon));

        this.revalidate();
        this.repaint();
    }

    /**
     * Handles the pause button click event. If the game mode is continuous, the game is paused and the button icon is changed to a pause icon.
     * If the game mode is step-by-step, the game is resumed and the button icon is changed to a play icon.
     *
     * @param e The ActionEvent object representing the pause button click event.
     */
    private void pauseButtonClickHandler(ActionEvent e) {
        ImageIcon pauseIcon = new ImageIcon("src/main/resources/icons/pa.png");
        ImageIcon playIcon = new ImageIcon("src/main/resources/icons/pl.png");
        if (controller.getMode() == GAME_MODE.CONTINUOUS) {
            pauseResumeButton.setIcon(pauseIcon);
            controller.setMode(GAME_MODE.STEP_BY_STEP);
            controller.stop();
        } else {
            pauseResumeButton.setIcon(playIcon);
            controller.setMode(GAME_MODE.CONTINUOUS);
            controller.start();
        }
    }

    /**
     * Handles the click event of the "Step" button. Parses the integer value from the text field
     * and passes it to the controller to jump to the corresponding step.
     *
     * @param e The ActionEvent object representing the click event
     */
    private void stepButtonClickHandler(ActionEvent e) {
        int step = Integer.parseInt(stepTextField.getText());
        this.controller.jumpToStep(step);
    }

    /**
     * Handles the UpdateReplayStepEvent by updating the step text field with the current step.
     *
     * @param event The UpdateReplayStepEvent to handle
     */
    @EventHandler
    private void handleReplayStepUpdate(UpdateReplayStepEvent event) {
        String step = event.getStep() < 0 ? "0" : String.valueOf(event.getStep());
        this.stepTextField.setText(step);
    }

    /**
     * Handles a change in the number of lives a player has and updates the UI accordingly.
     *
     * @param event The LivesChangeEvent containing the new number of lives.
     */
    @EventHandler
    private void handleLivesChange(LivesChangeEvent event) {
        this.drawLives(event.getLives());
    }

    /**
     * Handles the window closing event by removing the key listener, event listener, and destroying the controller.
     */
    private void handleWindowClosing() {
        this.removeKeyListener(KeyboardController.getInstance());

        EventManager.getInstance().removeEventListener(this);

        this.controller.destroy();
        this.controller = null;
    }
}
