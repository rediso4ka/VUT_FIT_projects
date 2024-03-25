/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.Place;

import com.fasterxml.jackson.annotation.JsonValue;

public enum PlaceStatus {
    PENDING("Pending"),
    ACCEPTED("Accepted"),
    REJECTED("Rejected");

    private final String status;

    PlaceStatus(String status) {
        this.status = status;
    }

    @JsonValue
    public String getStatus() {
        return status;
    }
}
