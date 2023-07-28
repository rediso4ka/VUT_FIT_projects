/**
 * LoggerController class is responsible for logging the game.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.logger;

import ija.ija2022.project.events.EventManager;
import ija.ija2022.project.events.events.UpdateReplayStepEvent;
import ija.ija2022.project.maze.configure.CHARACTER_MAP;
import ija.ija2022.project.objects.CommonMazeObject;
import ija.ija2022.project.objects.GhostObject;
import ija.ija2022.project.utils.Pair;
import org.json.JSONArray;
import org.json.JSONObject;
import org.json.JSONTokener;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class LoggerController {
    private final LOGGER_MODE mode;

    private String filePath;

    private ArrayList<LogEntry> entries = new ArrayList<>();

    private String mapText;

    private int index = 0;

    public LoggerController(LOGGER_MODE mode) {
        this.mode = mode;
        this.filePath = null;
    }

    public LoggerController(LOGGER_MODE mode, String filePath) {
        this.mode = mode;
        this.filePath = filePath;

        this.process();
    }

    /**
     * Process the log file and extract
     */
    private void process() {
        // If the logger is write mode then do not process the file.
        if (this.mode == LOGGER_MODE.WRITE) return;

        // If the file path is null throws an exception.
        if (this.filePath == null) {
            throw new IllegalStateException("Cannot process without file path");
        }


        FileInputStream is;
        try {
            is = new FileInputStream(this.filePath);
        } catch (FileNotFoundException e) {
            throw new RuntimeException(e);
        }

        JSONTokener tokener = new JSONTokener(is);
        JSONObject object = new JSONObject(tokener);
        JSONArray entries = object.getJSONArray("items");

        this.entries = new ArrayList<>(entries.length());

        // Add all entries in the list to the log entries.
        for (int i = 0; i < entries.length(); i++) {
            this.entries.add(LogEntry.fromJSONArray(entries.getJSONArray(i)));
        }

        this.mapText = object.getString("map");
    }

    /**
     * Adds an item to the log.
     *
     * @param item - the item to add
     */
    public void addItem(LogItem item) {
        // If the logger is in read mode then throw an exception.
        if (this.mode == LOGGER_MODE.READ) {
            throw new IllegalStateException("Cannot add item in read mode");
        }

        // Add a new entry to the list of entries.
        if (this.entries.size() == 0) {
            this.entries.add(new LogEntry(new ArrayList<>()));
        }

        // Add a new entry to the list of entries.
        if (this.entries.size() < this.index + 1) {
            this.entries.add(new LogEntry(new ArrayList<>()));
        }

        this.entries.get(this.index).items().add(item);
    }

    /**
     * Adds a LogItem to the maze
     *
     * @param object - CommonMazeObject to add to
     */
    public void addItem(CommonMazeObject object) {
        CHARACTER_MAP character = object instanceof GhostObject ? CHARACTER_MAP.GHOST : CHARACTER_MAP.PACMAN;

        Pair<Integer, Integer> from = new Pair<>(
                object.getRow() - object.getDirection().deltaRow(),
                object.getCol() - object.getDirection().deltaCol()
        );

        Pair<Integer, Integer> to = new Pair<>(object.getRow(), object.getCol());

        this.addItem(new LogItem(character, from, to));
    }

    /**
     * Moves to the next entry
     */
    public void nextEntry() {
        // Returns true if the index is less than the size of the entries array.
        if (this.index >= this.entries.size()) return;
        this.index++;
        EventManager.getInstance().fireEvent(new UpdateReplayStepEvent(this.index));
    }

    /**
     * Go to previous entry in the log
     */
    public void previousEntry() {
        if (this.index <= -1) return;
        this.index--;
        EventManager.getInstance().fireEvent(new UpdateReplayStepEvent(this.index));
    }

    /**
     * Returns the current entry.
     *
     * @return null if there are no
     */
    public LogEntry currentEntry() {
        // Returns the entry at the current position in the list.
        if (this.index >= this.entries.size() || this.index < 0)
            return null;

        return this.entries.get(this.index);
    }

    /**
     * Returns the LogEntry at the specified index.
     *
     * @param index - The index of the LogEntry to return.
     * @return The LogEntry at the specified index
     */
    public LogEntry getEntry(int index) {
        // Returns the entry at the given index or null if the index is out of range.
        if (index >= this.entries.size() || index < 0)
            return null;

        return this.entries.get(index);
    }

    /**
     * Returns the list of log entries
     */
    public List<LogEntry> getEntries() {
        return this.entries;
    }

    /**
     * Returns a sublist of the entries starting at the given index.
     *
     * @param start - index of the first
     */
    public List<LogEntry> getEntries(int start) {
        // Checks if the start index is within the list of entries.
        if (start < 0 || start > this.entries.size())
            throw new IllegalArgumentException("Invalid start index");

        return this.entries.subList(start, this.entries.size());
    }

    /**
     * Returns a sublist of the entries.
     *
     * @param start - the index of the first entry to return
     * @param end   - the index of the last entry to
     */
    public List<LogEntry> getEntries(int start, int end) {
        // Raises an exception if the start or end index is invalid.
        if (start < 0 || end > this.entries.size() || start > end)
            throw new IllegalArgumentException("Invalid start or end index");

        return this.entries.subList(start, end);
    }

    /**
     * Sets the index of the list.
     *
     * @param index - The index of the list
     */
    public void setIndex(int index) {
        this.index = index;
    }

    /**
     * Returns the index of the list.
     */
    public int getIndex() {
        return index;
    }

    /**
     * Returns the text that is used to generate the maze
     *
     * @return the text that is used to generate the maze
     */
    public String getMapText() {
        return mapText;
    }

    /**
     * Converts this Log to JSON.
     *
     * @return A JSON representation of this
     */
    public JSONObject toJSON() {
        JSONObject object = new JSONObject();

        object.put("items", new ArrayList<>());

        JSONArray[] entries = this.entries.stream().map(LogEntry::toJSONArray).toArray(JSONArray[]::new);
        for (JSONArray entry : entries) {
            object.getJSONArray("items").put(entry);
        }

        object.put("map", this.mapText);

        return object;
    }

    /**
     * Saves settings to file.
     */
    public void close() {
        // Close the log file if the logger is in read mode.
        if (this.mode == LOGGER_MODE.READ) {
            this.afterClose();
            return;
        }

        // Closes the file. If the file path is null throws an exception.
        if (this.filePath == null) {
            throw new IllegalStateException("Cannot close without file path");
        }

        FileOutputStream os = null;
        try {
            os = new FileOutputStream(this.filePath);
        } catch (FileNotFoundException e) {
            System.out.println("Settings file not found");
        }

        try {
            assert os != null;
            os.write(this.toJSON().toString().getBytes());
            os.close();
        } catch (IOException e) {
            System.out.println("Error while saving settings");
        }

        this.afterClose();
    }

    /**
     * Closes the reader and sets the file path and map text.
     *
     * @param filePath - The path to the file
     * @param mapText  - The map text to
     */
    public void close(String filePath, String mapText) {
        this.filePath = filePath;
        this.mapText = mapText;

        this.close();
    }

    /**
     * Clears the state after the file has been closed
     */
    public void afterClose() {
        this.filePath = null;
        this.mapText = null;
        this.entries.clear();
        this.entries = null;
        this.index = 0;
    }
}
