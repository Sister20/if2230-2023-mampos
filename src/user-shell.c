#include "lib-header/stdtype.h"
#include "filesystem/fat32.h"
#include "lib-header/framebuffer.h"

static uint32_t current_working_directory = ROOT_CLUSTER_NUMBER;

void syscall(uint32_t eax, uint32_t ebx, uint32_t ecx, uint32_t edx)
{
    __asm__ volatile("mov %0, %%ebx"
                     : /* <Empty> */
                     : "r"(ebx));
    __asm__ volatile("mov %0, %%ecx"
                     : /* <Empty> */
                     : "r"(ecx));
    __asm__ volatile("mov %0, %%edx"
                     : /* <Empty> */
                     : "r"(edx));
    __asm__ volatile("mov %0, %%eax"
                     : /* <Empty> */
                     : "r"(eax));
    // Note : gcc usually use %eax as intermediate register,
    //        so it need to be the last one to mov
    __asm__ volatile("int $0x30");
}

void puts(char *str, uint32_t len, uint8_t color)
{
    syscall(5, (uint32_t)str, len, color);
}

int memcmp(const void *s1, const void *s2, size_t n)
{
    const uint8_t *buf1 = (const uint8_t *)s1;
    const uint8_t *buf2 = (const uint8_t *)s2;
    for (size_t i = 0; i < n; i++)
    {
        if (buf1[i] < buf2[i])
            return -1;
        else if (buf1[i] > buf2[i])
            return 1;
    }

    return 0;
}

void *memcpy(void *restrict dest, const void *restrict src, size_t n)
{
    uint8_t *dstbuf = (uint8_t *)dest;
    const uint8_t *srcbuf = (const uint8_t *)src;
    for (size_t i = 0; i < n; i++)
        dstbuf[i] = srcbuf[i];
    return dstbuf;
}

struct ClusterBuffer cl = {0};
struct FAT32DriverRequest request = {0};
void parse_command(uint32_t buf)
{
    int32_t retcode;
    if (memcmp((char *)buf, "clear", 5) == 0)
    {
        syscall(6, 0, 0, 0);
    }
    else if (memcmp((char *)buf, "cd", 2) == 0)
    {
        syscall(5, buf + 3, 16 - 3, 0xF);
    }
    else if (memcmp((char *)buf, "ls", 2) == 0)
    {
        struct FAT32DriverRequest request = {
            .name = "root",
            .buf = &cl,
            .parent_cluster_number = current_working_directory,
            .buffer_size = 0,
        };
        struct FAT32DirectoryTable table = {0};
        request.buf = &table;
        syscall(1, (uint32_t)&request, (uint32_t)&retcode, 0);
        if (retcode == 0)
        {
            for (int i = 0; i < 16; i++)
            {
                if (table.table[i].name[0] == 0)
                {
                    break;
                }
                puts(table.table[i].name, 8, 0xF);
                if (table.table[i].ext[0] != 0) {
                    puts(".", 1, 0xF);
                    puts(table.table[i].ext, 3, 0xF);
                }
                puts("\n", 1, 0xF);
            }
        }
        else
        {
            puts("Read Failed", 11, 0x4);
        }
    }
    // else if (memcmp((char *)buf, "touch", 5) == 0)
    // {
    //     const char *name = (const char *)(buf + 6);
    //     struct FAT32DriverRequest request = {
    //         .parent_cluster_number = current_working_directory,
    //         .buffer_size = 0,
    //     };
    //     memcpy(request.name, name, sizeof(request.name) - 1);
    //     syscall(4, (uint32_t)&request, (uint32_t)&retcode, 0);
    //     if (retcode == 0)
    //     {
    //         puts("Write File Success", 19, 0x2);
    //     }
    //     else
    //     {
    //         puts("Write File Failed", 18, 0x4);
    //     }
    // }
    else if (memcmp((char *)buf, "mkdir", 5) == 0)
    {
        const char *name = (const char *)(buf + 6);
        struct FAT32DriverRequest request = {
            .parent_cluster_number = current_working_directory,
            .buffer_size = 0,
        };
        memcpy(request.name, name, sizeof(request.name) - 1);
        syscall(2, (uint32_t)&request, (uint32_t)&retcode, 0);
        if (retcode == 0)
        {
            puts("Write Directory Success", 23, 0x2);
        }
        else
        {
            puts("Write Directory Failed", 22, 0x4);
        }
    }
    else if (memcmp((char *)buf, "cat", 3) == 0)
    {
        const char *name = (const char *)(buf + 4);

        struct FAT32DriverRequest request =
            {
                .parent_cluster_number = current_working_directory,
                .buf = &cl,
                // .buffer_size = 256,
            };
        // loop until find .
        int count = 0;
        for (int i = 0; i < 8; i++)
        {
            if (name[i] == '.')
            {
                break;
            }
            request.name[i] = name[i];
            count++;
        }
        memcpy(request.name, name, count);
        memcpy(request.ext, name + count + 1, 3);
        syscall(0, (uint32_t)&request, (uint32_t)&retcode, 0);
        if (retcode == 0)
        {
            puts(request.buf, 256, 0xF);
            // syscall(7, 0, 0, 0);
        }
        else
        {
            puts("Read File Failed", 16, 0x4);
        }
    }
    else if (memcmp((char *)buf, "cp", 2) == 0)
    {
    }
    else if (memcmp((char *)buf, "rm", 2) == 0)
    {
        const char *name = (const char *)(buf + 3);
        struct FAT32DriverRequest request = {
            .parent_cluster_number = current_working_directory,
        };
        // loop until find .
        int count = 0;
        for (int i = 0; i < 8; i++)
        {
            if (name[i] == '.')
            {
                break;
            }
            request.name[i] = name[i];
            count++;
        }
        memcpy(request.name, name, count);
        memcpy(request.ext, name + count + 1, 3);
        syscall(3, (uint32_t)&request, (uint32_t)&retcode, 0);
        if (retcode == 0)
        {
            puts("Delete File Success", 20, 0x2);
        }
        else
        {
            puts("Delete File Failed", 19, 0x4);
        }
    }
    else if (memcmp((char *)buf, "mv", 2) == 0)
    {
    }
    else if (memcmp((char *)buf, "whereis", 7) == 0)
    {
    }
    else
    {
        puts("Command not found", 17, 0x4);
    }
}

int main(void)
{
    char buf[16];
    while (TRUE)
    {
        puts("mampOS@OS-IF2230", 16, 0x2);
        puts(":", 1, 0x8);
        puts("/", 1, 0x1);
        puts("$ ", 2, 0x8);
        syscall(4, (uint32_t)buf, 16, 0);
        parse_command((uint32_t)buf);
        syscall(7, 0, 0, 0);
    }

    return 0;
}
