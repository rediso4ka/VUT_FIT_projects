/**
 * @author Aleksandr Shevchenko (xshevc01)
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.Category;

import com.fasterxml.jackson.annotation.JsonValue;

public enum CategoryStatus {
    PENDING("Pending"),
    ACCEPTED("Accepted"),
    REJECTED("Rejected");

    private final String status;

    CategoryStatus(String status) {
        this.status = status;
    }

    @JsonValue
    public String getStatus() {
        return status;
    }
}
