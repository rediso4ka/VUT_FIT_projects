/**
 * @author Oleksandr Turytsia (xturyt00)
 */
package com.project.actionsandevents.Auth;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.support.DefaultMessageSourceResolvable;
import org.springframework.http.ResponseEntity;
import org.springframework.security.authentication.AuthenticationManager;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.core.Authentication;
import org.springframework.validation.BindingResult;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import com.project.actionsandevents.Auth.responses.TokenResponse;
import com.project.actionsandevents.User.AuthRequest;
import com.project.actionsandevents.User.JwtService;
import com.project.actionsandevents.User.User;
import com.project.actionsandevents.User.UserService;
import com.project.actionsandevents.User.exceptions.DuplicateUserException;
import com.project.actionsandevents.User.responses.UserPostResponse;
import com.project.actionsandevents.common.ResponseMessage;

import jakarta.validation.Valid;

@RestController
@RequestMapping("/auth")
public class AuthController {
    @Autowired
    private UserService service;
    
    @Autowired
    private JwtService jwtService;
    
    @Autowired
    private AuthenticationManager authenticationManager;
  
    @PostMapping("/register")
    public ResponseEntity<Object> register(@Valid @RequestBody User userInfo, BindingResult bindingResult) {
        if (bindingResult.hasErrors()) {

            String firstErrorMessage = bindingResult.getAllErrors().stream()
                .findFirst()
                .map(DefaultMessageSourceResolvable::getDefaultMessage)
                .orElse("Validation failed");

            return ResponseEntity.badRequest().body(new ResponseMessage(firstErrorMessage, ResponseMessage.Status.ERROR));
        }

        try {
            Long userId = service.addUser(userInfo);
            return ResponseEntity.ok(
                new UserPostResponse(userId,
                        "User was successfully registered", ResponseMessage.Status.SUCCESS));
        } catch (DuplicateUserException ex) {
            return ResponseEntity.badRequest().body(new ResponseMessage(ex.getMessage(), ResponseMessage.Status.ERROR));
        }
    }

    @PostMapping("/login")
    public ResponseEntity<Object> authenticateAndGetToken(@RequestBody AuthRequest authRequest) {
        try {
            Authentication authentication = authenticationManager.authenticate(
                    new UsernamePasswordAuthenticationToken(authRequest.getLogin(), authRequest.getPassword()));
            if (authentication.isAuthenticated()) {
                return ResponseEntity.ok().body(new TokenResponse(jwtService.generateToken(authRequest.getLogin())));
            } else {
                return ResponseEntity.badRequest().body(new ResponseMessage("Authentication failed for an unknown reason.", 
                    ResponseMessage.Status.ERROR));
            }
        } catch (Exception ex) {
            ex.printStackTrace();
            return ResponseEntity.badRequest().body(new ResponseMessage("Authentication failed: " + ex.getMessage(), 
                    ResponseMessage.Status.ERROR));
        }

    }
}
