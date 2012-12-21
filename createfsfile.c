#include "fsfileops.h"
#include <string.h>
long Create()
{
    long i;

    FILE *file;
    if((file=fopen(fsfilename,"wb"))==0)
    {
        puts ("Can't open output file.");
        return -1;
    }

    long max_inodes_count = MAX_INODES_COUNT;
    long free_inodes_count = FREE_INODES_COUNT;
    long max_blocks_count = MAX_BLOCKS_COUNT;
    long free_blocks_count = FREE_BLOCKS_COUNT;
    long block_size = BLOCK_SIZE;

    fwrite(&free_inodes_count, sizeof(free_inodes_count), 1, file);
    fwrite(&max_inodes_count, sizeof(max_inodes_count), 1, file);

    fwrite(&free_blocks_count, sizeof(free_blocks_count), 1, file);
    fwrite(&max_blocks_count, sizeof(max_blocks_count), 1, file);
    fwrite(&block_size, sizeof(block_size), 1, file);

    //fseek(output, 5*sizeof(long) + max_inodes_count * sizeof(struct dinode), SEEK_SET);

    for (i=0; i<max_inodes_count; i++)
    {
        struct dinode n;
        n.di_gen = i;
        fwrite(&n, sizeof(n), 1, file);
    }

    for (i=max_inodes_count-1; i>=0; i--)
    {
        long n = i;
        fwrite(&n, sizeof(n), 1, file);
    }

    for (i=max_blocks_count-1; i>=0; i--)
    {
        long n = i;
        fwrite(&n, sizeof(n), 1, file);
    }

    /*for (i=0; i<max_blocks_count; i++)
    {
        long j;
        for (j=0; j<block_size; j++)
        {
            char n = j % 128;
            if(j==0)
                n = i % 128;
            fwrite(&n, sizeof(n), 1, file);
        }
    }*/
    fclose(file);
    return 0;
}

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

long CreateRoot()
{
    struct dinode n;
    long index = GetNewInodeIndex();
    if(index < 0)
    {
        /*char message[50];
        sprintf(message, "Failed to create root");
        WriteToLog(message);*/
        return -1;
    }
    n.di_mode = S_IFDIR | 0777;
    n.di_nlink = 2;
    n.di_uid = 0;      /* owner's user id         */
    n.di_gid = 0;      /* owner's group id        */
    n.di_size = 0;     /* number of bytes in file */
    /*n.di_addr*/; /* disk block addresses    */
    n.di_gen = 0;      /* file generation number  */
    n.di_atime = time(NULL);    /* time last accessed      */
    n.di_mtime = time(NULL);    /* time last modified      */
    n.di_ctime = time(NULL);    /* time created            */
    struct dirent items[2];
    long i;
    for(i = 0; i<sizeof(n.di_addr)/sizeof(long); i++)
    {
        n.di_addr[i] = -1;
    }
    strcpy(items[0].d_name,".."); //parent
    items[0].d_ino=index;

    strcpy(items[1].d_name,"."); //this
    items[1].d_ino=index;

    for(i = 0; i<2; i++)
    {
        items[i].d_off = n.di_size + i * sizeof(struct dirent);       /* offset to this dirent */
        items[i].d_reclen = sizeof(struct dirent);    /* length of this record */
        items[i].d_type = -i;      /* type of file; not supported by all file system types */
    }
    if(WriteFile(&n, items, 0, sizeof(items)) < 0)
    {
        /*char message[50];
        sprintf(message, "Failed to create root");
        WriteToLog(message);*/
        return -1;
    }
    if(WriteInode(index, n) < 0)
    {
        /*char message[50];
        sprintf(message, "Failed to create root");
        WriteToLog(message);*/
        return -1;
    }
    printf("Root was created\n");
}

void PrintDir(const char *path)
{
    long i;
    long index = GetInodeIndexByPath(path);
    struct dinode n = ReadInode(index);
    if(n.di_size < 0)
        return;
    int count = n.di_size / sizeof(struct dirent);
    struct dirent items[count];
    printf("---------------------------------------------------\n");
    printf("[%s: size = %ld, index = %ld, links = %d]\n", path, n.di_size, index, n.di_nlink);

    ReadFile(&n, (void *)items, 0*sizeof(struct dirent), (count)*sizeof(struct dirent));
    for(i = 0; i<count; i++)
    {
        struct dinode item = ReadInode((items)[i].d_ino);
        printf(" %s: inode = %ld, offs = %ld, mode = %o, size = %ld\n", (items)[i].d_name, (items)[i].d_ino, (items)[i].d_off, item.di_mode, item.di_size);
    }
    printf("---------------------------------------------------\n");
}

int main(int argc, char *argv[])
{
    fsfilename = FILE_PATH;
    Create();
    Load(fsfilename);
    CreateRoot();
    /*CreateDirectory("/test", S_IFDIR | 0777);
    CreateDirectory("/test/test1", S_IFDIR | 0777);
    CreateFile("/test/testf", S_IFREG | 0777);
    //CreateFile("/test/test1/testf", S_IFREG | 0777);
    CreateDirectory("/test/test1/test2", S_IFDIR | 0777);
    //CreateFile("/test/test1/test2/testf", S_IFREG | 0777);
    /*CreateDirectory("/test3", S_IFDIR | 0777);
    /*CreateDirectory("/test4", S_IFDIR | 0777);
    CreateDirectory("/test5", S_IFDIR | 0777);*/
    /*struct dinode n = ReadInode(0);
    struct dirent items[n.di_size/sizeof(dirent)];
    ReadFile(n, &items, sizeof(struct dirent), n.di_size - sizeof(dirent));*/
    /*long k;
    for(k = 6; k < 10; k++)
    {
        char path[50];
        sprintf(path, "/test%ld", k);
        CreateDirectory(path, S_IFDIR | 0777);
    }*/

    Load(fsfilename);
    PrintDir("/");
    PrintFileSystemInfo();
}
