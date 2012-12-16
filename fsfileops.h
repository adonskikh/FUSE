#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "inode.h"


#define MAX_INODES_COUNT 10;
#define FREE_INODES_COUNT 10;
#define MAX_BLOCKS_COUNT 1000;
#define FREE_BLOCKS_COUNT 1000;
#define BLOCK_SIZE 1024;

char *fsfilename;
long max_inodes_count;
long free_inodes_count;
long max_blocks_count;
long free_blocks_count;
long block_size;

long param_count = 5;


void WriteToLog(const char *str)
{
    FILE *output;

    if((output=fopen("/media/Study/Z/3 курс/Операционные системы/FUSE/log","a+"))==0)
    {
        puts ("Can't open output file.");
        exit(-1);
    }
    fprintf(output, "%s\n", str);
    fclose(output);
}

long ReadFreeInodesCount()
{
    FILE *file;
    long result;
    if((file=fopen(fsfilename,"rb"))==0)
    {
        printf("Can't open input file.\n");
        return -1;
    }

    fseek(file, 0, SEEK_SET);
    fread(&result, sizeof(result), 1, file);
    fclose(file);
    char buf[50];
    sprintf(buf, "Read free inodes count: %ld", result);
    WriteToLog(buf);
    return result;
}

long WriteFreeInodesCount(long value)
{
    FILE *file;
    if((file=fopen(fsfilename,"r+b"))==0)
    {
        printf("Can't open input file.\n");
        return -1;
    }

    fseek(file, 0, SEEK_SET);
    fwrite(&value, sizeof(value), 1, file);
    fclose(file);
    char buf[50];
    sprintf(buf, "Wrote free inodes count: %ld", value);
    WriteToLog(buf);
    return 0;
}

long ReadMaxInodesCount()
{
    FILE *file;
    long result;
    if((file=fopen(fsfilename,"rb"))==0)
    {
        printf("Can't open input file.\n");
        return -1;
    }

    fseek(file, sizeof(long), SEEK_SET);
    fread(&result, sizeof(result), 1, file);
    fclose(file);
    char buf[50];
    sprintf(buf, "Read max inodes count: %ld", result);
    WriteToLog(buf);
    return result;
}

long WriteMaxInodesCount(long value)
{
    FILE *file;
    if((file=fopen(fsfilename,"r+b"))==0)
    {
        printf("Can't open input file.\n");
        return -1;
    }

    fseek(file, sizeof(long), SEEK_SET);
    fwrite(&value, sizeof(value), 1, file);
    fclose(file);
    char buf[50];
    sprintf(buf, "Wrote max inodes count: %ld", value);
    WriteToLog(buf);
    return 0;
}

long ReadFreeBlocksCount()
{
    FILE *file;
    long result;
    if((file=fopen(fsfilename,"rb"))==0)
    {
        printf("Can't open input file.\n");
        return -1;
    }

    fseek(file, 2*sizeof(long), SEEK_SET);
    fread(&result, sizeof(result), 1, file);
    fclose(file);
    char buf[50];
    sprintf(buf, "Read free blocks count: %ld", result);
    WriteToLog(buf);
    return result;
}

long WriteFreeBlocksCount(long value)
{
    FILE *file;
    if((file=fopen(fsfilename,"r+b"))==0)
    {
        printf("Can't open input file.\n");
        return -1;
    }

    fseek(file, 2*sizeof(long), SEEK_SET);
    fwrite(&value, sizeof(value), 1, file);
    fclose(file);
    char buf[50];
    sprintf(buf, "Wrote free blocks count: %ld", value);
    WriteToLog(buf);
    return 0;
}

long ReadMaxBlocksCount()
{
    FILE *file;
    long result;
    if((file=fopen(fsfilename,"rb"))==0)
    {
        printf("Can't open input file.\n");
        return -1;
    }

    fseek(file, 3*sizeof(long), SEEK_SET);
    fread(&result, sizeof(result), 1, file);
    fclose(file);
    char buf[50];
    sprintf(buf, "Read max blocks count: %ld", result);
    WriteToLog(buf);
    return result;
}

long WriteMaxBlocksCount(long value)
{
    FILE *file;
    if((file=fopen(fsfilename,"r+b"))==0)
    {
        printf("Can't open input file.\n");
        return -1;
    }

    fseek(file, 3*sizeof(long), SEEK_SET);
    fwrite(&value, sizeof(value), 1, file);
    fclose(file);
    char buf[50];
    sprintf(buf, "Wrote max blocks count: %ld", value);
    WriteToLog(buf);
    return 0;
}

long ReadBlockSize()
{
    FILE *file;
    long result;
    if((file=fopen(fsfilename,"rb"))==0)
    {
        printf("Can't open input file.\n");
        return -1;
    }

    fseek(file, 4*sizeof(long), SEEK_SET);
    fread(&result, sizeof(result), 1, file);
    fclose(file);
    char buf[50];
    sprintf(buf, "Read block size: %ld", result);
    WriteToLog(buf);
    return result;
}

long WriteBlockSize(long value)
{
    FILE *file;
    if((file=fopen(fsfilename,"r+b"))==0)
    {
        printf("Can't open input file.\n");
        return -1;
    }

    fseek(file, 4*sizeof(long), SEEK_SET);
    fwrite(&value, sizeof(value), 1, file);
    fclose(file);
    char buf[50];
    sprintf(buf, "Wrote block size: %ld", value);
    WriteToLog(buf);
    return 0;
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
        return -1;
    }
    long offset = param_count * sizeof(long) + max_inodes_count * sizeof(struct dinode) + (count - 1) * sizeof(long);
    //printf("offset = %ld\n", offset);
    //printf("offset = %ld\n", INODES_AREA_SIZE);
    fseek(file, offset, SEEK_SET);
    fread(&result, sizeof(result), 1, file);
    fclose(file);
    WriteFreeInodesCount(--count);
    char buf[50];
    sprintf(buf, "Got new inode index: %ld", result);
    WriteToLog(buf);
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
        return -1;
    }

    long offset = param_count * sizeof(long) + max_inodes_count * sizeof(struct dinode) + count * sizeof(long);
    fseek(file, offset, SEEK_SET);
    fwrite(&index, sizeof(index), 1, file);
    fclose(file);
    char buf[50];
    sprintf(buf, "Inode was freed: %ld", index);
    WriteToLog(buf);
    WriteFreeInodesCount(++count);
    return 0;
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
        return -1;
    }
    long offset = param_count * sizeof(long) + max_inodes_count * sizeof(struct dinode) + max_inodes_count * sizeof(long) + (count - 1) * sizeof(long);
    //printf("offset = %ld\n", offset);
    //printf("offset = %ld\n", INODES_AREA_SIZE);
    fseek(file, offset, SEEK_SET);
    fread(&result, sizeof(result), 1, file);
    fclose(file);
    WriteFreeBlocksCount(--count);
    char buf[50];
    sprintf(buf, "Got new block index: %ld", result);
    WriteToLog(buf);
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
        return -1;
    }

    long offset = param_count * sizeof(long) + max_inodes_count * sizeof(struct dinode) + max_inodes_count * sizeof(long) + count * sizeof(long);
    fseek(file, offset, SEEK_SET);
    fwrite(&index, sizeof(index), 1, file);
    fclose(file);
    WriteFreeBlocksCount(++count);
    char buf[50];
    sprintf(buf, "Block was freed: %ld", index);
    WriteToLog(buf);
    return 0;
}

struct dinode ReadInode(long index)
{
    FILE *file;
    struct dinode result;
    result.di_size = -1;
    result.di_gen = -1;
    if(index < 0 || index >= max_inodes_count)
        return result;
    if((file=fopen(fsfilename,"rb"))==0)
    {
        printf("Can't open input file.\n");
        return result;
    }
    long offset = param_count * sizeof(long) + index * sizeof(result);
    fseek(file, offset, SEEK_SET);
    fread(&result, sizeof(result), 1, file);
    fclose(file);
    char buf[50];
    sprintf(buf, "Read inode: %ld", index);
    WriteToLog(buf);
    return result;
}

long WriteInode(long index, struct dinode value)
{
    if(index < 0 || index >= max_inodes_count)
        return -1;

    FILE *file;
    if((file=fopen(fsfilename,"r+b"))==0)
    {
        printf("Can't open input file.\n");
        return -1;
    }
    long offset = param_count * sizeof(long) + index * sizeof(value);
    fseek(file, offset, SEEK_SET);
    fwrite(&value, sizeof(value), 1, file);
    fclose(file);
    char buf[50];
    sprintf(buf, "Wrote inode: %ld", index);
    WriteToLog(buf);
    return 0;
}

long ReadBlock(long index, void *buf, long offset, long size)
{
    //printf("index = %ld, max_blocks_count = %ld, offset + size = %ld < block_size = %ld\n", index, max_blocks_count, offset + size, block_size);
    FILE *file;
    if(index < 0 || index >= max_blocks_count || offset + size > block_size)
        return -1;

    if((file=fopen(fsfilename,"rb"))==0)
    {
        printf("Can't open input file.\n");
        return -1;
    }
    long offs = (param_count + max_blocks_count + max_inodes_count) * sizeof(long) + max_inodes_count * sizeof(struct dinode) + index * block_size + offset;

    fseek(file, offs, SEEK_SET);
    fread(buf, size, 1, file);

    fclose(file);
    char buff[50];
    sprintf(buff, "Read %ld bytes from block %ld; offset = %ld", size, index, offset);
    WriteToLog(buff);
    return 0;
}

long WriteBlock(long index, void *buf, long offset, long size)
{
    //printf("index = %ld, max_blocks_count = %ld, offset + size = %ld < block_size = %ld\n", index, max_blocks_count, offset + size, block_size);
    if(buf == NULL || index < 0 || index >= max_blocks_count || offset + size > block_size)
        return -1;

    FILE *file;
    if((file=fopen(fsfilename,"r+b"))==0)
    {
        printf("Can't open input file.\n");
        return -1;
    }
    long offs = (param_count + max_blocks_count + max_inodes_count) * sizeof(long) + max_inodes_count * sizeof(struct dinode) + index * block_size + offset;
    fseek(file, offs, SEEK_SET);
    fwrite(buf, size, 1, file);
    fclose(file);
    char buff[50];
    sprintf(buff, "Wrote %ld bytes to block %ld; offset = %ld", size, index, offset);
    WriteToLog(buff);
    return 0;
}

long Load(char *filename)
{
    //long i;
    FILE *file;

    fsfilename = filename;
    if((file=fopen(filename,"rb"))==0)
    {
        printf("Can't open input file.\n");
        return -1;
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
     return 0;
}

long WriteFile(struct dinode *inode, void *buf, long size)
{
    long bytes_wrote = 0;
    long addr_in_block = block_size/sizeof(long);
    char message[50];
    sprintf(message, "Started writing %ld bytes to a file", size);
    WriteToLog(message);

    printf("size = %ld, max_size = %ld\n", size, addr_in_block*addr_in_block*addr_in_block*block_size);
    //Очень большой файл, тройные непрямые адреса
    if(size > addr_in_block*addr_in_block*block_size && size <= addr_in_block*addr_in_block*addr_in_block*block_size)
    {
        printf("1\n");
        if(inode->di_addr[12] < 0)
        {
            inode->di_addr[12] = GetNewBlockIndex(); //индекс блока с двойными непрямыми адресами (адресами блоков с одинарными непрямыми адресами)
            if(inode->di_addr[12] < 0)
                return -1;
        }
        printf("1.1\n");
        long index0 = inode->di_addr[12];
        long count0 = 0;
        while(count0 < addr_in_block && bytes_wrote < size)
        {
            long count1 = 0;
            long index1 = GetNewBlockIndex(); //индекс блока с одинарными непрямыми адресами (адресами блоков с прямыми адресами)
            if(index1 < 0)
                return -1;
            if(WriteBlock(index0, &index1, count0 * sizeof(long), sizeof(long)) < 0)
                    return -1;

            while(count1 < addr_in_block && bytes_wrote < size)
            {
                long count2 = 0;
                long index2 = GetNewBlockIndex(); //индекс блока с прямыми адресами (адресами блоков с данными)
                if(index2 < 0)
                    return -1;
                if(WriteBlock(index1, &index2, count1 * sizeof(long), sizeof(long)) < 0)
                        return -1;

                while(count2 < addr_in_block && bytes_wrote < size)
                {
                    //long bytes_in_block = 0;
                    long index3 = GetNewBlockIndex();  //индекс блока с данными
                    if(index3 < 0)
                        return -1;
                    if(WriteBlock(index2, &index3, count2 * sizeof(long), sizeof(long)) < 0)
                            return -1;

                    long n = (size - bytes_wrote) < block_size ? (size - bytes_wrote) : block_size;
                    if(WriteBlock(index3, buf + bytes_wrote, 0, n) < 0)
                        return -1;
                    bytes_wrote+=n;
                    count2++;
                }
                count1++;
            }
            count0++;
        }
        sprintf(message, "Wrote %ld bytes to the huge file", size);
        WriteToLog(message);
        inode->di_size = size;
        return 0;
    }
    //Большой файл, двойные непрямые адреса
    else if(size > addr_in_block*block_size && size <= addr_in_block*addr_in_block*block_size)
    {
        printf("2\n");
        if(inode->di_addr[11] < 0)
        {
            inode->di_addr[11] = GetNewBlockIndex(); //индекс блока с двойными непрямыми адресами (адресами блоков с одинарными непрямыми адресами)
            if(inode->di_addr[11] < 0)
                return -1;
        }
        printf("index0! = %ld\n", inode->di_addr[11]);
        printf("2.1\n");
        long index0 = inode->di_addr[11];
        printf("index0 = %ld\n", index0);
        long count0 = 0;
        while(count0 < addr_in_block && bytes_wrote < size)
        {
            long count1 = 0;
            long index1 = GetNewBlockIndex(); //индекс блока с прямыми адресами (адресами блоков с данными)
            if(index1 < 0)
                return -1;
            printf("2.2\n");
            if(WriteBlock(index0, &index1, count0 * sizeof(long), sizeof(long)) < 0)
                    return -1;
            printf("2.3\n");
            while(count1 < addr_in_block && bytes_wrote < size)
            {
                long index2 = GetNewBlockIndex(); //индекс блока с данными
                if(index2 < 0)
                    return -1;
                if(WriteBlock(index1, &index2, count1 * sizeof(long), sizeof(long)) < 0)
                        return -1;

                long n = (size - bytes_wrote) < block_size ? (size - bytes_wrote) : block_size;
                if(WriteBlock(index2, buf + bytes_wrote, 0, n) < 0)
                    return -1;
                bytes_wrote+=n;
                count1++;
            }
            count0++;
        }
        sprintf(message, "Wrote %ld bytes to the big file", size);
        WriteToLog(message);
        inode->di_size = size;
        return 0;
    }
    //Средний файл, одинарные непрямые адреса
    else if(size > 10*block_size && size <= addr_in_block*block_size)
    {
        if(inode->di_addr[10] < 0)
        {
            inode->di_addr[10] = GetNewBlockIndex(); //индекс блока с двойными непрямыми адресами (адресами блоков с одинарными непрямыми адресами)
            if(inode->di_addr[10] < 0)
                return -1;
        }
        long index0 = inode->di_addr[10];
        long count0 = 0;
        while(count0 < addr_in_block && bytes_wrote < size)
        {
            long index1 = GetNewBlockIndex();//индекс блока с данными
            if(index1 < 0)
                return -1;
            if(WriteBlock(index0, &index1, count0 * sizeof(long), sizeof(long)) < 0)
                return -1;

            long n = (size - bytes_wrote) < block_size ? (size - bytes_wrote) : block_size;
            if(WriteBlock(index1, buf + bytes_wrote, 0, n) < 0)
                return -1;
            bytes_wrote+=n;
            count0++;
        }
        sprintf(message, "Wrote %ld bytes to the average file", size);
        WriteToLog(message);
        inode->di_size = size;
        return 0;
    }
    //Маленький файл, прямые адреса
    else if(size > 0 && size <= 10*block_size)
    {
        printf("4\n");
        int block_number = 0;
        while(bytes_wrote < size)
        {
            if(inode->di_addr[block_number] < 0)
            {
                inode->di_addr[block_number] = GetNewBlockIndex();
                if(inode->di_addr[block_number] < 0)
                    return -1;
            }
            long n = (size - bytes_wrote) < block_size ? (size - bytes_wrote) : block_size;
            if(WriteBlock(inode->di_addr[block_number], buf + bytes_wrote, 0, n) < 0)
                return -1;
            bytes_wrote+=n;
            block_number++;
        }
        /*for(block_number; block_number < 13; block_number++)
        {
            FreeBlockIndex(inode->di_addr[block_number]);
            inode->di_addr[block_number] = -1;
        }*/
        sprintf(message, "Wrote %ld bytes to the small file", size);
        WriteToLog(message);
        inode->di_size = size;
        return 0;
    }
    return -1;
}

long ReadFile(struct dinode *inode, void **buf)
{
    long size = inode->di_size;
    *buf = malloc(size);
    long bytes_read = 0;
    long addr_in_block = block_size/sizeof(long);

    char message[50];
    sprintf(message, "Started reading %ld bytes from the file", size);
    WriteToLog(message);

    //Очень большой файл, тройные непрямые адреса
    if(size > addr_in_block*addr_in_block*block_size && size <= addr_in_block*addr_in_block*addr_in_block*block_size)
    {
        printf("Reading the huge file\n");
        if(inode->di_addr[12] < 0)
            return -1;
        long index0 = inode->di_addr[12]; //индекс блока с двойными непрямыми адресами (адресами блоков с одинарными непрямыми адресами)
        long count0 = 0;
        while(count0 < addr_in_block && bytes_read < size)
        {
            long count1 = 0;
            long index1;
            if(ReadBlock(index0, &index1, count0 * sizeof(long), sizeof(long)) < 0) //индекс блока с одинарными непрямыми адресами (адресами блоков с прямыми адресами)
                return -1;

            while(count1 < addr_in_block && bytes_read < size)
            {
                long count2 = 0;
                long index2;
                if(ReadBlock(index1, &index2, count1 * sizeof(long), sizeof(long)) < 0) //индекс блока с прямыми адресами (адресами блоков с данными)
                    return -1;

                while(count2 < addr_in_block && bytes_read < size)
                {
                    //long bytes_in_block = 0;
                    long index3;
                    if(ReadBlock(index2, &index3, count2 * sizeof(long), sizeof(long)) < 0)  //индекс блока с данными
                        return -1;

                    long n = (size - bytes_read) < block_size ? (size - bytes_read) : block_size;
                    if(ReadBlock(index3, (*buf) + bytes_read, 0, n) < 0)
                        return -1;
                    bytes_read+=n;
                    count2++;
                }
                count1++;
            }
            count0++;
        }
        sprintf(message, "Read %ld bytes from the huge file", size);
        WriteToLog(message);
        return 0;
    }
    //Большой файл, двойные непрямые адреса
    else if(size > addr_in_block*block_size && size <= addr_in_block*addr_in_block*block_size)
    {
        printf("Reading the big file\n");
        if(inode->di_addr[11] < 0)
            return -1;
        long index0 = inode->di_addr[11]; //индекс блока с одинарными непрямыми адресами (адресами блоков с прямыми адресами)
        long count0 = 0;
        printf("2.1\n");
        while(count0 < addr_in_block && bytes_read < size)
        {
            long count1 = 0;
            long index1;
            if(ReadBlock(index0, &index1, count0 * sizeof(long), sizeof(long)) < 0) //индекс блока с прямыми адресами (адресами блоков с данными)
                return -1;

            printf("2.2\n");
            while(count1 < addr_in_block && bytes_read < size)
            {
                long index2;
                if(ReadBlock(index1, &index2, count1 * sizeof(long), sizeof(long)) < 0) //индекс блока с данными
                    return -1;

                long n = (size - bytes_read) < block_size ? (size - bytes_read) : block_size;
                if(ReadBlock(index2, (*buf) + bytes_read, 0, n) < 0)
                    return -1;
                bytes_read+=n;
                count1++;
            }
            count0++;
        }
        sprintf(message, "Read %ld bytes from the big file", size);
        WriteToLog(message);
        return 0;
    }
    //Средний файл, одинарные непрямые адреса
    else if(size > 10*block_size && size <= addr_in_block*block_size)
    {
        printf("Reading the average file\n");
        if(inode->di_addr[10] < 0)
            return -1;
        long index0 = inode->di_addr[10]; //индекс блока с прямыми адресами (адресами блоков с данными)
        long count0 = 0;
        while(count0 < addr_in_block && bytes_read < size)
        {
            long index1;
            if(ReadBlock(index0, &index1, count0 * sizeof(long), sizeof(long)) < 0) //индекс блока с данными
                return -1;

            long n = (size - bytes_read) < block_size ? (size - bytes_read) : block_size;
            if(ReadBlock(index1, (*buf) + bytes_read, 0, n) < 0)
                return -1;
            bytes_read+=n;
            count0++;
        }
        sprintf(message, "Read %ld bytes from the average file", size);
        WriteToLog(message);
        return 0;
    }
    //Маленький файл, прямые адреса
    else if(size > 0 && size <= 10*block_size)
    {
        printf("Reading the small file\n");
        int block_number = 0;
        while(bytes_read < size)
        {
            if(inode->di_addr[block_number] < 0)
                return -1;
            long n = (size - bytes_read) < block_size ? (size - bytes_read) : block_size;
            if(ReadBlock(inode->di_addr[block_number], (*buf) + bytes_read, 0, n) < 0)
                return -1;
            bytes_read+=n;
            block_number++;
        }
        /*for(block_number; block_number < 13; block_number++)
        {
            FreeBlockIndex(inode->di_addr[block_number]);
            inode->di_addr[block_number] = -1;
        }*/
        sprintf(message, "Read %ld bytes from the small file", size);
        WriteToLog(message);
        return 0;
    }
    return -1;
}
