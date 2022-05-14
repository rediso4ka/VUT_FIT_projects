# Algebra calculator - setcal

[![CodeFactor](https://www.codefactor.io/repository/github/signalr/signalr/badge?style=plastic)](https://www.codefactor.io/repository/github/rediso4ka/relace)

C program for calculations of algebraic sets and relation_t. 
No external libraries where used, so the program can be compiled almost in any environment.
Uses [CMake](https://cmake.org/cmake/help/v3.16/guide/tutorial/index.html) to build executable.

## Data Types
  * [Vector](#vector) - array of strings
  * [Set](#set) - algebraic set
  * [Set Vector](#set-vector) - array of algebraic sets
  * [Command](#command) - program instruction
  * [Command Vector](#command-vector) - array of program instructions
  * [Command System](#command-system) - commands controller (validation, initialization, execution, etc.)

### Vector

Data type for array of strings. Contains array of strings. Has size and capacity. Size indicates real number of items in array. Capacity indicates for what number of items has been the memory allocated. If sizes reaches vector's capacity, reallocates array and increases capacity x2.

**Definition**

```c
typedef struct {
    int size;
    int capacity;
    char **elements;
} vector_t;
```

**Methods**

Vector initialization

Params:
  * Capacity of vector. Must be greater 0

Returns: initialized vector

```c 
vector_t *vector_init(int capacity); 
```
Add item to vector

Params:
  * Pointer to the vector where to add
  * String to add

```c 
void vector_add(vector_t *v, char *s);
```

Resize vector items to the same length

Params:
  * Pointer to the vector to be resized
  * Length to resize

```c 
void resize_all(vector_t *v, unsigned int max);
```

Remove element from vector

Params:
  * Pointer to the vector where the element will be deleted
  * Index of the element

```c 
void vector_remove(vector_t *v, int index);
```

Apply function for each element of the vector 

Params:
  * Pointer to the vector with elements
  * Pointer to the function to be applied to each element of vector

```c 
void vector_foreach(vector_t *v, void (*f)(char *));
```

Find maximum item length in vector

Params:
  * Pointer to the vector with elements

Returns: Maximum length in the elements

```c 
unsigned int find_max_vector_element_size(vector_t *v);

```

Delete vector from memory

Params:
  * Pointer to the vector 

```c 
void vector_free(vector_t *v);
```

### Set

Data type for algebraic sets. Contains array of strings. Has size, capacity and index. Size indicates the real number of items in the array. Capacity indicates for what number of items has been the memory allocated. The index indicates order of the set and used to determine set. If sizes reaches vector's capacity, reallocates array and increases capacity x2.

**Definition**

```c
typedef struct set {
    unsigned int index;
    int size;
    int capacity;
    char **elements;
} set_t;
```

**Methods**

Set initialization

Params:
 * Capacity of the set 

Returns: initialized set

```c
set_t *set_init(int capacity);
```

Set initialization

Params:
 * Capacity of the set
 * Index of the set

Returns: initialized set

```c
set_t *set_init_indexed(int index, int capacity);
```

Add item to the set

Params:
 * Set to add an item
 * Item to be added to the set

```c
void set_add(set_t *s, char *e);
```

Print the set

Params: 
 * The set to print

```c
void set_print(set_t *s);
```

### Set Vector

Data type for vector of [sets](#set). Contains array of sets. Has size and capacity. Size indicates the real number of sets in the array. Capacity indicates for what number of sets has been the memory allocated. If sizes reaches vector's capacity, reallocates array and increases capacity x2.

**Definition**

```c
typedef struct set_vector_t {
    int size;
    int capacity;
    set_t **sets;
} set_vector_t;
```

**Methods**

Set vector initialization

Params:
  * Capacity of the set vector

Returns: Initialized set vector

```c
set_vector_t *set_vector_init(int capacity);
```

Add set to the set vector at index

Params:
  * Pointer to the set vector
  * Pointer to the set to be added
  * Index of the set

```c
void set_vector_add(set_vector_t *sv, set_t *s, int index);
```

Find set by index

Params:
  * Pointer to the set vector
  * Index of the set

Returns: Pointer to the set

```c
set_t *set_vector_find(set_vector_t *sv, int index);
```

Print the set vector

Params: 
 * The set vector to print

```c
void set_vector_print(set_vector_t *sv)
```

### Command

Data type for command. Command is a program instruction defined in input file. 
Contains command type and arguments. Command type can be one of the following: U S R C. 
Arguments is a [vector](#vector).

**Definition**

```c
typedef struct {
    commands type;
    vector_t args;
} command_t;
```

**Methods**

Command initialization

Returns: Initialized command

```c
command_t *init_command();
```

Command initialization with type

Params: 
  * Command type

Returns: Initialized command

```c
command_t *init_command_with_type(commands type);
```

Set command arguments

Params: 
  * Pointer to the command
  * Pointer to the vector with arguments

```c
void set_command_args(command_t *c, vector_t *args);
```

Add argument to the command

Params: 
  * Pointer to the command
  * Pointer to the argument

```c
void add_command_arg(command_t *c, char *arg);
```

Convert command to [set](#set)

Params: 
  * Pointer to the command

Returns: Pointer to the set

```c
set_t *command_to_set(command_t *c);
```

Copy command

Params: 
  * Pointer to the command

Returns: Pointer to the copied command

```c
command_t *command_copy(command_t *c);
```

Print command

Params: 
  * Pointer to the command

```c
void print_command(command_t *c);
```

Remove command from memory

Params: 
  * Pointer to the command

```c
void free_command(command_t *c);
```

### Command Vector

Data type for vector of [commands](#command). Contains array of commands. 
Has size, capacity and references to the [command system](#command-system). 
Size indicates the real number of commands in the array. 
Capacity indicates for what number of commands has been the memory allocated. 
If sizes reaches vector's capacity, reallocates array and increases capacity x2.

**Definition**

```c
typedef struct {
    int size;
    int capacity;
    command_t *commands;
    command_system_t *system;
} command_vector_t;
```

**Methods**

Initialize command vector

Params: 
  * Capacity of the command vector

Returns: Initialized command vector

```c
command_vector_t *command_vector_init(int capacity);
```

Add command to the command vector

Params: 
  * Pointer to the command vector
  * Pointer to the command to be added

```c
void command_vector_add(command_vector_t *cv, command_t c);
```

Replace command in the command vector by index

Params: 
  * Pointer to the command vector
  * Pointer to the command to be replaced
  * Index of the command

```c
void command_vector_replace(command_vector_t *cv, command_t c, int index);
```

Validate command vector using multiple rules

Params: 
  * Pointer to the command vector
  * Pointer to the operation vector

Returns: True if command vector is valid, false otherwise

```c
bool validate_command_vector(command_vector_t *cv, operation_vector_t *ov);
```

Attach command system to the command vector

Params: 
  * Pointer to the command vector
  * Pointer to the command system

```c
void attach_command_system(command_vector_t *cv, command_system_t *cs);
```

Get unique command types from the command vector

Params: 
  * Pointer to the command vector

Returns: Pointer to the vector with unique command types

```c
vector_t *get_unique_command_types(command_vector_t *cv);
```

Get command by index

Params: 
  * Pointer to the command vector
  * Index of the command

Returns: Pointer to the command

```c
command_t *get_command_by_index(command_vector_t *cv, int index);
```

Find command by type

Params: 
  * Pointer to the command vector
  * Command type

Returns: Pointer to the command

```c
command_t *find_command_by_type(command_vector_t *cv, commands type);
```

Find all commands by type

Params: 
  * Pointer to the command vector
  * Command type

Returns: Pointer to the vector with commands

```c
command_vector_t *find_command_by_type_all(command_vector_t *cv, commands type);
```

Slice command vector

Params: 
  * Pointer to the command vector
  * Start index
  * End index

Returns: Pointer to the sliced command vector

```c
command_vector_t * command_vector_slice(command_vector_t *cv, int start, int end);
```

Check if command vector contains the command with type

Params: 
  * Pointer to the command vector
  * Command type

Returns: True if command vector contains the command with type, false otherwise

```c
bool command_vector_contains_type(command_vector_t *cv, commands type);
```

Print command vector

Params: 
  * Pointer to the command vector

```c
void command_vector_print(command_vector_t *cv);
```

Remove command vector from memory

Params: 
  * Pointer to the command vector

```c
void command_vector_free(command_vector_t *cv);
```

Parse file with commands

Params: 
  * Name of the file

Returns: Pointer to the command vector

```c
command_vector_t *parse_file(char *filename);
```

### Command System

Data type for command system. Used to control the execution of commands.
Has filename of the file with commands.
Has pointer to the [command vector](#command-vector) where all commands from file are stored.
Has pointer to the operation vector where all allowed operations are stored.
Has pointer to the [set vector](#set-vector) where all sets are stored. 

**Definition**

```c
typedef struct command_system_t {
    char *filename;
    command_vector_t *cv;
    operation_vector_t *operation_vector;
    set_t *universe;
    set_vector_t *set_vector;
};
```

**Methods**

Initialize command system

Params: 
  * Name of the file with commands
  
Returns: Initialized command system 

```c
command_system_t *command_system_init(char *filename);
```

Basic command system initialization

Params: 
  * Pointer to the command system

```c
void command_system_init_base(command_system_t *cs);
```

Run validation of the command system

Params: 
  * Pointer to the command system

```c
void command_system_validate(command_system_t *cs);
```

Vectors initialization in the command system

Params: 
  * Pointer to the command system

```c
void command_system_init_vectors(command_system_t *cs);
```

Execution of the command system

Params: 
  * Pointer to the command system

```c
void command_system_exec(command_system_t *cs);
```

Remove command system from memory

Params: 
  * Pointer to the command system

```c
void command_system_free(command_system_t *cs);
```
