/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.Event.responses;

import java.util.List;

import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
public class EventLogsResponse {
    private List<Long> logs;

    public EventLogsResponse(List<Long> logs) {
        this.logs = logs;
    }
}
