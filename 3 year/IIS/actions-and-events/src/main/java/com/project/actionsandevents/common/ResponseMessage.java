/**
 * @author Oleksandr Turytsia (xturyt00)
 */
package com.project.actionsandevents.common;

import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public class ResponseMessage {
    private String message;
    private String status;

    static public enum Status {
        SUCCESS("success"),
        WARNING("warning"),
        ERROR("error");

        private String description;

        Status(String description) {
            this.description = description;
        }

        public String getDescription() {
            return description;
        }
    }

    public ResponseMessage(String message, Status status) {
        this.message = message;
        this.status = status.getDescription();
    }
}
