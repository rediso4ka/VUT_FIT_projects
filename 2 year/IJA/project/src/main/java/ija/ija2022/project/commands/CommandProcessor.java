/**
 * Command processor class is used to process commands from game history logs.
 * Commands have the following grammar:
 * ```text
 * <change> ::= <object>:<coords>-<coords>
 * <object> ::= "G" | "S"
 * <coords> ::= "(" <int> "," <int> ")"
 * <int> ::= <digit> | <digit> <int>
 * <digit> ::= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
 * ```
 * Where:
 * - `<change>` is a change of object position
 * - `<object>` is an object type
 * - `<coords>` is a pair of coordinates
 * - `<int>` is an integer number
 * - `<digit>` is a digit
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.commands;

import ija.ija2022.project.fields.CommonField;
import ija.ija2022.project.logger.LogEntry;
import ija.ija2022.project.logger.LogItem;
import ija.ija2022.project.logger.LoggerController;
import ija.ija2022.project.maze.CommonMaze;
import ija.ija2022.project.maze.configure.CHARACTER_MAP;
import ija.ija2022.project.objects.GhostObject;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;

public class CommandProcessor {
    private final CommonMaze maze;

    public CommandProcessor(CommonMaze maze, LoggerController loggerController) {
        this.maze = maze;
    }

    /**
     * Processes a log entry.
     *
     * @param entry - the entry to process
     */
    public void processEntry(LogEntry entry) {
        this.processEntry(entry, false);
    }

    /**
     * Processes a LogEntry and moves it.
     *
     * @param entry   - The LogEntry to process.
     * @param reverse - Whether or not the entry is reversed
     */
    public void processEntry(LogEntry entry, boolean reverse) {
        // Returns true if the entry is empty.
        if (entry == null || entry.items().size() == 0) return;

        // Move all entries in the entry to the next log item.
        for (int i = 0; i < entry.items().size(); i++) {
            LogItem item = entry.items().get(i);
            CommonField.Direction direction = reverse ? item.direction().opposite() : item.direction();

            // Move the item to the next position in the maze.
            if (item.character() == CHARACTER_MAP.PACMAN) {
                this.maze.getPacman().move(direction);
            } else if (item.character() == CHARACTER_MAP.GHOST) {
                boolean reviveGhost = (entry.getGhosts().size() > this.maze.ghosts().length) && reverse;

                Arrays.stream(this.maze.ghosts())
                        .filter(ghost -> ghost.getRow() == item.from(reverse).getKey() && ghost.getCol() == item.from(reverse).getValue())
                        .findFirst()
                        .ifPresentOrElse(ghost -> ghost.move(item.to(reverse).getKey(), item.to(reverse).getValue()), () -> {
                            if (reviveGhost) {
                                GhostObject ghost = new GhostObject(item.from().getKey(), item.from().getValue(), this.maze);
                                this.maze.putObject(ghost, item.from().getKey(), item.from().getValue());
                                this.maze.getPacman().incrLives();
                            }
                        });
            }
        }
    }

    /**
     * Processes a list of log entries.
     *
     * @param entries - the list of log entries to process
     * @param reverse - whether or not to reverse
     */
    public void processEntries(List<LogEntry> entries, boolean reverse) {
        // reverse the entries in reverse order.
        if (reverse)
            Collections.reverse(entries);

        for (LogEntry entry : entries) {
            this.processEntry(entry, reverse);
        }
    }
}
