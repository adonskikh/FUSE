#include "fsfileops.h"
#include <string.h>

void PrintFileSystemInfo()
{
    printf("========================\n");
    printf("File system info:\n");
    printf("free_inodes_count = %ld\n", free_inodes_count);
    printf("max_inodes_count = %ld\n", max_inodes_count);
    printf("free_blocks_count = %ld\n", free_blocks_count);
    printf("max_blocks_count = %ld\n", max_blocks_count);
    printf("block_size = %ld\n", block_size);
    printf("========================\n");
}
void PrintDir(const char *path)
{
    long i;
    long index = GetInodeIndexByPath(path);
    printf("index = %ld\n", index);
    struct dinode n = ReadInode(index);
    if(n.di_size < 0)
        return;
    //TruncFile(&n, 268*0);
    int count = n.di_size / sizeof(struct dirent);
    struct dirent items[count];
    printf("---------------------------------------------------\n");
    printf("[%s]: size = %ld, index = %ld, links = %d\n", path, n.di_size, index, n.di_nlink);

    ReadFile(&n, (void *)items, 0*sizeof(struct dirent), (count)*sizeof(struct dirent));
    for(i = 0; i<count; i++)
    {
        struct dinode item = ReadInode((items)[i].d_ino);
        printf(" %s: inode = %ld, offs = %ld, mode = %o, size = %ld\n", (items)[i].d_name, (items)[i].d_ino, (items)[i].d_off, item.di_mode, item.di_size);
        if((strcmp(items[i].d_name, "..") != 0) && (strcmp(items[i].d_name, ".") != 0) && (item.di_mode & S_IFDIR))
        {
            char newpath[strlen(path)+strlen(items[i].d_name)+2];
            strcpy(newpath, path);
            if(strcmp(path, "/") != 0) strcat(newpath, "/");
            strcat(newpath, items[i].d_name);
            printf("newpath = %s\n", newpath);
            PrintDir(newpath);
        }
    }
    printf("---------------------------------------------------\n");
}

int main(int argc, char *argv[])
{
    fsfilename = FILE_PATH;

    Load(fsfilename);
    /*int i;
    for(i = 0; i < 100; i++)
        printf("index = %ld\n", GetInodeIndexByPath("/Безымянная папка/FUSE.htm"));*/

    PrintFileSystemInfo();
    PrintDir("/");

    //fclose(file);
}
