/**
 * KeyView class represents a view of a key object.
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

public class KeyView implements ComponentView {
    private final FieldView parent;

    public KeyView(FieldView parent) {
        this.parent = parent;
    }

    /**
     * Paints the component with the key image in the center.
     *
     * @param g The graphics object to paint with
     */
    public void paintComponent(Graphics g) {
        Graphics2D g2 = (Graphics2D) g;

        String spriteName = ThemeManager.getInstance().getTheme().getKeySpriteName();

        BufferedImage keyImage = null;
        try {
            keyImage = ImageIO.read(getClass().getResource(spriteName));
        } catch (IOException e) {
            e.printStackTrace();
        }

        Rectangle bounds = this.parent.getBounds();
        double w = bounds.getWidth();
        double h = bounds.getHeight();
        double diameter = Math.min(h, w) - 10.0;
        double x = (w - diameter) / 2.0;
        double y = (h - diameter);

        g2.drawImage(keyImage, (int) x, (int) y, (int) diameter, (int) diameter / 2, null);
    }
}
