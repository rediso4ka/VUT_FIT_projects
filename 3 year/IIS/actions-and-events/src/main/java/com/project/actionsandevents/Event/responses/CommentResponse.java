/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.Event.responses;

import java.util.Date;

import com.project.actionsandevents.Event.Comment;
import com.project.actionsandevents.User.responses.UserResponse;

import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
public class CommentResponse {
    private Long id;
    private UserResponse user;
    private Date date;
    private int rating;
    private String text;

    public CommentResponse(Comment comments) {
        this.id = comments.getId();
        this.user = new UserResponse(comments.getUser());
        this.date = comments.getDate();
        this.rating = comments.getRating();
        this.text = comments.getText();
    }
}
