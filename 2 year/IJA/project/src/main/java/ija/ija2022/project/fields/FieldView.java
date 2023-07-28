/**
 * FieldView class represents view of field.
 * It renders field and all objects on it based on model.
 * It uses observer pattern to get notified about changes in model.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.fields;

import ija.ija2022.project.common.ComponentView;
import ija.ija2022.project.common.Observable;
import ija.ija2022.project.objects.*;
import ija.ija2022.project.ui.controllers.PathMouseController;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.List;

public class FieldView extends JPanel implements Observable.Observer {
    private final CommonField field;
    private final List<ComponentView> objects;
    private int changedModel = 0;

    public FieldView(CommonField field) {
        this.field = field;
        this.objects = new ArrayList<>();
        this.privUpdate();
        field.addObserver(this);
        this.addMouseListener(PathMouseController.getInstance());
    }

    /**
    * Paints the component.
    * 
    * @param g - The graphics context to paint
    */
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        // Paints all components of the game.
        for (int i = 0; i < this.objects.size(); i++)
            this.objects.get(i).paintComponent(g);
    }

    /**
    * Updates the view based on the state
    */
    private void privUpdate() {
        // Creates the view objects for this field.
        if (this.field instanceof WallField) {
            this.objects.add(new WallView(this));
        } else {
            this.objects.clear();

            // Add a floor view to the view.
            if (this.field.isPacmanPath())
                this.setBackground(Color.green.brighter());
            else
                this.objects.add(new FloorView(this));

            ArrayList<CommonMazeObject> objects = this.field.get();

            objects.forEach(o -> {
                ComponentView v = null;
                // Create a new TargetView object.
                if (o instanceof TargetObject)
                    v = new TargetView(this, (TargetObject) o);
                if (o instanceof PacmanObject)
                    v = new PacmanView(this, (PacmanObject) o);
                else if (o instanceof HeartObject)
                    v = new HeartView(this);
                else if (o instanceof KeyObject)
                    v = new KeyView(this);
                else if (o instanceof ClockObject)
                    v = new ClockView(this);
                else if (o instanceof GhostObject)
                    v = new GhostView(this, (GhostObject) o);

                // Add a new object to the list of objects.
                if (v != null)
                    this.objects.add(v);
            });
        }

        updateUI();
    }

    /**
    * Called when the model changes.
    * 
    * @param observable - The observable that notified
    */
    public final void update(Observable observable) {
        ++this.changedModel;
        this.privUpdate();
    }

    /**
    * Returns the number of updates
    */
    public int numberUpdates() {
        return this.changedModel;
    }

    /**
    * Clears the changed flag.
    */
    public void clearChanged() {
        this.changedModel = 0;
    }

    /**
    * Returns the CommonField that this view is associated with.
    * 
    * 
    * @return the CommonField that this view is associated with
    */
    public CommonField getField() {
        return this.field;
    }
}
