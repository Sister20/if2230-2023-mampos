#include "../lib-header/stdtype.h"
#include "fat32.h"
#include "../lib-header/stdmem.h"

struct FAT32DriverState fs_state = {0};

const uint8_t fs_signature[BLOCK_SIZE] = {
    'C',
    'o',
    'u',
    'r',
    's',
    'e',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    'D',
    'e',
    's',
    'i',
    'g',
    'n',
    'e',
    'd',
    ' ',
    'b',
    'y',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    'L',
    'a',
    'b',
    ' ',
    'S',
    'i',
    's',
    't',
    'e',
    'r',
    ' ',
    'I',
    'T',
    'B',
    ' ',
    ' ',
    'M',
    'a',
    'd',
    'e',
    ' ',
    'w',
    'i',
    't',
    'h',
    ' ',
    '<',
    '3',
    ' ',
    ' ',
    ' ',
    ' ',
    '-',
    '-',
    '-',
    '-',
    '-',
    '-',
    '-',
    '-',
    '-',
    '-',
    '-',
    '2',
    '0',
    '2',
    '3',
    '\n',
    [BLOCK_SIZE - 2] = 'O',
    [BLOCK_SIZE - 1] = 'k',
};

uint32_t cluster_to_lba(uint32_t cluster)
{
    return (cluster)*CLUSTER_BLOCK_COUNT;
}

void init_directory_table(struct FAT32DirectoryTable *dir_table, char *name, uint32_t parent_dir_cluster)
{
    // Set the directory name
    memcpy(dir_table->table[0].name, name, 8);

    // Set the directory attributes
    dir_table->table[0].user_attribute = UATTR_NOT_EMPTY;
    dir_table->table[0].attribute = ATTR_SUBDIRECTORY;

    // Set the parent directory cluster and the filesize to 0
    dir_table->table[0].cluster_high = (uint16_t)(parent_dir_cluster >> 16);
    dir_table->table[0].cluster_low = (uint16_t)(parent_dir_cluster & 0xFFFF);
    dir_table->table[0].filesize = 0;
}

bool is_empty_storage(void)
{
    // read_clusters(fs_state.cluster_buf.buf, BOOT_SECTOR, 1);
    read_clusters(&fs_state.cluster_buf, BOOT_SECTOR, 1);
    return memcmp(&fs_state.cluster_buf, fs_signature, BLOCK_SIZE);
}

void create_fat32(void)
{
    // Write fs_signature into boot sector
    memcpy(&fs_state.cluster_buf, fs_signature, BLOCK_SIZE);
    // write_clusters(fs_state.cluster_buf.buf, BOOT_SECTOR, 1);
    write_clusters(&fs_state.cluster_buf, BOOT_SECTOR, 1);

    // Write proper FileAllocationTable (contain CLUSTER_0_VALUE, CLUSTER_1_VALUE, and initialized root directory) into cluster number 1
    memset(&fs_state.fat_table, 0, BLOCK_SIZE);
    fs_state.fat_table.cluster_map[0] = CLUSTER_0_VALUE;
    fs_state.fat_table.cluster_map[1] = CLUSTER_1_VALUE;
    fs_state.fat_table.cluster_map[2] = FAT32_FAT_END_OF_FILE;
    for (int i = 3; i < CLUSTER_MAP_SIZE; i++)
    {
        fs_state.fat_table.cluster_map[i] = 0x00000000;
    }
    write_clusters(&fs_state.fat_table, FAT_CLUSTER_NUMBER, 1);

    // Write root directory into cluster number 2
    memset(&fs_state.dir_table_buf, 0, BLOCK_SIZE);
    init_directory_table(&fs_state.dir_table_buf, "root\0\0\0", ROOT_CLUSTER_NUMBER);
    write_clusters(&fs_state.dir_table_buf, ROOT_CLUSTER_NUMBER, 1);
}

void initialize_filesystem_fat32(void)
{
    if (is_empty_storage())
    {
        create_fat32();
    }
    else
    {
        read_clusters(&fs_state.fat_table, FAT_CLUSTER_NUMBER, 1);
        read_clusters(&fs_state.dir_table_buf, ROOT_CLUSTER_NUMBER, 1);
    }
}

void write_clusters(const void *ptr, uint32_t cluster_number, uint8_t cluster_count)
{
    write_blocks(ptr, cluster_to_lba(cluster_number), cluster_count * CLUSTER_BLOCK_COUNT);
}

void read_clusters(void *ptr, uint32_t cluster_number, uint8_t cluster_count)
{
    read_blocks(ptr, cluster_to_lba(cluster_number), cluster_count * CLUSTER_BLOCK_COUNT);
}

bool parent_cluster_valid(struct FAT32DriverRequest request)
{
    return request.parent_cluster_number < CLUSTER_MAP_SIZE && fs_state.fat_table.cluster_map[request.parent_cluster_number] != FAT32_FAT_EMPTY_ENTRY;
}

bool file_exist(struct FAT32DriverRequest request)
{
    memset(&fs_state.dir_table_buf, 0, CLUSTER_SIZE);
    read_clusters(&fs_state.dir_table_buf, request.parent_cluster_number, 1);

    for (uint32_t i = 1; i < CLUSTER_SIZE / sizeof(struct FAT32DirectoryEntry); i++)
    {
        if (memcmp(fs_state.dir_table_buf.table[i].name, request.name, 8) == 0 && memcmp(fs_state.dir_table_buf.table[i].ext, request.ext, 3) == 0)
        {
            return TRUE;
        }
    }

    memset(&fs_state.dir_table_buf, 0, CLUSTER_SIZE);
    return FALSE;
}

bool find_entry_index(struct FAT32DriverRequest request)
{
    memset(&fs_state.dir_table_buf, 0, CLUSTER_SIZE);
    read_clusters(&fs_state.dir_table_buf, request.parent_cluster_number, 1);

    for (uint32_t i = 1; i < CLUSTER_SIZE / sizeof(struct FAT32DirectoryEntry); i++)
    {
        if (memcmp(fs_state.dir_table_buf.table[i].name, request.name, 8) == 0 && memcmp(fs_state.dir_table_buf.table[i].ext, request.ext, 3) == 0)
        {
            return i;
        }
    }
    memset(&fs_state.dir_table_buf, 0, CLUSTER_SIZE);
    return -1;
}

uint32_t find_empty_cluster(void)
{
    memset(&fs_state.fat_table, 0, CLUSTER_SIZE);
    read_clusters(&fs_state.fat_table, FAT_CLUSTER_NUMBER, 1);

    for (uint32_t i = 3; i < CLUSTER_MAP_SIZE; i++)
    {
        if (fs_state.fat_table.cluster_map[i] == FAT32_FAT_EMPTY_ENTRY)
        {
            return i;
        }
    }

    memset(&fs_state.fat_table, 0, CLUSTER_SIZE);
    return 0;
}

bool is_dir_empty(struct FAT32DriverRequest request)
{
    read_clusters(&fs_state.fat_table, FAT_CLUSTER_NUMBER, 1);
    for (uint32_t i = 3; i < CLUSTER_MAP_SIZE; i++)
    {
        read_clusters(&fs_state.dir_table_buf, i, 1);
        if (fs_state.dir_table_buf.table[i].filesize == 0 && memcmp(request.name, fs_state.dir_table_buf.table[i].name, 8) == 0)
        {
            for (uint32_t j = 1; j < CLUSTER_SIZE / sizeof(struct FAT32DirectoryEntry); j++)
            {
                if (fs_state.dir_table_buf.table[j].user_attribute != UATTR_NOT_EMPTY)
                {
                    return FALSE;
                }
            }
        }
    }

    return TRUE;
}

int8_t write(struct FAT32DriverRequest request)
{
    // 1. check if parent cluster is valid
    if (!parent_cluster_valid(request))
    {
        return 2;
    }

    // 2. check if the file is already exist
    if (file_exist(request))
    {
        return 1;
    }

    // 3. check if the request is directory or file
    bool is_dir = FALSE;
    if (request.buffer_size == 0)
    {
        is_dir = TRUE;
    }

    // 4. Calculate how many clusters needed depending on buffer size
    int32_t clusters_needed = (request.buffer_size / CLUSTER_SIZE) + 1;
    for (int i = 0; i < clusters_needed; i++)
    {
        // 5. Find an empty cluster
        uint32_t empty_cluster_number = find_empty_cluster();
        if (empty_cluster_number == 0)
        {
            return -1;
        }

        if (i == 0)
        {
            // 6. create new directory entry
            struct FAT32DirectoryEntry new_entry = {0};
            memset(new_entry.name, 0, 8);
            memcpy(new_entry.name, request.name, 8);
            if (is_dir) // directory flag attribute
            {
                new_entry.attribute = ATTR_SUBDIRECTORY;
            }
            else
            {
                memset(new_entry.ext, 0, 3);
                memcpy(new_entry.ext, request.ext, 3);
            }

            new_entry.user_attribute = UATTR_NOT_EMPTY;
            new_entry.cluster_high = (uint16_t)(empty_cluster_number) >> 16;
            new_entry.cluster_low = (uint16_t)(empty_cluster_number)&0xFFFF;
            new_entry.filesize = request.buffer_size;

            // 7. update parent directory entry
            memset(&fs_state.dir_table_buf, 0, CLUSTER_SIZE);
            read_clusters(&fs_state.dir_table_buf, request.parent_cluster_number, 1);
            for (uint8_t i = 1; i < CLUSTER_SIZE / sizeof(struct FAT32DirectoryEntry); i++)
            {
                if (fs_state.dir_table_buf.table[i].user_attribute != UATTR_NOT_EMPTY)
                {
                    fs_state.dir_table_buf.table[i] = new_entry;
                    break;
                }
            }
            write_clusters(&fs_state.dir_table_buf, request.parent_cluster_number, 1);
        }

        // 8. update FAT mapping
        read_clusters(&fs_state.fat_table, FAT_CLUSTER_NUMBER, 1);
        fs_state.fat_table.cluster_map[empty_cluster_number] = FAT32_FAT_END_OF_FILE; // Insert dummy fat entry to find next_empty_cluster_number
        write_clusters(&fs_state.fat_table, FAT_CLUSTER_NUMBER, 1);

        read_clusters(&fs_state.fat_table, FAT_CLUSTER_NUMBER, 1);
        if (is_dir)
        {
            fs_state.fat_table.cluster_map[empty_cluster_number] = FAT32_FAT_END_OF_FILE;
        }
        else
        {
            if (i == clusters_needed - 1)
            {
                fs_state.fat_table.cluster_map[empty_cluster_number] = FAT32_FAT_END_OF_FILE;
            }
            else
            {
                uint32_t next_empty_cluster_number = find_empty_cluster();
                fs_state.fat_table.cluster_map[empty_cluster_number] = next_empty_cluster_number;
            }
        }
        write_clusters(&fs_state.fat_table, FAT_CLUSTER_NUMBER, 1);

        // 9. write cluster
        if (is_dir)
        {
            memset(&fs_state.dir_table_buf, 0, CLUSTER_SIZE);
            read_clusters(&fs_state.dir_table_buf, empty_cluster_number, 1);
            init_directory_table(&fs_state.dir_table_buf, request.name, request.parent_cluster_number);
            write_clusters(&fs_state.dir_table_buf, empty_cluster_number, 1);
            break;
        }
        else
        {
            memset(&fs_state.cluster_buf, 0, CLUSTER_SIZE);
            if (i != clusters_needed - 1)
            {
                memcpy(&fs_state.cluster_buf, request.buf + (i * CLUSTER_SIZE), CLUSTER_SIZE);
            }
            else
            {
                memcpy(&fs_state.cluster_buf, request.buf + (i * CLUSTER_SIZE), request.buffer_size % CLUSTER_SIZE);
            }
            write_clusters(&fs_state.cluster_buf, empty_cluster_number, 1);
        }
    }
    return 0;
}

int8_t delete(struct FAT32DriverRequest request)
{
    // check is parent valid
    if (!parent_cluster_valid(request))
    {
        return -1;
    }

    // check is the file exist
    if (!file_exist(request))
    {
        return 1;
    }

    // check if the request is directory
    bool is_dir = FALSE;
    if (request.buffer_size == 0)
    {
        is_dir = TRUE;
    }

    read_clusters(&fs_state.fat_table, FAT_CLUSTER_NUMBER, 1);
    read_clusters(&fs_state.dir_table_buf, request.parent_cluster_number, 1);

    uint32_t entry_cluster = 0x0;
    // find from parent dir table
    for (uint32_t i = 1; i < (CLUSTER_SIZE / sizeof(struct FAT32DirectoryEntry)); i++)
    {
        if (memcmp(fs_state.dir_table_buf.table[i].name, request.name, 8) == 0 && memcmp(fs_state.dir_table_buf.table[i].ext, request.ext, 3) == 0)
        {
            entry_cluster = (uint32_t)fs_state.dir_table_buf.table[i].cluster_high << 16 | (uint32_t)fs_state.dir_table_buf.table[i].cluster_low;
            // entry_cluster = (fs_state.dir_table_buf.table[i].cluster_high << 16) | fs_state.dir_table_buf.table[i].cluster_low;
            struct FAT32DirectoryEntry new_entry = {0};
            fs_state.dir_table_buf.table[i] = new_entry;
            break;
        }
    }

    if (is_dir
        // && !is_dir_empty(request)
    )
    {
        // return 2;
    }

    // delete from parent dir table
    write_clusters(&fs_state.dir_table_buf, request.parent_cluster_number, 1);

    // delete from FAT
    fs_state.fat_table.cluster_map[entry_cluster] = 0x00;
    write_clusters(&fs_state.fat_table, FAT_CLUSTER_NUMBER, 1);

    return 0;
}

int8_t read(struct FAT32DriverRequest request)
{
    // // iterate all the directory entries
    // for (int i = 0; i < 64; i++)
    // {
    //     // check if the file is exist
    //     if (memcmp(fs_state.dir_table_buf.table[i].name, request.name, 8) == 0 && memcmp(fs_state.dir_table_buf.table[i].ext, request.ext, 3) == 0)
    //     {
    //         // check if buffer size enough
    //         // if (request.buffer_size < fs_state.dir_table_buf.table[i].filesize)
    //         // {
    //         //     return 2;
    //         //     // check if its a file
    //         // }
    //         // else
    //         if (fs_state.dir_table_buf.table[i].attribute == 1)
    //         {
    //             return 1;
    //         }
    //         uint32_t clusters = fs_state.dir_table_buf.table[i].filesize / CLUSTER_SIZE;
    //         if (clusters * CLUSTER_SIZE < fs_state.dir_table_buf.table[i].filesize)
    //         {
    //             clusters += 1;
    //         }
    //         // location is taken from cluster high and cluster low
    //         uint32_t loc = (fs_state.dir_table_buf.table[i].cluster_high << 16) | fs_state.dir_table_buf.table[i].cluster_low;
    //         for (uint32_t j = 0; j < clusters; j++)
    //         {
    //             if (j == 0)
    //             {
    //                 read_clusters(request.buf + CLUSTER_SIZE * j, loc, 1);
    //             }
    //             else
    //             {
    //                 read_clusters(request.buf + CLUSTER_SIZE * j, fs_state.fat_table.cluster_map[loc], 1);
    //                 loc = fs_state.fat_table.cluster_map[loc];
    //             }
    //         }
    //         return 0;
    //     }
    // }
    // return -1;

    // 1. check if parent cluster is valid
    if (!parent_cluster_valid(request))
    {
        return -1;
    }

    // 2. check if the file is already exist
    int8_t entryIdx = find_entry_index(request);
    if (entryIdx == -1)
    {
        return 3;
    }

    // 3. Read the directory table
    read_clusters(&fs_state.dir_table_buf, request.parent_cluster_number, 1);

    // 4. Read the entry
    struct FAT32DirectoryEntry entry = fs_state.dir_table_buf.table[entryIdx];

    // // 4. Check if buffer is enough
    // if (request.buffer_size < entry.filesize)
    // {
    //     return 2;
    // }

    // 5. Check if request is a file
    if (entry.attribute == ATTR_SUBDIRECTORY)
    {
        return 1;
    }

    int32_t loc = entry.cluster_high << 16 | entry.cluster_low;
    int32_t curr_cluster = loc;
    int16_t cluster_count = 0;
    int32_t next_cluster;
    do
    {
        read_clusters(request.buf + cluster_count * CLUSTER_SIZE, curr_cluster, 1);
        next_cluster = fs_state.fat_table.cluster_map[curr_cluster];
        curr_cluster = next_cluster;
        cluster_count++;
    } while (next_cluster != FAT32_FAT_END_OF_FILE);

    return 0;
}

// pertama read cluster trs loop nyari name ext yg sama kl ketemu abis itu
int8_t read_directory(struct FAT32DriverRequest request)
{
    // // iterate all the directory entries
    // for (int i = 0; i < 64; i++)
    // {
    //     // check if the file is exist
    //     if (memcmp(fs_state.dir_table_buf.table[i].name, request.name, 8) == 0 && memcmp(fs_state.dir_table_buf.table[i].ext, request.ext, 3) == 0)
    //     {
    //         // check if buffer size enough
    //         if (request.buffer_size < fs_state.dir_table_buf.table[i].filesize)
    //         {
    //             return 2;
    //             // check if its a file
    //         }
    //         else if (fs_state.dir_table_buf.table[i].attribute == 1)
    //         {
    //             return 1;
    //         }
    //         uint32_t clusters = fs_state.dir_table_buf.table[i].filesize / CLUSTER_SIZE;
    //         if (clusters * CLUSTER_SIZE < fs_state.dir_table_buf.table[i].filesize)
    //         {
    //             clusters += 1;
    //         }
    //         else
    //         {
    //             // location is taken from cluster high and cluster low
    //             uint32_t loc = (fs_state.dir_table_buf.table[i].cluster_high << 16) | fs_state.dir_table_buf.table[i].cluster_low;
    //             read_clusters(request.buf, loc, clusters);
    //             return 0;
    //         }
    //     }
    // }
    // return -1;
    read_clusters(&fs_state.dir_table_buf, request.parent_cluster_number, 1);
    int idx = -1;
    for (uint32_t i = 0; i < 64; i++)
    {
        if (memcmp(fs_state.dir_table_buf.table[i].name, request.name, 8) == 0)
        {
            idx = i;
            break;
        }
    }
    // Not Found
    if (idx == -1)
    {
        return 3;
    }

    // Inisialisasi entry
    // fat32_driver_state.dir_table_buf.table[idx];

    // Error jika buffer size < filesize
    if (request.buffer_size < fs_state.dir_table_buf.table[idx].filesize)
    {
        return 2;
    }

    // Error jika flag subdirectory menyala
    else if (fs_state.dir_table_buf.table[idx].attribute == 1)
    {
        return 1;
    }
    else
    {
        uint32_t entry = fs_state.dir_table_buf.table[idx].cluster_low | fs_state.dir_table_buf.table[idx].cluster_high << 16;
        read_clusters(request.buf, entry, 1);
        return 0;
    }
    return -1;
}
// {
//     // Mencari direktori yang terletak pada folder parent request.parent_cluster_number
//     uint32_t current_cluster = request.parent_cluster_number;

//     read_clusters(&fs_state.dir_table_buf, current_cluster, 1);
//     read_clusters(&fs_state.fat_table, 1, 1);

//     // Mencari file yang terletak pada folder parent request.parent_cluster_number
//     struct FAT32DirectoryEntry *entry = NULL;
//     for (uint32_t i = 0; i < CLUSTER_SIZE / sizeof(struct FAT32DirectoryEntry); i++) {
//         if (memcmp(fs_state.dir_table_buf.table[i].name, request.name, sizeof(request.name)) == 0 &&
//             (fs_state.dir_table_buf.table[i].attribute & ATTR_SUBDIRECTORY)) {
//             entry = &fs_state.dir_table_buf.table[i];
//             break;
//         }
//     }

//     if (entry == NULL) {
//         return -1;
//     }

//     uint32_t cluster_number = (entry->cluster_high << 16) | entry->cluster_low;
//     read_clusters(request.buf, cluster_number, 1);

//     return 0;
// }