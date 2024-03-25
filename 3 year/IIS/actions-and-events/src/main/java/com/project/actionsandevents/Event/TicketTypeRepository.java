/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.Event;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;

import java.util.List;

import com.project.actionsandevents.User.User;

@Repository
public interface TicketTypeRepository extends JpaRepository<TicketType, Long> {
    @Query("SELECT tt.id FROM TicketType tt WHERE tt.event = :event")
    List<Long> findAllIdsByEvent(@Param("event") Event event);

    @Query("SELECT tt.id FROM TicketType tt WHERE tt.id IN ( SELECT r.ticketType FROM Registers r WHERE r.user = :user )")
    List<Long> findAllIdsByUser(@Param("user") User user);

    List<TicketType> findAllByEvent(Event event);
}
