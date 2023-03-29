#include "../lib-header/stdtype.h"
#include "fat32.h"
#include "../lib-header/stdmem.h"

struct FAT32DriverState fs_state;

const uint8_t fs_signature[BLOCK_SIZE] = {
    'C', 'o', 'u', 'r', 's', 'e', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  ' ',
    'D', 'e', 's', 'i', 'g', 'n', 'e', 'd', ' ', 'b', 'y', ' ', ' ', ' ', ' ',  ' ',
    'L', 'a', 'b', ' ', 'S', 'i', 's', 't', 'e', 'r', ' ', 'I', 'T', 'B', ' ',  ' ',
    'M', 'a', 'd', 'e', ' ', 'w', 'i', 't', 'h', ' ', '<', '3', ' ', ' ', ' ',  ' ',
    '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '2', '0', '2', '3', '\n',
    [BLOCK_SIZE-2] = 'O',
    [BLOCK_SIZE-1] = 'k',
};

uint32_t find_empty_cluster(void) {
    for (int i = 3; i < CLUSTER_MAP_SIZE; i++) {
        if (fs_state.fat_table.cluster_map[i] == FAT32_FAT_EMPTY_ENTRY) {
            return i;
        }
    }
    return 0;
}


uint32_t cluster_to_lba(uint32_t cluster) {
    return (cluster) * CLUSTER_BLOCK_COUNT;
}

void init_directory_table(struct FAT32DirectoryTable *dir_table, char *name, uint32_t parent_dir_cluster) {
    // Set the directory name
    memcpy(dir_table->table[0].name, name, 8);

    // Set the directory attributes
    dir_table->table[0].user_attribute = UATTR_NOT_EMPTY;
    dir_table->table[0].attribute = ATTR_SUBDIRECTORY ;

    // Set the parent directory cluster and the filesize to 0
    dir_table->table[0].cluster_high = (uint16_t) (parent_dir_cluster >> 16);
    dir_table->table[0].cluster_low = (uint16_t) (parent_dir_cluster & 0xFFFF);
    dir_table->table[0].filesize = 0;
}

bool is_empty_storage(void) {
    read_clusters(fs_state.cluster_buf.buf, BOOT_SECTOR, 1);
    return memcmp(fs_state.cluster_buf.buf, fs_signature, BLOCK_SIZE);
}


void create_fat32(void) {
    // Write fs_signature into boot sector
    memcpy(fs_state.cluster_buf.buf, fs_signature, BLOCK_SIZE);
    write_clusters(fs_state.cluster_buf.buf, BOOT_SECTOR, 1);

    // Write proper FileAllocationTable (contain CLUSTER_0_VALUE, CLUSTER_1_VALUE, and initialized root directory) into cluster number 1
    memset(fs_state.fat_table.cluster_map, 0, BLOCK_SIZE);
    fs_state.fat_table.cluster_map[0] = CLUSTER_0_VALUE;
    fs_state.fat_table.cluster_map[1] = CLUSTER_1_VALUE;
    fs_state.fat_table.cluster_map[2] = FAT32_FAT_END_OF_FILE;
    for (int i = 3; i < CLUSTER_MAP_SIZE; i++) {
        fs_state.fat_table.cluster_map[i] = 0x00000000;
    }
    write_clusters(fs_state.fat_table.cluster_map, FAT_CLUSTER_NUMBER, 1);

    // Write root directory into cluster number 2
    memset(fs_state.dir_table_buf.table, 0, BLOCK_SIZE);
    init_directory_table(&fs_state.dir_table_buf, "root", ROOT_CLUSTER_NUMBER);
    write_clusters(fs_state.dir_table_buf.table, ROOT_CLUSTER_NUMBER, 1);
}

void initialize_filesystem_fat32(void) {
    if (is_empty_storage()) {
        create_fat32();
    }
    else {
        read_clusters(fs_state.fat_table.cluster_map, FAT_CLUSTER_NUMBER, 1);
        read_clusters(fs_state.dir_table_buf.table, ROOT_CLUSTER_NUMBER, 1);
    }
}

void write_clusters(const void *ptr, uint32_t cluster_number, uint8_t cluster_count) {
    write_blocks(ptr, cluster_to_lba(cluster_number), cluster_count*CLUSTER_BLOCK_COUNT);
}

void read_clusters(void *ptr, uint32_t cluster_number, uint8_t cluster_count) {
    read_blocks(ptr, cluster_to_lba(cluster_number), cluster_count*CLUSTER_BLOCK_COUNT);
}

int8_t write(struct FAT32DriverRequest request) {
    // check if parent cluster is valid
    if (
    request.parent_cluster_number > CLUSTER_MAP_SIZE 
    || fs_state.fat_table.cluster_map[request.parent_cluster_number] == FAT32_FAT_EMPTY_ENTRY
    || fs_state.dir_table_buf.table[request.parent_cluster_number].ext == 0x00) 
    {
        return 2;
    }

    // Check if the file is already exist
    for (int i = 0; i < CLUSTER_MAP_SIZE; i++) {
        if (fs_state.fat_table.cluster_map[i] != FAT32_FAT_EMPTY_ENTRY) {
            if (request.name == fs_state.dir_table_buf.table[i].name
            && request.ext == fs_state.dir_table_buf.table[i].ext)
            {
                return 1;
            }
        }
    }

    // // Find an empty directory entry
    // int8_t empty_dir_entry = -1;
    // for (int i = 0; i < MAX_FILE_COUNT; i++) {
    //     if (fs_state.dir_table_buf.table[i].name[0] == 0x00) {
    //         empty_dir_entry = i;
    //         break;
    //     }
    // }
    // if (empty_dir_entry == -1) {
    //     return -1;
    // }

    // Find an empty cluster
    uint32_t empty_cluster = find_empty_cluster();
    if (empty_cluster == 0) {
        return -1;
    }
    // Update the directory entry
    memcpy(fs_state.dir_table_buf.table[empty_cluster].name, request.name, 8);
    // memcpy(fs_state.dir_table_buf.table[empty_cluster].ext, request.ext, 3);
    fs_state.dir_table_buf.table[empty_cluster].user_attribute = UATTR_NOT_EMPTY;
    fs_state.dir_table_buf.table[empty_cluster].attribute = ATTR_SUBDIRECTORY;
    fs_state.dir_table_buf.table[empty_cluster].cluster_high = (uint16_t) (empty_cluster >> 16);
    fs_state.dir_table_buf.table[empty_cluster].cluster_low = (uint16_t) (empty_cluster & 0xFFFF);
    fs_state.dir_table_buf.table[empty_cluster].filesize = request.buffer_size;

    // Update the FAT table
    fs_state.fat_table.cluster_map[empty_cluster] = empty_cluster;

    // Write the file into the empty cluster
    write_clusters(&request, empty_cluster, 1);

    // Write the updated directory entry and FAT table into the storage
    write_clusters(fs_state.dir_table_buf.table, ROOT_CLUSTER_NUMBER, 1);
    write_clusters(fs_state.fat_table.cluster_map, FAT_CLUSTER_NUMBER, 1);
    return 0;
}