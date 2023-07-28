/**
 * Project settings view
 * It is a part of MVC design pattern.
 * It is used to display the settings.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.settings;

import ija.ija2022.project.theming.THEME_NAMES;

import javax.swing.*;
import javax.swing.event.ChangeEvent;
import java.awt.*;
import java.awt.event.ActionEvent;

public class SettingsView extends JFrame {
    private SettingsController controller;
    private JLabel maxLivesLabel;
    private JLabel freezingStepsLabel;
    private JLabel themeLabel;

    public SettingsView() {
        super("Settings");
        this.controller = new SettingsController();

        this.setLayout(new GridBagLayout());
        this.setSize(new Dimension(500, 500));
        this.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);

        GridBagConstraints c = new GridBagConstraints();

        JLabel gameModeLabel = new JLabel("Game Mode");
        c.fill = GridBagConstraints.HORIZONTAL;
        c.anchor = GridBagConstraints.PAGE_START;
        c.gridx = c.gridy = 0;
        this.add(gameModeLabel, c);

        GAME_MODE selectedGameMode = this.controller.getGameMode();

        JPanel gameModeButtonsPanel = new JPanel();
        gameModeButtonsPanel.setLayout(new BoxLayout(gameModeButtonsPanel, BoxLayout.Y_AXIS));
        JRadioButton stepByStepButton = new JRadioButton("Step by step", selectedGameMode == GAME_MODE.STEP_BY_STEP);
        stepByStepButton.addActionListener(e -> this.controller.setStepByStepMode());
        JRadioButton continuousButton = new JRadioButton("Continuous", selectedGameMode == GAME_MODE.CONTINUOUS);
        continuousButton.addActionListener(e -> this.controller.setContinuousMode());
        ButtonGroup gameModeButtonGroup = new ButtonGroup();
        gameModeButtonGroup.add(stepByStepButton);
        gameModeButtonGroup.add(continuousButton);
        gameModeButtonsPanel.add(stepByStepButton);
        gameModeButtonsPanel.add(continuousButton);
        c.gridx = 1;
        this.add(gameModeButtonsPanel, c);

        maxLivesLabel = new JLabel("Max Lives: " + this.controller.getMaxLives());

        c.fill = GridBagConstraints.HORIZONTAL;
        c.anchor = GridBagConstraints.CENTER;
        c.gridx = 0;
        c.gridy = 1;
        this.add(maxLivesLabel, c);

        JSlider maxLivesSlider = new JSlider(JSlider.HORIZONTAL, 1, 10, this.controller.getMaxLives());
        maxLivesSlider.addChangeListener(this::maxLivesSliderChange);
        maxLivesSlider.setMajorTickSpacing(1);
        maxLivesSlider.setMinorTickSpacing(1);
        c.gridx = 1;
        this.add(maxLivesSlider, c);

        freezingStepsLabel = new JLabel("Freezing Steps: " + this.controller.getFreezeSteps());
        c.fill = GridBagConstraints.HORIZONTAL;
        c.anchor = GridBagConstraints.CENTER;
        c.gridx = 0;
        c.gridy = 2;
        this.add(freezingStepsLabel, c);

        JSlider freezingStepsSlider = new JSlider(JSlider.HORIZONTAL, 1, 50, this.controller.getFreezeSteps());
        freezingStepsSlider.addChangeListener(this::freezingStepsSliderChange);
        freezingStepsSlider.setMajorTickSpacing(5);
        freezingStepsSlider.setMinorTickSpacing(5);
        c.gridx = 1;
        this.add(freezingStepsSlider, c);

        themeLabel = new JLabel("Theme: " + controller.getTheme().getName());
        c.fill = GridBagConstraints.HORIZONTAL;
        c.anchor = GridBagConstraints.CENTER;
        c.gridx = 0;
        c.gridy = 3;
        this.add(themeLabel, c);

        JComboBox<THEME_NAMES> themeComboBox = new JComboBox<>(THEME_NAMES.values());
        themeComboBox.setSelectedItem(controller.getTheme());
        themeComboBox.addActionListener(this::themeComboBoxChange);
        c.gridx = 1;
        this.add(themeComboBox, c);

        JButton saveButton = new JButton("Save");
        saveButton.addActionListener(this::saveButtonClick);
        c.fill = GridBagConstraints.HORIZONTAL;
        c.gridwidth = 2;
        c.gridx = 0;
        c.gridy = 4;
        c.weightx = 0.0;
        this.add(saveButton, c);
    }

    /**
     * Updates the maximum number of lives allowed in the game when the maxLivesSlider is changed.
     *
     * @param e The ChangeEvent that triggered this method
     */
    private void maxLivesSliderChange(ChangeEvent e) {
        this.controller.setMaxLives(((JSlider) e.getSource()).getValue());
        maxLivesLabel.setText("Max Lives: " + this.controller.getMaxLives());
    }

    /**
     * Updates the number of freezing steps in the controller when the freezing steps slider is changed.
     *
     * @param e The ChangeEvent object representing the change in the slider
     */
    private void freezingStepsSliderChange(ChangeEvent e) {
        this.controller.setFreezeSteps(((JSlider) e.getSource()).getValue());
        freezingStepsLabel.setText("Freezing Steps: " + this.controller.getFreezeSteps());
    }

    /**
     * Handles the event when the theme combo box is changed. Sets the theme of the controller
     * to the selected item and updates the theme label.
     *
     * @param e The ActionEvent triggered by the theme combo box being changed
     */
    private void themeComboBoxChange(ActionEvent e) {
        this.controller.setTheme(((THEME_NAMES) (((JComboBox) e.getSource()).getSelectedItem())));
        themeLabel.setText("Theme: " + this.controller.getTheme().getName());
    }

    /**
     * Saves the current settings and closes the window.
     *
     * @param e The ActionEvent that triggered this method.
     */
    private void saveButtonClick(ActionEvent e) {
        this.controller.saveSettings();
        this.dispose();
    }

    /**
     * Disposes of the resources used by this object.
     * This method should be called when the object is no longer needed to free up resources.
     */
    @Override
    public void dispose() {
        super.dispose();

        this.controller = null;
    }
}