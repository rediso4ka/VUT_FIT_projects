/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.Event.responses;

import java.util.Date;

import com.project.actionsandevents.Event.EventLog;
import com.project.actionsandevents.Event.EventLogAction;

import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public class EventLogResponse {
    private Date date;
    private String text;
    private EventLogAction action;

    public EventLogResponse(EventLog eventLog) {
        this.date = eventLog.getDate();
        this.text = eventLog.getText();
        this.action = eventLog.getAction();
    }
}
