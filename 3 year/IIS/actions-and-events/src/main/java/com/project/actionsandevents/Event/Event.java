/**
 * @author Vadim Goncearenco (xgonce00)
 */

package com.project.actionsandevents.Event;

import jakarta.persistence.Column;
import jakarta.persistence.Entity;
import jakarta.persistence.EnumType;
import jakarta.persistence.Enumerated;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.Id;
import jakarta.persistence.JoinColumn;
import jakarta.persistence.ManyToOne;
import jakarta.persistence.OneToMany;
import jakarta.persistence.Temporal;
import jakarta.persistence.TemporalType;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import jakarta.persistence.CascadeType;

import com.project.actionsandevents.Category.Category;
import com.project.actionsandevents.Place.Place;
import com.project.actionsandevents.User.User;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.util.Date;
import java.util.List;


@Getter
@Setter
@NoArgsConstructor
@AllArgsConstructor
@Entity
public class Event {
    @Id
    @GeneratedValue
    private Long id;

    @NotNull(message = "Date from is required")
    @Column(nullable = false)
    @Temporal(TemporalType.TIMESTAMP)
    private Date dateFrom;

    @Column(nullable = true)
    @Temporal(TemporalType.TIMESTAMP)
    private Date dateTo;

    @Column(nullable = true)
    private String description;

    @NotBlank(message = "Title is required")
    @Column(nullable = false)
    private String title;

    @Column(nullable = true)
    private String icon;

    @Column(nullable = true)
    private String image;

    @Enumerated(EnumType.STRING)
    @Column(nullable = false)
    private EventStatus status;

    @ManyToOne
    @JoinColumn(name = "author", referencedColumnName = "id", nullable = true)
    private User author;

    @ManyToOne
    @JoinColumn(referencedColumnName = "id")
    private User userManages;

    @NotNull(message = "Place is required")
    @ManyToOne
    @JoinColumn(referencedColumnName = "id")
    private Place place;

    @NotNull(message = "Category is required")
    @ManyToOne
    @JoinColumn(referencedColumnName = "id")
    private Category category;

    // Delete all ticket types when event is deleted
    @OneToMany(mappedBy = "event", cascade = CascadeType.ALL, orphanRemoval = true)
    private List<TicketType> ticketTypes;

    // Delete all comments when event is deleted
    @OneToMany(mappedBy = "event", cascade = CascadeType.ALL, orphanRemoval = true)
    private List<Comment> comments;

    @OneToMany(mappedBy = "event", cascade = CascadeType.ALL)
    private List<EventLog> eventLogs;
}
