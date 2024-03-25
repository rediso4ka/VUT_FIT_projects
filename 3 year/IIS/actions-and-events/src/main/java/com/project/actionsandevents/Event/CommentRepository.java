/**
 * @author Aleksandr Shevchenko (xshevc01)
 */
package com.project.actionsandevents.Event;

import java.util.List;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import com.project.actionsandevents.User.User;

@Repository
public interface CommentRepository extends JpaRepository<Comment, Long> {

    @Query("SELECT c.id FROM Comment c WHERE c.event = :event")
    List<Long> findAllIdsByEvent(@Param("event") Event event);

    @Query("SELECT c.id FROM Comment c WHERE c.event = :event AND c.user = :user")
    List<Long> findCommentByEventAndUser(@Param("event") Event event, @Param("user") User user);

    List<TicketType> findAllByEvent(Event event);
    
}
