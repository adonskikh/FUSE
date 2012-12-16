#include "fsfileops.h"
#include <string.h>
//#include <dirent.h>
#include <fcntl.h>

long Create()
{
    FILE *file;
    long i;

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

    for (i=0; i<max_blocks_count; i++)
    {
        long j;
        for (j=0; j<block_size; j++)
        {
            char n = j % 128;
            if(j==0)
                n = i % 128;
            fwrite(&n, sizeof(n), 1, file);
        }
    }
    fclose(file);
    return 0;
}

void PrintAll()
{
    printf("Print all:\n");
    printf("free_inodes_count = %ld\n", free_inodes_count);
    printf("max_inodes_count = %ld\n", max_inodes_count);
    printf("free_blocks_count = %ld\n", free_blocks_count);
    printf("max_blocks_count = %ld\n", max_blocks_count);
    printf("block_size = %ld\n", block_size);
    printf("\n\n");
}
/*void WriteToLog(char *str)
{
    FILE *output;

    if((output=fopen("log","a+"))==0)
    {
        puts ("Can't open output file.");
        exit(-1);
    }
    fprintf(output, "%s\n", str);
    fclose(output);
}*/
long CreateRoot()
{
    struct dinode n;
    long index = GetNewInodeIndex();
    //printf("index = %ld\n", index);
    if(index < 0)
    {
        char message[50];
        sprintf(message, "Failed to create root");
        WriteToLog(message);
        return -1;
    }
    n.di_mode = S_IFDIR | 0777;
    n.di_nlink = 2;
    n.di_uid = 0;      /* owner's user id         */
    n.di_gid = 0;      /* owner's group id        */
    //n.di_size = 2 * sizeof(struct dirent);     /* number of bytes in file */
    /*n.di_addr*/; /* disk block addresses    */
    n.di_gen = 0;      /* file generation number  */
    n.di_atime = time(NULL);    /* time last accessed      */
    n.di_mtime = time(NULL);    /* time last modified      */
    n.di_ctime = time(NULL);    /* time created            */
    struct dirent items[10];
    long i;
    for(i = 0; i<sizeof(n.di_addr)/sizeof(long); i++)
    {
        n.di_addr[i] = -1;
    }
    strcpy(items[0].d_name,".."); //parent
    items[0].d_ino=index;

    strcpy(items[1].d_name,"."); //this
    items[1].d_ino=index;

    for(i = 0; i<10; i++)
    {
        if(i > 1)
        {
            sprintf(items[i].d_name, "inode's %ld file number %ld", index, i); /* filename */
            items[i].d_ino = i+1; /* inode number */
        }
        items[i].d_off = sizeof(struct dirent);       /* offset to the next dirent */
        items[i].d_reclen = sizeof(items);    /* length of this record */
        items[i].d_type = -i;      /* type of file; not supported by all file system types */
    }
    if(WriteFile(&n, items, sizeof(items)) < 0)
    {
        char message[50];
        sprintf(message, "Failed to create root");
        WriteToLog(message);
        return -1;
    }
    if(WriteInode(index, n) < 0)
    {
        char message[50];
        sprintf(message, "Failed to create root");
        WriteToLog(message);
        return -1;
    }
}

long CreateDemoFile()
{
    long index = GetNewInodeIndex();
    //printf("index = %ld\n", index);
    if(index < 0)
    {
        char message[50];
        sprintf(message, "Failed to create demo file");
        WriteToLog(message);
        return -1;
    }
    struct dinode n;
    n.di_mode = S_IFDIR | 0777;
    n.di_nlink = 2;
    n.di_uid = 0;      /* owner's user id         */
    n.di_gid = 0;      /* owner's group id        */
    //n.di_size = 2 * sizeof(struct dirent);     /* number of bytes in file */
    /*n.di_addr*/; /* disk block addresses    */
    n.di_gen = 0;      /* file generation number  */
    n.di_atime = time(NULL);    /* time last accessed      */
    n.di_mtime = time(NULL);    /* time last modified      */
    n.di_ctime = time(NULL);    /* time created            */
    struct dirent items[10];
    long i;
    for(i = 0; i<sizeof(n.di_addr)/sizeof(long); i++)
    {
        n.di_addr[i] = -1;
    }
    for(i = 0; i<10; i++)
    {
        items[i].d_ino = i+1;       /* inode number */
        items[i].d_off = sizeof(struct dirent);       /* offset to the next dirent */
        items[i].d_reclen = sizeof(items);    /* length of this record */
        items[i].d_type = -i;      /* type of file; not supported by all file system types */
        sprintf(items[i].d_name, "inode's %ld file number %ld", index, i); /* filename */
    }
    if(WriteFile(&n, items, sizeof(items)) < 0)
    {
        char message[50];
        sprintf(message, "Failed to create demo file");
        WriteToLog(message);
        return;
    }
    printf("n.file = %ld\n", n.di_addr[12]);
    printf("index = %ld\n", index);
    if(WriteInode(index, n) < 0)
    {
        char message[50];
        sprintf(message, "Failed to create demo file");
        WriteToLog(message);
        return -1;
    }
}


int main(int argc, char *argv[])
{
    fsfilename = "/media/Study/Z/3 курс/Операционные системы/FUSE/file";
    Create();
    Load("/media/Study/Z/3 курс/Операционные системы/FUSE/file");
    CreateRoot();
    Load("/media/Study/Z/3 курс/Операционные системы/FUSE/file");
    PrintAll();

    /*printf("new inode index = %ld\n", GetNewInodeIndex());
    printf("new inode index = %ld\n", GetNewInodeIndex());
    printf("new inode index = %ld\n", GetNewInodeIndex());
    printf("new inode index = %ld\n", GetNewInodeIndex());
    printf("new inode index = %ld\n", GetNewInodeIndex());
    FreeInodeIndex(3);
    printf("new inode index = %ld\n", GetNewInodeIndex());
    printf("new inode index = %ld\n", GetNewInodeIndex());


    printf("new block address = %ld\n", GetNewBlockIndex());
    printf("new block address = %ld\n", GetNewBlockIndex());
    printf("new block address = %ld\n", GetNewBlockIndex());
    printf("new block address = %ld\n", GetNewBlockIndex());
    printf("new block address = %ld\n", GetNewBlockIndex());
    printf("new block address = %ld\n", GetNewBlockIndex());
    printf("new block address = %ld\n", GetNewBlockIndex());
    printf("new block address = %ld\n", GetNewBlockIndex());
    printf("new block address = %ld\n", GetNewBlockIndex());
    printf("new block address = %ld\n", GetNewBlockIndex());

    printf("new block address = %ld\n", GetNewBlockIndex());
    FreeBlockIndex(5);

    printf("new block address = %ld\n", GetNewBlockIndex());
    printf("new block address = %ld\n", GetNewBlockIndex());
    printf("new block address = %ld\n", GetNewBlockIndex());
    printf("new block address = %ld\n", GetNewBlockIndex());
    printf("new block address = %ld\n", GetNewBlockIndex());
    printf("new block address = %ld\n", GetNewBlockIndex());
    printf("new block address = %ld\n", GetNewBlockIndex());
    printf("new block address = %ld\n", GetNewBlockIndex());
    printf("new block address = %ld\n", GetNewBlockIndex());
    printf("new block address = %ld\n", GetNewBlockIndex());
    printf("new block address = %ld\n", GetNewBlockIndex());
    printf("new block address = %ld\n", GetNewBlockIndex());
    printf("new block address = %ld\n", GetNewBlockIndex());
    printf("new block address = %ld\n", GetNewBlockIndex());
    printf("new block address = %ld\n", GetNewBlockIndex());
    printf("inode = %ld\n", ReadInode(-1).di_gen);
    printf("inode = %ld\n", ReadInode(0).di_gen);
    printf("inode = %ld\n", ReadInode(1).di_gen);
    struct dinode n = ReadInode(1);
    n.di_gen = 999;
    WriteInode(1, n);
    printf("inode = %ld\n", ReadInode(1).di_gen);
    printf("inode = %ld\n", ReadInode(2).di_gen);
    printf("inode = %ld\n", ReadInode(3).di_gen);
    printf("inode = %ld\n", ReadInode(4).di_gen);
    printf("inode = %ld\n", ReadInode(5).di_gen);
    printf("inode = %ld\n", ReadInode(6).di_gen);

    long i;
    char *arr1 = malloc(block_size);
    for(i = 0; i < block_size; i++)
    {
        arr1[i] = 127;
    }
    //WriteToLog((arr1));
    WriteBlock(1, arr1, 0, block_size);
    free(arr1);
    printf("qqqqqqqqqqqqqqqqqqqqq");
    for(i = -1; i <= max_blocks_count; i++)
    {
        char *arr = malloc(block_size);

        long j;
        if(!(ReadBlock(i, arr, 0, block_size) < 0))
        {
            for(j = 0; j < block_size; j++)
                printf("%u ", arr[j]);
            printf("\n");
        }
        else
            printf("NULL\n");
        free(arr);
    }*/
    /*WriteFreeInodesCount(14);
    printf("free_inodes_count = %ld\n", ReadFreeInodesCount());
    WriteMaxInodesCount(15);
    printf("max_inodes_count = %ld\n", ReadMaxInodesCount());
    WriteMaxInodesCount(5);
    printf("max_inodes_count = %ld\n", ReadMaxInodesCount());
    WriteFreeBlocksCount(19);
    printf("free_blocks_count = %ld\n", ReadFreeBlocksCount());
    WriteMaxBlocksCount(20);
    printf("max_blocks_count = %ld\n", ReadMaxBlocksCount());
    WriteBlockSize(8);
    printf("block_size = %ld\n", ReadBlockSize());
    printf("\n\n");
    Load();
    PrintAll();*/
    long k;
    for(k=1;k<10;k++)
    {
        printf("createfile status = %ld\n", CreateDemoFile());
    }
    for(k=0;k<10;k++)
    {
        struct dirent *items;
        //(*items) = malloc(10 * sizeof(struct dirent));
        long i;
        struct dinode n = ReadInode(k);
        printf("n.size = %ld\n", n.di_size);
        printf("n.file = %ld\n", n.di_addr[12]);
        ReadFile(&n, (void **)&items);
        printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        for(i = 0; i<n.di_size/sizeof(struct dirent); i++)
        {
            printf("inode = %ld, name = %s\n", (items)[i].d_ino, (items)[i].d_name);
        }
        free(items);
    }

    //Create();
    Load("/media/Study/Z/3 курс/Операционные системы/FUSE/file");
    PrintAll();
}
