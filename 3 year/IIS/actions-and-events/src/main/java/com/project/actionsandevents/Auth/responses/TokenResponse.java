/**
 * @author Oleksandr Turytsia (xturyt00)
 */
package com.project.actionsandevents.Auth.responses;

import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
public class TokenResponse {
    private String token;

    public TokenResponse(String token) {
        this.token = token;
    }
}
