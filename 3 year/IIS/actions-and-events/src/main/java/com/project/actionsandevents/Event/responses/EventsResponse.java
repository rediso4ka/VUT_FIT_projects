/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.Event.responses;

import lombok.Getter;
import lombok.Setter;

import java.util.List;

@Setter
@Getter
public class EventsResponse {
    private List<Long> events;

    public EventsResponse(List<Long> ids) {
        this.events = ids;
    }
}
