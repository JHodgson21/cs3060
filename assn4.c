#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define PAGE_TABLE_SIZE 8
#define OFFSET_BITS 5
#define PAGE_NUMBER_BITS 9
#define LOGICAL_ADDRESS_BITS 14
#define FRAME_NUMBER_BITS 19

typedef struct {
    uint32_t entries[PAGE_TABLE_SIZE];
} PageTable;

typedef struct {
    int frame_number;
    PageTable table;
} PageTableEntry;

PageTableEntry *page_tables;
int num_page_tables;

/**
 * basically finding the page table and looking for a the frame number that corresponds. 
 */
PageTable *get_page_table(int frame) {
    for (int i = 0; i < num_page_tables; i++) {
        if (page_tables[i].frame_number == frame) {
            return &page_tables[i].table;
        }
    }
    return NULL;
}

/**
 * Read input files and parse them!!!!
 */
void parse_input(FILE *input) {
    if (fscanf(input, "%d", &num_page_tables) != 1) {
        fprintf(stderr, "Error: Failed to read number of page tables.\n");
        exit(EXIT_FAILURE);
    }

    page_tables = malloc(num_page_tables * sizeof(PageTableEntry));
    if (!page_tables) {
        fprintf(stderr, "Error: Memory allocation failed for page tables.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_page_tables; i++) {
        char label[10];
        if (fscanf(input, "%s %d", label, &page_tables[i].frame_number) != 2) {
            fprintf(stderr, "Error: Failed to read frame number for page table %d.\n", i);
            exit(EXIT_FAILURE);
        }

        for (int j = 0; j < PAGE_TABLE_SIZE; j++) {
            if (fscanf(input, "%u", &page_tables[i].table.entries[j]) != 1) {
                fprintf(stderr, "Error: Failed to read entry %d in frame %d.\n", j, page_tables[i].frame_number);
                exit(EXIT_FAILURE);
            }
        }
    }
}

/**
 * Extracting the frame number. this needs to be extracted correctly from a the page table
 */
int extract_frame(uint32_t entry) {
    return (entry >> 5) & 0x7FFF;
}

/**
 * Trying with the 3-level page table to translate the logical addresses...
 */
void translate_address(char operation, uint16_t logical_address, int op_num) {
    if (logical_address >= (1 << LOGICAL_ADDRESS_BITS)) {
        printf("Op #%d (%c 0x%04X) Page fault. Logical address out of bounds.\n", op_num, operation, logical_address);
        return;
    }

    int offset = logical_address & ((1 << OFFSET_BITS) - 1);
    int page_number = (logical_address >> OFFSET_BITS) & ((1 << PAGE_NUMBER_BITS) - 1);
    int l0_index = (page_number >> 6) & 0x7;
    int l1_index = (page_number >> 3) & 0x7;
    int l2_index = page_number & 0x7;

    int frame = 0;
    PageTable *table = get_page_table(frame);
    if (!table) {
        printf("Op #%d (%c 0x%04X) Page fault. Level 0 page table not found at frame %d.\n", op_num, operation, logical_address, frame);
        return;
    }

    uint32_t entry = table->entries[l0_index];
    if (!(entry & 0x10)) {
        printf("Op #%d (%c 0x%04X) Page fault. Entry %d in table at frame %d is not valid.\n", op_num, operation, logical_address, l0_index, frame);
        return;
    }
    frame = extract_frame(entry);
    table = get_page_table(frame);
    if (!table) {
        printf("Op #%d (%c 0x%04X) Page fault. Level 1 page table not found at frame %d.\n", op_num, operation, logical_address, frame);
        return;
    }

    entry = table->entries[l1_index];
    if (!(entry & 0x10)) {
        printf("Op #%d (%c 0x%04X) Page fault. Entry %d in table at frame %d is not valid.\n", op_num, operation, logical_address, l1_index, frame);
        return;
    }
    frame = extract_frame(entry);
    table = get_page_table(frame);
    if (!table) {
        printf("Op #%d (%c 0x%04X) Page fault. Level 2 page table not found at frame %d.\n", op_num, operation, logical_address, frame);
        return;
    }

    entry = table->entries[l2_index];
    if (!(entry & 0x10)) {
        printf("Op #%d (%c 0x%04X) Page fault. Entry %d in table at frame %d is not valid.\n", op_num, operation, logical_address, l2_index, frame);
        return;
    }

    char access[4] = "---";
    if (entry & 0x08) access[0] = 'R';
    if (entry & 0x04) access[1] = 'W';
    if (entry & 0x02) access[2] = 'X';
    if ((operation == 'R' && !(entry & 0x08)) ||
        (operation == 'W' && !(entry & 0x04)) ||
        (operation == 'X' && !(entry & 0x02))) {
        printf("Op #%d (%c 0x%04X) Page fault. Illegal access to frame %d due to entry %d in table at frame %d with access %s.\n",
               op_num, operation, logical_address, frame, l2_index, frame, access);
        return;
    }

    int data_frame = extract_frame(entry);
    int physical_address = (data_frame << OFFSET_BITS) | offset;
    printf("Op #%d (%c 0x%04X) Success. Physical address 0x%05X (Frame %d, Access %s).\n",
           op_num, operation, logical_address, physical_address, data_frame, access);
}

int main(int argc, char *argv[]) {
    FILE *input = (argc > 1) ? fopen(argv[1], "r") : stdin;
    if (!input) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    parse_input(input);

    int num_tests;
    if (fscanf(input, "%d", &num_tests) != 1) {
        fprintf(stderr, "Error: Failed to read number of tests.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_tests; i++) {
        char operation;
        uint16_t logical_address;
        if (fscanf(input, " %c %hu", &operation, &logical_address) != 2) {
            fprintf(stderr, "Error: Failed to read operation %d.\n", i);
            continue;
        }
        translate_address(operation, logical_address, i);
    }

    free(page_tables);
    if (input != stdin) fclose(input);
    return 0;
}