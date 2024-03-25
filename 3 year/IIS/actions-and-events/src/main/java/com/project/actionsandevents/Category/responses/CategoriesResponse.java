/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.Category.responses;

import lombok.Getter;
import lombok.Setter;

import java.util.List;

@Setter
@Getter
public class CategoriesResponse {
    private List<Long> categories;

    public CategoriesResponse(List<Long> ids) {
        this.categories = ids;
    }
}
