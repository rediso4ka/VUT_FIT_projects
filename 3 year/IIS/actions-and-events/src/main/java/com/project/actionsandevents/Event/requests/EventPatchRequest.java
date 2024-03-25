/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.Event.requests;

import java.util.Date;

import com.project.actionsandevents.Event.EventStatus;

import jakarta.persistence.Temporal;
import jakarta.persistence.TemporalType;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
public class EventPatchRequest {
    @NotNull
    @Temporal(TemporalType.TIMESTAMP)
    private Date dateFrom;

    @Temporal(TemporalType.TIMESTAMP)
    private Date dateTo;

    private String description;

    @NotBlank(message = "Title is required")
    private String title;

    private String icon;

    private String image;

    private EventStatus status;
}
