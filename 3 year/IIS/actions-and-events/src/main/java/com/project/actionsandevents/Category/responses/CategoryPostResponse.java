/**
 * @author Aleksandr Shevchenko (xshevc01)
 */
package com.project.actionsandevents.Category.responses;

import com.project.actionsandevents.common.ResponseMessage;

import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
public class CategoryPostResponse {
    private Long categoryId;
    private ResponseMessage message;

    public CategoryPostResponse(Long id, String message, ResponseMessage.Status status) {
        this.categoryId = id;
        this.message = new ResponseMessage(message, status);
    }
}
