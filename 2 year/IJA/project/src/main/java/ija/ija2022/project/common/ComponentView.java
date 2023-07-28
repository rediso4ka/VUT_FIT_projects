/**
 * ComponentView interface is implemented in different views of the maze
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.common;

import java.awt.*;

public interface ComponentView {
    /**
    * Paints the component. 
    * 
    * @param g - the graphics context to use for painting.
    */
    void paintComponent(Graphics g);
}
