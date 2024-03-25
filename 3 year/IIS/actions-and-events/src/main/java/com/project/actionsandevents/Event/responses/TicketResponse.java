/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.Event.responses;

import com.project.actionsandevents.Event.TicketType;

import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
public class TicketResponse {
    private Long id;
    private String name;
    private Long capacity;
    private String description;
    // Pass event by id to avoid infinite recursion when sending response
    private Long eventId;
    private Float price;
    
    public TicketResponse(TicketType ticketType) {
        this.id = ticketType.getId();
        this.name = ticketType.getName();
        this.capacity = ticketType.getCapacity();
        this.description = ticketType.getDescription();
        this.price = ticketType.getPrice();
        this.eventId = ticketType.getEvent().getId();
    }
}
