/**
 * @author Vadim Goncearenco (xgonce00)
 */

package com.project.actionsandevents.Event;

import java.util.List;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

@Repository
public interface EventLogRepository extends JpaRepository<EventLog, Long> {
    @Query("SELECT el.id FROM EventLog el")
    List<Long> findAllIds();

    @Query("SELECT el.id FROM EventLog el WHERE el.event = :event")
    List<Long> findAllIdsByEvent(@Param("event") Event event);
}
