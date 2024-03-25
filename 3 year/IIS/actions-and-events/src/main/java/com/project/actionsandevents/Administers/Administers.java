/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.Administers;

import jakarta.persistence.Column;
import jakarta.persistence.Entity;
import jakarta.persistence.Id;
import jakarta.persistence.ManyToOne;
import jakarta.persistence.Temporal;
import jakarta.persistence.TemporalType;
import jakarta.validation.constraints.NotNull;
import jakarta.persistence.GeneratedValue;

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
public class Administers {
    @Id
    @GeneratedValue
    private Long id;

    @ManyToOne
    private User admin;
    
    @NotNull(message = "User login cannot be null")
    private String userLogin;

    @NotNull(message = "Date cannot be null")
    @Temporal(TemporalType.TIMESTAMP)
    private Date date;

    @Column(nullable = true)
    private String text;

    public Administers(User admin, String userLogin, String text) {
        this.admin = admin;
        this.userLogin = userLogin;
        this.date = new Date();
        this.text = text;
    }
}
