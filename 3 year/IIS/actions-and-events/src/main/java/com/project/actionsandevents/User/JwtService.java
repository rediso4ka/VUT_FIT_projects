/**
 * This file contains class that implements JWT generators, validators and accessors.
 * 
 * @see https://www.baeldung.com/spring-boot-add-filter
 *
 * @author Oleksandr Turytsia (xturyt00)
 */
package com.project.actionsandevents.User;

import io.jsonwebtoken.Claims;
import io.jsonwebtoken.ExpiredJwtException;
import io.jsonwebtoken.Jwts;
import io.jsonwebtoken.MalformedJwtException;
import io.jsonwebtoken.SignatureAlgorithm;
import io.jsonwebtoken.UnsupportedJwtException;
import io.jsonwebtoken.io.Decoders;
import io.jsonwebtoken.security.Keys;
import io.jsonwebtoken.security.SignatureException;

import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.stereotype.Component;
  
import java.security.Key;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import java.util.function.Function;
  
@Component
public class JwtService {
  
    public static final String SECRET = "5367566B59703373367639792F423F4528482B4D6251655468576D5A71347437";

    public String generateToken(String userName) {
        Map<String, Object> claims = new HashMap<>();
        return createToken(claims, userName);
    }
    
    /**
     * Create JWT token
     * 
     * @param claims Payload
     * @param userName Username
     * @return Token
     */
    private String createToken(Map<String, Object> claims, String userName) {
        return Jwts.builder()
                .setClaims(claims)
                .setSubject(userName)
                .setIssuedAt(new Date(System.currentTimeMillis()))
                .setExpiration(new Date(System.currentTimeMillis() + 1000 * 60 * 60 * 24))
                .signWith(getSignKey(), SignatureAlgorithm.HS256).compact();
    }
    
    /**
     * Create signing key for the token
     * 
     * @return Signing key
     */
    private Key getSignKey() {
        byte[] keyBytes= Decoders.BASE64.decode(SECRET);
        return Keys.hmacShaKeyFor(keyBytes);
    }
    
    /**
     * Extract username from a given token
     * 
     * @param token JWT token
     * @return Username
     */
    public String extractUsername(String token) throws ExpiredJwtException, UnsupportedJwtException,
            MalformedJwtException, SignatureException, IllegalArgumentException {
        return extractClaim(token, Claims::getSubject);
    }
    
    /**
     * Extract expiration from a given token
     * 
     * @param token JWT token
     * @return Expiration date
     */
    public Date extractExpiration(String token) throws ExpiredJwtException, UnsupportedJwtException,
            MalformedJwtException, SignatureException, IllegalArgumentException {
        return extractClaim(token, Claims::getExpiration);
    }
    
    /**
     * Extracts attributes from the payload of a JWT
     * 
     * @param <T> Template
     * @param token JWT token
     * @param claimsResolver Specifies what should be extracted from the payload
     * @return Extracted data from the token
     */
    public <T> T extractClaim(String token, Function<Claims, T> claimsResolver) throws ExpiredJwtException,
            UnsupportedJwtException, MalformedJwtException, SignatureException, IllegalArgumentException {
        final Claims claims = extractAllClaims(token);
        return claimsResolver.apply(claims);
    }
    
    /**
     * Extracts all claims
     * 
     * "Claims" refer to the attributes encoded within the JWT token.
     * 
     * @param token JWT token
     * @return Claims
     */
    private Claims extractAllClaims(String token) throws ExpiredJwtException, UnsupportedJwtException, MalformedJwtException, SignatureException, IllegalArgumentException {
        return Jwts
                .parserBuilder()
                .setSigningKey(getSignKey())
                .build()
                .parseClaimsJws(token)
                .getBody();
    }
    
    /**
     * Check if a token is expired
     * 
     * @param token JWT token
     * @return True if expiration date is in the future
     */
    private Boolean isTokenExpired(String token) {
        return extractExpiration(token).before(new Date());
    }
  
    /**
     * Validate a token
     * 
     * @param token JWT token
     * @param userDetails User details
     * @return True if the token is valid, otherwise false
     */
    public Boolean validateToken(String token, UserDetails userDetails) {
        final String username = extractUsername(token);
        return (username.equals(userDetails.getUsername()) && !isTokenExpired(token));
    }
}
