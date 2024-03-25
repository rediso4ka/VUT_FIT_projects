/**
 * This file contains class that represents User in SQL database.
 *
 * @author Oleksandr Turytsia (xturyt00)
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.User;

import java.util.Set;

import org.hibernate.validator.constraints.Length;

import com.project.actionsandevents.Administers.Administers;
import com.project.actionsandevents.Event.Comment;
import com.project.actionsandevents.Event.Event;
import com.project.actionsandevents.Event.Registers;

import jakarta.persistence.CascadeType;
import jakarta.persistence.Column;
import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.Id;
import jakarta.persistence.OneToMany;
import jakarta.persistence.Table;
import jakarta.validation.constraints.Email;
import jakarta.validation.constraints.NotBlank;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

@Getter
@Setter
@NoArgsConstructor
@AllArgsConstructor
@Entity
@Table(name="_user")
public class User {
    @Id
    @GeneratedValue
    private Long id;

    @NotBlank(message = "Login is mandatory")
    
    @Column(nullable = false, unique = true)
    private String login;

    @Column(nullable = true)
    private String firstname;

    @Column(nullable = true)
    private String lastname;

    @NotBlank(message = "Email is mandatory")
    @Email(message = "Email format is not valid")
    @Column(nullable = false, unique = true)
    private String email;

    @Column(nullable = true)
    private String phone;

    @NotBlank(message = "Password is mandatory")
    @Length(min = 8, message = "Password must be at least 8 characters long")
    @Column(nullable = false)
    private String password;

    private String roles;

    // Delete all events when user is deleted
    @OneToMany(mappedBy = "author", cascade = CascadeType.ALL, orphanRemoval = true)
    private Set<Event> createdEvents;

    @OneToMany(mappedBy = "user", cascade = CascadeType.ALL, orphanRemoval = true)
    private Set<Comment> comments;

    @OneToMany(mappedBy = "admin", cascade = CascadeType.ALL, orphanRemoval = true)
    private Set<Administers> administers;

    @OneToMany(mappedBy = "user", cascade = CascadeType.ALL, orphanRemoval = true)
    private Set<Registers> registers;
}
