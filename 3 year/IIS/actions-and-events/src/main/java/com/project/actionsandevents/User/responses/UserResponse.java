/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.User.responses;

import com.project.actionsandevents.User.User;

import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
public class UserResponse {
    private Long id;
    private String email;
    private String login;
    private String firstname;
    private String lastname;
    private String phone;
    private String role;

    public UserResponse(User user) {
        this.id = user.getId();
        this.email = user.getEmail();
        this.login = user.getLogin();
        this.firstname = user.getFirstname();
        this.lastname = user.getLastname();
        this.phone = user.getPhone();
        this.role = user.getRoles();
    }
}
