/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.Administers.responses;

import java.util.List;

import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
public class LogsResponse {
    private List<Long> logs;

    public LogsResponse(List<Long> ids) {
        this.logs = ids;
    }
}
