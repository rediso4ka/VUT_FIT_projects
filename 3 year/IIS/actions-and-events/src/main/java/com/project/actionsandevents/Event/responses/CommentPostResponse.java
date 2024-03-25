/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.Event.responses;

import com.project.actionsandevents.common.ResponseMessage;

import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
public class CommentPostResponse {
    private Long id;
    private ResponseMessage message;

    public CommentPostResponse(Long id, ResponseMessage responseMessage) {
        this.id = id;
        this.message = responseMessage;
    }
}
