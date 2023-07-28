/**
 * MazePresenter class represents a graphical interface of the maze.
 * It renders the maze and its objects.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.maze;

import ija.ija2022.project.fields.FieldView;

import javax.swing.*;
import java.awt.*;

public class MazePresenter extends JPanel {
    private final CommonMaze maze;

    public MazePresenter(CommonMaze maze) {
        this.maze = maze;
        this.setLayout(new GridLayout(this.maze.numRows(), this.maze.numCols()));
        this.setSize(350, 400);
        this.setBackground(Color.gray);

        this.initializeInterface();
    }

    /**
    * Initializes the graphical interface of the maze.
    */
    private void initializeInterface() {
        // Add all the fields in the maze to the view.
        for (int i = 0; i < this.maze.numRows(); ++i) {
            // Add all fields in the maze to the view.
            for (int j = 0; j < this.maze.numCols(); ++j) {
                FieldView field = new FieldView(this.maze.getField(i, j));
                this.add(field);
            }
        }
    }

    /**
    * Returns the preferred size of this maze.
    * 
    * 
    * @return the preferred size of this maz
    */
    @Override
    public Dimension getPreferredSize() {
        Rectangle bounds = this.getParent().getBounds();

        // Returns the preferred size of the bounds.
        if (bounds == null)
            return super.getPreferredSize();

        int squareSize = Math.min(bounds.width / this.maze.numCols(), bounds.height / this.maze.numRows());
        int width = squareSize * this.maze.numCols();
        int height = squareSize * this.maze.numRows();

        return new Dimension(width, height);
    }
}
