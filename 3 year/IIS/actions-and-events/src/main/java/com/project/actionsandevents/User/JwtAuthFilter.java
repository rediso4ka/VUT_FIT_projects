/**
 * This file contains class that configures response filter in order to
 * validate JWT token.
 * 
 * @see https://www.baeldung.com/spring-boot-add-filter
 *
 * @author Oleksandr Turytsia (xturyt00)
 */
package com.project.actionsandevents.User;

import jakarta.servlet.FilterChain;
import jakarta.servlet.ServletException;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.MediaType;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.web.authentication.WebAuthenticationDetailsSource;
import org.springframework.stereotype.Component;
import org.springframework.web.filter.OncePerRequestFilter;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.project.actionsandevents.common.ResponseMessage.Status;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
  
// This class helps us to validate the generated jwt token
@Component
public class JwtAuthFilter extends OncePerRequestFilter {
    
    @Autowired
    private JwtService jwtService;

    @Autowired
    private UserService userDetailsService;
    
    @Override
    protected void doFilterInternal(HttpServletRequest request, HttpServletResponse response, FilterChain filterChain) throws ServletException, IOException {
        String authHeader = request.getHeader("Authorization");
        String token = null;
        String username = null;
        try {
            if (authHeader != null && authHeader.startsWith("Bearer ")) {
                token = authHeader.substring(7);
                username = jwtService.extractUsername(token);
            }
        } catch (Exception e) {
            Map<String, Object> message = new HashMap<>();
            message.put("message", "Invalid token");
            message.put("status", Status.ERROR);

            response.setStatus(HttpStatus.FORBIDDEN.value());
            response.setContentType(MediaType.APPLICATION_JSON_VALUE);
            
            ObjectMapper mapper = new ObjectMapper();
            mapper.writeValue(response.getWriter(), message);
        }

        
        if (username != null && SecurityContextHolder.getContext().getAuthentication() == null) {
            try {
                UserDetails userDetails = userDetailsService.loadUserByUsername(username);
                if (jwtService.validateToken(token, userDetails)) {
                    UsernamePasswordAuthenticationToken authToken = new UsernamePasswordAuthenticationToken(userDetails,
                            null, userDetails.getAuthorities());
                    authToken.setDetails(new WebAuthenticationDetailsSource().buildDetails(request));
                    SecurityContextHolder.getContext().setAuthentication(authToken);
                } else {
                    Map<String, Object> message = new HashMap<>();
                    message.put("message", "Invalid token");
                    message.put("status", Status.ERROR);
    
                    response.setStatus(HttpStatus.FORBIDDEN.value());
                    response.setContentType(MediaType.APPLICATION_JSON_VALUE);
                    
                    ObjectMapper mapper = new ObjectMapper();
                    mapper.writeValue(response.getWriter(), message);
    
                    // Return here to stop further processing
                    return;
                }
            } catch (Exception e) {
                Map<String, Object> message = new HashMap<>();
                message.put("message", "Invalid token");
                message.put("status", Status.ERROR);

                response.setStatus(HttpStatus.FORBIDDEN.value());
                response.setContentType(MediaType.APPLICATION_JSON_VALUE);
                
                ObjectMapper mapper = new ObjectMapper();
                mapper.writeValue(response.getWriter(), message);

                // Return here to stop further processing
                return;
            }
        }

        filterChain.doFilter(request, response);
    }
}
