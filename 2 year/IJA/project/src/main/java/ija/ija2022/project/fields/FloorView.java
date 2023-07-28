/**
 * FloorView class represents a view of a floor field.
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

public class FloorView implements ComponentView {
    private final FieldView parent;

    public FloorView(FieldView parent) {
        this.parent = parent;
    }

    /**
    * Paints the Floor Sprite.
    * 
    * @param g - The Graphics to paint
    */
    public void paintComponent(Graphics g) {
        Graphics2D g2 = (Graphics2D) g;

        String spriteName = ThemeManager.getInstance().getTheme().getFloorSpriteName();

        BufferedImage floorImage = null;
        try {
            floorImage = ImageIO.read(getClass().getResource(spriteName));
        } catch (IOException e) {
            e.printStackTrace();
        }

        Rectangle bounds = this.parent.getBounds();
        double w = bounds.getWidth();
        double h = bounds.getHeight();
        double diameter = Math.min(h, w);
        double x = (w - diameter) / w;
        double y = (h - diameter) / h;

        g2.drawImage(floorImage, (int) x, (int) y, (int) diameter, (int) diameter, null);
    }
}
