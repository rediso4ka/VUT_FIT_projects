/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.Event.responses;

import lombok.Getter;
import lombok.Setter;

import java.util.List;

@Setter
@Getter
public class UsersRegisteredToEventResponse {
    private List<EventUserRegisters> userRegisters;


    public UsersRegisteredToEventResponse(List<EventUserRegisters> userRegisters) {
        this.userRegisters = userRegisters;
    }
}
