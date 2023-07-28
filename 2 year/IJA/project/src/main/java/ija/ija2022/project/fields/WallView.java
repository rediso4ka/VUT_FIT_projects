/**
 * WallView is used to render a wall field.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.fields;

import ija.ija2022.project.common.ComponentView;
import ija.ija2022.project.theming.ThemeManager;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.IOException;

public class WallView implements ComponentView {
    private final FieldView parent;

    public WallView(FieldView parent) {
        this.parent = parent;
    }

    /**
    * Paints the wall sprite.
    * 
    * @param g - The graphics to paint
    */
    public void paintComponent(Graphics g) {
        Graphics2D g2 = (Graphics2D) g;

        String spriteName = ThemeManager.getInstance().getTheme().getWallSpriteName();

        BufferedImage wallImage = null;
        try {
            wallImage = ImageIO.read(getClass().getResource(spriteName));
        } catch (IOException e) {
            e.printStackTrace();
        }

        Rectangle bounds = this.parent.getBounds();
        double w = bounds.getWidth();
        double h = bounds.getHeight();
        double diameter = Math.max(h, w);
        double x = (w - diameter) / 2.0;
        double y = (h - diameter) / 2.0;

        g2.drawImage(wallImage, (int) x, (int) y, (int) diameter, (int) diameter, null);
    }
}
