/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.Administers;

import java.util.List;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.stereotype.Repository;
  
@Repository
public interface AdministersRepository extends JpaRepository<Administers, Long> {
    @Query("SELECT a.id FROM Administers a")
    List<Long> findAllIds();
}
