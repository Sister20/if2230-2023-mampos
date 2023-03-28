#include "lib-header/stdtype.h"
#include "fat32.h"
#include "lib-header/stdmem.h"


const uint8_t fs_signature[BLOCK_SIZE] = {
    'C', 'o', 'u', 'r', 's', 'e', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  ' ',
    'D', 'e', 's', 'i', 'g', 'n', 'e', 'd', ' ', 'b', 'y', ' ', ' ', ' ', ' ',  ' ',
    'L', 'a', 'b', ' ', 'S', 'i', 's', 't', 'e', 'r', ' ', 'I', 'T', 'B', ' ',  ' ',
    'M', 'a', 'd', 'e', ' ', 'w', 'i', 't', 'h', ' ', '<', '3', ' ', ' ', ' ',  ' ',
    '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '2', '0', '2', '3', '\n',
    [BLOCK_SIZE-2] = 'O',
    [BLOCK_SIZE-1] = 'k',
};

uint32_t cluster_to_lba(uint32_t cluster) {
    return (cluster - 2) * CLUSTER_BLOCK_COUNT + 1;
}

// void init_directory_table(struct FAT32DirectoryTable *dir_table, char *name, uint32_t parent_dir_cluster) {
//     dir_table->parent_dir_cluster = parent_dir_cluster;
//     dir_table->cluster_number = 0;
//     dir_table->size = 0;
//     dir_table->entry_count = 0;
//     dir_table->entries = NULL;
//     dir_table->name = name;
// }

bool is_empty_storage(void) {
    uint8_t boot_sector[BLOCK_SIZE];
    read_block(boot_sector, 0);
    return !memcmp(boot_sector, fs_signature, BLOCK_SIZE); //masih bs kebalik
}

void create_fat32(void) {
    uint8_t boot_sector[BLOCK_SIZE];
    uint8_t fat[BLOCK_SIZE];
    uint8_t root_dir[BLOCK_SIZE];
    uint32_t *fat_ptr = (uint32_t *) fat;

    memset(boot_sector, 0, BLOCK_SIZE);
    memset(fat, 0, BLOCK_SIZE);
    memset(root_dir, 0, BLOCK_SIZE);

    memcpy(boot_sector, fs_signature, BLOCK_SIZE);
    fat_ptr[0] = CLUSTER_0_VALUE;
    fat_ptr[1] = CLUSTER_1_VALUE;
    write_block(boot_sector, 0);
    write_block(fat, 1);
    write_block(root_dir, 2);
}

void initialize_filesystem_fat32(void) {
    if (is_empty_storage()) {
        create_fat32();
    }
    else {
        // Else, read and cache entire FileAllocationTable (located at cluster number 1) into driver state
        
    }
}

void write_clusters(const void *ptr, uint32_t cluster_number, uint8_t cluster_count) {
    uint32_t lba = cluster_to_lba(cluster_number);
    write_blocks(ptr, lba, cluster_count);
}

void read_clusters(void *ptr, uint32_t cluster_number, uint8_t cluster_count) {
    uint32_t lba = cluster_to_lba(cluster_number);
    read_blocks(ptr, lba, cluster_count);
}
