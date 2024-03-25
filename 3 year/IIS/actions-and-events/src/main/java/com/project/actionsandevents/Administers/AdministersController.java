/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.Administers;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.security.access.prepost.PreAuthorize;
import org.springframework.security.core.Authentication;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import com.project.actionsandevents.Administers.responses.LogResponse;
import com.project.actionsandevents.Administers.responses.LogsResponse;
import com.project.actionsandevents.common.ResponseMessage;
import com.project.actionsandevents.Administers.exceptions.AdministerLogNotFoundException;


@RestController
@RequestMapping("/admin")
public class AdministersController {

    @Autowired
    private AdministersService administersService;

    @GetMapping("/logs")
    @PreAuthorize("hasAuthority('ROLE_ADMIN')")
    public ResponseEntity<Object> getAdministerIds(Authentication authentication) {
        return ResponseEntity.ok(new LogsResponse(administersService.getAdministerIds()));
    }

    @GetMapping("/log/{id}")
    @PreAuthorize("hasAuthority('ROLE_ADMIN')")
    public ResponseEntity<Object> getAdministerById(@PathVariable Long id, Authentication authentication) {
        try {
            return ResponseEntity.ok(new LogResponse(administersService.getAdministerById(id)));
        } catch (AdministerLogNotFoundException ex) {
            return ResponseEntity.badRequest().body(new ResponseMessage(
                    ex.getMessage(), ResponseMessage.Status.ERROR));
        }
    }
}
