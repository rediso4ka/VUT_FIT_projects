/**
 * @author Aleksandr Shevchenko (xshevc01)
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.Place;

import java.util.List;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.stereotype.Repository;
  
import java.util.Optional;

@Repository
public interface PlaceRepository extends JpaRepository<Place, Long>{
    Optional<Place> findByName(String name);

    @Query("SELECT p.id FROM Place p")
    List<Long> findAllIds();
}
