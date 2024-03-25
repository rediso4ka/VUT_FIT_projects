/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.Event.exceptions;

public class DuplicateEventException extends RuntimeException {
    public DuplicateEventException(String message) {
        super(message);
    }
}
