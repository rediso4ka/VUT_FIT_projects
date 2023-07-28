/**
 * LogItem record represent a single change of object position in the game.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.logger;

import ija.ija2022.project.fields.CommonField;
import ija.ija2022.project.maze.configure.CHARACTER_MAP;
import ija.ija2022.project.utils.Pair;

import java.util.Arrays;

public record LogItem(CHARACTER_MAP character, Pair<Integer, Integer> from, Pair<Integer, Integer> to) {
    public LogItem {
        // Throws an IllegalArgumentException if the character is null.
        if (character == null) {
            throw new IllegalArgumentException("Character cannot be null");
        }
        // thrown if from is null.
        if (from == null) {
            throw new IllegalArgumentException("From cannot be null");
        }
        // Throws an IllegalArgumentException if the to parameter is null.
        if (to == null) {
            throw new IllegalArgumentException("To cannot be null");
        }
    }

    /**
    * Returns a string representation of this LogItem.
    * 
    * 
    * @return a string representation of this
    */
    @Override
    public String toString() {
        return String.format("%s:(%d,%d)-(%d,%d)", this.character.getCharacter(), this.from.getKey(), this.from.getValue(), this.to.getKey(), this.to.getValue());
    }

    /**
    * Returns the direction of the item
    * 
    * @param reverse - true for reverse false for
    */
    public CommonField.Direction direction(boolean reverse) {
        return CommonField.Direction.from(
                this.to(reverse).getKey() - this.from(reverse).getKey(),
                this.to(reverse).getValue() - this.from(reverse).getValue()
        );
    }

    /**
    * Returns the direction of the item
    */
    public CommonField.Direction direction() {
        return this.direction(false);
    }

    /**
    * Get the start and end indices
    * 
    * @param reverse - Whether to reverse the indices
    */
    public Pair<Integer, Integer> from(boolean reverse) {
        return reverse ? this.to : this.from;
    }

    /**
    * Returns the start and end values
    */
    @Override
    public Pair<Integer, Integer> from() {
        return this.from(false);
    }

    /**
    * Get the end position.
    * 
    * @param reverse - True to reverse the end position
    */
    public Pair<Integer, Integer> to(boolean reverse) {
        return reverse ? this.from : this.to;
    }

    /**
    * Get the end position.
    */
    @Override
    public Pair<Integer, Integer> to() {
        return this.to(false);
    }

    /**
    * Creates a LogItem from a string
    * 
    * @param string - the string to parse.
    * 
    * @return the newly created LogItem
    */
    public static LogItem fromString(String string) {
        String[] parts = string.split(":");

        if (parts.length != 2) {
            throw new IllegalArgumentException("Invalid string format");
        }

        String[] coordinatesPairs = parts[1].split("-");

        if (coordinatesPairs.length != 2) {
            throw new IllegalArgumentException("Invalid string format");
        }

        Integer[] fromCoordinates = Arrays.stream(coordinatesPairs[0].replaceAll("[()]", "").split(","))
                .map(Integer::parseInt)
                .toArray(Integer[]::new);

        Integer[] toCoordinates = Arrays.stream(coordinatesPairs[1].replaceAll("[()]", "").split(","))
                .map(Integer::parseInt)
                .toArray(Integer[]::new);

        return new LogItem(
                CHARACTER_MAP.from(parts[0].charAt(0)),
                new Pair<>(fromCoordinates[0], fromCoordinates[1]),
                new Pair<>(toCoordinates[0], toCoordinates[1])
        );
    }
}