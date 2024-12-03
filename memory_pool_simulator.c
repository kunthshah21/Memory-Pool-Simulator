#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 100
#define MEMORY_POOL_SIZE 100

const char* filename = "memory_state.txt";

char memory_pool[MEMORY_POOL_SIZE];

void handle_alloc(int size);
void handle_free(int start_address, int size);
void print_memory_map();
void compact_memory();
void save_array(char* array);
void load_array(char* array);
int is_valid_allocation(int size);
int is_valid_free(int start_address, int size);

int main() {
    memset(memory_pool, '-', MEMORY_POOL_SIZE);

    char input[MAX_INPUT];
    char command[20];
    int size, start_address;

    printf("Memory Pool Simulator\n");
    printf("Available commands:\n");
    printf("- alloc <size>\n");
    printf("- free <start_address> <size>\n");
    printf("- print_map\n");
    printf("- compact\n");
    printf("- save\n");
    printf("- load\n");
    printf("- exit\n\n");

    while (1) {
        printf("> ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        input[strcspn(input, "\n")] = 0;

        if (sscanf(input, "%s", command) == 1) {
            if (strcmp(command, "alloc") == 0) {
                if (sscanf(input, "alloc %d", &size) == 1) {
                    handle_alloc(size);
                } else {
                    printf("Invalid alloc command. Usage: alloc <size>\n");
                }
            } else if (strcmp(command, "free") == 0) {
                if (sscanf(input, "free %d %d", &start_address, &size) == 2) {
                    handle_free(start_address, size);
                } else {
                    printf("Invalid free command. Usage: free <start_address> <size>\n");
                }
            } else if (strcmp(command, "print_map") == 0) {
                print_memory_map();
            } else if (strcmp(command, "compact") == 0) {
                compact_memory();
            } else if (strcmp(command, "save") == 0) {
                save_array(memory_pool);
            } else if (strcmp(command, "load") == 0) {
                load_array(memory_pool);
            } else if (strcmp(command, "exit") == 0) {
                printf("Exiting Memory Pool Simulator\n");
                break;
            } else {
                printf("Invalid command: %s\n", command);
            }
        }
    }

    return 0;
}

void handle_alloc(int size) {
    if (size <= 0 || size > MEMORY_POOL_SIZE) {
        printf("Allocation failed: Invalid size %d. Must be between 1 and %d.\n", size, MEMORY_POOL_SIZE);
        return;
    }

    if (!is_valid_allocation(size)) {
        printf("Allocation failed: Not enough continuous free space for %d blocks.\n", size);
        return;
    }

    int found_start = -1;
    int free_count = 0;

    for (int i = 0; i < MEMORY_POOL_SIZE; i++) {
        if (memory_pool[i] == '-') {
            if (free_count == 0) {
                found_start = i;
            }
            free_count++;
            if (free_count == size) {
                break;
            }
        } else {
            free_count = 0;
        }
    }

    for (int i = found_start; i < found_start + size; i++) {
        memory_pool[i] = 'X';
    }

    printf("Allocated %d blocks starting at index %d\n", size, found_start);
}

void handle_free(int start_address, int size) {
    if (size <= 0) {
        printf("Free failed: Invalid size %d. Size must be positive.\n", size);
        return;
    }

    if (!is_valid_free(start_address, size)) {
        printf("Free failed: Invalid address range %d to %d.\n", start_address, start_address + size - 1);
        return;
    }

    for (int i = start_address; i < start_address + size; i++) {
        memory_pool[i] = '-';
    }

    printf("Freed %d blocks starting at index %d\n", size, start_address);
}

void print_memory_map() {
    printf("Current Memory Map:\n");

<<<<<<< HEAD
    for (int i = 0; i < MEMORY_POOL_SIZE; i++) {
=======
    int bytes_per_line = 20;
    for (int i = 0; i < MEMORY_POOL_SIZE ; i++) {
>>>>>>> 2a8ce3e88b22401f00f3f4d593a02bc388df0ef6
        printf("%c", memory_pool[i]);
        if ((i + 1) % bytes_per_line == 0) {
            printf("\n");
        }
    }
<<<<<<< HEAD
    if (MEMORY_POOL_SIZE % 20 != 0) {
=======

//below if condition is not really needed, as the memory_pool will always be 100.
    if (MEMORY_POOL_SIZE % bytes_per_line != 0) {
>>>>>>> 2a8ce3e88b22401f00f3f4d593a02bc388df0ef6
        printf("\n");
    }
}

void compact_memory() {
    int write_index = 0;

    for (int read_index = 0; read_index < MEMORY_POOL_SIZE; read_index++) {
        if (memory_pool[read_index] == 'X') {
            memory_pool[write_index++] = 'X';
        }
    }

    for (int i = write_index; i < MEMORY_POOL_SIZE; i++) {
        memory_pool[i] = '-';
    }

    printf("Memory compacted.\n");
    print_memory_map();
}

void save_array(char* array) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file for writing");
        return;
    }

<<<<<<< HEAD
    fwrite(array, sizeof(char), MEMORY_POOL_SIZE, file);
=======
    // Writing each character from the array to the file
    for (int i = 0; i < 100; i++) {
        fprintf(file, "%c", array[i]);  // Print each character in the file without spaces
    }

>>>>>>> 2a8ce3e88b22401f00f3f4d593a02bc388df0ef6
    fclose(file);

    printf("Memory state saved to %s\n", filename);
}

<<<<<<< HEAD
void load_array(char* array) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: File %s not found.\n", filename);
=======
// Function to load the array from a file
void load_array(char *array) {
    FILE *file = fopen(filename, "r");  // Open file for reading
    if (file == NULL) {
        printf("File not found.\n");
>>>>>>> 2a8ce3e88b22401f00f3f4d593a02bc388df0ef6
        return;
    }

    fread(array, sizeof(char), MEMORY_POOL_SIZE, file);
    fclose(file);

<<<<<<< HEAD
    printf("Memory state loaded from %s\n", filename);
    print_memory_map();
}

int is_valid_allocation(int size) {
    int free_count = 0;

    for (int i = 0; i < MEMORY_POOL_SIZE; i++) {
        if (memory_pool[i] == '-') {
            free_count++;
            if (free_count == size) {
                return 1;
            }
        } else {
            free_count = 0;
        }
    }

    return 0;
}

int is_valid_free(int start_address, int size) {
    if (start_address < 0 || start_address >= MEMORY_POOL_SIZE) {
        return 0;
    }
    if (start_address + size > MEMORY_POOL_SIZE) {
        return 0;
    }
    
    for (int i = start_address; i < start_address + size; i++) {
        if (memory_pool[i] != 'X') {
            return 0;
        }
    }

    return 1;
=======
        // Check if the character is either 'x' or '-'
        if (array[i] != 'X' && array[i] != '-') {
            printf("Error: Invalid character '%c' found in file at position %d. Expected 'x' or '-'.\n", array[i], i);
            fclose(file);
            //the file reading stops, as the memory pool is corrupted, and nothing gets returned.
            return;
        }
    }

    fclose(file);
    printf("Loading done\n");
>>>>>>> 2a8ce3e88b22401f00f3f4d593a02bc388df0ef6
}
