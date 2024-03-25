/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.Administers;

import java.util.List;
import java.util.Optional;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.project.actionsandevents.Administers.exceptions.AdministerLogNotFoundException;

@Service
public class AdministersService {
    @Autowired
    private AdministersRepository administersRepository;

    public List<Long> getAdministerIds() 
    {
        return administersRepository.findAllIds();
    }

    public Administers getAdministerById(Long id) 
        throws AdministerLogNotFoundException 
    {
        Optional<Administers> administers = administersRepository.findById(id);

        if (!administers.isPresent()) {
            throw new AdministerLogNotFoundException("Administer log was not found");
        }

        return administers.get();
    }
}
