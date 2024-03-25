/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.User.requests;

import jakarta.validation.constraints.Email;
import jakarta.validation.constraints.NotBlank;

import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
public class UserPatchRequest {
    private String firstname;

    private String lastname;

    @NotBlank(message = "Email is required")
    @Email(message = "Invalid email format")
    private String email;

    private String phone;
}
