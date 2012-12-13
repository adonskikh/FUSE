#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "inode.h"


#define MAX_INODES_COUNT 5;
#define FREE_INODES_COUNT 4;
#define MAX_BLOCKS_COUNT 10;
#define FREE_BLOCKS_COUNT 10;
#define BLOCK_SIZE 16;

char *fsfilename;
long max_inodes_count;
long free_inodes_count;
long max_blocks_count;
long free_blocks_count;
long block_size;

long param_count = 5;


long ReadFreeInodesCount()
{
    FILE *file;
    long result;
    if((file=fopen(fsfilename,"rb"))==0)
    {
        printf("Can't open input file.\n");
        exit(-1);
    }

    fseek(file, 0, SEEK_SET);
    fread(&result, sizeof(result), 1, file);
    fclose(file);
    return result;
}

void WriteFreeInodesCount(long value)
{
    FILE *file;
    if((file=fopen(fsfilename,"r+b"))==0)
    {
        printf("Can't open input file.\n");
        exit(-1);
    }

    fseek(file, 0, SEEK_SET);
    fwrite(&value, sizeof(value), 1, file);
    fclose(file);
}

long ReadMaxInodesCount()
{
    FILE *file;
    long result;
    if((file=fopen(fsfilename,"rb"))==0)
    {
        printf("Can't open input file.\n");
        exit(-1);
    }

    fseek(file, sizeof(long), SEEK_SET);
    fread(&result, sizeof(result), 1, file);
    fclose(file);
    return result;
}

void WriteMaxInodesCount(long value)
{
    FILE *file;
    if((file=fopen(fsfilename,"r+b"))==0)
    {
        printf("Can't open input file.\n");
        exit(-1);
    }

    fseek(file, sizeof(long), SEEK_SET);
    fwrite(&value, sizeof(value), 1, file);
    fclose(file);
}

long ReadFreeBlocksCount()
{
    FILE *file;
    long result;
    if((file=fopen(fsfilename,"rb"))==0)
    {
        printf("Can't open input file.\n");
        exit(-1);
    }

    fseek(file, 2*sizeof(long), SEEK_SET);
    fread(&result, sizeof(result), 1, file);
    fclose(file);
    return result;
}

void WriteFreeBlocksCount(long value)
{
    FILE *file;
    if((file=fopen(fsfilename,"r+b"))==0)
    {
        printf("Can't open input file.\n");
        exit(-1);
    }

    fseek(file, 2*sizeof(long), SEEK_SET);
    fwrite(&value, sizeof(value), 1, file);
    fclose(file);
}

long ReadMaxBlocksCount()
{
    FILE *file;
    long result;
    if((file=fopen(fsfilename,"rb"))==0)
    {
        printf("Can't open input file.\n");
        exit(-1);
    }

    fseek(file, 3*sizeof(long), SEEK_SET);
    fread(&result, sizeof(result), 1, file);
    fclose(file);
    return result;
}

void WriteMaxBlocksCount(long value)
{
    FILE *file;
    if((file=fopen(fsfilename,"r+b"))==0)
    {
        printf("Can't open input file.\n");
        exit(-1);
    }

    fseek(file, 3*sizeof(long), SEEK_SET);
    fwrite(&value, sizeof(value), 1, file);
    fclose(file);
}

long ReadBlockSize()
{
    FILE *file;
    long result;
    if((file=fopen(fsfilename,"rb"))==0)
    {
        printf("Can't open input file.\n");
        exit(-1);
    }

    fseek(file, 4*sizeof(long), SEEK_SET);
    fread(&result, sizeof(result), 1, file);
    fclose(file);
    return result;
}

void WriteBlockSize(long value)
{
    FILE *file;
    if((file=fopen(fsfilename,"r+b"))==0)
    {
        printf("Can't open input file.\n");
        exit(-1);
    }

    fseek(file, 4*sizeof(long), SEEK_SET);
    fwrite(&value, sizeof(value), 1, file);
    fclose(file);
}

void Load()
{
    //long i;
    FILE *file;

     if((file=fopen(fsfilename,"rb"))==0)
     {
         printf("Can't open input file.\n");
         exit(-1);
     }

     fread(&free_inodes_count, sizeof(free_inodes_count), 1, file);
     fread(&max_inodes_count, sizeof(max_inodes_count), 1, file);

     fread(&free_blocks_count, sizeof(free_blocks_count), 1, file);
     fread(&max_blocks_count, sizeof(max_blocks_count), 1, file);
     fread(&block_size, sizeof(block_size), 1, file);

     //fseek(file, 2*sizeof(long) + max_inodes_count * sizeof(struct dinode) + max_inodes_count * sizeof(long), SEEK_SET);
     /*for(i = 0; i < max_inodes_count; i++)
     {
         struct dinode node;
         fread(&node, sizeof(node), 1, input);
         printf("%ld\n", node.di_gen);
     }
     for(i = 0; i < max_inodes_count; i++)
     {
         long n;
         fread(&n, sizeof(n), 1, input);
         printf("%ld\n", n);
     }*/

     fclose(file);
}

void Create()
{
    FILE *file;
    long i;

    if((file=fopen(fsfilename,"wb"))==0)
    {
        puts ("Can't open output file.");
        exit(-1);
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

    for (i=max_inodes_count-1; i>=0; i--)
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
    fclose(file);
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

long GetNewInodeIndex()
{
    long count = ReadFreeInodesCount();

    if(count == 0)
        return -1;
    FILE *file;
    long result;
    if((file=fopen(fsfilename,"rb"))==0)
    {
        printf("Can't open input file.\n");
        exit(-1);
    }
    long offset = param_count * sizeof(long) + max_inodes_count * sizeof(struct dinode) + (count - 1) * sizeof(long);
    printf("offset = %ld\n", offset);
    //printf("offset = %ld\n", INODES_AREA_SIZE);
    fseek(file, offset, SEEK_SET);
    fread(&result, sizeof(result), 1, file);
    fclose(file);
    WriteFreeInodesCount(--count);
    return result;
}

long FreeInodeIndex(long index)
{
    if(index < 0 || index >= max_inodes_count)
        return -1;
    long count = ReadFreeInodesCount();

    FILE *file;
    if((file=fopen(fsfilename,"r+b"))==0)
    {
        printf("Can't open input file.\n");
        exit(-1);
    }

    long offset = param_count * sizeof(long) + max_inodes_count * sizeof(struct dinode) + count * sizeof(long);
    fseek(file, offset, SEEK_SET);
    fwrite(&index, sizeof(index), 1, file);
    fclose(file);
    WriteFreeInodesCount(++count);
    return index;
}

long GetNewBlockIndex()
{
    long count = ReadFreeBlocksCount();

    if(count == 0)
        return -1;
    FILE *file;
    long result;
    if((file=fopen(fsfilename,"rb"))==0)
    {
        printf("Can't open input file.\n");
        exit(-1);
    }
    long offset = param_count * sizeof(long) + max_inodes_count * sizeof(struct dinode) + max_inodes_count * sizeof(long) + (count - 1) * sizeof(long);
    printf("offset = %ld\n", offset);
    //printf("offset = %ld\n", INODES_AREA_SIZE);
    fseek(file, offset, SEEK_SET);
    fread(&result, sizeof(result), 1, file);
    fclose(file);
    WriteFreeBlocksCount(--count);
    return result;
}

long FreeBlockIndex(long index)
{
    if(index < 0 || index >= max_blocks_count)
        return -1;
    long count = ReadFreeBlocksCount();

    FILE *file;
    if((file=fopen(fsfilename,"r+b"))==0)
    {
        printf("Can't open input file.\n");
        exit(-1);
    }

    long offset = param_count * sizeof(long) + max_inodes_count * sizeof(struct dinode) + max_inodes_count * sizeof(long) + count * sizeof(long);
    fseek(file, offset, SEEK_SET);
    fwrite(&index, sizeof(index), 1, file);
    fclose(file);
    WriteFreeBlocksCount(++count);
    return index;
}

int main(int argc, char *argv[])
{
    fsfilename = argv[1];
    Create();
    Load();
    PrintAll();

    printf("new inode index = %ld\n", GetNewInodeIndex());
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

    //Create();
    Load();
    PrintAll();
}
