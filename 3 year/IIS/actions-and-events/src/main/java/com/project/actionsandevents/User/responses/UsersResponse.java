/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.User.responses;

import lombok.Getter;
import lombok.Setter;

import java.util.List;

@Setter
@Getter
public class UsersResponse {
    private List<Long> users;

    public UsersResponse(List<Long> ids) {
        this.users = ids;
    }
}
