/**
 * PacmanView class represents the view of the PacmanObject.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.objects;

import ija.ija2022.project.common.ComponentView;
import ija.ija2022.project.fields.FieldView;
import ija.ija2022.project.theming.ThemeManager;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.IOException;

public class PacmanView implements ComponentView {
    private final FieldView parent;
    private final PacmanObject pacman;

    public PacmanView(FieldView parent, PacmanObject pacman) {
        this.parent = parent;
        this.pacman = pacman;
    }

    /**
     * Paints the Pacman sprite on the game board.
     *
     * @param g The graphics object to paint on
     */
    public void paintComponent(Graphics g) {
        Graphics2D g2 = (Graphics2D) g;

        String spriteName = ThemeManager.getInstance().getTheme().getPacmanSpriteName(this.pacman.getDirection());

        BufferedImage pacmanImage = null;
        try {
            pacmanImage = ImageIO.read(getClass().getResource(spriteName));
        } catch (IOException e) {
            e.printStackTrace();
        }

        Rectangle bounds = this.parent.getBounds();
        double w = bounds.getWidth();
        double h = bounds.getHeight();
        double diameter = Math.min(h, w) - 1.0;
        double x = (w - diameter) / 2.0;
        double y = (h - diameter) / 2.0;

        g2.drawImage(pacmanImage, (int) x, (int) y, (int) diameter, (int) diameter, null);

        g2.setColor(Color.black);
        g2.setFont(new Font("Serif", Font.BOLD, 20));
    }
}
