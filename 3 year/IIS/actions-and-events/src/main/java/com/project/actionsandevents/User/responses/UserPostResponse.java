/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.User.responses;

import com.project.actionsandevents.common.ResponseMessage;

import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
public class UserPostResponse {
    private Long userId;
    private ResponseMessage message;

    public UserPostResponse(Long id, String message, ResponseMessage.Status status) {
        this.userId = id;
        this.message = new ResponseMessage(message, status);
    }
}
