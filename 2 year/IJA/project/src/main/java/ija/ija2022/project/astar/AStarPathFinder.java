/**
 * AStar path finding algorithm implementation.
 * This class is used to find the shortest path between two points in a maze.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.astar;

import ija.ija2022.project.fields.BaseField;
import ija.ija2022.project.fields.CommonField;
import ija.ija2022.project.fields.WallField;
import ija.ija2022.project.maze.CommonMaze;

import java.util.*;

/**
 * A* Path Finder implementation to find the shortest path in a maze from a starting point to an ending point.
 * Uses a heuristic function to estimate the remaining distance from each point to the goal and a priority queue to
 * determine the next node to explore based on the f value (g + h) of each node. Nodes that have already been explored
 * are added to the closed set to avoid revisiting them.
 */
public class AStarPathFinder {
    private final CommonMaze maze;
    private final int[][] heuristic;
    private final PriorityQueue<Node> openSet;
    private final Set<Node> closedSet;

    public AStarPathFinder(CommonMaze maze) {
        this.maze = maze;
        this.heuristic = new int[maze.numRows()][maze.numCols()];
        this.openSet = new PriorityQueue<>(Comparator.comparingInt(Node::getF));
        this.closedSet = new HashSet<>();
    }

    /**
    * Finds path from start to end. This method is thread safe. Use only when there is no need to lock maze
    *
    * @param startX - x coordinate of start field
    * @param startY - y coordinate of start field ( must be greater than 0 )
    * @param endX - x coordinate of end field ( must be greater than 0 )
    * @param endY - y coordinate of end field ( must be greater than 0
    */
    public List<int[]> findPath(int startX, int startY, int endX, int endY) {
        // Computes the heuristic of the heuristic.
        for (int x = 0; x < heuristic.length; x++) {
            // Set the heuristic to the x y value of the heuristic.
            for (int y = 0; y < heuristic[0].length; y++) {
                heuristic[x][y] = Math.abs(x - endX) + Math.abs(y - endY);
            }
        }
        Node startNode = new Node((BaseField) maze.getField(startX, startY), null, 0, heuristic[startX][startY], null);
        openSet.offer(startNode);

        // Returns the path of the open set.
        while (!openSet.isEmpty()) {
            Node current = openSet.poll();
            // Returns a list of integers in the form of row col.
            if (current.getField().equals(maze.getField(endX, endY))) {
                List<int[]> path = new ArrayList<>();
                // Add a new path to the path.
                while (current.getParent() != null) {
                    path.add(new int[]{current.getField().getRow(), current.getField().getCol()});
                    current = current.getParent();
                }
                path.add(new int[]{current.getField().getRow(), current.getField().getCol()});
                Collections.reverse(path);
                return path;
            }
            for (CommonField.Direction direction : CommonField.Direction.values()) {
                BaseField neighbourField = (BaseField) maze.getField(current.getField().getRow(), current.getField().getCol()).nextField(direction);
                int g = current.getG() + 1;
                int h = heuristic[neighbourField.getRow()][neighbourField.getCol()];
                Node neighbourNode = new Node(neighbourField, current, g, h, direction);
                // offer the node to the open set
                if (!(neighbourField instanceof WallField) && !closedSet.contains(neighbourNode)) {
                    openSet.offer(neighbourNode);
                }
            }
            closedSet.add(current);
        }
        return Collections.emptyList();
    }
}
