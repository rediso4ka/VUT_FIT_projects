/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.Event;

import org.hibernate.validator.constraints.Range;

import jakarta.persistence.CascadeType;
import jakarta.persistence.Column;
import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.Id;
import jakarta.persistence.JoinColumn;
import jakarta.persistence.ManyToOne;
import jakarta.persistence.OneToMany;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.util.List;

@Getter
@Setter
@NoArgsConstructor
@AllArgsConstructor
@Entity
public class TicketType {
    @Id
    @GeneratedValue
    private Long id;

    @Column(nullable = false) // unique = true, 
    private String name;

    @Range(min = 0, max = 1000000)
    private Float price;

    @Range(min = 0, max = 1000000)
    private Long capacity;

    @Column
    private String description;

    @ManyToOne
    @JoinColumn(referencedColumnName = "id")
    private Event event;

    @OneToMany(mappedBy = "ticketType", cascade = CascadeType.ALL, orphanRemoval = true)
    private List<Registers> registers;
}
