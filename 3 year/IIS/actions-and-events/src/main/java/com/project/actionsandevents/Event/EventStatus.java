/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.Event;

import com.fasterxml.jackson.annotation.JsonValue;

public enum EventStatus {
    PENDING("Pending"),
    ACCEPTED("Accepted"),
    REJECTED("Rejected");

    private final String status;

    EventStatus(String status) {
        this.status = status;
    }

    @JsonValue
    public String getStatus() {
        return status;
    }
}
