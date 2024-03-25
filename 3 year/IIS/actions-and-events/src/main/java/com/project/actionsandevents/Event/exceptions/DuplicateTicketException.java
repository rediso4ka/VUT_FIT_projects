/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.Event.exceptions;

public class DuplicateTicketException extends RuntimeException {
    public DuplicateTicketException(String message) {
        super(message);
    }
}
