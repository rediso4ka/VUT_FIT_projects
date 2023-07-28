/**
 * PathField class represents a field that is a part of the path.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.fields;

import ija.ija2022.project.objects.TargetObject;

import java.util.List;

public class PathField extends BaseField implements CommonField {
    public PathField(int row, int col) {
        super(row, col);
    }

    /**
    * Returns true if pacman can move to this field
    */
    @Override
    public boolean canMove() {
        boolean containsClosedTarget = this.get().stream()
                .anyMatch(o -> o instanceof TargetObject && !this.commonMaze.isAllKeysCollected());

        return !containsClosedTarget;
    }

    /**
    * Returns true if the field is a part of pacman's path
    */
    @Override
    public boolean isPacmanPath() {
        List<int[]> path = this.commonMaze.getPacmanPath();

        return path.stream().anyMatch(p -> p[0] == this.row && p[1] == this.col);
    }
}
