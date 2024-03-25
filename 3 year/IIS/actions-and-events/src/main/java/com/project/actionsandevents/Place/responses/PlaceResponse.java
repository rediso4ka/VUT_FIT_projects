/**
 * @author Aleksandr Shevchenko (xshevc01)
 */
package com.project.actionsandevents.Place.responses;

import com.project.actionsandevents.Place.Place;
import com.project.actionsandevents.Place.PlaceStatus;

import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
public class PlaceResponse {
    private Long id;
    private String name;
    private String image;
    private String description;
    private String address;
    private PlaceStatus status;

    public PlaceResponse(Place place) {
        this.id = place.getId();
        this.name = place.getName();
        this.image = place.getImage();
        this.description = place.getDescription();
        this.address = place.getAddress();
        this.status = place.getStatus();
    }
}
