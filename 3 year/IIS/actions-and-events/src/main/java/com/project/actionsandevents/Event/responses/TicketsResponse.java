/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.Event.responses;

import java.util.List;

import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
public class TicketsResponse {
    private List<Long> tickets;

    public TicketsResponse(List<Long> ids) {
        this.tickets = ids;
    }
}
