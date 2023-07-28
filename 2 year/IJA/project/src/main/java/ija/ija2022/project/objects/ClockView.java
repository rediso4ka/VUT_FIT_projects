/**
 * ClockView is a class that represents a view of a clock object.
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

public class ClockView implements ComponentView {
    private final FieldView parent;

    public ClockView(FieldView parent) {
        this.parent = parent;
    }

    /**
    * Paints the clock sprite.
    * 
    * @param g - The graphics to paint
    */
    public void paintComponent(Graphics g) {
        Graphics2D g2 = (Graphics2D) g;

        String spriteName = ThemeManager.getInstance().getTheme().getClockSpriteName();

        BufferedImage clockImage = null;
        try {
            clockImage = ImageIO.read(getClass().getResource(spriteName));
        } catch (IOException e) {
            e.printStackTrace();
        }

        Rectangle bounds = this.parent.getBounds();
        double w = bounds.getWidth();
        double h = bounds.getHeight();
        double diameter = Math.min(h, w) - 10.0;
        double x = (w - diameter);
        double y = (h - diameter) / 2.0;

        g2.drawImage(clockImage, (int) x, (int) y, (int) (diameter / 1.5), (int) diameter, null);
    }
}
