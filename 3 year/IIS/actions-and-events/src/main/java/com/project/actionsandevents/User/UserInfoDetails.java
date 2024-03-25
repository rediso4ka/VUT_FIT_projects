/**
 * This file contains class that implements security methods for the user.
 *
 * @author Oleksandr Turytsia (xturyt00)
 */
package com.project.actionsandevents.User;

import org.springframework.security.core.GrantedAuthority;
import org.springframework.security.core.authority.SimpleGrantedAuthority;
import org.springframework.security.core.userdetails.UserDetails;
  
import java.util.Arrays;
import java.util.Collection;
import java.util.List;
import java.util.stream.Collectors;
  
public class UserInfoDetails implements UserDetails {
  
    private String login;
    private String password;
    private List<GrantedAuthority> authorities;
    private Long id;
  
    public UserInfoDetails(User userInfo) {
        login = userInfo.getLogin();
        password = userInfo.getPassword();
        authorities = Arrays.stream(userInfo.getRoles().split(","))
                .map(SimpleGrantedAuthority::new)
                .collect(Collectors.toList());
        id = userInfo.getId();
    }
  
    @Override
    public Collection<? extends GrantedAuthority> getAuthorities() {
        return authorities;
    }
  
    @Override
    public String getPassword() {
        return password;
    }
  
    @Override
    public String getUsername() {
        return login;
    }
  
    @Override
    public boolean isAccountNonExpired() {
        return true;
    }
  
    @Override
    public boolean isAccountNonLocked() {
        return true;
    }
  
    @Override
    public boolean isCredentialsNonExpired() {
        return true;
    }
  
    @Override
    public boolean isEnabled() {
        return true;
    }

    public Long getId() {
        return id;
    }
}