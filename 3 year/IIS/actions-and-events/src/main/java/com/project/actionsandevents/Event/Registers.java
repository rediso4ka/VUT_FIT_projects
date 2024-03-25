/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.Event;

import jakarta.persistence.CascadeType;
import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.Id;
import jakarta.persistence.ManyToOne;
import jakarta.persistence.Temporal;
import jakarta.persistence.TemporalType;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.util.Date;

import com.project.actionsandevents.User.User;

@Getter
@Setter
@NoArgsConstructor
@AllArgsConstructor
@Entity
public class Registers {
    @Id
    @GeneratedValue
    private Long id;

    //@Id
    @ManyToOne
    private User user;

    //@Id
    @ManyToOne
    private TicketType ticketType;

    @Temporal(TemporalType.TIMESTAMP)
    private Date date;

    private RegistersStatus status;
}
