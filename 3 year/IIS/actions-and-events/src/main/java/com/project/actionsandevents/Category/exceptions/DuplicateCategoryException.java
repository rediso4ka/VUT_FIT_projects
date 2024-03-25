/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.Category.exceptions;

public class DuplicateCategoryException extends RuntimeException {
    public DuplicateCategoryException(String message) {
        super(message);
    }
}
