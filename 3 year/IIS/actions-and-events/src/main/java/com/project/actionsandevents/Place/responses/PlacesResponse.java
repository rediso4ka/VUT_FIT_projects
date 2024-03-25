/**
 * @author Aleksandr Shevchenko (xshevc01)
 */
package com.project.actionsandevents.Place.responses;

import lombok.Getter;
import lombok.Setter;

import java.util.List;

@Setter
@Getter
public class PlacesResponse {
    private List<Long> places;

    public PlacesResponse(List<Long> ids) {
        this.places = ids;
    }
}
