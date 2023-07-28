/**
 * GhostView is a class that represents a view of a ghost object.
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

public class GhostView implements ComponentView {
    private final FieldView parent;
    private final GhostObject ghost;

    public GhostView(FieldView parent, GhostObject ghost) {
        this.parent = parent;
        this.ghost = ghost;
    }

    /**
     * Paints the ghost component on the screen.
     *
     * @param g The graphics object to paint with
     */
    public void paintComponent(Graphics g) {
        Graphics2D g2 = (Graphics2D) g;

        String spriteName = this.ghost.isFrozen() ?
                ThemeManager.getInstance().getTheme().getGhostSpriteName(true) :
                ThemeManager.getInstance().getTheme().getGhostSpriteName(this.ghost.getDirection());

        BufferedImage ghostImage = null;
        try {
            ghostImage = ImageIO.read(getClass().getResource(spriteName));
        } catch (IOException e) {
            e.printStackTrace();
        }

        Rectangle bounds = this.parent.getBounds();
        double w = bounds.getWidth();
        double h = bounds.getHeight();
        double diameter = Math.min(h, w) - 10.0;
        double x = (w - diameter) / 2.0;
        double y = (h - diameter) / 2.0;

        g2.drawImage(ghostImage, (int) x, (int) y, (int) diameter, (int) diameter, null);
    }
}
