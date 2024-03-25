/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.Event.responses;

import com.project.actionsandevents.common.ResponseMessage;

import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
public class EventPostResponse {
    private Long eventId;
    private ResponseMessage message;
    
    public EventPostResponse(Long id, String message, ResponseMessage.Status status) {
        this.eventId = id;
        this.message = new ResponseMessage(message, status);
    }
}
