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

    if (free_count == size) {
        for (int i = found_start; i < found_start + size; i++) {
            memory_pool[i] = 'X';
        }
        printf("Allocated %d bytes starting at index %d\n", size, found_start);
    } else {
        printf("Allocation failed: Not enough continuous free space for %d bytes\n", size);
    }
}

void handle_free(int start_address, int size) {
    if (start_address < 0 || start_address >= MEMORY_POOL_SIZE) {
        printf("Free failed: Start address %d is out of bounds\n", start_address);
        return;
    }
    if (start_address + size > MEMORY_POOL_SIZE) {
        printf("Free failed: Size %d goes out of bounds from start address %d\n", size, start_address);
        return;
    }

    for (int i = start_address; i < start_address + size; i++) {
        if (memory_pool[i] != 'X') {
            printf("Free failed: Address %d is not part of an allocated block\n", i);
            return;
        }
    }

    for (int i = start_address; i < start_address + size; i++) {
        memory_pool[i] = '-';
    }
    printf("Freed %d bytes starting at index %d\n", size, start_address);
}

void print_memory_map() {
    printf("Current Memory Map:\n");

    int bytes_per_line = 32;
    for (int i = 0; i < MEMORY_POOL_SIZE; i++) {
        printf("%c", memory_pool[i]);
        if ((i + 1) % bytes_per_line == 0) {
            printf("\n");
        }
    }

    if (MEMORY_POOL_SIZE % bytes_per_line != 0) {
        printf("\n");
    }
}

void compact_memory() {
    int write_index = 0;

    for (int read_index = 0; read_index < MEMORY_POOL_SIZE; read_index++) {
        if (memory_pool[read_index] != '-') {
            memory_pool[write_index] = memory_pool[read_index];
            if (read_index != write_index) {
                memory_pool[read_index] = '-';
            }
            write_index++;
        }
    }

    printf("Memory compacted.\n");
}

// Function to save the array to a file
void save_array(char *array) {
    FILE *file = fopen(filename, "w");  // Open file for writing (create it if it doesn't exist)
    if (file == NULL) {
        perror("Error opening file for writing");
        return;
    }

    // Writing each character from the array to the file
    for (int i = 0; i < 100; i++) {
        fprintf(file, "%c", array[i]);  // Print each character without spaces
    }

    fclose(file);
    printf("Array saved to %s\n", filename);
}

// Function to load the array from a file
void load_array(char *array) {
    FILE *file = fopen(filename, "r");  // Open file for reading
    if (file == NULL) {
        // If the file doesn't exist, create it and initialize the array
        printf("File not found. Creating file and initializing array.\n");
        return;
    }

    // Reading each character from the file into the array
    for (int i = 0; i < 100; i++) {
        if (fscanf(file, "%c", &array[i]) != 1) {
            perror("Error reading file");
            fclose(file);
            return;
        }

        // Check if the character is either 'x' or '-'
        if (array[i] != 'X' && array[i] != '-') {
            printf("Error: Invalid character '%c' found in file at position %d. Expected 'x' or '-'.\n", array[i], i);
            fclose(file);
            return;
        }
    }

    fclose(file);
    printf("Loading done\n");
}