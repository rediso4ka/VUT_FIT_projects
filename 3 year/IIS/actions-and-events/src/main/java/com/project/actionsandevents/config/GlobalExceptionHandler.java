/**
 * @author Oleksandr Turytsia (xturyt00)
 */
package com.project.actionsandevents.config;

import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.ControllerAdvice;
import org.springframework.web.bind.annotation.ExceptionHandler;

import com.project.actionsandevents.Administers.exceptions.AdministerLogNotFoundException;
import com.project.actionsandevents.User.exceptions.UserNotFoundException;
import com.project.actionsandevents.common.ResponseMessage;

@ControllerAdvice
public class GlobalExceptionHandler {
    @ExceptionHandler(UserNotFoundException.class)
    public ResponseEntity<Object> handleUserNotFoundException(UserNotFoundException ex) {
        return ResponseEntity.status(HttpStatus.NOT_FOUND)
                .body(new ResponseMessage(ex.getMessage(), ResponseMessage.Status.ERROR));
    }

    @ExceptionHandler(AdministerLogNotFoundException.class)
    public ResponseEntity<Object> handleUserNotFoundException(AdministerLogNotFoundException ex) {
        return ResponseEntity.status(HttpStatus.NOT_FOUND)
                .body(new ResponseMessage(ex.getMessage(), ResponseMessage.Status.ERROR));
    }
}
