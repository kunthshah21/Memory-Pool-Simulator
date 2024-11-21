#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 100
#define MEMORY_POOL_SIZE 100

char memory_pool[MEMORY_POOL_SIZE];

void handle_alloc(int size);
void handle_free(int start_address, int size);
void print_memory_map();
void compact_memory();
void save_memory();
void load_memory();

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
                save_memory();
            } else if (strcmp(command, "load") == 0) {
                load_memory();
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

void save_memory() {
    printf("Saving memory state\n");
}

void load_memory() {
    printf("Loading memory state\n");
}
