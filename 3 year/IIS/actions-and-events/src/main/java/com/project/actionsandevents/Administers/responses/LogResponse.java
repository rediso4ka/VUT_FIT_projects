/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.Administers.responses;

import com.project.actionsandevents.Administers.Administers;
import com.project.actionsandevents.User.responses.UserResponse;

import lombok.Getter;
import lombok.Setter;

import java.util.Date;

@Setter
@Getter
public class LogResponse {
    private UserResponse admin;
    private String userLogin;
    private Date date;
    private String text;

    public LogResponse(Administers administers) {
        this.admin = new UserResponse(administers.getAdmin());
        this.userLogin = administers.getUserLogin();
        this.date = administers.getDate();
        this.text = administers.getText();
    }
}
