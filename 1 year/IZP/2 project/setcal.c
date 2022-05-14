/**
 * IZP Project 2 - setcal
 *
 * Participants logins:
 * - xshevc01
 * - xpetro27
 * - xmoise01
 * - xafana01
 */

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#define SET_OPERATIONS_COUNT 9
#define RELATION_OPERATIONS_COUNT 10
#define COMMON_OPERATIONS_COUNT 3

/**
 * -----------------------------------------------------------------------------
 * SYSTEM MODULE
 * -----------------------------------------------------------------------------
 */

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

void print_string(char *s) {
    printf("%s\n", s);
}

void print_string_size(char *s) {
    printf("%lu\n", strlen(s));
}

/**
 * Prints the error message and exits the program.
 * @param filename The name of the file where the error occurred.
 * @param line The line number where the error occurred.
 * @param function The function where the error occurred.
 * @param msg The error message.
 */
void print_error(const char *filename, int line, const char *function,
                 const char *msg) {
    fprintf(stderr, "%s:%d - [%s] %s\n", filename, line, function, msg);
    exit(EXIT_FAILURE);
}

/**
 * -----------------------------------------------------------------------------
 * STRING MODULE
 * -----------------------------------------------------------------------------
 */

void int_to_string(char *str, int number);

char *pad_string(char *str, unsigned int max);

char *replace_char(char *str, char s, char r);

int find_substr(const char *str, const char *substr);

//char *replace_substring(char *str, char *s, char *r);

char *remove_char(char *str, char r);

void remove_spaces(char *str);

void remove_newlines(char *str);

//char *string_duplicate(char *str);

int n_count(int n) {
    int count = 0;
    while (n != 0) {
        count++;
        n /= 10;
    }
    return count;
}

/**
 * Convert int to string.
 * @param number The number to convert.
 * @return The string representation of the number.
 */
void int_to_string(char *str, int number) {
    sprintf(str, "%d", number);
}

/**
 * Pads the string with spaces at the end.
 * @param str The string.
 * @param max The max length of the string.
 * @return The padded string.
 */
char *pad_string(char *str, unsigned int max) {
    char *new_str = (char *) malloc(sizeof(char) * (max + 1));

    if (new_str == NULL) {
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");
    }

    strcpy(new_str, str);

    if (strcmp(str, new_str) != 0) {
        print_error(__FILENAME__, __LINE__, __func__, "strcpy failed");
    }

    for (unsigned int i = strlen(str); i < max; i++) {
        new_str[i] = ' ';
    }
    new_str[max] = '\0';
    return new_str;
}

// create a function to check is string contains only characters
bool is_string_only_characters(char *str) {
    for (unsigned int i = 0; i < strlen(str); i++) {
//        printf("%c\n", str[i]);
//        printf("%d - %d\n", isalpha(str[i]), isspace(str[i]));
        if (isalpha(str[i]) == 0 && isspace(str[i]) == 0) {
            return false;
        }
    }
    return true;
}

/**
 * Replaces all occurrences of a character in a string with another character.
 * @param str The string.
 * @param s The character to be replaced.
 * @param r The character to replace with.
 * @return The new string.
 */
char *replace_char(char *str, char s, char r) {
    char *new_str = (char *) malloc(sizeof(char) * (strlen(str) + 1));

    if (new_str == NULL) {
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");
    }

    strcpy(new_str, str);

    if (strcmp(str, new_str) != 0) {
        print_error(__FILENAME__, __LINE__, __func__, "strcpy failed");
    }

    for (unsigned int i = 0; i < strlen(str); i++) {
        if (str[i] == s) {
            new_str[i] = r;
        }
    }
    new_str[strlen(str)] = '\0';
    return new_str;
}

int find_substr(const char *str, const char *substr) {
    int i, j, k;

    for (i = 0; str[i] != '\0'; i++) {
        for (j = i, k = 0; substr[k] != '\0' && str[j] == substr[k]; j++, k++);

        if (substr[k] == '\0')
            return i;
    }

    return -1;
}


void replace_substring(char *dst, char *str, const char *substr,
                       const char *new_substr) {
    int i, j, k, l;
    int substr_len = (int) strlen(substr);
    int new_substr_len = (int) strlen(new_substr);
    int str_len = (int) strlen(str);

    int substr_pos = find_substr(str, substr);

    if (substr_pos == -1) {
        strcpy(dst, str);
        return;
    }

    char *new_str = malloc(str_len + 1);

    if (new_str == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");

    for (i = 0; i < substr_pos; i++)
        new_str[i] = str[i];

    for (j = 0, k = substr_pos, l = 0; l < new_substr_len; j++, k++, l++)
        new_str[k] = new_substr[l];

    for (i = substr_pos + substr_len; i < str_len; i++)
        new_str[k++] = str[i];

    new_str[k] = '\0';

    if (find_substr(new_str, substr) != -1) {
        replace_substring(new_str, new_str, substr, new_substr);
    }

    strcpy(dst, new_str);

    free(new_str);
}


/**
 * Removes spaces everywhere in the string.
 * @param str The string.
 * @return The string without spaces.
 */
char *remove_char(char *str, char r) {
    size_t len = strlen(str) + 1;
    char new_str[len];
    char *return_value = (char *) malloc(sizeof(char) * len);

    int j = 0;
    for (unsigned int i = 0; i < strlen(str); i++) {
        if (str[i] != r) {
            new_str[j] = str[i];
            j++;
        }
    }
    new_str[j] = '\0';
    strcpy(return_value, new_str);
    return return_value;
}

/**
 * Removes spaces from the string.
 * @param str The string.
 */
void remove_spaces(char *str) {
    char *new_str = remove_char(str, ' ');
    strcpy(str, new_str);

    if (strcmp(str, new_str) != 0)
        print_error(__FILENAME__, __LINE__, __func__, "strcpy failed");

    free(new_str);
}

/**
 * Removes new lines from the string.
 * @param str The string.
 */
void remove_newlines(char *str) {
    char *new_str = (char *) malloc(sizeof(char) * 31);
    strcpy(new_str, remove_char(str, '\n'));
    strcpy(new_str, remove_char(new_str, '\r'));
    strcpy(str, new_str);

    if (strcmp(str, new_str) != 0)
        print_error(__FILENAME__, __LINE__, __func__, "strcpy failed");

    free(new_str);
}


void string_duplicate(char *dst, char *str) {
    strcpy(dst, str);
}
/**
 * -----------------------------------------------------------------------------
 * VECTOR MODULE
 * -----------------------------------------------------------------------------
 */

/**
 * Vector data structure type
 */
typedef struct {
    int size;
    int capacity;
    char **elements;
} vector_t;

vector_t *vector_init(int capacity);

void vector_add(vector_t *v, char *s);

void vector_add_no_transform(vector_t *v, char *s);

bool vector_contains(vector_t *v, char *s);

void vector_remove(vector_t *v, int index);

void vector_foreach(vector_t *v, void (*f)(char *));

void vector_free(vector_t *v);

vector_t *string_to_vector(char *str, const char *delim);

/**
 * Initializes the vector
 * @param capacity The capacity of the vector.
 * @return The initialized vector.
 */
vector_t *vector_init(int capacity) {
    if (capacity < 1) {
        print_error(__FILENAME__, __LINE__, __func__,
                    "Capacity must be greater than 0");
    }

    vector_t *v = (vector_t *) malloc(sizeof(vector_t));

    if (v == NULL) {
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");
    }

    v->size = 0;
    v->capacity = capacity;
    v->elements = (char **) malloc(capacity * sizeof(char *));

    for (int i = 0; i < capacity; i++) {
        v->elements[i] = malloc(sizeof(char) * 31);
    }

    if (v->elements == NULL) {
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");
    }

    return v;
}

/**
 * Adds a string to the vector.
 * @param v The vector.
 * @param s The string.
 */
void vector_add(vector_t *v, char *s) {
    if (v->size == v->capacity) {
        v->capacity += 1;
        v->elements = (char **) realloc(v->elements,
                                        v->capacity * sizeof(char *));

        if (v->elements == NULL) {
            print_error(__FILENAME__, __LINE__, __func__, "Realloc failed");
        }

        v->elements[v->size] = malloc(sizeof(char) * 31);
    }

    strcpy(v->elements[v->size], remove_char(s, ' '));
    v->size++;
}

/**
 * Adds a string to the vector without transforming it.
 * @param v The vector.
 * @param s The string.
 */
void vector_add_no_transform(vector_t *v, char *s) {
    if (v->size == v->capacity) {
        v->capacity += 1;
        v->elements = (char **) realloc(v->elements,
                                        v->capacity * sizeof(char *));

        v->elements[v->size] = malloc(sizeof(char) * (strlen(s) + 1));

        if (v->elements == NULL) {
            print_error(__FILENAME__, __LINE__, __func__, "Realloc failed");
        }
    }

    strcpy(v->elements[v->size], s);
    v->size++;
}

void vector_copy(vector_t *dst, vector_t *src) {
    if (dst == NULL) {
        dst = vector_init(src->capacity);
    }

    dst->size = src->size;
    dst->capacity = src->capacity;
    dst->elements = (char **) malloc(src->capacity * sizeof(char *));

    for (int i = 0; i < src->capacity; i++) {
        dst->elements[i] = malloc(sizeof(char) * 31);
    }

    for (int i = 0; i < src->size; i++) {
        strcpy(dst->elements[i], src->elements[i]);
    }
}

bool vector_contains(vector_t *v, char *s) {
    for (int i = 0; i < v->size; i++) {
        if (strcmp(v->elements[i], s) == 0) {
            return true;
        }
    }
    return false;
}

/**
 * Remove element from the vector.
 * @param v The vector.
 * @param index The index of the element.
 */
void vector_remove(vector_t *v, int index) {
    if (index < 0 || index >= v->size) {
        print_error(__FILENAME__, __LINE__, __func__, "Index out of bounds");
    }

    for (int i = index; i < v->size - 1; i++) {
        v->elements[i] = v->elements[i + 1];
    }
    v->size--;
}

/**
 * Converts string to vector.
 * @param str String to convert.
 * @param delim Delimiter.
 * @return Vector with items from string separated by delimiter.
 */
vector_t *string_to_vector(char *str, const char *delim) {
    vector_t *v = vector_init(1);
    char *token = strtok(str, delim);

    if (token == NULL) {
        return v;
    }

    while (token != NULL) {
        vector_add_no_transform(v, token);
        token = strtok(NULL, delim);
    }

//    free(token);

    return v;
}

/**
 * Applies the function to each element of vector.
 * @param v The vector.
 * @param f The function.
 */
void vector_foreach(vector_t *v, void (*f)(char *)) {
    if (v == NULL) {
        print_error(__FILENAME__, __LINE__, __func__, "Vector is NULL");
    }

    if (f == NULL) {
        print_error(__FILENAME__, __LINE__, __func__, "Function is NULL");
    }

    for (int i = 0; i < v->size; i++) {
        f(v->elements[i]);
    }
}

// create a function to convert vector args to string with delimiter
char *vector_to_string(vector_t *v, char *delim) {
    char *str = (char *) malloc(sizeof(char) * (v->size + 1));

    if (str == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");

    str[0] = '\0';
    for (int i = 0; i < v->size; i++) {
        strcat(str, v->elements[i]);
        if (i != v->size - 1) {
            strcat(str, delim);
        }
    }
    return str;
}

/**
 * Frees the vector.
 * @param v The vector.
 */
void vector_free(vector_t *v) {
    if (v == NULL) {
        print_error(__FILENAME__, __LINE__, __func__, "Vector is NULL");
    }

    if (v->elements == NULL) {
        print_error(__FILENAME__, __LINE__, __func__,
                    "Vector elements are NULL");
    }

    for (int i = 0; i < v->size; i++) {
        if (v->elements[i] == NULL) {
            print_error(__FILENAME__, __LINE__, __func__,
                        "Vector element is NULL");
        }
    }

    for (int i = 0; i < v->size; i++) {
        free(v->elements[i]);
        v->elements[i] = NULL;
    }

    free(v->elements);
    v->elements = NULL;
    free(v);
}

/**
 * -----------------------------------------------------------------------------
 * SET MODULE [SET]
 * -----------------------------------------------------------------------------
 */

/**
 * Set type.
 */
typedef struct set {
    unsigned int index;
    int size;
    int capacity;
    char **elements;
} set_t;

set_t *set_init(int capacity);

set_t *set_init_indexed(int index, int capacity);

int set_item_index(set_t *s, char *item);

void set_add(set_t *s, char *e);

void set_add_row(set_t *s, int row);

void set_print(set_t *s);

void set_free(set_t *s);

/**
 * Creates a new set.
 * @param capacity The initial capacity of the set.
 * @return The new set.
 */
set_t *set_init(int capacity) {
    set_t *s = malloc(sizeof(set_t));

    if (s == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");

    s->index = 0;
    s->size = 0;
    s->capacity = capacity;
    s->elements = malloc(sizeof(char *) * capacity);

    if (s->elements == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");

    return s;
}

/**
 * Creates a new set.
 * @param index The index of the set.
 * @param capacity The initial capacity of the set.
 * @return The new set.
 */
set_t *set_init_indexed(int index, int capacity) {
    if (index < 1)
        print_error(__FILENAME__, __LINE__, __func__,
                    "Index is negative or zero");

    set_t *s = malloc(sizeof(set_t));

    if (s == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");

    s->index = index;
    s->size = 0;
    s->capacity = capacity;
    s->elements = malloc(sizeof(char *) * capacity);

    if (s->elements == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");

    return s;
}

/**
 * Returns the index of the item in the set.
 * @param s The set.
 * @param item The item.
 * @return The index of the item in the set.
 */
int set_item_index(set_t *s, char *item) {
    for (int i = 0; i < s->size; i++) {
        if (strcmp(s->elements[i], item) == 0) {
            return i;
        }
    }
    return -1;
}

/**
 * Adds an element to the set.
 * @param s The set.
 * @param e The element to add.
 */
void set_add(set_t *s, char *e) {
    if (s->size == s->capacity) {
        s->capacity += 1;
        s->elements = realloc(s->elements, sizeof(char *) * s->capacity);

        if (s->elements == NULL)
            print_error(__FILENAME__, __LINE__, __func__, "Realloc failed");
    }
    s->elements[s->size] = e;
    s->size++;
}

///**
// * Adds a row number to the set.
// * @param s The set.
// * @param e The element to add.
// */
//void set_add_row(set_t *s, int row) {
//    s->row = row;
//}

/**
 * Prints the set.
 * @param s The set.
 */
void set_print(set_t *s) {
    printf("Set %d: ", s->index);
    for (int i = 0; i < s->size; i++) {
        printf("%s ", s->elements[i]);
    }
    printf("\n");
}

void set_free(set_t *s) {
    if (s == NULL) {
        print_error(__FILENAME__, __LINE__, __func__, "Set is NULL");
    }

    if (s->elements == NULL) {
        print_error(__FILENAME__, __LINE__, __func__,
                    "Set elements are NULL");
    }

    for (int i = 0; i < s->size; i++) {
        if (s->elements[i] == NULL) {
            print_error(__FILENAME__, __LINE__, __func__,
                        "Set element is NULL");
        }
    }

    for (int i = 0; i < s->size; i++) {
        free(s->elements[i]);
    }
    free(s->elements);
    free(s);
}

/**
 * -----------------------------------------------------------------------------
 * SET MODULE [SET VECTOR]
 * -----------------------------------------------------------------------------
 */

/**
 * Set vector type.
 */
typedef struct set_vector_t {
    int size;
    int capacity;
    set_t **sets;
} set_vector_t;

set_vector_t *set_vector_init(int capacity);

void set_vector_add(set_vector_t *sv, set_t *s, unsigned int index);

set_t *set_vector_find(set_vector_t *sv, unsigned int index);

void set_vector_print(set_vector_t *sv);

void set_vector_free(set_vector_t *sv);

/**
 * Creates a new set vector.
 * @param capacity The initial capacity of the set vector.
 * @return The new set vector.
 */
set_vector_t *set_vector_init(int capacity) {
    if (capacity < 1)
        print_error(__FILENAME__, __LINE__, __func__, "Invalid capacity");

    set_vector_t *sv = malloc(sizeof(set_vector_t));

    if (sv == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");

    sv->size = 0;
    sv->capacity = capacity;
    sv->sets = malloc(sizeof(set_t *) * capacity);

    if (sv->sets == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");

    return sv;
}

/**
 * Adds a set to the set vector.
 * @param sv The set vector.
 * @param s The set to add.
 * @param index The index of the set.
 */
void set_vector_add(set_vector_t *sv, set_t *s, unsigned int index) {
    if (sv->size == sv->capacity) {
        sv->capacity += 1;
        sv->sets = realloc(sv->sets, sizeof(set_t *) * sv->capacity);

        if (sv->sets == NULL)
            print_error(__FILENAME__, __LINE__, __func__, "Realloc failed");
    }

    sv->sets[sv->size] = s;
    sv->sets[sv->size]->index = sv->sets[sv->size]->index
                                ? sv->sets[sv->size]->index : index;
    sv->size++;
}

/**
 * Finds a set in the set vector.
 * @param sv The set vector.
 * @param index The index of the set.
 * @return The set.
 */
set_t *set_vector_find(set_vector_t *sv, unsigned int index) {
    for (int i = 0; i < sv->size; i++) {
        if (sv->sets[i]->index == index) {
            return sv->sets[i];
        }
    }
    return NULL;
}

/**
 * Prints the set vector.
 * @param sv The set vector.
 */
void set_vector_print(set_vector_t *sv) {
    for (int i = 0; i < sv->size; i++) {
        set_print(sv->sets[i]);
    }
}

void set_vector_free(set_vector_t *sv) {
    for (int i = 0; i < sv->size; i++) {
        set_free(sv->sets[i]);
    }
    free(sv->sets);
    free(sv);
}

/**
 * -----------------------------------------------------------------------------
 * SET MODULE [SET MATH]
 * -----------------------------------------------------------------------------
 */

bool _set_is_empty(set_t *s);

bool set_is_empty(int n, ...);

int _set_card(set_t *s);

int set_card(int n, ...);

set_t *_set_complement(set_t *s1, set_t *s2);

set_t *set_complement(int n, ...);

set_t *_set_union(set_t *s1, set_t *s2);

set_t *set_union(int n, ...);

set_t *_set_intersection(set_t *s1, set_t *s2);

set_t *set_intersection(int n, ...);

set_t *_set_diff(set_t *s1, set_t *s2);

set_t *set_diff(int n, ...);

bool _set_is_subseteq(set_t *s1, set_t *s2);

bool set_is_subseteq(int n, ...);

bool _set_is_subset(set_t *s1, set_t *s2);

bool set_is_subset(int n, ...);

bool _set_is_equal(set_t *s1, set_t *s2);

bool set_is_equal(int n, ...);

/**
 * Checks if a set is empty.
 * @param s The set.
 * @return True if the set is empty, false otherwise.
 */
bool _set_is_empty(set_t *s) {
    return s->size == 0;
}
/**
 * Checks if a set is empty.
 * @param n The number of sets.
 * @param ... The sets.
 * @return True if the set is empty, false otherwise.
 */
bool set_is_empty(int n, ...) {
    va_list args;
    va_start(args, n);

    for (int i = 0; i < n; i++) {
        set_t *s = va_arg(args, set_t *);
        if (!_set_is_empty(s)) {
            va_end(args);
            return false;
        }
    }

    va_end(args);
    return true;
}

/**
 * Counts the number of elements in a set.
 * @param s The set.
 * @return The number of elements in the set.
 */
int _set_card(set_t *s) {
    return s->size;
}

/**
 * Counts the number of elements in a set.
 * @param n The number of sets.
 * @param ... The sets.
 * @return The number of elements in the set.
 */
int set_card(int n, ...) {
    va_list args;
    va_start(args, n);

    int count = 0;
    for (int i = 0; i < n; i++) {
        set_t *s = va_arg(args, set_t *);
        count += _set_card(s);
    }

    va_end(args);
    return count;
}

/**
 * Computes the complement of a set.
 * @param s1 The set.
 * @param s2 The set.
 * @return The complement of the set.
 */
set_t *_set_complement(set_t *s1, set_t *s2) {
    return _set_diff(s1, s2);
}

/**
 * Computes the complement of a set.
 * @param n The number of sets.
 * @param ... The sets.
 * @return The complement of the set.
 */
set_t *set_complement(int n, ...) {
    va_list args;
    va_start(args, n);

    set_t *s1 = va_arg(args, set_t *);
    set_t *s2 = va_arg(args, set_t *);

    va_end(args);
    return _set_complement(s1, s2);
}

/**
 * Returns the union of two sets.
 * @param s1 The first set.
 * @param s2 The second set.
 * @return The union of the two sets.
 */
set_t *_set_union(set_t *s1, set_t *s2) {
    set_t *s = set_init(s1->capacity + s2->capacity);

    for (int i = 0; i < s1->size; i++) {
        set_add(s, s1->elements[i]);
    }

    for (int i = 0; i < s2->size; i++) {
        bool is_in_set = false;

        for (int j = 0; j < s->size; j++) {
            if (strcmp(s2->elements[i], s->elements[j]) == 0) {
                is_in_set = true;
            }
        }

        if (!is_in_set) {
            set_add(s, s2->elements[i]);
        }
    }

    return s;
}

/**
 * Returns the union of sets.
 * @param n The number of sets.
 * @param ... The sets.
 * @return The union of the sets.
 */
set_t *set_union(int n, ...) {
    va_list sets;
    va_start(sets, n);

    set_t *s = set_init(1);

    for (int i = 0; i < n; i++) {
        set_t *s_i = va_arg(sets, set_t *);
        set_t *s_union = _set_union(s, s_i);
        s = s_union;
    }

    va_end(sets);

    return s;
}

/**
 * Returns the intersection of sets.
 * @param s1 The first set.
 * @param s2 The second set.
 * @return The intersection of the sets.
 */
set_t *_set_intersection(set_t *s1, set_t *s2) {
    set_t *s = set_init(s1->capacity + s2->capacity);

    for (int i = 0; i < s1->size; i++) {
        for (int j = 0; j < s2->size; j++) {
            if (strcmp(s1->elements[i], s2->elements[j]) == 0) {
                set_add(s, s1->elements[i]);
            }
        }
    }

    return s;
}

/**
 * Returns the intersection of sets.
 * @param n The number of sets.
 * @param ... The sets.
 * @return The intersection of the sets.
 */
set_t *set_intersection(int n, ...) {
    va_list sets;
    va_start(sets, n);

    set_t *s = va_arg(sets, set_t *);

    for (int i = 1; i < n; i++) {
        set_t *s_i = va_arg(sets, set_t *);
        set_t *s_intersection = _set_intersection(s, s_i);
        s = s_intersection;
    }

    va_end(sets);

    return s;
}

/**
 * Difference of two sets.
 * @param s1 The first set.
 * @param s2 The second set.
 * @return The difference of the two sets.
 */
set_t *_set_diff(set_t *s1, set_t *s2) {
    set_t *s = set_init(s1->capacity + s2->capacity);

    for (int i = 0; i < s1->size; i++) {
        bool is_in_set = false;

        for (int j = 0; j < s2->size; j++) {
            if (strcmp(s1->elements[i], s2->elements[j]) == 0) {
                is_in_set = true;
            }
        }

        if (!is_in_set) {
            set_add(s, s1->elements[i]);
        }
    }

    return s;
}

/**
 * Difference of sets.
 * @param n The number of sets.
 * @param ... The sets.
 * @return The difference of the sets.
 */
set_t *set_diff(int n, ...) {
    va_list sets;
    va_start(sets, n);

    set_t *s = va_arg(sets, set_t *);

    for (int i = 1; i < n; i++) {
        set_t *s_i = va_arg(sets, set_t *);
        set_t *s_diff = _set_diff(s, s_i);
        s = s_diff;
    }

    va_end(sets);

    return s;
}

/**
 * Checks if the first set is a subset of the second set.
 * @param s1 The first set.
 * @param s2 The second set.
 * @return true if the first set is a subset of the second set, false otherwise.
 */
bool _set_is_subseteq(set_t *s1, set_t *s2) {
    for (int i = 0; i < s1->size; i++) {
        bool is_in_set = false;

        for (int j = 0; j < s2->size; j++) {
            if (strcmp(s1->elements[i], s2->elements[j]) == 0) {
                is_in_set = true;
            }
        }

        if (!is_in_set) {
            return false;
        }
    }

    return true;
}

/**
 * Checks if the first set is a subset of the second set.
 * @param n The number of sets.
 * @param ... The sets.
 * @return true if the first set is a subset of the second set, false otherwise.
 */
bool set_is_subseteq(int n, ...) {
    va_list sets;
    va_start(sets, n);

    set_t *s1 = va_arg(sets, set_t *);
    set_t *s2 = va_arg(sets, set_t *);

    bool is_subset = _set_is_subseteq(s1, s2);

    va_end(sets);

    return is_subset;
}

/**
 * Checks if the first set is a superset of the second set.
 * @param s1 The first set.
 * @param s2 The second set.
 * @return true if the first set is a superset of the second set, false otherwise.
 */
bool _set_is_subset(set_t *s1, set_t *s2) {
    return _set_is_subseteq(s1, s2) && !_set_is_equal(s1, s2);
}

/**
 * Checks if the first set is a superset of the second set.
 * @param n The number of sets.
 * @param ... The sets.
 * @return
 */
bool set_is_subset(int n, ...) {
    va_list sets;
    va_start(sets, n);

    set_t *s1 = va_arg(sets, set_t *);
    set_t *s2 = va_arg(sets, set_t *);

    bool is_subset = _set_is_subset(s1, s2);

    va_end(sets);

    return is_subset;
}

/**
 * Checks if the first set is equal to the second set.
 * @param s1 The first set.
 * @param s2 The second set.
 * @return true if the first set is equal to the second set, false otherwise.
 */
bool _set_is_equal(set_t *s1, set_t *s2) {
    if (s1->size != s2->size) {
        return false;
    }

    for (int i = 0; i < s1->size; i++) {
        bool is_in_set = false;

        for (int j = 0; j < s2->size; j++) {
            if (strcmp(s1->elements[i], s2->elements[j]) == 0) {
                is_in_set = true;
            }
        }

        if (!is_in_set) {
            return false;
        }
    }
    return true;
}

/**
 * Checks if the first set is equal to the second set.
 * @param n The number of sets.
 * @param ... The sets.
 * @return true if the first set is equal to the second set, false otherwise.
 */
bool set_is_equal(int n, ...) {
    va_list sets;
    va_start(sets, n);

    set_t *s1 = va_arg(sets, set_t *);
    set_t *s2 = va_arg(sets, set_t *);

    bool is_equal = _set_is_equal(s1, s2);

    va_end(sets);

    return is_equal;
}

/**
 * Definition for relation_t
 */

typedef struct {
    char ***pairs;
    int count_pairs;
} relation_t;

typedef struct {
    char *element_a;
    char *element_b;
} new_relations_t;

new_relations_t *relation_init(char *element_a, char *element_b);

void relation_print(new_relations_t *r);

void relation_free(new_relations_t *r);

new_relations_t *relation_init(char *element_a, char *element_b) {
    new_relations_t *new_relations = malloc(sizeof(new_relations_t));

    new_relations->element_a = element_a;
    new_relations->element_b = element_b;

    return new_relations;
}

void relation_print(new_relations_t *r) {
    if (r == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Invalid pointer");

    printf("%s %s\n", r->element_a, r->element_b);
}

void relation_free(new_relations_t *r) {
    if (r == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Invalid pointer");

    free(r->element_a);
    free(r->element_b);
    free(r);
}

/**
* Definition for relations set
*/

typedef struct {
    unsigned int index;
    int size;
    int capacity;
    new_relations_t **relations;
} relation_set_t;

relation_set_t *relation_set_init(int capacity);

/**
 * Initializes a relation_set_t.
 * @param capacity The capacity of the relation_set_t.
 * @return The initialized relation_set_t.
 */
relation_set_t *relation_set_init(int capacity) {
    relation_set_t *rv = malloc(sizeof(relation_set_t));

    if (rv == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Invalid pointer");

    rv->index = 0;
    rv->size = 0;
    rv->capacity = capacity;
    rv->relations = malloc(sizeof(new_relations_t *) * capacity);

    return rv;
}

/**
 * Adds a new relation to the relation_set_t.
 * @param rv The relation_set_t.
 * @param r The relation_t.
 */
void relation_set_add_relation(relation_set_t *rv, new_relations_t *r) {
    if (rv->size == rv->capacity) {
        rv->capacity += 1;
        rv->relations = realloc(rv->relations,
                                sizeof(new_relations_t *) * rv->capacity);
        rv->relations[rv->size] = malloc(sizeof(new_relations_t));
    }

    rv->relations[rv->size] = r;
    rv->size++;
}

/**
 * Checks if the relation_set_t is injective.
 * @param rv The relation_set_t.
 * @param element_a The first element.
 * @param element_b The second element.
 */
void
relation_set_add(relation_set_t *rv, char *element_a, char *element_b) {
    if (rv->size == rv->capacity) {
        rv->capacity += 1;
        rv->relations = realloc(rv->relations,
                                sizeof(new_relations_t *) * rv->capacity);
    }

    new_relations_t *r = malloc(sizeof(new_relations_t));

    r->element_a = element_a;
    r->element_b = element_b;

    relation_set_add_relation(rv, r);
}

/**
 * Prints vector of relations.
 * @param rv The relation_set_t.
 */
void relation_set_print(relation_set_t *rv) {
    for (int i = 0; i < rv->size; i++) {
        printf("%s %s\n", rv->relations[i]->element_a,
               rv->relations[i]->element_b);
    }
}

/**
 * Frees the memory of the relation_set_t.
 * @param rv The relation_set_t.
 */
void relation_set_free(relation_set_t *rv) {
    for (int i = 0; i < rv->size; i++) {
        relation_free(rv->relations[i]);
    }

    free(rv->relations);
    free(rv);
}

/**
 * Relation vector definition
 */
typedef struct {
    int size;
    int capacity;
    relation_set_t **relations;
} relation_vector_t;

relation_vector_t *relation_vector_init(int capacity);

void relation_vector_add(relation_vector_t *rv, relation_set_t *s,
                         unsigned int index);

relation_set_t *relation_vector_find(relation_vector_t *rv, unsigned int index);

void
relation_vector_add_relation_set(relation_vector_t *rv, relation_set_t *rs);

void relation_vector_print(relation_vector_t *rv);

void relation_vector_free(relation_vector_t *rv);


/**
 * Initializes a relation_vector_t.
 * @param capacity The capacity of the relation_vector_t.
 * @return The initialized relation_vector_t.
 */
relation_vector_t *relation_vector_init(int capacity) {
    if (capacity < 0)
        print_error(__FILENAME__, __LINE__, __func__, "Invalid capacity");

    relation_vector_t *rv = malloc(sizeof(relation_vector_t));

    if (rv == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Invalid pointer");

    rv->size = 0;
    rv->capacity = capacity;
    rv->relations = malloc(sizeof(relation_set_t *) * capacity);

    return rv;
}


void relation_vector_add(relation_vector_t *rv, relation_set_t *rs,
                         unsigned int index) {
    if (rv->size == rv->capacity) {
        rv->capacity += 1;
        rv->relations = realloc(rv->relations,
                                sizeof(relation_set_t *) * rv->capacity);
    }

    rv->relations[rv->size] = rs;
    rv->relations[rv->size]->index = rv->relations[rv->size]->index ?
                                     rv->relations[rv->size]->index : index;
    rv->size++;

}

relation_set_t *
relation_vector_find(relation_vector_t *rv, unsigned int index) {
    for (int i = 0; i < rv->size; i++) {
        if (rv->relations[i]->index == index) {
            return rv->relations[i];
        }
    }

    return relation_set_init(0);
}

/**
 * Adds a new relation to the relation_vector_t.
 * @param rv The relation_vector_t.
 * @param rs The relation_set_t.
 */
void
relation_vector_add_relation_set(relation_vector_t *rv, relation_set_t *rs) {
    if (rv->size == rv->capacity) {
        rv->capacity += 1;
        rv->relations = realloc(rv->relations,
                                sizeof(relation_set_t *) * rv->capacity);
    }

    rv->relations[rv->size] = rs;
    rv->size++;
}

/**
 * Prints vector of relations.
 * @param rv The relation_vector_t.
 */
void relation_vector_print(relation_vector_t *rv) {
    for (int i = 0; i < rv->size; i++) {
        relation_set_print(rv->relations[i]);
        printf("\n");
    }
}

/**
 * Prints vector of relations.
 * @param rv The relation_vector_t.
 */
void relation_vector_free(relation_vector_t *rv) {
    for (int i = 0; i < rv->size; i++) {
        relation_set_free(rv->relations[i]);
    }
    free(rv->relations);
    free(rv);
}

/**
 * Definition for relation table
 */
typedef struct {
    int rows;
    int columns;
    set_t *row_items;
    set_t *column_items;
    int **matrix;
} relation_table_t;

relation_table_t *relation_table_init(set_t *row_items, set_t *column_items);

relation_table_t *relation_table_init_relation(set_t *row_items,
                                               set_t *column_items,
                                               relation_set_t *rv);

void relation_table_add_relation(relation_table_t *rt, new_relations_t *r);

void relation_table_remove_relation(relation_table_t *rt, new_relations_t *r);

void relation_table_print(relation_table_t *rt);

void relation_table_print_with_names(relation_table_t *rt);

/**
 * Initializes a relation_table_t.
 * @param row_items The set of row items.
 * @param column_items The set of column items.
 * @return The initialized relation_table_t.
 */
relation_table_t *relation_table_init(set_t *row_items, set_t *column_items) {
    relation_table_t *rt = malloc(sizeof(relation_table_t));

    rt->rows = row_items->size;
    rt->columns = column_items->size;
    rt->row_items = row_items;
    rt->column_items = column_items;
    rt->matrix = malloc(sizeof(int *) * rt->rows);

    for (int i = 0; i < rt->rows; i++) {
        rt->matrix[i] = malloc(sizeof(int) * rt->columns);

        for (int j = 0; j < rt->columns; j++) {
            rt->matrix[i][j] = 0;
        }
    }

    return rt;
}

/**
 * Initializes a relation_table_t using relation.
 * @param row_items The set of row items.
 * @param column_items The set of column items.
 * @param rv The relation_set_t.
 * @return The initialized relation_table_t.
 */
relation_table_t *relation_table_init_relation(set_t *row_items,
                                               set_t *column_items,
                                               relation_set_t *rv) {
    relation_table_t *rt = relation_table_init(row_items, column_items);

    for (int i = 0; i < rv->size; i++) {
        new_relations_t *r = rv->relations[i];

        int row_index = set_item_index(rt->row_items, r->element_a);
        int column_index = set_item_index(rt->column_items, r->element_b);

        rt->matrix[row_index][column_index] = 1;
    }

    return rt;
}

void relation_table_add_relation(relation_table_t *rt, new_relations_t *r) {
    int row_index = set_item_index(rt->row_items, r->element_a);
    int column_index = set_item_index(rt->column_items, r->element_b);

    rt->matrix[row_index][column_index] = 1;
}

void relation_table_remove_relation(relation_table_t *rt, new_relations_t *r) {
    int row_index = set_item_index(rt->row_items, r->element_a);
    int column_index = set_item_index(rt->column_items, r->element_b);

    rt->matrix[row_index][column_index] = 0;
}

void relation_table_print(relation_table_t *rt) {
    for (int i = 0; i < rt->rows; i++) {
        for (int j = 0; j < rt->columns; j++) {
            printf("%d ", rt->matrix[i][j]);
        }
        printf("\n");
    }
}

// create a function to print relation table with row and column names
void relation_table_print_with_names(relation_table_t *rt) {
    printf("R ");
    for (int i = 0; i < rt->columns; i++) {
        if (i < rt->columns) {
            printf("%s ", rt->column_items->elements[i]);
        }
    }
    printf("\n");
    for (int i = 0; i < rt->rows; i++) {
        printf("%s ", rt->row_items->elements[i]);
        for (int j = 0; j < rt->columns; j++) {
            printf("%d ", rt->matrix[i][j]);
        }
        printf("\n");
    }
}

/**
 * Relation math
 */

void rel_table(int **table, relation_t *rel_arr, set_t *univerzum);

bool _relation_is_reflexive(relation_set_t *rv, set_t *universe);

bool relation_is_reflexive(int n, ...);

bool _relation_is_symmetric(relation_set_t *rv, set_t *universe);

bool relation_is_symmetric(int n, ...);

bool _relation_is_antisymmetric(relation_set_t *rv, set_t *universe);

bool relation_is_antisymmetric(int n, ...);

bool _relation_is_transitive(relation_set_t *rv, set_t *universe);

bool relation_is_transitive(int n, ...);

bool _relation_is_function(relation_set_t *rv, set_t *universe);

bool relation_is_function(int n, ...);

set_t *_relation_domain(relation_set_t *rv, set_t *universe);

set_t *relation_domain(int n, ...);

set_t *_relation_codomain(relation_set_t *rv, set_t *universe);

set_t *relation_codomain(int n, ...);

bool _relation_is_injective(relation_set_t *rv, set_t *s1, set_t *s2);

bool relation_is_injective(int n, ...);

bool _relation_is_surjective(relation_set_t *rv, set_t *s1, set_t *s2);

bool relation_is_surjective(int n, ...);

bool _relation_is_bijective(relation_set_t *rv, set_t *s1, set_t *s2);

bool relation_is_bijective(int n, ...);

relation_set_t *_closure_ref(relation_set_t *rv, set_t *universe);

relation_set_t *closure_ref(int n, ...);

relation_set_t *_closure_sym(relation_set_t *rv, set_t *universe);

relation_set_t *closure_sym(int n, ...);

relation_set_t *_closure_trans(relation_set_t *rv, set_t *universe);

relation_set_t *closure_trans(int n, ...);


/**
 * table of 0 and 1 for relation_t
 */
void rel_table(int **table, relation_t *rel_arr, set_t *universe) {
    for (int i = 0; i < universe->size; i++) {
        for (int j = 0; j < universe->size; j++) {
            table[i][j] = 0;
        }
    }
    for (int i = 0; i < rel_arr->count_pairs; i++) {
        for (int j = 0; j < universe->size; j++) {
            if (strcmp(rel_arr->pairs[i][0], universe->elements[j]) == 0) {
                for (int k = 0; k < universe->size; k++) {
                    if (strcmp(rel_arr->pairs[i][1], universe->elements[k]) ==
                        0) {
                        table[j][k] = 1;
                        break;
                    }
                }
                break;
            }
        }
    }
}

/**
 * Checks if the relation is reflexive.
 * @param rel_arr The array of relation pairs.
 * @param universe The universe.
 * @return true if the relation is reflexive, false otherwise.
 */
bool _relation_is_reflexive(relation_set_t *rv, set_t *universe) {
    if (universe->size == 0 && rv->size == 0) {
        return true;
    }
    relation_table_t *rt = relation_table_init_relation(
            universe, universe, rv);

    for (int i = 0; i < universe->size; i++) {
        if (rt->matrix[i][i] != 1) {
            return false;
        }
    }

    return true;
}

/**
 * Checks if the relation is reflexive.
 * @param n The number of relation_t.
 * @param ... The rows of relation_t and universe.
 * @return True if the relation is reflexive, false otherwise.
 */
bool relation_is_reflexive(int n, ...) {
    va_list args;
    va_start(args, n);

    relation_set_t *rv = va_arg(args, relation_set_t *);
    set_t *univerzum = va_arg(args, set_t *);

    bool result = _relation_is_reflexive(rv, univerzum);

    va_end(args);

    return result;
}

/**
 * Checks if the relation is symmetric.
 * @param rel_arr The array of relation pairs.
 * @param universe The universe.
 * @return true if the relation is symmetric, false otherwise.
 */
bool _relation_is_symmetric(relation_set_t *rv, set_t *universe) {
    if (universe->size == 0 && rv->size == 0) {
        return true;
    }
    relation_table_t *rt = relation_table_init_relation(
            universe, universe, rv);

    for (int i = 0; i < universe->size; i++) {
        for (int j = 0; j < universe->size; j++) {
            if (rt->matrix[i][j] == 1 && rt->matrix[j][i] == 0) {
                return false;
            }
        }
    }

    return true;
}

/**
 * Checks if the relation is symmetric.
 * @param n The number of relation_t.
 * @param ... The rows of relation_t and universe.
 * @return True if the relation is symmetric, false otherwise.
 */
bool relation_is_symmetric(int n, ...) {
    va_list args;
    va_start(args, n);

    relation_set_t *rv = va_arg(args, relation_set_t *);
    set_t *universe = va_arg(args, set_t *);

    bool result = _relation_is_symmetric(rv, universe);

    va_end(args);

    return result;
}

/**
 * Checks if the relation is antisymmetric.
 * @param rel_arr The array of relation pairs.
 * @param universe The universe.
 * @return true if the relation is antisymmetric, false otherwise.
 */
bool _relation_is_antisymmetric(relation_set_t *rv, set_t *universe) {
    if (universe->size == 0 && rv->size == 0) {
        return true;
    }
    relation_table_t *rt = relation_table_init_relation(
            universe, universe, rv);

    for (int i = 0; i < universe->size; i++) {
        for (int j = 0; j < universe->size; j++) {
            if (rt->matrix[i][j] == 1 && rt->matrix[j][i] == 1 && i != j) {
                return false;
            }
        }
    }

    return true;
}

/**
 * Checks if the relation is antisymmetric.
 * @param n The number of relation_t.
 * @param ... The rows of relation_t and universe.
 * @return True if the relation is antisymmetric, false otherwise.
 */
bool relation_is_antisymmetric(int n, ...) {
    va_list args;
    va_start(args, n);

    relation_set_t *rv = va_arg(args, relation_set_t *);
    set_t *universe = va_arg(args, set_t *);

    bool result = _relation_is_antisymmetric(rv, universe);

    va_end(args);

    return result;
}

/**
 * Checks if the relation is transitive.
 * @param rel_arr The array of relation pairs.
 * @param universe The universe.
 * @return true if the relation is transitive, false otherwise.
 */
bool _relation_is_transitive(relation_set_t *rv, set_t *universe) {
    if (universe->size == 0 && rv->size == 0) {
        return true;
    }
    relation_table_t *rt = relation_table_init_relation(
            universe, universe, rv);

    for (int i = 0; i < universe->size; i++) {
        for (int j = 0; j < universe->size; j++) {
            for (int k = 0; k < universe->size; k++) {
                if (rt->matrix[i][j] == 1 && rt->matrix[j][k] == 1 &&
                    rt->matrix[i][k] == 0) {
                    return false;
                }
            }
        }
    }

    return true;
}

/**
 * Checks if the relation is transitive.
 * @param n The number of relation_t.
 * @param ... The rows of relation_t and universe.
 * @return True if the relation is transitive, false otherwise.
 */
bool relation_is_transitive(int n, ...) {
    va_list args;
    va_start(args, n);

    relation_set_t *rv = va_arg(args, relation_set_t *);
    set_t *universe = va_arg(args, set_t *);

    bool result = _relation_is_transitive(rv, universe);

    va_end(args);

    return result;
}

/**
 * Checks if the relation is function.
 * @param rel_arr The array of relation pairs.
 * @param universe The universe.
 * @return true if the relation is function, false otherwise.
 */
bool _relation_is_function(relation_set_t *rv, set_t *universe) {
    if (universe->size == 0 && rv->size == 0) {
        return true;
    }
    relation_table_t *rt = relation_table_init_relation(
            universe, universe, rv);

    for (int i = 0; i < universe->size; i++) {
        int total_in_row = 0;
        for (int j = 0; j < universe->size; j++) {
            if (rt->matrix[i][j] == 1) {
                total_in_row++;
            }
            if (total_in_row == 2) {
                return false;
            }
        }
    }

    return true;
}
/**
 * Checks if the relation is a function.
 * @param n The number of relation_t.
 * @param ... The rows of relation_t and universe.
 * @return True if the relation is a function, false otherwise.
 */
bool relation_is_function(int n, ...) {
    va_list args;
    va_start(args, n);

    relation_set_t *rv = va_arg(args, relation_set_t *);
    set_t *universe = va_arg(args, set_t *);

    bool result = _relation_is_function(rv, universe);

    va_end(args);

    return result;
}

/**
 * Finds domain of the relation
 * @param rel_arr The array of relation pairs.
 * @param universe The universe.
 */
set_t *_relation_domain(relation_set_t *rv, set_t *universe) {
    set_t *domain = set_init(0);

    for (int i = 0; i < rv->size; i++) {
        int flag = 0;
        for (int j = 0; j < universe->size; j++) {
            if (strcmp(rv->relations[i]->element_a, universe->elements[j]) ==
                0) {
                for (int k = 0; k < domain->size; k++) {
                    if (strcmp(rv->relations[i]->element_a,
                               domain->elements[k]) == 0) {
                        flag = 1;
                        break;
                    }
                }
                if (flag == 0) {
                    set_add(domain, rv->relations[i]->element_a);
                }
                break;
            }
        }
    }

    return domain;
}

/**
 * Finds domain of the relation_t.
 * @param n The number of relation_t.
 * @param ... The rows of relation_t and universe.
 */
set_t *relation_domain(int n, ...) {
    va_list args;
    va_start(args, n);

    relation_set_t *rv = va_arg(args, relation_set_t *);
    set_t *universe = va_arg(args, set_t *);

    set_t *result = _relation_domain(rv, universe);
    va_end(args);

    return result;
}

/**
 * Finds codomain of the relation
 * @param rel_arr The array of relation pairs.
 * @param universe The universe.
 */
set_t *_relation_codomain(relation_set_t *rv, set_t *universe) {
    set_t *codomain = set_init(0);

    for (int i = 0; i < rv->size; i++) {
        int flag = 0;
        for (int j = 0; j < universe->size; j++) {
            if (strcmp(rv->relations[i]->element_b, universe->elements[j]) ==
                0) {
                for (int k = 0; k < codomain->size; k++) {
                    if (strcmp(rv->relations[i]->element_b,
                               codomain->elements[k]) ==
                        0) {
                        flag = 1;
                        break;
                    }
                }
                if (flag == 0) {
                    set_add(codomain, rv->relations[i]->element_b);
                }
                break;
            }
        }
    }

    return codomain;
}

/**
 * Finds codomain of the relation_t.
 * @param n The number of relation_t.
 * @param ... The rows of relation_t and universe.
 */
set_t *relation_codomain(int n, ...) {
    va_list args;
    va_start(args, n);

    relation_set_t *rv = va_arg(args, relation_set_t *);
    set_t *universe = va_arg(args, set_t *);

    set_t *result = _relation_codomain(rv, universe);
    va_end(args);

    return result;
}

/**
 * Checks if the relation is injective.
 * @param rel_arr The relation
 * @param s1 The first set (domain)
 * @param s2 The second set (codomain)
 * @return True if the relation is injective, false otherwise.
 */
bool _relation_is_injective(relation_set_t *rv, set_t *s1, set_t *s2) {
    for (int i = 0; i < rv->size; i++) {
        if (set_item_index(s1, rv->relations[i]->element_a) == -1 ||
            set_item_index(s2, rv->relations[i]->element_b) == -1) {
            return false;
        }
    }


    relation_table_t *rt = relation_table_init_relation(
            s1, s2, rv);

    for (int i = 0; i < s1->size; i++) {
        int total_in_row = 0;
        for (int j = 0; j < s2->size; j++) {
            if (rt->matrix[i][j] == 1) {
                total_in_row++;
            }
        }
        if (total_in_row != 1) {
            return false;
        }
    }
    for (int i = 0; i < s2->size; i++) {
        int total_in_column = 0;
        for (int j = 0; j < s1->size; j++) {
            if (rt->matrix[j][i] == 1) {
                total_in_column++;
            }
            if (total_in_column > 1) {
                return false;
            }
        }
    }
    return true;
}

/**
 * Checks if the relation is injective.
 * @param n The number of relation_t.
 * @param ... The rows of relation_t and sets.
 * @return True if the relation is injective, false otherwise.
 */
bool relation_is_injective(int n, ...) {
    va_list args;
    va_start(args, n);

    relation_set_t *rv = va_arg(args, relation_set_t *);
    set_t *s1 = va_arg(args, set_t *);
    set_t *s2 = va_arg(args, set_t *);

    bool result = _relation_is_injective(rv, s1, s2);

    va_end(args);

    return result;
}

/**
 * Checks if the relation is surjective.
 * @param rel_arr The relation
 * @param s1 The first set (domain)
 * @param s2 The second set (codomain)
 * @return True if the relation is surjective, false otherwise.
 */
bool _relation_is_surjective(relation_set_t *rv, set_t *s1, set_t *s2) {
    for (int i = 0; i < rv->size; i++) {
        if (set_item_index(s1, rv->relations[i]->element_a) == -1 ||
            set_item_index(s2, rv->relations[i]->element_b) == -1) {
            return false;
        }
    }

    relation_table_t *rt = relation_table_init_relation(
            s1, s2, rv);

    for (int i = 0; i < s1->size; i++) {
        int total_in_row = 0;
        for (int j = 0; j < s2->size; j++) {
            if (rt->matrix[i][j] == 1) {
                total_in_row++;
            }
        }
        if (total_in_row != 1) {     //it has to be a function as well
            return false;
        }
    }
    for (int i = 0; i < s2->size; i++) {
        int total_in_column = 0;
        for (int j = 0; j < s1->size; j++) {
            if (rt->matrix[j][i] == 1) {
                total_in_column++;
            }
        }
        if (total_in_column == 0) {
            return false;
        }
    }
    return true;
}

/**
 * Checks if the relation is surjective.
 * @param n The number of relation_t.
 * @param ... The rows of relation_t and sets.
 * @return True if the relation is surjective, false otherwise.
 */
bool relation_is_surjective(int n, ...) {
    va_list args;
    va_start(args, n);

    relation_set_t *rv = va_arg(args, relation_set_t *);
    set_t *s1 = va_arg(args, set_t *);
    set_t *s2 = va_arg(args, set_t *);

    bool result = _relation_is_surjective(rv, s1, s2);

    va_end(args);

    return result;
}

/**
 * Checks if the relation is bijective.
 * @param rel_arr The relation
 * @param s1 The first set (domain)
 * @param s2 The second set (codomain)
 * @return True if the relation is bijective, false otherwise.
 */
bool _relation_is_bijective(relation_set_t *rv, set_t *s1, set_t *s2) {
    if (_relation_is_injective(rv, s1, s2) == true &&
        _relation_is_surjective(rv, s1, s2) == true) {
        return true;
    } else {
        return false;
    }
}

/**
 * Checks if the relation is bijective.
 * @param n The number of relation_t.
 * @param ... The rows of relation_t and sets.
 * @return True if the relation is bijective, false otherwise.
 */
bool relation_is_bijective(int n, ...) {
    va_list args;
    va_start(args, n);

    relation_set_t *rv = va_arg(args, relation_set_t *);
    set_t *s1 = va_arg(args, set_t *);
    set_t *s2 = va_arg(args, set_t *);

    bool result = _relation_is_bijective(rv, s1, s2);

    va_end(args);

    return result;
}

/**
 * Finds the reflexive closure of the relation.
 * @param rv The relation set.
 * @param universe The universe.
 * @return The reflexive closure of the relation.
 */
relation_set_t *_closure_ref(relation_set_t *rv, set_t *universe) {
    relation_table_t *rt = relation_table_init_relation(
            universe, universe, rv);

    for (int i = 0; i < universe->size; i++) {
        if (rt->matrix[i][i] == 0) {
            rt->matrix[i][i] = 1;
            new_relations_t *newrel = relation_init(universe->elements[i],
                                                    universe->elements[i]);
            relation_set_add_relation(rv, newrel);
        }
    }
    return rv;
}

/**
 * Finds the reflexive closure of the relation.
 * @param n The number of arguments.
 * @param ... The set of relation and the universe.
 * @return the reflexive closure of the relation.
 */
relation_set_t *closure_ref(int n, ...) {
    va_list args;
    va_start(args, n);

    relation_set_t *rv = va_arg(args, relation_set_t *);
    set_t *universe = va_arg(args, set_t*);

    relation_set_t *result = _closure_ref(rv, universe);
    va_end(args);

    return result;
}

/**
 * Finds the symmetric closure of the relation.
 * @param rv The relation set.
 * @param universe The universe.
 * @return The symmetric closure of the relation.
 */
relation_set_t *_closure_sym(relation_set_t *rv, set_t *universe) {
    relation_table_t *rt = relation_table_init_relation(universe, universe, rv);

    for (int i = 0; i < universe->size; i++) {
        for (int j = 0; j < universe->size; j++) {
            if (rt->matrix[i][j] == 1 && rt->matrix[j][i] == 0) {
                rt->matrix[j][i] = 1;
                new_relations_t *newrel = relation_init(universe->elements[j],
                                                        universe->elements[i]);
                relation_set_add_relation(rv, newrel);
            }
        }
    }
    return rv;
}

/**
 * Finds the symmetric closure of the relation.
 * @param n The number of arguments.
 * @param ... The set of relation and the universe.
 * @return the symmetric closure of the relation.
 */
relation_set_t *closure_sym(int n, ...) {
    va_list args;
    va_start(args, n);

    relation_set_t *rv = va_arg(args, relation_set_t *);
    set_t *universe = va_arg(args, set_t*);

    relation_set_t *result = _closure_sym(rv, universe);
    va_end(args);

    return result;
}

/**
 * Finds the transitive closure of the relation.
 * @param rv The relation set.
 * @param universe The universe.
 * @return The transitive closure of the relation.
 */
relation_set_t *_closure_trans(relation_set_t *rv, set_t *universe) {
    relation_table_t *rt = relation_table_init_relation(universe, universe, rv);

    for (int i = 0; i < universe->size; i++) {
        for (int j = 0; j < universe->size; j++) {
            for (int k = 0; k < universe->size; k++) {
                if (rt->matrix[i][j] == 1 && rt->matrix[j][k] == 1 &&
                    rt->matrix[i][k] == 0) {
                    rt->matrix[i][k] = 1;
                    new_relations_t *newrel = relation_init(
                            universe->elements[i], universe->elements[k]);
                    relation_set_add_relation(rv, newrel);
                }
            }
        }
    }
    return rv;
}

/**
 * Finds the transitive closure of the relation.
 * @param n The number of arguments.
 * @param ... The set of relation and the universe.
 * @return the transitive closure of the relation.
 */
relation_set_t *closure_trans(int n, ...) {
    va_list args;
    va_start(args, n);

    relation_set_t *rv = va_arg(args, relation_set_t *);
    set_t *universe = va_arg(args, set_t*);

    relation_set_t *result = _closure_trans(rv, universe);
    va_end(args);

    return result;
}


/**
 * -----------------------------------------------------------------------------
 * COMMAND MODULE [COMMAND]
 * -----------------------------------------------------------------------------
 */

/**
 * Enumerates command types
 */
typedef enum {
    U = 'U',
    S = 'S',
    R = 'R',
    C = 'C',
} commands;

/**
 * Operation definition
 */

typedef struct {
    char *name;
    commands type;
    int argc;
} operation;

operation *operation_init(char *name, commands type, int argc);

void operation_free(operation *o);

/**
 * Operation definition
 */

typedef struct operation_vector_t {
    int size;
    int capacity;
    operation **operations;
} operation_vector_t;

operation_vector_t *operation_vector_init(int capacity);

void operation_vector_add(operation_vector_t *ov, operation *o);

operation *operation_vector_find(operation_vector_t *ov, char *name);

operation_vector_t *operation_vector_find_all(operation_vector_t *ov,
                                              char *name);

bool operation_vector_has(operation_vector_t *ov, char *name);

bool operation_vector_has_name_type(operation_vector_t *ov, char *name,
                                    commands type);

bool operation_vector_contains(operation_vector_t *ov, char *name);

void operation_vector_free(operation_vector_t *ov);

/**
 * Command definition
 */

typedef struct {
    commands type;
    vector_t *args;
} command_t;

command_t *init_command();

command_t *init_command_with_type(commands type);

void set_command_args(command_t *c, vector_t *args);

void add_command_arg(command_t *c, char *arg);

set_t *command_to_set(command_t *c);

command_t *command_copy(command_t *c);

void print_command(command_t *c);

void free_command(command_t *c);

commands get_command_type_from_char(char c);

command_t *parse_command(char *s);

command_t *bool_to_command(bool b);

command_t *int_to_command(int i);

command_t *set_to_command(set_t *s);

relation_set_t *command_to_relation_set(command_t *c);

/**
 * Command system definition
 */

typedef struct command_system_t command_system_t;

command_system_t *command_system_init(char *filename);

void command_system_init_base(command_system_t *cs);

void command_system_validate(command_system_t *cs);

void command_system_init_vectors(command_system_t *cs);

void command_system_exec(command_system_t *cs);

void command_system_free(command_system_t *cs);

/**
 * Command vector definition
 */
typedef struct {
    int size;
    int capacity;
    command_t **commands;
    command_system_t *system;
} command_vector_t;

command_vector_t *command_vector_init(int capacity);

void command_vector_add(command_vector_t *cv, command_t *c);

void command_vector_replace(command_vector_t *cv, command_t *c, int index);

bool validate_command_vector(command_vector_t *cv, operation_vector_t *ov);

void attach_command_system(command_vector_t *cv, command_system_t *cs);

vector_t *get_unique_command_types(command_vector_t *cv);

command_t *get_command_by_index(command_vector_t *cv, int index);

command_t *find_command_by_type(command_vector_t *cv, commands type);

command_vector_t *find_command_by_type_all(command_vector_t *cv, commands type);

command_vector_t *
command_vector_slice(command_vector_t *cv, int start, int end);

bool command_vector_contains_type(command_vector_t *cv, commands type);

void command_vector_print(command_vector_t *cv);

void command_vector_free(command_vector_t *cv);

command_vector_t *parse_file(char *filename);

typedef struct command_system_t {
    char *filename;
    command_vector_t *cv;
    operation_vector_t *operation_vector;
    set_t *universe;
    set_vector_t *set_vector;
    relation_vector_t *relation_vector;
} command_system_t;

/**
 * Initializes a command.
 * @return The initialized command.
 */
command_t *init_command() {
    command_t *c = (command_t *) malloc(sizeof(command_t));

    if (c == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");

    c->type = 0;
    c->args = vector_init(4);
    return c;
}

/**
 * Initializes a command with a type.
 * @param type The type of the command.
 * @return The initialized command.
 */
command_t *init_command_with_type(commands type) {
    command_t *c = init_command();
    c->type = type;
    return c;
}


/**
 * Sets the command arguments.
 * @param c The command.
 * @param args The arguments.
 */
void set_command_args(command_t *c, vector_t *args) {
    if (args == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Args is NULL");

    if (c->args == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Command args is NULL");

    c->args = args;
}

/**
 * Adds a command argument.
 * @param c The command.
 * @param arg The argument.
 */
void add_command_arg(command_t *c, char *arg) {
    if (c->args == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Command args is NULL");

    vector_add(c->args, arg);
}

/**
 * Converts a command to a set.
 * @param c The command.
 * @return The set.
 */
set_t *command_to_set(command_t *c) {
    set_t *s = set_init(1);
    s->size = c->args->size;
    s->capacity = c->args->capacity;
    s->elements = (char **) malloc(sizeof(char *) * c->args->size);

    if (c->type != U && c->type != S)
        print_error(__FILENAME__, __LINE__, __func__,
                    "Command type is not U or S");

    if (s->elements == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");

    for (int i = 0; i < c->args->size; i++) {
        s->elements[i] = c->args->elements[i];
    }

    return s;
}

/**
 * Copies a command.
 * @param c The command.
 * @return The copy of the command.
 */
command_t *command_copy(command_t *c) {
    command_t *c_copy = init_command_with_type(c->type);

    for (int i = 0; i < c->args->size; i++) {
        add_command_arg(c_copy, c->args->elements[i]);
    }

    return c_copy;
}

/**
 * Prints the command.
 * @param c The command.
 */
void print_command(command_t *c) {
    switch (c->type) {
        case U:
            printf("U");
            break;
        case S:
            printf("S");
            break;
        case R:
            printf("R");
            break;
        case C:
            printf("C");
            break;
    }

    if (c->args->size > 0 && c->type != 0) {
        printf(" ");
    }

    for (int i = 0; i < c->args->size; i++) {
        if (c->type == R)printf("(");

        printf("%s", (char *) c->args->elements[i]);

        if (c->type == R)printf(")");

        if (i < c->args->size - 1)
            printf(" ");
    }
    printf("\n");
}

/**
 * Frees the command.
 * @param c The command.
 */
void free_command(command_t *c) {
    if (c == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Command is NULL");

    vector_free(c->args);
    free(c);
}

/**
 * Initializes a command vector.
 * @param c capacity The capacity of the vector.
 * @return The initialized command vector.
 */
commands get_command_type_from_char(char c) {
    switch (c) {
        case 'U':
            return U;
        case 'S':
            return S;
        case 'R':
            return R;
        case 'C':
            return C;
        default:
            return 0;
    }
}

command_t *parse_relation_command(char *str) {
    char *rel_string = malloc(sizeof(char) * strlen(str));
    replace_substring(rel_string, str, ") (", ")/(");

    remove_newlines(rel_string);

    vector_t *v = string_to_vector(rel_string, ")/(");

    command_t *c = init_command();
    vector_t *args = vector_init(1);

    c->type = get_command_type_from_char(v->elements[0][0]);

    if (c->type == 0)
        print_error(__FILENAME__, __LINE__, __func__, "Invalid command");

    for (int i = 1; i < v->size; i++) {
        char *rel_str = malloc(sizeof(char) * (strlen(v->elements[i]) + 1));
        string_duplicate(rel_str, v->elements[i]);
        vector_add_no_transform(args, rel_str);
        free(rel_str);
    }

    vector_foreach(args, remove_newlines);
    c->args = args;

    return c;
}

/**
 * Parses a command.
 * @param s The command string.
 * @return The command.
 */
command_t *parse_command(char *s) {
    if (s[1] != ' ' && s[1] != '\0' && s[1] != '\n' && s[1] != '\r')
        print_error(__FILENAME__, __LINE__, __func__, "Invalid command");

    if (s[0] == 'R') {
        return parse_relation_command(s);
    }

    command_t *c = init_command();
    vector_t *args = vector_init(10);

    if (s == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "String is NULL");

    char *copy = malloc(sizeof(char) * (strlen(s) + 1));
    string_duplicate(copy, s);
    char *token = strtok(copy, " ");

    while (token != NULL) {
        vector_add(args, token);
        token = strtok(NULL, " ");
    }
    free(copy);
    free(token);

    if (args->size == 0)
        print_error(__FILENAME__, __LINE__, __FUNCTION__, "Invalid command");

    vector_foreach(args, remove_spaces);
    vector_foreach(args, remove_newlines);

    if (strcmp(args->elements[0], "U") == 0) {
        c->type = U;
    } else if (strcmp(args->elements[0], "S") == 0) {
        c->type = S;
    } else if (strcmp(args->elements[0], "R") == 0) {
        c->type = R;
    } else if (strcmp(args->elements[0], "C") == 0) {
        c->type = C;
    } else {
        print_error(__FILENAME__, __LINE__, __FUNCTION__, "Invalid command");
    }

    vector_remove(args, 0);

    c->args = args;
    return c;
}

/**
 * Converts boolean to command.
 * @param b The boolean.
 * @return The command.
 */
command_t *bool_to_command(bool b) {
    command_t *c = init_command();

    c->args = vector_init(1);
    vector_add(c->args, b ? "true" : "false");

    return c;
}

/**
 * Converts integer to command.
 * @param i The integer.
 * @return The command.
 */
command_t *int_to_command(int i) {
    command_t *c = init_command();

    c->args = vector_init(1);
    char *str = malloc(sizeof(char *) * n_count(i));
    int_to_string(str, i);
    vector_add(c->args, str);

    return c;
}

/**
 * Convert set to command.
 * @param s The set.
 * @return The command.
 */
command_t *set_to_command(set_t *s) {
    command_t *c = init_command();
    vector_t *args = vector_init(1);

    if (s == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Set is NULL");

    c->type = S;

    for (int i = 0; i < s->size; i++) {
        vector_add(args, s->elements[i]);
    }

    c->args = args;

    return c;
}

relation_set_t *command_to_relation_set(command_t *c) {
    relation_set_t *rv = relation_set_init(0);

    for (int i = 0; i < c->args->size; i++) {
        char *rel_str = malloc(
                sizeof(char) * (strlen(c->args->elements[i]) + 1));
        string_duplicate(rel_str, c->args->elements[i]);
        char *token = strtok(rel_str, " ");

        vector_t *v2 = vector_init(2);

        while (token != NULL) {
            vector_add_no_transform(v2, token);
            token = strtok(NULL, " ");
        }

        new_relations_t *new_rel = relation_init(v2->elements[0],
                                                 v2->elements[1]);
        relation_set_add_relation(rv, new_rel);

        free(rel_str);
        free(token);
    }

    return rv;
}

command_t *relation_set_to_command(relation_set_t *r) {
    command_t *c = init_command();
    vector_t *args = vector_init(1);

    c->type = R;

    for (int i = 0; i < r->size; i++) {
        char *vector_str = malloc(
                sizeof(char) * (strlen(r->relations[i]->element_a) + 1));
        string_duplicate(vector_str, r->relations[i]->element_a);
        strcat(vector_str, " ");
        strcat(vector_str, r->relations[i]->element_b);

        vector_add_no_transform(args, vector_str);

        free(vector_str);
    }

    c->args = args;

//    vector_free(args);

    return c;
}

/**
 * -----------------------------------------------------------------------------
 * COMMAND MODULE [COMMAND VECTOR]
 * -----------------------------------------------------------------------------
 */

/**
 * Initializes a command vector.
 * @param capacity The capacity.
 * @return The initialized command vector.
 */
command_vector_t *command_vector_init(int capacity) {
    command_vector_t *cv = (command_vector_t *) malloc(
            sizeof(command_vector_t));

    if (cv == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");

    cv->size = 0;
    cv->capacity = capacity;
    cv->commands = (command_t **) malloc(capacity * sizeof(command_t *));

    for(int i = 0; i < capacity; i++) {
        cv->commands[i] = init_command();
    }

    if (cv->commands == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");

    return cv;
}

/**
 * Adds a command to the command vector.
 * @param cv The command vector.
 * @param c The command.
 */
void command_vector_add(command_vector_t *cv, command_t *c) {
    if (cv->size == cv->capacity) {
        cv->capacity *= 2;
        cv->commands = (command_t **) realloc(cv->commands,
                                             cv->capacity * sizeof(command_t *));

        if (cv->commands == NULL)
            print_error(__FILENAME__, __LINE__, __func__, "Realloc failed");
    }

    cv->commands[cv->size] = c;
    cv->size++;
}

/**
 * Replaces a command in the command vector.
 * @param cv The command vector.
 * @param c The command.
 * @param index The index.
 */
void command_vector_replace(command_vector_t *cv, command_t *c, int index) {
    if (index < 0 || index >= cv->size)
        print_error(__FILENAME__, __LINE__, __FUNCTION__,
                    "Index out of bounds");

    cv->commands[index] = c;

    command_system_init_vectors(cv->system);
}

/**
 * Validate command vector.
 * @param cv The command vector.
 * @return true if valid, false otherwise.
 */
bool validate_command_vector(command_vector_t *cv, operation_vector_t *ov) {
    if (cv == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Command vector is NULL");

    if (cv->size == 0)
        print_error(__FILENAME__, __LINE__, __FUNCTION__,
                    "Invalid command vector");

    command_t *u_command = find_command_by_type(cv, U);
    command_vector_t *s_commands = find_command_by_type_all(cv, S);
    command_vector_t *c_commands = find_command_by_type_all(cv, C);
    command_vector_t *r_commands = find_command_by_type_all(cv, R);
    vector_t *unique_command_types = get_unique_command_types(cv);

    /**
     * Commands vector maximum size is 1000.
     */
    if (cv->size > 1000)
        print_error(__FILENAME__, __LINE__, __FUNCTION__,
                    "Commands count is greater than 1000");

    /**
     * Universe command is required.
     */
    if (u_command == NULL)
        print_error(__FILENAME__, __LINE__, __FUNCTION__,
                    "Universe command is required");

    /**
     * Maximum element size in universe is 30.
     */
    for (int i = 0; i < u_command->args->size; i++) {
        if (strlen(u_command->args->elements[i]) > 30)
            print_error(__FILENAME__, __LINE__, __FUNCTION__,
                        "U command elements are greater than 30");
    }

    /**
     * Commands vector must contain only the following commands: U, S, R, C.
     */
    for (int i = 0; i < cv->size; i++) {
        if (cv->commands[i]->type != U && cv->commands[i]->type != S &&
            cv->commands[i]->type != R && cv->commands[i]->type != C) {
            print_error(__FILENAME__, __LINE__, __FUNCTION__,
                        "File has unknown command");
        }
    }

    /**
     * Commands must be in order: U, S/R, C
     */
    for (int i = 0; i < cv->size; i++) {
        if (cv->commands[i]->type == U) {
            if (i != 0) {
                print_error(__FILENAME__, __LINE__, __FUNCTION__,
                            "U command must be the first command");
            }
        } else if (cv->commands[i]->type == S || cv->commands[i]->type == R) {
            if (i == 0) {
                print_error(__FILENAME__, __LINE__, __FUNCTION__,
                            "S command must be after U command");
            } else {
                command_vector_t *commands_before = command_vector_slice(cv, 0,
                                                                         i);
                command_vector_t *commands_after = command_vector_slice(cv, i,
                                                                        cv->size -
                                                                        1);

                if (command_vector_contains_type(commands_before, C) ||
                    command_vector_contains_type(commands_after, U)) {
                    print_error(__FILENAME__, __LINE__, __FUNCTION__,
                                "S and R commands must be after U command and before C commands");
                }

                command_vector_free(commands_before);
                command_vector_free(commands_after);
            }
        } else if (cv->commands[i]->type == C) {
            if (i == 0) {
                print_error(__FILENAME__, __LINE__, __FUNCTION__,
                            "C command must be after U command");
            } else {
                command_vector_t *commands_after =
                        command_vector_slice(cv, i, cv->size - 1);
                if (command_vector_contains_type(commands_after, U) ||
                    command_vector_contains_type(commands_after, S) ||
                    command_vector_contains_type(commands_after, R)) {
                    print_error(__FILENAME__, __LINE__, __FUNCTION__,
                                "C command must be after U, S and R commands. No other commands cannot be after C");
                }
                command_vector_free(commands_after);
            }
        }
    }

    /**
     * Commands must have at least one C command
     */
    for (int i = 0; i < cv->size; i++) {
        if (cv->commands[i]->type == C) {
            break;
        }

        if (i == cv->size - 1) {
            print_error(__FILENAME__, __LINE__, __FUNCTION__,
                        "No C commands found");
        }
    }

    /**
     * Commands must contain at least two command
     */
    if (unique_command_types->size < 2) {
        print_error(__FILENAME__, __LINE__, __FUNCTION__,
                    "File must contain at least two commands");
    }

    /**
     * Commands must contain at least one S or R command
     */
    for (int i = 0; i < unique_command_types->size; i++) {
        if (strcmp(unique_command_types->elements[i], "S") == 0 ||
            strcmp(unique_command_types->elements[i], "R") == 0) {
            break;
        }

        if (i == unique_command_types->size - 1) {
            print_error(__FILENAME__, __LINE__, __FUNCTION__,
                        "No S or R commands found");
        }
    }


    /**
     * Commands must contains something except U command
     */
    for (int i = 0; i < unique_command_types->size; i++) {
        if (strcmp(unique_command_types->elements[i], "U") == 0 &&
            unique_command_types->size == 1) {
            print_error(__FILENAME__, __LINE__, __FUNCTION__,
                        "File must not contain only universe");
        }
    }

    /**
     * Command vector must contain only one U command.
     */
    int u_count = 0;
    for (int i = 0; i < cv->size; i++) {
        if (cv->commands[i]->type == U) {
            u_count++;
        }
    }
    if (u_count != 1) {
        print_error(__FILENAME__, __LINE__, __FUNCTION__,
                    "There are multiple universes");
    }

    /**
     * Universe items length must not be greater than 30.
     */
    for (int i = 0; i < u_command->args->size; i++) {
        if (strlen(u_command->args->elements[i]) > 30) {
            print_error(__FILENAME__, __LINE__, __FUNCTION__,
                        "Universe item has length greater than 30");
        }
    }

    /**
     * Universe command can contain only strings.
     */
    char *universe_string = vector_to_string(u_command->args, " ");

    if (is_string_only_characters(universe_string) == false) {
        print_error(__FILENAME__, __LINE__, __FUNCTION__,
                    "Invalid command vector");
    }

    free(universe_string);

    /**
     * Check if universe does not contain false and true
     */
    for (int i = 0; i < u_command->args->size; i++)
        if (strcmp(u_command->args->elements[i], "false") == 0 ||
            strcmp(u_command->args->elements[i], "true") == 0) {
            print_error(__FILENAME__, __LINE__, __FUNCTION__,
                        "Universe contains true or false");
        }

    /**
     * Check if universe does not contain operations.
     */
    for (int i = 0; i < ov->size; i++) {
        if (vector_contains(u_command->args, ov->operations[i]->name) ==
            true) {
            print_error(__FILENAME__, __LINE__, __FUNCTION__,
                        "Universe contains operations");
        }
    }

    /**
     * Check if there are no repeats in the universe
     */
    for (int i = 0; i < u_command->args->size; i++) {
        for (int j = i + 1; j < u_command->args->size; j++) {
            if (strcmp(u_command->args->elements[i],
                       u_command->args->elements[j]) == 0) {
                print_error(__FILENAME__, __LINE__, __FUNCTION__,
                            "Universe contains repeating elements");
            }
        }
    }

    /**
     * Check if all sets has items only form universe.
     */
    for (int i = 0; i < s_commands->size; i++) {
        for (int j = 0; j < s_commands->commands[i]->args->size; j++) {
            if (vector_contains(u_command->args,
                                s_commands->commands[i]->args->elements[j]) ==
                false) {
                print_error(__FILENAME__, __LINE__, __FUNCTION__,
                            "Set contains elements not from universe");
            }
        }
    }

    /**
     * Check if there are not repeats in S commands
     */
    for (int i = 0; i < s_commands->size; i++) {
        command_t *s_command = s_commands->commands[i];
        for (int j = 0; j < s_command->args->size; j++) {
            for (int k = j + 1; k < s_command->args->size; k++) {
                if (strcmp(s_command->args->elements[j],
                           s_command->args->elements[k]) == 0) {
                    print_error(__FILENAME__, __LINE__, __FUNCTION__,
                                "Set contains repeating elements");
                }
            }
        }
    }

    /**
     * Check if all operations in command vector exist
     */
    for (int i = 0; i < c_commands->size; i++) {
        command_t *c_command = c_commands->commands[i];
        for (int j = 0; j < ov->size; j++) {
            if (strcmp(
                    c_command->args->elements[0],
                    ov->operations[j]->name) == 0)
                break;

            if (j == ov->size - 1) {
                print_error(__FILENAME__, __LINE__, __FUNCTION__,
                            "Operation does not exist");
            }
        }
    }

    /**
     * Check if all elements in R commands are from universe
     */
    for (int i = 0; i < r_commands->size; i++) {
        command_t *r_command = (r_commands->commands)[i];
        relation_set_t *rs = command_to_relation_set(r_command);

        for (int j = 0; j < rs->size; j++) {
            if ((vector_contains(u_command->args,
                                 rs->relations[j]->element_a) &&
                 vector_contains(u_command->args,
                                 rs->relations[j]->element_b)) == false) {
                print_error(__FILENAME__, __LINE__, __FUNCTION__,
                            "Relation set contains elements not from universe");
            }
        }

        relation_set_free(rs);
    }

    /**
     * Check if there are not repeats in R commands
     */
    for (int i = 0; i < r_commands->size; i++) {
        command_t *r_command = r_commands->commands[i];
        relation_set_t *rs = command_to_relation_set(r_command);

        for (int j = 0; j < rs->size; j++) {
            for (int k = j + 1; k < rs->size; k++) {
                if (strcmp(rs->relations[j]->element_a,
                           rs->relations[k]->element_a) == 0 &&
                    strcmp(rs->relations[j]->element_b,
                           rs->relations[k]->element_b) == 0) {
                    print_error(__FILENAME__, __LINE__, __FUNCTION__,
                                "Relation set contains repeating elements");
                }
            }
        }

        relation_set_free(rs);
    }

    return true;
}

/**
 * Attaches commands system to the given command vector.
 * @param cv Command vector.
 * @param cs Command system.
 */
void attach_command_system(command_vector_t *cv, command_system_t *cs) {
    cv->system = cs;
}

/**
 * Gets unique command types from the given command vector.
 * @param cv Command vector.
 * @return Unique command types.
 */
vector_t *get_unique_command_types(command_vector_t *cv) {
    vector_t *unique_command_types = vector_init(1);

    for (int i = 0; i < cv->size; i++) {
        char *command_type = " ";
        switch (cv->commands[i]->type) {
            case U:
                command_type = "U";
                break;
            case S:
                command_type = "S";
                break;
            case R:
                command_type = "R";
                break;
            case C:
                command_type = "C";
                break;
        }

        if (vector_contains(unique_command_types,
                            command_type) ==
            false) {
            vector_add(unique_command_types, command_type);
        }
    }

    return unique_command_types;
}

/**
 * Gets command by index from the given command vector.
 * @param cv Command vector.
 * @param index Index of the command.
 * @return Command.
 */
command_t *get_command_by_index(command_vector_t *cv, int index) {
    if (index < 0 || index >= cv->size) {
        print_error(__FILENAME__, __LINE__, __FUNCTION__,
                    "Index out of bounds");
    }

    return cv->commands[index];
}

/**
 * Finds a command in the command vector.
 * @param cv The command vector.
 * @param type The command type.
 * @return The command or NULL if not found.
 */
command_t *find_command_by_type(command_vector_t *cv, commands type) {
    for (int i = 0; i < cv->size; i++) {
        if (cv->commands[i]->type == type) {
            return cv->commands[i];
        }
    }
    return NULL;
}

// create a function to find all commands of a certain type
command_vector_t *
find_command_by_type_all(command_vector_t *cv, commands type) {
    command_vector_t *commands = command_vector_init(1);

    for (int i = 0; i < cv->size; i++) {
        if (cv->commands[i]->type == type) {
            command_vector_add(commands, (cv->commands)[i]);
        }
    }

    return commands;
}


command_vector_t *
command_vector_slice(command_vector_t *cv, int start, int end) {
    if (start < 0 || end > cv->size || start > end) {
        print_error(__FILENAME__, __LINE__, __FUNCTION__,
                    "Invalid start or end");
    }

    command_vector_t *commands = command_vector_init(1);

    for (int i = start; i < end; i++) {
        command_vector_add(commands, cv->commands[i]);
    }

    return commands;
}

/**
 * Checks if there is a command with the given type in the command vector.
 * @param cv The command vector.
 * @param type The command type.
 * @return True if there is a command with the given type, false otherwise.
 */
bool command_vector_contains_type(command_vector_t *cv, commands type) {
    for (int i = 0; i < cv->size; i++) {
        if (cv->commands[i]->type == type) {
            return true;
        }
    }
    return false;
}

/**
 * Prints the command vector.
 * @param cv The command vector.
 */
void command_vector_print(command_vector_t *cv) {
    for (int i = 0; i < cv->size; i++) {
        print_command(cv->commands[i]);
    }
}

/**
 * Frees the command vector.
 * @param cv The command vector.
 */
void command_vector_free(command_vector_t *cv) {
    if (cv->commands == NULL)
        print_error(__FILENAME__, __LINE__, __FUNCTION__,
                    "Invalid pointer");

    free(cv->commands);
    free(cv);
}

/**
 * Parses file to command vector.
 * @param filename The file name.
 * @return The command vector.
 */
command_vector_t *parse_file(char *filename) {
    command_vector_t *cv = command_vector_init(1);
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "File not found");

    char line[300];

    while (fgets(line, 255, fp) != NULL) {
        command_t *c = parse_command(line);
        command_vector_add(cv, c);
    }

    fclose(fp);

    return cv;
}

/**
 * -----------------------------------------------------------------------------
 * COMMAND MODULE [OPERATION]
 * -----------------------------------------------------------------------------
 */

/**
 * Initializes an operation.
 * @param name The name.
 * @return The initialized operation.
 */
operation *operation_init(char *name, commands type, int argc) {
    operation *o = (operation *) malloc(sizeof(operation));

    if (o == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");

    o->name = name;
    o->type = type;
    o->argc = argc;

    return o;
}

/**
 * Frees an operation.
 * @param o The operation.
 */
void operation_free(operation *o) {
    if (o == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Invalid pointer");

    free(o);
}

/**
 * -----------------------------------------------------------------------------
 * COMMAND MODULE [OPERATION VECTOR]
 * -----------------------------------------------------------------------------
 */

/**
 * Initializes an operation vector.
 * @param capacity The capacity.
 * @return The initialized operation vector.
 */
operation_vector_t *operation_vector_init(int capacity) {
    if (capacity < 1)
        print_error(__FILENAME__, __LINE__, __func__, "Invalid capacity");

    operation_vector_t *ov = (operation_vector_t *) malloc(
            sizeof(operation_vector_t));

    if (ov == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");

    ov->size = 0;
    ov->capacity = capacity;
    ov->operations = (operation **) malloc(sizeof(operation *) * capacity);

    if (ov->operations == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");

    return ov;
}

/**
 * Adds an operation to the operation vector.
 * @param ov The operation vector.
 * @param o The operation.
 */
void operation_vector_add(operation_vector_t *ov, operation *o) {
    if (ov->size == ov->capacity) {
        ov->capacity *= 2;
        ov->operations = (operation **) realloc(ov->operations,
                                                sizeof(operation *) *
                                                ov->capacity);

        if (ov->operations == NULL)
            print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");
    }

    operation *new_o = operation_init(o->name, o->type, o->argc);

    ov->operations[ov->size] = new_o;
    ov->size++;
}

/**
 * Gets an operation from the operation vector.
 * @param ov The operation vector.
 * @param name The name.
 * @return The operation.
 */
operation *operation_vector_find(operation_vector_t *ov, char *name) {
    for (int i = 0; i < ov->size; i++) {
        if (strcmp(ov->operations[i]->name, name) == 0)
            return ov->operations[i];
    }
    return NULL;
}

/**
 * Gets all operations from the operation vector by name.
 * @param ov The operation vector.
 * @param name The name.
 * @return The operations.
 */
operation_vector_t *
operation_vector_find_all(operation_vector_t *ov, char *name) {
    operation_vector_t *ov_array = operation_vector_init(1);

    for (int i = 0; i < ov->size; i++) {
        if (strcmp(ov->operations[i]->name, name) == 0) {
            operation_vector_add(ov_array, ov->operations[i]);
        }
    }

    return ov_array;
}

/**
 * Checks if the operation vector contains an operation with the given name.
 * @param ov The operation vector.
 * @param name The name.
 * @return True if the operation vector contains an operation with the given
 */
bool operation_vector_has(operation_vector_t *ov, char *name) {
    return operation_vector_find(ov, name) != NULL;
}

/**
 * Checks if the operation vector contains an operation with the given name and type.
 * @param ov The operation vector.
 * @param name The name.
 * @param type The type.
 * @return True if the operation vector contains the operation, false otherwise.
 */
bool operation_vector_has_name_type(operation_vector_t *ov, char *name,
                                    commands type) {
    operation_vector_t *ov_array = operation_vector_find_all(ov, name);

//    printf("%s %c\n", name, type);

    for (int i = 0; i < ov_array->size; i++) {
        if (ov_array->operations[i]->type == type) {
            operation_vector_free(ov_array);
            return true;
        }
    }

    operation_vector_free(ov_array);
    return false;
}

bool operation_vector_has_command(operation_vector_t *ov, command_t *c) {
    if (c->args->elements[0] == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Invalid command");

    return operation_vector_has_name_type(ov, c->args->elements[0], c->type);
}

bool operation_vector_contains(operation_vector_t *ov, char *name) {
    for (int i = 0; i < ov->size; i++) {
        if (strcmp(ov->operations[i]->name, name) == 0)
            return true;
    }

    return false;
}

void print_operation_vector(operation_vector_t *ov) {
    for (int i = 0; i < ov->size; i++) {
        printf("%c: %s\n", ov->operations[i]->type, ov->operations[i]->name);
    }

    printf("\n");
}

/**
 * Frees an operation vector.
 * @param ov The operation vector.
 */
void operation_vector_free(operation_vector_t *ov) {
    if (ov->operations == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Invalid pointer");

    for (int i = 0; i < ov->size; i++) {
        operation_free(ov->operations[i]);
    }

    free(ov->operations);
    free(ov);
}


/**
 * -----------------------------------------------------------------------------
 * COMMAND MODULE [COMMAND SYSTEM]
 * -----------------------------------------------------------------------------
 */

/**
 * Initializes a command system.
 * @param filename The file name.
 * @return The initialized command system.
 */
command_system_t *command_system_init(char *filename) {
    command_system_t *cs = (command_system_t *) malloc(
            sizeof(command_system_t));

    /**
     * Initialize the command vector from file.
     */

    if (cs == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");

    cs->filename = filename;
    cs->cv = parse_file(filename);
    attach_command_system(cs->cv, cs);

    command_system_init_base(cs);
    command_system_validate(cs);
    command_system_init_vectors(cs);

    /**
     * Initialize the operation map.
     * @todo Implement this.
     */



    return cs;
}

void command_system_init_base(command_system_t *cs) {
    /**
     * Initialize the operation vector.
     */
    cs->operation_vector = operation_vector_init(1);

    char *set_operations[SET_OPERATIONS_COUNT] = {
            "empty",
            "card",
            "complement",
            "union",
            "intersect",
            "minus",
            "subseteq",
            "subset",
            "equals"
    };

    int set_operations_argc[SET_OPERATIONS_COUNT] = {1, 1, 1, 2, 2, 2, 2, 2, 2};

    char *relation_operations[RELATION_OPERATIONS_COUNT] = {
            "reflexive",
            "symmetric",
            "antisymmetric",
            "transitive",
            "function",
            "domain",
            "codomain",
            "closure_ref",
            "closure_sym",
            "closure_trans",
    };

    int relation_operations_argc[RELATION_OPERATIONS_COUNT] = {1, 1, 1, 1, 1, 1,
                                                               1, 1, 1, 1};

    char *common_operations[COMMON_OPERATIONS_COUNT] = {
            "injective",
            "surjective",
            "bijective"
    };

    int common_operations_argc[RELATION_OPERATIONS_COUNT] = {3, 3, 3};

    for (int i = 0; i < SET_OPERATIONS_COUNT; i++) {
        operation *o = operation_init(set_operations[i], S,
                                      set_operations_argc[i]);

        operation_vector_add(cs->operation_vector, o);

        operation_free(o);
    }

    for (int i = 0; i < SET_OPERATIONS_COUNT; i++) {
        operation *o = operation_init(set_operations[i], U,
                                      set_operations_argc[i]);
        operation_vector_add(cs->operation_vector, o);

        operation_free(o);
    }

    for (int i = 0; i < RELATION_OPERATIONS_COUNT; i++) {
        operation *o = operation_init(relation_operations[i], R,
                                      relation_operations_argc[i]);

        operation_vector_add(cs->operation_vector, o);

        operation_free(o);
    }

    for (int i = 0; i < COMMON_OPERATIONS_COUNT; i++) {
        operation *uo = operation_init(common_operations[i], U,
                                       common_operations_argc[i]);
        operation *so = operation_init(common_operations[i], S,
                                       common_operations_argc[i]);
        operation *ro = operation_init(common_operations[i], R,
                                       common_operations_argc[i]);


        operation_vector_add(cs->operation_vector, uo);
        operation_vector_add(cs->operation_vector, so);
        operation_vector_add(cs->operation_vector, ro);

        operation_free(uo);
        operation_free(so);
        operation_free(ro);
    }
}

void command_system_validate(command_system_t *cs) {
    if (validate_command_vector(cs->cv, cs->operation_vector) == false)
        print_error(__FILENAME__, __LINE__, __func__, "Invalid input");
}

void command_system_init_vectors(command_system_t *cs) {
    /**
     * Initialize the universe set.
     */
    cs->set_vector = set_vector_init(1);
    cs->relation_vector = relation_vector_init(1);

    command_t *universe_command = find_command_by_type(cs->cv, U);

    set_vector_add(cs->set_vector, command_to_set(universe_command), 1);

    for (int i = 0; i < cs->cv->size; i++) {
        if (cs->cv->commands[i]->type == S) {
            set_t *set = set_init_indexed(i + 1,
                                          cs->cv->commands[i]->args->size);

            for (int j = 0; j < cs->cv->commands[i]->args->size; j++) {
                set_add(set, cs->cv->commands[i]->args->elements[j]);
            }

            set_vector_add(cs->set_vector, set, i);
        } else if (cs->cv->commands[i]->type == R) {
            command_t *command = cs->cv->commands[i];
            relation_set_t *relation_set = command_to_relation_set(command);
            relation_set->index = i + 1;

            relation_vector_add(cs->relation_vector, relation_set, i);
        }
    }
}

/**
 * Starts the command system.
 * @param cs The command system.
 */
void command_system_exec(command_system_t *cs) {
    for (int i = 0; i < cs->cv->size; i++) {
        command_t *command = cs->cv->commands[i];
        if (command->type != C) continue;

        char *operation_name = command->args->elements[0];

        /**
         * Operation check. Operation type must relate to argument type.
         */
        operation *operation = operation_vector_find(cs->operation_vector,
                                                     operation_name);

        if (operation->argc != command->args->size - 1)
            print_error(__FILENAME__, __LINE__, __func__,
                        "Invalid number of arguments");

        bool has_third_command = command->args->elements[3] != NULL;

        command_t *first_cmd = init_command();
        command_t *second_cmd = init_command();
        command_t *third_cmd = init_command();

        int first_index = atoi(command->args->elements[1]);
        int second_index = atoi(command->args->elements[2]);
        int third_index = 0;

        if (has_third_command) {
            third_index = atoi(command->args->elements[3]);
        }

        if (first_index) {
            first_cmd = command_copy(
                    get_command_by_index(cs->cv, first_index - 1));

            if (operation_vector_has_name_type(
                    cs->operation_vector,
                    operation_name,
                    first_cmd->type) == false)
                print_error(__FILENAME__, __LINE__, __func__,
                            "Invalid argument type");

            first_cmd->args->elements[0] = command->args->elements[0];
        }

        if (second_index) {
            second_cmd = command_copy(
                    get_command_by_index(cs->cv, second_index - 1));

            if (operation_vector_has_name_type(
                    cs->operation_vector,
                    operation_name,
                    second_cmd->type) == false)
                print_error(__FILENAME__, __LINE__, __func__,
                            "Invalid argument type");

            second_cmd->args->elements[0] = command->args->elements[0];
        }

        if (third_index) {
            third_cmd = command_copy(
                    get_command_by_index(cs->cv, third_index - 1));

            if (operation_vector_has_name_type(
                    cs->operation_vector,
                    operation_name,
                    third_cmd->type) == false)
                print_error(__FILENAME__, __LINE__, __func__,
                            "Invalid argument type");

            third_cmd->args->elements[0] = command->args->elements[0];
        }

        if ((operation_vector_has_command(
                cs->operation_vector, first_cmd) ||
             operation_vector_has_command(
                     cs->operation_vector, second_cmd) ||
             (has_third_command && operation_vector_has_command(
                     cs->operation_vector, third_cmd))) == false) {
            print_error(__FILENAME__, __LINE__, __func__,
                        "Invalid command");
        }

        if (strcmp(operation_name, "empty") == 0) {
            bool is_empty = set_is_empty(1, set_vector_find(cs->set_vector,
                                                            first_index));

            command_vector_replace(cs->cv, bool_to_command(is_empty), i);
        } else if (strcmp(operation_name, "card") == 0) {
            int card = set_card(1, set_vector_find(cs->set_vector,
                                                   first_index));

            command_vector_replace(cs->cv, int_to_command(card), i);
        } else if (strcmp(operation_name, "complement") == 0) {
            set_t *s = set_complement(
                    2,
                    set_vector_find(cs->set_vector, 1),
                    set_vector_find(cs->set_vector, first_index));

            command_vector_replace(cs->cv, set_to_command(s), i);
        } else if (strcmp(operation_name, "union") == 0) {
            set_t *s = set_union(
                    2,
                    set_vector_find(cs->set_vector, first_index),
                    set_vector_find(cs->set_vector, second_index));
            command_vector_replace(cs->cv, set_to_command(s), i);
        } else if (strcmp(operation_name, "intersect") == 0) {
            set_t *s = set_intersection(
                    2,
                    set_vector_find(cs->set_vector, first_index),
                    set_vector_find(cs->set_vector, second_index));
            command_vector_replace(cs->cv, set_to_command(s), i);
        } else if (strcmp(operation_name, "minus") == 0) {
            set_t *s = set_diff(
                    2,
                    set_vector_find(cs->set_vector, first_index),
                    set_vector_find(cs->set_vector, second_index));

            command_vector_replace(cs->cv, set_to_command(s), i);
        } else if (strcmp(operation_name, "subseteq") == 0) {
            bool is_subseteq = set_is_subseteq(
                    2,
                    set_vector_find(cs->set_vector, first_index),
                    set_vector_find(cs->set_vector, second_index));

            command_vector_replace(cs->cv, bool_to_command(is_subseteq), i);
        } else if (strcmp(operation_name, "subset") == 0) {
            bool is_subset = set_is_subset(
                    2,
                    set_vector_find(cs->set_vector, first_index),
                    set_vector_find(cs->set_vector, second_index));

            command_vector_replace(cs->cv, bool_to_command(is_subset), i);
        } else if (strcmp(operation_name, "equals") == 0) {
            bool is_equals = set_is_equal(
                    2,
                    set_vector_find(cs->set_vector, first_index),
                    set_vector_find(cs->set_vector, second_index));

            command_vector_replace(cs->cv, bool_to_command(is_equals), i);
        } else if (strcmp(operation_name, "reflexive") == 0) {
            bool is_reflexive = relation_is_reflexive(
                    2,
                    relation_vector_find(cs->relation_vector, first_index),
                    cs->set_vector->sets[0]);

            command_vector_replace(cs->cv, bool_to_command(is_reflexive),
                                   i);
        } else if (strcmp(operation_name, "symmetric") == 0) {
            bool is_symmetric = relation_is_symmetric(
                    2,
                    relation_vector_find(cs->relation_vector, first_index),
                    cs->set_vector->sets[0]);

            command_vector_replace(cs->cv, bool_to_command(is_symmetric),
                                   i);
        } else if (strcmp(operation_name, "antisymmetric") == 0) {
            bool is_antisymmetric = relation_is_antisymmetric(
                    2,
                    relation_vector_find(cs->relation_vector, first_index),
                    cs->set_vector->sets[0]);

            command_vector_replace(cs->cv,
                                   bool_to_command(is_antisymmetric),
                                   i);
        } else if (strcmp(operation_name, "transitive") == 0) {
            bool is_transitive = relation_is_transitive(
                    2,
                    relation_vector_find(cs->relation_vector, first_index),
                    cs->set_vector->sets[0]);

            command_vector_replace(cs->cv, bool_to_command(is_transitive),
                                   i);
        } else if (strcmp(operation_name, "function") == 0) {
            bool is_function = relation_is_function(
                    2,
                    relation_vector_find(cs->relation_vector, first_index),
                    cs->set_vector->sets[0]);

            command_vector_replace(cs->cv, bool_to_command(is_function), i);
        } else if (strcmp(operation_name, "domain") == 0) {
            set_t *s = relation_domain(
                    2,
                    relation_vector_find(cs->relation_vector, first_index),
                    cs->set_vector->sets[0]);
            command_vector_replace(cs->cv, set_to_command(s), i);
        } else if (strcmp(operation_name, "codomain") == 0) {
            set_t *s = relation_codomain(
                    2,
                    relation_vector_find(cs->relation_vector, first_index),
                    cs->set_vector->sets[0]);

            command_vector_replace(cs->cv, set_to_command(s), i);
        } else if ((strcmp(operation_name, "injective") == 0) ||
                   (strcmp(operation_name, "surjective") == 0) ||
                   (strcmp(operation_name, "bijective") == 0)) {
            bool result = false;

            if (first_cmd->type != R || second_cmd->type != S ||
                third_cmd->type != S)
                print_error(__FILENAME__, __LINE__, __func__,
                            "Invalid argument type");

            if (strcmp(operation_name, "injective") == 0) {
                result = relation_is_injective(
                        3,
                        relation_vector_find(cs->relation_vector,
                                             first_index),
                        set_vector_find(cs->set_vector, second_index),
                        set_vector_find(cs->set_vector, third_index));
            } else if (strcmp(operation_name, "surjective") == 0) {
                result = relation_is_surjective(
                        3,
                        relation_vector_find(cs->relation_vector,
                                             first_index),
                        set_vector_find(cs->set_vector, second_index),
                        set_vector_find(cs->set_vector, third_index));
            } else {
                result = relation_is_bijective(
                        3,
                        relation_vector_find(cs->relation_vector,
                                             first_index),
                        set_vector_find(cs->set_vector, second_index),
                        set_vector_find(cs->set_vector, third_index));
            }
//            command_t res_cmd = bool_to_command(result);
//
//            free_command(&res_cmd);
//
            command_vector_replace(cs->cv, bool_to_command(result), i);
        } else if (strcmp(operation_name, "closure_ref") == 0) {
            relation_set_t *rs = closure_ref(2,
                                             relation_vector_find(
                                                     cs->relation_vector,
                                                     first_index),
                                             cs->set_vector->sets[0]);

            command_vector_replace(cs->cv, relation_set_to_command(rs), i);
        } else if (strcmp(operation_name, "closure_sym") == 0) {
            relation_set_t *rs = closure_sym(2,
                                             relation_vector_find(
                                                     cs->relation_vector,
                                                     first_index),
                                             cs->set_vector->sets[0]);

            command_vector_replace(cs->cv, relation_set_to_command(rs), i);
        } else if (strcmp(operation_name, "closure_trans") == 0) {
            relation_set_t *rs = closure_trans(2,
                                               relation_vector_find(
                                                       cs->relation_vector,
                                                       first_index),
                                               cs->set_vector->sets[0]);

            command_vector_replace(cs->cv, relation_set_to_command(rs), i);
        }
    }
}

void command_system_free(command_system_t *cs) {
    if (cs == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Invalid pointer");

    command_vector_free(cs->cv);
    operation_vector_free(cs->operation_vector);
    set_vector_free(cs->set_vector);
    relation_vector_free(cs->relation_vector);

    free(cs);
}


/**
 * Main function.
 * @param argc The number of arguments.
 * @param argv The arguments.
 * @return 0 if the program ran successfully, 1 otherwise.
 */
int main(int argc, char *argv[]) {
    if (argc != 2)
        print_error(__FILENAME__, __LINE__, __func__,
                    "Invalid number of arguments");

    command_system_t *cs = command_system_init(argv[1]);

    command_system_exec(cs);

    command_vector_print(cs->cv);

    command_system_free(cs);

    return 0;
}


