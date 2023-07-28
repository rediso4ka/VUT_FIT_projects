/**
 * CHARACTER_MAP enumeration represents all possible characters in the maze file
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.maze.configure;

public enum CHARACTER_MAP {
    PATH('.'),
    WALL('X'),
    PACMAN('S'),
    GHOST('G'),
    HEART('H'),
    KEY('K'),
    CLOCK('F'),
    TARGET('T');

    private final char character;

    CHARACTER_MAP(char character) {
        this.character = character;
    }

    /**
    * Returns the character associated with this key
    */
    public char getCharacter() {
        return this.character;
    }

    /**
    * Returns the character map that corresponds to the character
    * 
    * @param character - the character to look up
    * 
    * @return the character map that corresponds to
    */
    public static CHARACTER_MAP from(Character character) {
        for (CHARACTER_MAP value : CHARACTER_MAP.values()) {
            // Returns the value of the character.
            if (value.getCharacter() == character) {
                return value;
            }
        }

        throw new IllegalArgumentException("Invalid character");
    }
}
