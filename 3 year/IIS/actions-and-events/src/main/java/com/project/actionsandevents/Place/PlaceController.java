/**
 * @author Aleksandr Shevchenko (xshevc01)
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.Place;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.security.access.prepost.PreAuthorize;
import org.springframework.security.core.Authentication;
import org.springframework.validation.BindingResult;

import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PatchMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import com.project.actionsandevents.Place.exceptions.PlaceNotFoundException;
import com.project.actionsandevents.Place.exceptions.DuplicatePlaceException;

import com.project.actionsandevents.Place.requests.PlacePatchRequest;

import com.project.actionsandevents.Place.responses.PlacePostResponse;
import com.project.actionsandevents.Place.responses.PlaceResponse;
import com.project.actionsandevents.Place.responses.PlacesResponse;
import com.project.actionsandevents.User.User;
import com.project.actionsandevents.User.UserInfoDetails;
import com.project.actionsandevents.User.UserService;
import com.project.actionsandevents.User.exceptions.UserNotFoundException;
import com.project.actionsandevents.common.ResponseMessage;

import jakarta.validation.Valid;

@RestController
@RequestMapping
public class PlaceController {
    @Autowired
    private PlaceService placeService;

    @Autowired
    private UserService userService;

    @GetMapping("/place/{id}")
    public ResponseEntity<Object> getPlaceById(@PathVariable Long id, Authentication authentication) throws PlaceNotFoundException {
        Place place = placeService.getPlaceById(id);
        
        return ResponseEntity.ok(new PlaceResponse(place)); 
    }

    @GetMapping("/places")
    public ResponseEntity<Object> getPlaceIds(Authentication authentication) {
        return ResponseEntity.ok(new PlacesResponse(placeService.getPlaceIds()));
    }

    @PatchMapping("/place/{id}")
    @PreAuthorize("hasAnyAuthority('ROLE_MANAGER', 'ROLE_ADMIN')")
    public ResponseEntity<Object> patchPlaceById(
                @PathVariable Long id,
                @Valid @RequestBody PlacePatchRequest patchRequest,
                BindingResult bindingResult,
                Authentication authentication) throws PlaceNotFoundException 
    {
        if (bindingResult.hasErrors()) {
            return ResponseEntity.badRequest().body(new ResponseMessage(
                        "Validation failed: " + bindingResult.getAllErrors(), ResponseMessage.Status.ERROR));
        }

        try {
            placeService.patchPlaceById(id, patchRequest);
        } catch (PlaceNotFoundException | DuplicatePlaceException ex) {
            return ResponseEntity.badRequest().body(new ResponseMessage(
                        ex.getMessage(), ResponseMessage.Status.ERROR));
        }

        return ResponseEntity.ok(new ResponseMessage("Place was successfully updated", ResponseMessage.Status.SUCCESS));
    }

    @PostMapping("/place")
    @PreAuthorize("hasAnyAuthority('ROLE_USER', 'ROLE_MANAGER', 'ROLE_ADMIN')")
    public ResponseEntity<Object> addPlace(
                @Valid @RequestBody Place place,
                BindingResult bindingResult,
                Authentication authentication) 
    {
        if (bindingResult.hasErrors()) {
            return ResponseEntity.badRequest().body(new PlacePostResponse(null,
                    "Validation failed: " + bindingResult.getAllErrors(), ResponseMessage.Status.ERROR));
        }
        
        UserInfoDetails userDetails = (UserInfoDetails) authentication.getPrincipal();

        try {
            User user = userService.getUserById(userDetails.getId());

            if (user.getRoles().equals("ROLE_USER")) {
                place.setStatus(PlaceStatus.PENDING);
            } else {
                place.setStatus(PlaceStatus.ACCEPTED);
            }

            Long placeId = placeService.addPlace(place);
            return ResponseEntity.ok(new PlacePostResponse(placeId, 
                    "Place was successfully added", ResponseMessage.Status.SUCCESS));
        } catch (DuplicatePlaceException | UserNotFoundException ex) {
            return ResponseEntity.badRequest().body(new PlacePostResponse(null, 
                            ex.getMessage(), ResponseMessage.Status.ERROR));
        }
    }

    @DeleteMapping("/place/{id}")
    @PreAuthorize("hasAnyAuthority('ROLE_MANAGER', 'ROLE_ADMIN')")
    public ResponseEntity<Object> deletePlace(@PathVariable Long id, Authentication authentication) throws PlaceNotFoundException {
        placeService.deletePlaceById(id);

        return ResponseEntity.ok(new ResponseMessage("Place was successfully removed", ResponseMessage.Status.SUCCESS));
    }
}
