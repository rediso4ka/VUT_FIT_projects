/**
 * Utility class for storing pairs of objects.
 *
 * @author xmoise01, Nikita Moiseev
 * @author xshevc01, Aleksandr Shevchenko
 */
package ija.ija2022.project.utils;

import java.io.Serializable;

/**
 * A simple key-value pair implementation that is serializable.
 *
 * @param <K> The type of the key.
 * @param <V> The type of the value.
 */
public class Pair<K, V> implements Serializable {
    private K key;
    private V value;

    public K getKey() {
        return this.key;
    }

    public V getValue() {
        return this.value;
    }

    public Pair(K var1, V var2) {
        this.key = var1;
        this.value = var2;
    }

    /**
     * Returns a string representation of the key-value pair.
     *
     * @return A string representation of the key-value pair in the format "key=value".
     */
    public String toString() {
        String str = String.valueOf(this.key);
        return str + "=" + String.valueOf(this.value);
    }

    /**
     * Computes the hash code for this key-value pair.
     *
     * @return The hash code for this key-value pair.
     */
    public int hashCode() {
        int code = 7;
        code = 31 * code + (this.key != null ? this.key.hashCode() : 0);
        code = 31 * code + (this.value != null ? this.value.hashCode() : 0);
        return code;
    }

    /**
     * Compares this Pair object to another object to check if they are equal.
     *
     * @param var1 The object to compare to this Pair object.
     * @return true if the objects are equal, false otherwise.
     */
    public boolean equals(Object var1) {
        if (this == var1) {
            return true;
        } else if (!(var1 instanceof Pair)) {
            return false;
        } else {
            Pair var2 = (Pair) var1;
            if (this.key != null) {
                if (!this.key.equals(var2.key)) {
                    return false;
                }
            } else if (var2.key != null) {
                return false;
            }

            if (this.value != null) {
                if (!this.value.equals(var2.value)) {
                    return false;
                }
            } else if (var2.value != null) {
                return false;
            }

            return true;
        }
    }
}