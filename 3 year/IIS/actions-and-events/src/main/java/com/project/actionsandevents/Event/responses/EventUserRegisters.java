/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.Event.responses;

import lombok.Getter;
import lombok.Setter;

import com.project.actionsandevents.Event.RegistersStatus;
import com.project.actionsandevents.User.User;

@Setter
@Getter
public class EventUserRegisters {
    private User user;
    private RegistersStatus status;

    public EventUserRegisters(User user, RegistersStatus status) {
        this.user = user;
        this.status = status;
    }
}
