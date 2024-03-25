/**
 * @author Aleksandr Shevchenko (xshevc01)
 */
package com.project.actionsandevents.Category.responses;

import com.project.actionsandevents.Category.Category;
import com.project.actionsandevents.Category.CategoryStatus;

import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
public class CategoryResponse {
    private Long id;
    private String name;
    private Long parentCategory;
    private CategoryStatus status;

    public CategoryResponse(Category category) {
        this.id = category.getId();
        this.name = category.getName();
        this.parentCategory = category.getParentCategory() != null ? category.getParentCategory().getId() : null;
        this.status = category.getStatus();
    }
}
