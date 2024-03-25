/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.Event.responses;

import java.util.List;

import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
public class CommentsResponse {
    private List<Long> comments;

    public CommentsResponse(List<Long> ids) {
        this.comments = ids;
    }
}
