/**
 * LogEntry record represents a single entry in the log of the game.
 * It contains a list of LogItem objects.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.logger;

import ija.ija2022.project.maze.configure.CHARACTER_MAP;
import org.json.JSONArray;

import java.util.ArrayList;

public record LogEntry(ArrayList<LogItem> items) {
    public LogEntry {
        // Throws an IllegalArgumentException if the items is null.
        if (items == null) {
            throw new IllegalArgumentException("Items cannot be null");
        }
    }

    /**
    * Returns a JSON array representation of the LogItems.
    * 
    * 
    * @return A JSON array representation of the Log
    */
    public JSONArray toJSONArray() {
        JSONArray array = new JSONArray();

        for (LogItem item : this.items) {
            array.put(item.toString());
        }

        return array;
    }

    /**
    * Returns all ghosts in the log.
    */
    public ArrayList<LogItem> getGhosts() {
        return this.items.stream()
                .filter(item -> item.character() == CHARACTER_MAP.GHOST)
                .collect(ArrayList::new, ArrayList::add, ArrayList::addAll);
    }

    /**
    * Creates a LogEntry from a JSONArray
    * 
    * @param array - The array to convert to a LogEntry
    * 
    * @return A LogEntry created from the
    */
    public static LogEntry fromJSONArray(JSONArray array) {
        ArrayList<LogItem> items = new ArrayList<>(array.length());

        // Add log items to the log items.
        for (int i = 0; i < array.length(); i++) {
            String string = array.getString(i);
            items.add(LogItem.fromString(string));
        }

        return new LogEntry(items);
    }
}
