package com.project.actionsandevents.Event.exceptions;
/**
 * @author Vadim Goncearenco (xgonce00)
 */
public class DuplicateRegistrationException extends RuntimeException {
    public DuplicateRegistrationException(String message) {
        super(message);
    }
}
