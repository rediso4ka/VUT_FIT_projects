/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.Event.responses;

import java.util.List;

import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
public class RegisterListResponse {
    private List<Long> registers;

    public RegisterListResponse(List<Long> registers) {
        this.registers = registers;
    }
}
