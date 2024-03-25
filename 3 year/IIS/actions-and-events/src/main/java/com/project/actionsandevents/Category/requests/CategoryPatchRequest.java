/**
 * @author Vadim Goncearenco (xgonce00)
 */
package com.project.actionsandevents.Category.requests;

import com.project.actionsandevents.Category.CategoryStatus;

import jakarta.validation.constraints.NotBlank;
import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
public class CategoryPatchRequest {
    private Long parentCategory;

    @NotBlank(message = "Name is required")
    private String name;

    private CategoryStatus status;
}
