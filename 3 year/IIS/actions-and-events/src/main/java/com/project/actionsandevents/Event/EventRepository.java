/**
 * @author Vadim Goncearenco (xgonce00)
 */

package com.project.actionsandevents.Event;

import java.util.List;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.stereotype.Repository;

import com.project.actionsandevents.User.User;

@Repository
public interface EventRepository extends JpaRepository<Event, Long> {
    
    @Query("SELECT e.id FROM Event e")
    List<Long> findAllIds();

    @Query("SELECT e FROM Event e WHERE e.author = ?1")
    List<Event> findAllByAuthor(User author);
}
