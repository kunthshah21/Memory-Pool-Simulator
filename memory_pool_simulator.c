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
                    printf("Invalid alloc command");
                }
            } else if (strcmp(command, "free") == 0) {
                if (sscanf(input, "free %d %d", &start_address, &size) == 2) {
                    handle_free(start_address, size);
                } else {
                    printf("Invalid free command");
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

// handles memory allocation for a given size and checks if the size is valid/ enough contiguous free memory.
void handle_alloc(int size) {
    if (size <= 0 || size > MEMORY_POOL_SIZE) {
        printf("Invalid size %d. Must be between 1 and %d.\n", size, MEMORY_POOL_SIZE);
        return;
    }

    if (!is_valid_allocation(size)) {
        printf("Not enough continuous free space", size);
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

// handles freeing memory starting at a specified address and checks if the address and size are valid.
void handle_free(int start_address, int size) {
    if (size <= 0) {
        printf("Invalid size %d", size);
        return;
    }

    if (!is_valid_free(start_address, size)) {
        printf("Invalid address range", start_address, start_address + size - 1);
        return;
    }

    for (int i = start_address; i < start_address + size; i++) {
        memory_pool[i] = '-';
    }

    printf("Freed %d blocks starting at index %d\n", size, start_address);
}

// prints the memory map to display allocation and free blocks.
void print_memory_map() {
    printf("Current Memory Map:\n");

    for (int i = 0; i < MEMORY_POOL_SIZE; i++) {
        printf("%c", memory_pool[i]);
        if ((i + 1) % 20 == 0) {
            printf("\n");
        }
    }
    if (MEMORY_POOL_SIZE % 20 != 0) {
        printf("\n");
    }
}

// shifts allocated memory to the front and fills the remaining space with free blocks.
void compact_memory() {
    int write_index = 0;

    for (int read_index = 0; read_index < MEMORY_POOL_SIZE; read_index++) {
        if (memory_pool[read_index] != '-') {
            memory_pool[write_index++] = memory_pool[read_index];
        }
    }

    for (int i = write_index; i < MEMORY_POOL_SIZE; i++) {
        memory_pool[i] = '-';
    }

    printf("Memory compacted.\n");
    print_memory_map();
}

// saves the current memory pool state to a file.
void save_array(char* array) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file for writing");
        return;
    }

    fwrite(array, sizeof(char), MEMORY_POOL_SIZE, file);
    fclose(file);

    printf("Memory state saved to %s\n", filename);
}

// loads the memory pool state from a file.
void load_array(char* array) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("File %s not found.\n", filename);
        return;
    }

    fread(array, sizeof(char), MEMORY_POOL_SIZE, file);
    fclose(file);

    printf("Memory state loaded from %s\n", filename);
    print_memory_map();
}

// checks if there is enough contiguous free memory, returning 0 or 1. it is an edge case handler
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

// checks if the specified range can be freed, returning 0 or 1. it is an edge case handler
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
}
