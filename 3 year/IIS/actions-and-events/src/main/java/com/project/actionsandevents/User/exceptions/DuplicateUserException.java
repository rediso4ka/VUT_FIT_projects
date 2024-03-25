/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.User.exceptions;

public class DuplicateUserException extends RuntimeException {
    public DuplicateUserException(String message) {
        super(message);
    }
}
