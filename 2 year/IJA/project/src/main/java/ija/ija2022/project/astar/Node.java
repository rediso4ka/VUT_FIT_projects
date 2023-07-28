/**
 * AStar path finding node implementation.
 * This class is used to represent a node in the AStar algorithm.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.astar;

import ija.ija2022.project.fields.BaseField;
import ija.ija2022.project.fields.CommonField;

/**
 * AStar path finding node implementation.
 * This class is used to represent a node in the AStar algorithm.
 */
public class Node {
    private final BaseField field;
    private final Node parent;
    private final int g;
    private final int h;
    private final int f;
    private final CommonField.Direction direction;

    /**
     * Constructor for creating a new Node object.
     *
     * @param field the field associated with this node
     * @param parent the parent node of this node
     * @param g the cost of the path from the start node to this node
     * @param h the heuristic cost from this node to the goal node
     * @param direction the direction from the parent node to this node
     */
    public Node(BaseField field, Node parent, int g, int h, CommonField.Direction direction) {
        this.field = field;
        this.parent = parent;
        this.g = g;
        this.h = h;
        this.f = g + h;
        this.direction = direction;
    }

    /**
    * Returns true if this Node is equal to the specified Node.
    *
    * @param o - the Node to compare this Node to for equality
    */
    @Override
    public boolean equals(Object o) {
        // Returns true if this object is the same as the receiver.
        if (this == o) return true;
        // Returns true if this object is a subclass of the same class.
        if (o == null || getClass() != o.getClass()) return false;
        Node node = (Node) o;
        return field == node.field;
    }

    /**
    * Returns hash code for this field. This is equivalent to Field#hashCode () except that it does not depend on the type
    */
    @Override
    public int hashCode() {
        return field.hashCode();
    }

    /**
    * Returns the f value of the vector.
    */
    public int getF() {
        return f;
    }

    /**
    * Returns the value of the g property.
    */
    public int getG() {
        return g;
    }

    /**
    * Returns the field that this event is associated with.
    *
    *
    * @return The field that this event is associated with.
    */
    public BaseField getField() {
        return field;
    }

    /**
    * Returns the parent of this node.
    *
    *
    * @return the parent of this node
    */
    public Node getParent() {
        return parent;
    }

    /**
    * Returns the direction of this field.
    */
    public CommonField.Direction getDirection() {
        return direction;
    }
}
