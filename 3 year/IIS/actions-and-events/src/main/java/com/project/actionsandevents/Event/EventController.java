/**
 * @author Vadim Goncearenco (xgonce00)
 * @author Oleksandr Turytsia (xturyt00)
 */
package com.project.actionsandevents.Event;

import java.util.Date;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.security.access.prepost.PreAuthorize;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.GrantedAuthority;
import org.springframework.validation.BindingResult;
import org.springframework.web.bind.annotation.*;

import com.project.actionsandevents.Category.Category;
import com.project.actionsandevents.Category.CategoryService;
import com.project.actionsandevents.Category.exceptions.CategoryNotFoundException;
import com.project.actionsandevents.Event.exceptions.DuplicateEventException;
import com.project.actionsandevents.Event.exceptions.DuplicateRegistrationException;
import com.project.actionsandevents.Event.exceptions.EventLogNotFoundException;
import com.project.actionsandevents.Event.exceptions.EventNotFoundException;
import com.project.actionsandevents.Event.exceptions.RegistrationAlreadyExists;
import com.project.actionsandevents.Event.exceptions.RegistrationNotFoundException;
import com.project.actionsandevents.Event.exceptions.TicketNotFoundException;

import com.project.actionsandevents.Event.requests.EventPatchRequest;
import com.project.actionsandevents.Event.requests.EventPostRequest;
import com.project.actionsandevents.Event.responses.EventResponse;
import com.project.actionsandevents.Event.responses.EventsResponse;
import com.project.actionsandevents.Event.responses.RegisterListResponse;
import com.project.actionsandevents.Event.responses.TicketResponse;
import com.project.actionsandevents.Event.responses.TicketsResponse;
import com.project.actionsandevents.Place.Place;
import com.project.actionsandevents.Place.PlaceService;
import com.project.actionsandevents.Place.exceptions.PlaceNotFoundException;
import com.project.actionsandevents.Event.responses.CommentPostResponse;
import com.project.actionsandevents.Event.responses.CommentResponse;
import com.project.actionsandevents.Event.responses.CommentsResponse;
import com.project.actionsandevents.Event.responses.EventLogResponse;
import com.project.actionsandevents.Event.responses.EventLogsResponse;
import com.project.actionsandevents.Event.responses.EventPostResponse;

import com.project.actionsandevents.User.UserInfoDetails;
import com.project.actionsandevents.User.exceptions.UserNotFoundException;
import com.project.actionsandevents.User.responses.RegistersResponse;
import com.project.actionsandevents.User.UserService;
import com.project.actionsandevents.User.User;
import com.project.actionsandevents.common.ResponseMessage;

import jakarta.validation.Valid;

@RestController
@RequestMapping
public class EventController {
    @Autowired
    private EventService eventService;

    @Autowired
    private UserService userService;

    @Autowired
    private CommentRepository commentRepository;

    @Autowired
    private CategoryService categoryService;

    @Autowired
    private PlaceService placeService;

    private boolean hasElevatedPrivileges(Authentication authentication) {

        for (GrantedAuthority auth : authentication.getAuthorities()) {
            if (auth.getAuthority().equals("ROLE_ADMIN") ||
                    auth.getAuthority().equals("ROLE_MANAGER")) {

                return true;
            }
        }

        return false;
    }

    private boolean hasPrivilegesOnEvent(Authentication authentication, Event event) {
        // Among regular users only author of the event can modify it or its tickets
        UserInfoDetails userDetails = (UserInfoDetails) authentication.getPrincipal();

        boolean isAuthor = userDetails.getId().equals(event.getAuthor().getId());

        return isAuthor || hasElevatedPrivileges(authentication);
    }

    private boolean hasPrivilegesOnComment(Authentication authentication, Comment comment) {
        // Among regular users only author of the event can modify it or its tickets
        UserInfoDetails userDetails = (UserInfoDetails) authentication.getPrincipal();

        boolean isAuthor = userDetails.getId().equals(comment.getUser().getId());

        return isAuthor || hasElevatedPrivileges(authentication);
    }


   



    @GetMapping("/event/{id}")
    public ResponseEntity<Object> getEventById(@PathVariable Long id, Authentication authentication)
    {
        try {
            return ResponseEntity.ok(new EventResponse(eventService.getEventById(id)));
        } catch (EventNotFoundException ex) {
            return ResponseEntity.badRequest().body(new ResponseMessage(
                    ex.getMessage(), ResponseMessage.Status.ERROR));
        }
    }

    @GetMapping("/events")
    public ResponseEntity<Object> getEventIds() {
        return ResponseEntity.ok(new EventsResponse(eventService.getEventIds()));
    }

    @PatchMapping("/event/{id}")
    @PreAuthorize("hasAnyAuthority('ROLE_USER', 'ROLE_MANAGER', 'ROLE_ADMIN')")
    public ResponseEntity<Object> patchEventById(
            @PathVariable Long id,
            @Valid @RequestBody EventPatchRequest patchRequest,
            BindingResult bindingResult,
            Authentication authentication)  
    {

        if (bindingResult.hasErrors()) {
            return ResponseEntity.badRequest().body(new ResponseMessage(
                    "Validation failed: " + bindingResult.getAllErrors(), ResponseMessage.Status.ERROR));
        }

        try {

            if (patchRequest.getDateFrom().getTime() < new Date().getTime()) {
                return ResponseEntity.badRequest()
                        .body(new ResponseMessage("You cannot set start date as today or in the past", ResponseMessage.Status.ERROR));
            }
            
            if (patchRequest.getDateTo() != null && patchRequest.getDateFrom().getTime() > patchRequest.getDateTo().getTime()) {
                return ResponseEntity.badRequest().body(new ResponseMessage("Start date cannot be greater than end date", ResponseMessage.Status.ERROR));
            }

            eventService.patchEventById(id, patchRequest);
            return ResponseEntity.ok(new ResponseMessage(
                            "Event was successfully updated", ResponseMessage.Status.SUCCESS));
        } catch (EventNotFoundException | DuplicateEventException ex) {
            return ResponseEntity.badRequest().body(new ResponseMessage(
                    ex.getMessage(), ResponseMessage.Status.ERROR));
        }
    }

    @PostMapping("/event")
    @PreAuthorize("hasAnyAuthority('ROLE_USER', 'ROLE_MANAGER', 'ROLE_ADMIN')")
    public ResponseEntity<Object> addEvent(
            @Valid @RequestBody EventPostRequest event,
            BindingResult bindingResult,
            Authentication authentication) 
    {
        if (bindingResult.hasErrors()) {
            return ResponseEntity.badRequest().body(new ResponseMessage(
                    "Validation failed: " + bindingResult.getAllErrors(), ResponseMessage.Status.ERROR));
        }

        UserInfoDetails userDetails = (UserInfoDetails) authentication.getPrincipal();

        try {
            if (event.getDateFrom().getTime() < new Date().getTime()) {
                return ResponseEntity.badRequest()
                        .body(new ResponseMessage("You cannot set start date as today or in the past", ResponseMessage.Status.ERROR));
            }
            
            if (event.getDateTo() != null && event.getDateFrom().getTime() > event.getDateTo().getTime()) {
                return ResponseEntity.badRequest().body(new ResponseMessage("Start date cannot be greater than end date", ResponseMessage.Status.ERROR));
            }

            User author = userService.getUserById(userDetails.getId());

            Category category = categoryService.getCategoryById(event.getCategoryId());

            Place place = placeService.getPlaceById(event.getPlaceId());

            Event newEvent = new Event();

            newEvent.setTitle(event.getTitle());
            newEvent.setDescription(event.getDescription());
            newEvent.setDateFrom(event.getDateFrom());
            newEvent.setDateTo(event.getDateTo());
            newEvent.setImage(event.getImage());
            newEvent.setPlace(place);
            newEvent.setCategory(category);
            newEvent.setAuthor(author);

            if (author.getRoles().equals("ROLE_USER")) {
                newEvent.setStatus(EventStatus.PENDING);
            } else {
                newEvent.setStatus(EventStatus.ACCEPTED);
            }

            return ResponseEntity.ok(
                new EventPostResponse(eventService.addEvent(newEvent),
                                "Event was successfully added", ResponseMessage.Status.SUCCESS));
        } catch (UserNotFoundException | DuplicateEventException | CategoryNotFoundException | PlaceNotFoundException ex) {
            return ResponseEntity.badRequest().body(new EventPostResponse(null,
                    ex.getMessage(), ResponseMessage.Status.ERROR));
        }
    }

    @DeleteMapping("/event/{id}")
    @PreAuthorize("hasAnyAuthority('ROLE_USER', 'ROLE_MANAGER', 'ROLE_ADMIN')")
    public ResponseEntity<Object> deleteEvent(@PathVariable Long id, Authentication authentication)
    {
        try {
            if (!hasPrivilegesOnEvent(authentication, eventService.getEventById(id))) {
                return ResponseEntity.badRequest().body(new ResponseMessage(
                        "You are not allowed to delete this event", ResponseMessage.Status.ERROR));
            }

            eventService.deleteEventById(id);
        } catch (EventNotFoundException ex) {
            return ResponseEntity.badRequest().body(new ResponseMessage(
                    ex.getMessage(), ResponseMessage.Status.ERROR));
        }

        return ResponseEntity.ok(new ResponseMessage("Event was successfully removed", ResponseMessage.Status.SUCCESS));
    }

    @GetMapping("/event/{id}/comments")
    public ResponseEntity<Object> getEventComments(@PathVariable Long id, Authentication authentication)
    {
        try {
            return ResponseEntity.ok(new CommentsResponse(eventService.getCommentsIds(id)));
        } catch (EventNotFoundException ex) {
            return ResponseEntity.badRequest().body(new ResponseMessage(
                    ex.getMessage(), ResponseMessage.Status.ERROR));
        }
    }

    @GetMapping("/event/comment/{id}")
    public ResponseEntity<Object> getEventCommentById(@PathVariable Long id, Authentication authentication)
    {
        try {
            return ResponseEntity.ok(new CommentResponse(eventService.getCommentById(id)));
        } catch (EventNotFoundException ex) {
            return ResponseEntity.badRequest().body(new ResponseMessage(
                    ex.getMessage(), ResponseMessage.Status.ERROR));
        }
    }

    @PostMapping("/event/{id}/comment")
    @PreAuthorize("hasAnyAuthority('ROLE_USER', 'ROLE_MANAGER', 'ROLE_ADMIN')")
    public ResponseEntity<Object> addEventComment(
            @PathVariable Long id,
            @Valid @RequestBody Comment comment,
            BindingResult bindingResult,
            Authentication authentication) 
    {
        if (bindingResult.hasErrors()) {
            return ResponseEntity.badRequest().body(new ResponseMessage(
                    "Validation failed: " + bindingResult.getAllErrors(), ResponseMessage.Status.ERROR));
        }

        

        UserInfoDetails userDetails = (UserInfoDetails) authentication.getPrincipal();

        try {
            if (commentRepository.findCommentByEventAndUser(eventService.getEventById(id), userService.getUserById(userDetails.getId())).size() != 0) {
                return ResponseEntity.badRequest().body(new ResponseMessage("User can only leave 1 feedback", ResponseMessage.Status.ERROR));
            }

            comment.setUser(userService.getUserById(userDetails.getId()));

            Long commentId = eventService.addComment(id, comment);

            ResponseMessage message = new ResponseMessage("Comment was successfuly created",
                    ResponseMessage.Status.SUCCESS);

            return ResponseEntity.ok(new CommentPostResponse(commentId, message));
        } catch (UserNotFoundException | EventNotFoundException ex) {
            return ResponseEntity.badRequest().body(new ResponseMessage(
                    ex.getMessage(), ResponseMessage.Status.ERROR));
        }
    }

    @PatchMapping("/event/comment/{id}")
    @PreAuthorize("hasAnyAuthority('ROLE_USER', 'ROLE_MANAGER', 'ROLE_ADMIN')")
    public ResponseEntity<Object> patchEventCommentById(
            @PathVariable Long id,
            @Valid @RequestBody Comment comment,
            BindingResult bindingResult,
            Authentication authentication) 
    {
        if (bindingResult.hasErrors()) {
            return ResponseEntity.badRequest().body(new ResponseMessage(
                    "Validation failed: " + bindingResult.getAllErrors(), ResponseMessage.Status.ERROR));
        }

        try {
            if (!hasPrivilegesOnComment(authentication, eventService.getCommentById(id))) {
                return ResponseEntity.badRequest().body(new ResponseMessage(
                        "You are not allowed to patch this comment", ResponseMessage.Status.ERROR));
            }

            return ResponseEntity
                    .ok(new ResponseMessage(eventService.patchCommentById(id, comment), ResponseMessage.Status.SUCCESS));
        } catch (EventNotFoundException ex) {
            return ResponseEntity.badRequest().body(new ResponseMessage(
                    ex.getMessage(), ResponseMessage.Status.ERROR));
        }
    }

    @DeleteMapping("/event/comment/{id}")
    @PreAuthorize("hasAnyAuthority('ROLE_USER', 'ROLE_MANAGER', 'ROLE_ADMIN')")
    public ResponseEntity<Object> deleteEventComment(@PathVariable Long id, Authentication authentication)
    {
        try {
            if (!hasPrivilegesOnComment(authentication, eventService.getCommentById(id))) {
                return ResponseEntity.badRequest().body(new ResponseMessage(
                        "You are not allowed to delete this comment", ResponseMessage.Status.ERROR));
            }

            eventService.deleteCommentById(id);
        } catch (EventNotFoundException ex) {
            return ResponseEntity.badRequest().body(new ResponseMessage(
                    ex.getMessage(), ResponseMessage.Status.ERROR));
        }

        return ResponseEntity
                .ok(new ResponseMessage("Comment was successfully removed", ResponseMessage.Status.SUCCESS));
    }

    @GetMapping("/event/{id}/tickets")
    public ResponseEntity<Object> getEventTickets(@PathVariable Long id, Authentication authentication)
    {
        try {
            return ResponseEntity.ok(new TicketsResponse(eventService.getTicketTypeIds(id)));
        } catch (EventNotFoundException ex) {
            return ResponseEntity.badRequest().body(new ResponseMessage(
                    ex.getMessage(), ResponseMessage.Status.ERROR));
        }
    }

    @GetMapping("/event/ticket/{id}")
    public ResponseEntity<Object> getTicketById(@PathVariable Long id, Authentication authentication)
    {
        try {
            return ResponseEntity.ok(new TicketResponse(eventService.getTicketTypeById(id)));
        } catch (TicketNotFoundException ex) {
            return ResponseEntity.badRequest().body(new ResponseMessage(
                    ex.getMessage(), ResponseMessage.Status.ERROR));
        }
    }

    @PostMapping("/event/{id}/ticket")
    @PreAuthorize("hasAnyAuthority('ROLE_USER', 'ROLE_MANAGER', 'ROLE_ADMIN')")
    public ResponseEntity<Object> addEventTicket(
            @PathVariable Long id,
            @Valid @RequestBody TicketType ticketType,
            BindingResult bindingResult,
            Authentication authentication)
    {
        if (bindingResult.hasErrors()) {
            return ResponseEntity.badRequest().body(new ResponseMessage(
                    "Validation failed: " + bindingResult.getAllErrors(), ResponseMessage.Status.ERROR));
        }

        try {
            if (!hasPrivilegesOnEvent(authentication, eventService.getEventById(id))) {
                return ResponseEntity.badRequest().body(new ResponseMessage(
                        "You are not allowed to add tickets to this event", ResponseMessage.Status.ERROR));
            }
            return ResponseEntity
                    .ok(new ResponseMessage(eventService.addTicketType(id, ticketType), ResponseMessage.Status.SUCCESS));
        } catch (EventNotFoundException | DuplicateEventException ex) {
            return ResponseEntity.badRequest().body(new ResponseMessage(
                    ex.getMessage(), ResponseMessage.Status.ERROR));
        }
    }

    @PatchMapping("/event/ticket/{id}")
    @PreAuthorize("hasAnyAuthority('ROLE_USER', 'ROLE_MANAGER', 'ROLE_ADMIN')")
    public ResponseEntity<Object> patchTicketById(
            @PathVariable Long id,
            @Valid @RequestBody TicketType ticketType,
            BindingResult bindingResult,
            Authentication authentication)
    {
        if (bindingResult.hasErrors()) {
            return ResponseEntity.badRequest().body(new ResponseMessage(
                    "Validation failed: " + bindingResult.getAllErrors(), ResponseMessage.Status.ERROR));
        }

        try {
            if (!hasPrivilegesOnEvent(authentication, eventService.getTicketTypeById(id).getEvent())) {
                return ResponseEntity.badRequest().body(new ResponseMessage(
                        "You are not allowed to patch this ticket", ResponseMessage.Status.ERROR));
            }

            return ResponseEntity.ok(
                    new ResponseMessage(eventService.patchTicketTypeById(id, ticketType), ResponseMessage.Status.SUCCESS));
        } catch (TicketNotFoundException ex) {
            return ResponseEntity.badRequest().body(new ResponseMessage(
                    ex.getMessage(), ResponseMessage.Status.ERROR));
        }
    }

    @DeleteMapping("/event/ticket/{id}")
    @PreAuthorize("hasAnyAuthority('ROLE_USER', 'ROLE_MANAGER', 'ROLE_ADMIN')")
    public ResponseEntity<Object> deleteTicket(@PathVariable Long id, Authentication authentication)
    {
        try {
            if (!hasPrivilegesOnEvent(authentication, eventService.getTicketTypeById(id).getEvent())) {
                return ResponseEntity.badRequest().body(new ResponseMessage(
                        "You are not allowed to delete this ticket", ResponseMessage.Status.ERROR));
            }

            eventService.deleteTicketTypeById(id);
        } catch (TicketNotFoundException ex) {
            return ResponseEntity.badRequest().body(new ResponseMessage(
                    ex.getMessage(), ResponseMessage.Status.ERROR));
        }

        return ResponseEntity
                .ok(new ResponseMessage("Ticket type was successfully removed", ResponseMessage.Status.SUCCESS));
    }




    @GetMapping("/event/ticket/{id}/register/{userId}")
    @PreAuthorize("hasAnyAuthority('ROLE_USER', 'ROLE_MANAGER', 'ROLE_ADMIN')")
    public ResponseEntity<Object> registerUserForTicketType(
            @PathVariable Long id,
            @PathVariable Long userId,
            Authentication authentication)
    {
        try {
            return ResponseEntity.ok(
                    new ResponseMessage(eventService.registerUserForTicketType(id, userId),
                            ResponseMessage.Status.SUCCESS));
        } catch (TicketNotFoundException | UserNotFoundException | RegistrationAlreadyExists ex) {
            return ResponseEntity.badRequest().body(new ResponseMessage(
                    ex.getMessage(), ResponseMessage.Status.ERROR));
        }
    }

    @GetMapping("/event/ticket/{id}/registrations")
    public ResponseEntity<Object> getTicketRegistrations(@PathVariable Long id, Authentication authentication)
    {
        try {
            return ResponseEntity.ok(new RegisterListResponse(eventService.getTicketRegistrations(id)));
        } catch (TicketNotFoundException ex) {
            return ResponseEntity.badRequest().body(new ResponseMessage(
                    ex.getMessage(), ResponseMessage.Status.ERROR));
        }
    }

    @GetMapping("/event/ticket/registration/{id}")
    public ResponseEntity<Object> getTicketRegistrationById(
            @PathVariable Long id, Authentication authentication)
    {
        try {
            return ResponseEntity.ok(new RegistersResponse(eventService.getTicketRegistrationById(id)));
        } catch (RegistrationNotFoundException ex) {
            return ResponseEntity.badRequest().body(new ResponseMessage(
                    ex.getMessage(), ResponseMessage.Status.ERROR));
        }
    }

    @PatchMapping("/event/ticket/registration/{id}")
    @PreAuthorize("hasAnyAuthority('ROLE_USER', 'ROLE_MANAGER', 'ROLE_ADMIN')")
    public ResponseEntity<Object> patchTicketRegistrationById(
            @PathVariable Long id,
            @Valid @RequestBody Registers registers,
            BindingResult bindingResult,
            Authentication authentication)
    {

        if (bindingResult.hasErrors()) {
            return ResponseEntity.badRequest().body(new ResponseMessage(
                    "Validation failed: " + bindingResult.getAllErrors(), ResponseMessage.Status.ERROR));
        }

        try {
            Registers reg = eventService.getTicketRegistrationById(id);

            if (!hasPrivilegesOnEvent(authentication,
                    reg.getTicketType().getEvent())) {
                return ResponseEntity.badRequest().body(new ResponseMessage(
                        "You are not allowed to patch this ticket registration", ResponseMessage.Status.ERROR));
            }

            return ResponseEntity.ok(new ResponseMessage(
                    eventService.patchTicketRegistrationById(id, registers.getStatus()), ResponseMessage.Status.SUCCESS));
        } catch (RegistrationNotFoundException | DuplicateRegistrationException ex) {
            return ResponseEntity.badRequest().body(new ResponseMessage(
                    ex.getMessage(), ResponseMessage.Status.ERROR));
        }
    }

  



    @PostMapping("/event/{id}/approve")
    @PreAuthorize("hasAnyAuthority('ROLE_MANAGER', 'ROLE_ADMIN')")
    public ResponseEntity<Object> approveEvent(
            @PathVariable Long id,
            Authentication authentication)
    {
        try {
            return ResponseEntity.ok(new ResponseMessage(eventService.approveEvent(id), ResponseMessage.Status.SUCCESS));
        } catch (EventNotFoundException ex) {
            return ResponseEntity.badRequest().body(new ResponseMessage(
                    ex.getMessage(), ResponseMessage.Status.ERROR));
        }
    }

    @GetMapping("/event/{id}/logs")
    @PreAuthorize("hasAnyAuthority('ROLE_MANAGER', 'ROLE_ADMIN')")
    public ResponseEntity<Object> getEventLogs(@PathVariable Long id, Authentication authentication)
    {
        try {
            return ResponseEntity.ok(new EventLogsResponse(eventService.getEventLogs(id)));
        } catch (EventNotFoundException ex) {
            return ResponseEntity.badRequest().body(new ResponseMessage(
                    ex.getMessage(), ResponseMessage.Status.ERROR));
        }
    }

    @GetMapping("/event/log/{id}")
    @PreAuthorize("hasAnyAuthority('ROLE_MANAGER', 'ROLE_ADMIN')")
    public ResponseEntity<Object> getEventLogById(
            @PathVariable Long id, Authentication authentication)
            throws EventLogNotFoundException 
    {
        try {
            return ResponseEntity.ok(new EventLogResponse(eventService.getEventLogById(id)));
        } catch (EventLogNotFoundException ex) {
            return ResponseEntity.badRequest().body(new ResponseMessage(
                    ex.getMessage(), ResponseMessage.Status.ERROR));
        }
    }

    @DeleteMapping("/event/log/{id}")
    @PreAuthorize("hasAnyAuthority('ROLE_ADMIN')")
    public ResponseEntity<Object> deleteEventLogById(
            @PathVariable Long id, Authentication authentication)
    {
        try {
            return ResponseEntity
                    .ok(new ResponseMessage(eventService.deleteEventLogById(id), ResponseMessage.Status.SUCCESS));
        } catch (EventLogNotFoundException ex) {
            return ResponseEntity.badRequest().body(new ResponseMessage(
                    ex.getMessage(), ResponseMessage.Status.ERROR));
        }
    }
}
