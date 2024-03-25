/**
 * @author Aleksandr Shevchenko (xshevc01)
 */
package com.project.actionsandevents.Place.responses;

import com.project.actionsandevents.common.ResponseMessage;

import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
public class PlacePostResponse {
    private Long placeId;
    private ResponseMessage message;

    public PlacePostResponse(Long id, String message, ResponseMessage.Status status) {
        this.placeId = id;
        this.message = new ResponseMessage(message, status);
    }
}
