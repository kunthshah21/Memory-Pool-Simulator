#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 100
#define MEMORY_POOL_SIZE 100

// Global memory pool
char memory_pool[MEMORY_POOL_SIZE];

// Function prototypes
void handle_alloc(int size);
void handle_free(int start_address, int size);
void print_memory_map();
void compact_memory();
void save_memory();
void load_memory();

int main() {
    // Initialize memory pool with '-'
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
        
        // Read entire line
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        // Remove newline character
        input[strcspn(input, "\n")] = 0;

        // Parse command
        if (sscanf(input, "%s", command) == 1) {
            // Allocation command
            if (strcmp(command, "alloc") == 0) {
                if (sscanf(input, "alloc %d", &size) == 1) {
                    handle_alloc(size);
                } else {
                    printf("Invalid alloc command. Usage: alloc <size>\n");
                }
            }
            // Free command
            else if (strcmp(command, "free") == 0) {
                if (sscanf(input, "free %d %d", &start_address, &size) == 2) {
                    handle_free(start_address, size);
                } else {
                    printf("Invalid free command. Usage: free <start_address> <size>\n");
                }
            }
            // Print map command
            else if (strcmp(command, "print_map") == 0) {
                print_memory_map();
            }
            // Compact command
            else if (strcmp(command, "compact") == 0) {
                compact_memory();
            }
            // Save command
            else if (strcmp(command, "save") == 0) {
                save_memory();
            }
            // Load command
            else if (strcmp(command, "load") == 0) {
                load_memory();
            }
            // Exit command
            else if (strcmp(command, "exit") == 0) {
                printf("Exiting Memory Pool Simulator\n");
                break;
            }
            // Invalid command
            else {
                printf("Invalid command: %s\n", command);
            }
        }
    }

    return 0;
}

// Placeholder implementation for allocation
void handle_alloc(int size) {
    int found_start = -1; // Start index of a free block
    int free_count = 0;   // Count of continuous free spaces

    // Search for a continuous block of `size` free spaces
    for (int i = 0; i < MEMORY_POOL_SIZE; i++) {
        if (memory_pool[i] == '-') {
            if (free_count == 0) {
                found_start = i; // Mark the start of the free block
            }
            free_count++;

            if (free_count == size) {
                // Found a block large enough
                break;
            }
        } else {
            // Reset if block is interrupted
            free_count = 0;
        }
    }

    // If a suitable block was found
    if (free_count == size) {
        for (int i = found_start; i < found_start + size; i++) {
            memory_pool[i] = 'X'; // Allocate the block
        }
        printf("Allocated %d bytes starting at index %d\n", size, found_start);
    } else {
        printf("Allocation failed: Not enough continuous free space for %d bytes\n", size);
    }
}

// Placeholder implementation for free
void handle_free(int start_address, int size) {
    printf("Freeing %d bytes starting at address %d\n", size, start_address);
    // TODO: Implement actual memory freeing logic
}

// Placeholder implementation for print memory map
void print_memory_map() {
    printf("Current Memory Map:\n");

    int bytes_per_line = 32; // Adjust as needed for readability
    for (int i = 0; i < MEMORY_POOL_SIZE; i++) {
        printf("%c", memory_pool[i]);
        // Add a newline every `bytes_per_line` characters
        if ((i + 1) % bytes_per_line == 0) {
            printf("\n");
        }
    }

    // Add a final newline if the last line wasn't complete
    if (MEMORY_POOL_SIZE % bytes_per_line != 0) {
        printf("\n");
    }
}

// Placeholder implementation for memory compaction
void compact_memory() {
    int write_index = 0; // Index where the next allocated block will be moved

    // Iterate through the memory pool
    for (int read_index = 0; read_index < MEMORY_POOL_SIZE; read_index++) {
        if (memory_pool[read_index] != '-') {
            // If the current block is allocated, move it to the `write_index`
            memory_pool[write_index] = memory_pool[read_index];
            // If `read_index` and `write_index` differ, mark `read_index` as free
            if (read_index != write_index) {
                memory_pool[read_index] = '-';
            }
            write_index++; // Increment the write index
        }
    }

    printf("Memory compacted.\n");
    print_memory_map(); // Print the updated memory map after compaction
}


// Placeholder implementation for saving memory
void save_memory() {
    printf("Saving memory state\n");
    // TODO: Implement memory state saving
}

// Placeholder implementation for loading memory
void load_memory() {
    printf("Loading memory state\n");
    // TODO: Implement memory state loading
}