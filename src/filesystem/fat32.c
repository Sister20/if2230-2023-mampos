#include "../lib-header/stdtype.h"
#include "fat32.h"
#include "../lib-header/stdmem.h"

const uint8_t fs_signature[BLOCK_SIZE] = {
    'C', 'o', 'u', 'r', 's', 'e', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  ' ',
    'D', 'e', 's', 'i', 'g', 'n', 'e', 'd', ' ', 'b', 'y', ' ', ' ', ' ', ' ',  ' ',
    'L', 'a', 'b', ' ', 'S', 'i', 's', 't', 'e', 'r', ' ', 'I', 'T', 'B', ' ',  ' ',
    'M', 'a', 'd', 'e', ' ', 'w', 'i', 't', 'h', ' ', '<', '3', ' ', ' ', ' ',  ' ',
    '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '2', '0', '2', '3', '\n',
    [BLOCK_SIZE-2] = 'O',
    [BLOCK_SIZE-1] = 'k',
};



// uint32_t cluster_to_lba(uint32_t cluster) {
//     return (cluster - 2) * CLUSTER_BLOCK_COUNT + 1;
// }

void init_directory_table(struct FAT32DirectoryTable *dir_table, char *name, uint32_t parent_dir_cluster) {
    // Set the directory name
    memcpy(dir_table->table[0].name, name, 8);

    // Set the directory attributes
    dir_table->table[0].attribute = ATTR_SUBDIRECTORY ;

    // Set the parent directory cluster and the filesize to 0
    dir_table->table[0].cluster_high = (uint16_t) (parent_dir_cluster >> 16);
    dir_table->table[0].cluster_low = (uint16_t) (parent_dir_cluster & 0xFFFF);
    dir_table->table[0].filesize = 0;
}

bool is_empty_storage(void) {
    uint32_t boot_sector[BLOCK_SIZE];
    read_blocks(boot_sector, BOOT_SECTOR, 1);
    return !memcmp(boot_sector, fs_signature, BLOCK_SIZE);
}


void create_fat32(void) {
    struct FAT32DriverState fs_state;

    // Write fs_signature into boot sector
    memcpy(fs_state.cluster_buf.buf, fs_signature, BLOCK_SIZE);
    write_blocks(fs_state.cluster_buf.buf, BOOT_SECTOR, 1);

    // Write proper FileAllocationTable (contain CLUSTER_0_VALUE, CLUSTER_1_VALUE, and initialized root directory) into cluster number 1
    memset(fs_state.fat_table.cluster_map, 0, BLOCK_SIZE);
    fs_state.fat_table.cluster_map[0] = CLUSTER_0_VALUE;
    fs_state.fat_table.cluster_map[1] = CLUSTER_1_VALUE;
    fs_state.fat_table.cluster_map[2] = FAT32_FAT_END_OF_FILE;
    write_blocks(fs_state.fat_table.cluster_map, 1, 1);

    // Write root directory into cluster number 2
    memset(fs_state.dir_table_buf.table, 0, BLOCK_SIZE);
    init_directory_table(&fs_state.dir_table_buf, "root", 0);
    write_blocks(fs_state.fat_table.cluster_map, 2, 1);
}

void initialize_filesystem_fat32(void) {
    struct FAT32DriverState fs_state;
    if (is_empty_storage()) {
        create_fat32();
    }
    else {
        read_blocks(fs_state.cluster_buf.buf, 0, 1);
        read_blocks(fs_state.fat_table.cluster_map, 1, 1);
        read_blocks(fs_state.dir_table_buf.table, 2, 1);
    }
}

// void write_clusters(const void *ptr, uint32_t cluster_number, uint8_t cluster_count) {
//     uint32_t lba = cluster_to_lba(cluster_number);
//     write_blocks(ptr, lba, cluster_count);
// }

// void read_clusters(void *ptr, uint32_t cluster_number, uint8_t cluster_count) {
//     uint32_t lba = cluster_to_lba(cluster_number);
//     read_blocks(ptr, lba, cluster_count);
// }