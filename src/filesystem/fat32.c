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
    init_directory_table(&fs_state.dir_table_buf, "root", ROOT_CLUSTER_NUMBER);
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
        if (fs_state.dir_table_buf.table[i].user_attribute == UATTR_NOT_EMPTY)
        {
            if (memcmp(fs_state.dir_table_buf.table[i].name, request.name, 8) == 0 && memcmp(fs_state.dir_table_buf.table[i].ext, request.ext, 3) == 0)
            {
                return TRUE;
            }
        }
        else
        {
            break;
        }
    }

    memset(&fs_state.dir_table_buf, 0, CLUSTER_SIZE);
    return FALSE;
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
    // check if parent cluster is valid
    if (!parent_cluster_valid(request))
    {
        return 2;
    }

    // check if the file is already exist
    if (file_exist(request))
    {
        return 1;
    }

    // find an empty cluster
    uint32_t empty_cluster_number = find_empty_cluster();
    if (empty_cluster_number == 0)
    {
        return -1;
    }

    // check if the request is directory or file
    bool is_dir = FALSE;
    if (request.buffer_size == 0)
    {
        is_dir = TRUE;
    }

    // create new entry
    struct FAT32DirectoryEntry new_entry = {0};
    memset(new_entry.name, 0, 8);
    memcpy(new_entry.name, request.name, 8);
    memset(new_entry.ext, 0, 3);
    memcpy(new_entry.ext, request.ext, 3);
    if (is_dir) // directory flag attribute
    {
        new_entry.attribute = ATTR_SUBDIRECTORY;
    }
    new_entry.user_attribute = UATTR_NOT_EMPTY;
    new_entry.cluster_high = (uint16_t)(empty_cluster_number) >> 16;
    new_entry.cluster_low = (uint16_t)(empty_cluster_number)&0xFFFF;
    new_entry.filesize = request.buffer_size;

    // update parent directory entry
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

    // update FAT mapping
    read_clusters(&fs_state.fat_table, FAT_CLUSTER_NUMBER, 1);
    if (is_dir)
    {
        fs_state.fat_table.cluster_map[empty_cluster_number] = FAT32_FAT_END_OF_FILE;
    }
    else
    {
        fs_state.fat_table.cluster_map[empty_cluster_number] = FAT32_FAT_END_OF_FILE;
    }
    write_clusters(&fs_state.fat_table, FAT_CLUSTER_NUMBER, 1);

    // write cluster
    if (is_dir)
    {
        memset(&fs_state.dir_table_buf, 0, CLUSTER_SIZE);
        read_clusters(&fs_state.dir_table_buf, empty_cluster_number, 1);
        init_directory_table(&fs_state.dir_table_buf, request.name, request.parent_cluster_number);
        write_clusters(&fs_state.dir_table_buf, empty_cluster_number, 1);
    }
    else
    {
        memset(&fs_state.cluster_buf, 0, CLUSTER_SIZE);
        memcpy(&fs_state.cluster_buf, request.buf, request.buffer_size);
        write_clusters(&fs_state.cluster_buf, empty_cluster_number, 1);
    }
    return 0;
}

int8_t delete(struct FAT32DriverRequest request)
{
    // for (int i = 0; i < (int)(CLUSTER_SIZE / sizeof(struct FAT32DirectoryEntry)); i++)
    // {
    //     if (!memcmp(fs_state.dir_table_buf.table[i].name, request.name, 8) && !memcmp(fs_state.dir_table_buf.table[i].ext, request.ext, 3))
    //     {
    //         uint32_t entry_cluster = (fs_state.dir_table_buf.table[i].cluster_high << 16) | fs_state.dir_table_buf.table[i].cluster_low;
    //         fs_state.dir_table_buf.table[i].user_attribute = 0;
    //         memcpy(fs_state.dir_table_buf.table[i].name, "\0\0\0\0\0\0\0\0", 8);
    //         memcpy(fs_state.dir_table_buf.table[i].ext, "\0\0\0", 3);
    //         write_clusters(fs_state.dir_table_buf.table, request.parent_cluster_number, 1);
    //         do
    //         {
    //             uint32_t next_cluster = fs_state.fat_table.cluster_map[entry_cluster];
    //             fs_state.fat_table.cluster_map[entry_cluster] = FAT32_FAT_EMPTY_ENTRY;
    //             entry_cluster = next_cluster;
    //         } while (entry_cluster != FAT32_FAT_END_OF_FILE);
    //         fs_state.fat_table.cluster_map[entry_cluster] = FAT32_FAT_EMPTY_ENTRY;
    //         write_clusters(&fs_state.fat_table, FAT_CLUSTER_NUMBER, 1);
    //     }
    //     else if (!memcmp(fs_state.dir_table_buf.table[i].name, request.name, 8))
    //     {
    //         uint32_t entry_cluster = (fs_state.dir_table_buf.table[i].cluster_high << 16) | fs_state.dir_table_buf.table[i].cluster_low;
    //         fs_state.fat_table.cluster_map[entry_cluster] = FAT32_FAT_EMPTY_ENTRY;
    //         memcpy(fs_state.dir_table_buf.table[i].name, "\0\0\0\0\0\0\0\0", 8);
    //         write_clusters(&fs_state.dir_table_buf, request.parent_cluster_number, 1);
    //         write_clusters(&fs_state.fat_table, FAT_CLUSTER_NUMBER, 1);
    //     }
    // }

    // check is parent valid
    if (!parent_cluster_valid(request))
    {
        return 1;
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
    // for (int i = 0; i < CLUSTER_MAP_SIZE; i++)
    // {
    //     if (fs_state.fat_table.cluster_map[i] != FAT32_FAT_EMPTY_ENTRY)
    //     {
    //         if (request.name == fs_state.dir_table_buf.table[i].name && request.ext == fs_state.dir_table_buf.table[i].ext)
    //         {
    //             // Check if the file is a file
    //             if (fs_state.dir_table_buf.table[i].attribute != 1)
    //             {
    //                 return 1;
    //             }
    //             // Check if the buffer is enough
    //             if (request.buffer_size < fs_state.dir_table_buf.table[i].filesize)
    //             {
    //                 return 2;
    //             }
    //             // Read the file
    //             read_clusters(request.buf, i, 1);
    //             return 0;
    //         }
    //     }
    // }

    // Check if the file is exist
    if (!file_exist(request))
    {
        return 3;
    }

    if (request.buffer_size <= 0)
    {
        return 1;
    }

    read_clusters(&fs_state.fat_table, FAT_CLUSTER_NUMBER, 1);
    for (uint32_t i = 2; i < CLUSTER_MAP_SIZE; i++)
    {
        // find and read from parent dir table
        read_clusters(&fs_state.dir_table_buf, i, 1);
        for (uint32_t j = 1; j < (CLUSTER_SIZE / sizeof(struct FAT32DirectoryEntry)); j++)
        {
            if (memcmp(fs_state.dir_table_buf.table[j].name, request.name, 8) == 0 && memcmp(fs_state.dir_table_buf.table[j].ext, request.ext, 3) == 0)
            {
                read_clusters(request.buf, i, 1);
                return 0;
            }
        }
    }

    return -1;
}

// int8_t read_directory(struct FAT32DriverRequest request)
// {
//     // Check if the directory is exist
//     for (int i = 0; i < CLUSTER_MAP_SIZE; i++)
//     {
//         if (fs_state.fat_table.cluster_map[i] != FAT32_FAT_EMPTY_ENTRY)
//         {
//             if (request.name == fs_state.dir_table_buf.table[i].name && request.ext == fs_state.dir_table_buf.table[i].ext)
//             {
//                 // Check if the directory is a directory
//                 if (fs_state.dir_table_buf.table[i].attribute == 1)
//                 {
//                     return 1;
//                 }
//                 // Check if the buffer is enough
//                 if (request.buffer_size < sizeof(struct FAT32DirectoryTable))
//                 {
//                     return 2;
//                 }
//                 // Read the directory
//                 read_clusters(request.buf, i, 1);
//                 return 0;
//             }
//         }
//     }
// }

// int32_t fat32_read_file(struct fat32_read_file_request request)
// {
//     // check if parent cluster is valid
//     if (request.parent_cluster_number > CLUSTER_MAP_SIZE || fs_state.fat_table.cluster_map[request.parent_cluster_number] == FAT32_FAT_EMPTY_ENTRY || fs_state.dir_table_buf.table[request.parent_cluster_number].ext == 0x00)
//     {
//         return 2;
//     }

//     // check if file is exist
//     uint32_t file_cluster_number = 0;
//     for (uint32_t i = 0; i < CLUSTER_MAP_SIZE; i++)
//     {
//         if (fs_state.fat_table.cluster_map[i] != FAT32_FAT_EMPTY_ENTRY)
//         {
//             if (request.name == fs_state.dir_table_buf.table[i].name && request.ext == fs_state.dir_table_buf.table[i].ext)
//             {
//                 file_cluster_number = i;
//                 break;
//             }
//         }
//     }
//     if (file_cluster_number == 0)
//     {
//         return 1;
//     }

//     // read file
//     memset(&fs_state.cluster_buf, 0, CLUSTER_SIZE);
//     read_clusters(&fs_state.cluster_buf, file_cluster_number, 1);
//     memcpy(request.buf, &fs_state.cluster_buf, request.buffer_size);

//     return 0;
// }
