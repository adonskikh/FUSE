#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
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

long WriteFile(struct dinode *inode, void *buf, long offset, long size)
{
    long bytes_wrote = 0;
    int block_number = 0;
    long pos = offset < inode->di_size ? offset : inode->di_size;
    long addr_in_block = block_size/sizeof(long);

    if(pos >= 0 && pos < 10 * block_size)
    {
        block_number = pos / block_size;
        while(bytes_wrote < size && block_number < 10)
        {
            if(inode->di_addr[block_number] < 0)
            {
                inode->di_addr[block_number] = GetNewBlockIndex();
                if(inode->di_addr[block_number] < 0)
                    return -1;
            }
            if(pos >= offset || (pos < offset && offset < pos + block_size))
            {
                long offs = pos - block_size * block_number;
                long n = (size - bytes_wrote) < block_size - offs ? (size - bytes_wrote) : block_size - offs;
                printf("Writing: bytes_wrote = %ld, pos = %ld, block = %ld, n = %ld, offs = %ld\n", bytes_wrote, pos, inode->di_addr[block_number], n, offs);
                if(WriteBlock(inode->di_addr[block_number], buf + bytes_wrote, offs, n) < 0)
                    return -1;
                bytes_wrote+=n;
            }
            pos = (block_number + 1) * block_size;
            block_number++;
        }
    }
    if(pos >= 10 * block_size && pos < addr_in_block*block_size)
    {
        if(inode->di_addr[10] < 0)
        {
            inode->di_addr[10] = GetNewBlockIndex();
            if(inode->di_addr[10] < 0)
                return -1;
        }
        long index0 = inode->di_addr[10]; //индекс блока с прямыми адресами (адресами блоков с данными)
        long count0 = 0;
        block_number = pos / block_size;

        while(count0 < addr_in_block && bytes_wrote < size)
        {
            long index1;//индекс блока с данными
            printf("!di_size = %ld > %ld\n", inode->di_size, 10 * block_size + count0 * block_size);
            if(10 * block_size + count0 * block_size < inode->di_size)
            {
                ReadBlock(index0, &index1, count0 * sizeof(long), sizeof(long));
                if(index1 < 0)
                    return -1;
            }
            else
            {
                printf("getting new block\n");
                index1 = GetNewBlockIndex();
                if(index1 < 0)
                    return -1;
                if(WriteBlock(index0, &index1, count0 * sizeof(long), sizeof(long)) < 0)
                    return -1;
            }

            if(pos >= offset || (pos < offset && offset < pos + block_size))
            {
                long offs = pos - block_size * block_number;
                long n = (size - bytes_wrote) < block_size - offs ? (size - bytes_wrote) : block_size - offs;
                printf("!Writing: bytes_wrote = %ld, pos = %ld, block = %ld, n = %ld, offs = %ld\n", bytes_wrote, pos, index1, n, offs);
                if(WriteBlock(index1, buf + bytes_wrote, offs, n) < 0)
                    return -1;
                bytes_wrote+=n;
            }
            pos = (block_number + 1) * block_size;
            count0++;
            block_number++;
        }
    }
    if(pos >= addr_in_block*block_size && pos < addr_in_block*addr_in_block*block_size)
    {
        if(inode->di_addr[11] < 0)
        {
            inode->di_addr[11] = GetNewBlockIndex();
            if(inode->di_addr[11] < 0)
                return -1;
        }
        long index0 = inode->di_addr[11]; //индекс блока с одинарными непрямыми адресами (адресами блоков с прямыми адресами)
        long count0 = 0;
        block_number = pos / block_size;

        while(count0 < addr_in_block && bytes_wrote < size)
        {
            long index1; //индекс блока с прямыми адресами (адресами блоков с данными)
            printf("!di_size = %ld > %ld\n", inode->di_size, 10 * block_size + count0 * block_size);
            if(10 * block_size + addr_in_block * block_size + count0 * block_size * addr_in_block < inode->di_size)
            {
                ReadBlock(index0, &index1, count0 * sizeof(long), sizeof(long));
                if(index1 < 0)
                    return -1;
            }
            else
            {
                printf("getting new block\n");
                index1 = GetNewBlockIndex();
                if(index1 < 0)
                    return -1;
                if(WriteBlock(index0, &index1, count0 * sizeof(long), sizeof(long)) < 0)
                    return -1;
            }
            long count1 = 0;

            printf("!index1 = %ld \n", index1);


            while(count1 < addr_in_block && bytes_wrote < size)
            {
                long index2; //индекс блока с данными
                printf("!!di_size = %ld > %ld\n", inode->di_size, 10 * block_size + count0 * block_size);
                if(10 * block_size + addr_in_block * block_size + count0 * block_size * addr_in_block + count1 * block_size < inode->di_size)
                {
                    ReadBlock(index1, &index2, count1 * sizeof(long), sizeof(long));
                    if(index1 < 0)
                        return -1;
                }
                else
                {
                    printf("!getting new block\n");
                    index2 = GetNewBlockIndex();
                    if(index2 < 0)
                        return -1;
                    if(WriteBlock(index1, &index2, count1 * sizeof(long), sizeof(long)) < 0)
                        return -1;
                }

                if(pos >= offset || (pos < offset && offset < pos + block_size))
                {
                    long offs = pos - block_size * block_number;
                    long n = (size - bytes_wrote) < block_size - offs ? (size - bytes_wrote) : block_size - offs;
                    printf("!!Writing: bytes_wrote = %ld, pos = %ld, block = %ld, n = %ld, offs = %ld\n", bytes_wrote, pos, index2, n, offs);
                    if(WriteBlock(index2, buf + bytes_wrote, offs, n) < 0)
                        return -1;
                    bytes_wrote+=n;
                }
                pos = (block_number + 1) * block_size;
                count1++;
                block_number++;
            }
            count0++;
        }
    }
    if(pos >= addr_in_block*addr_in_block*block_size && pos < addr_in_block*addr_in_block*addr_in_block*block_size)
    {
        if(inode->di_addr[12] < 0)
        {
            inode->di_addr[12] = GetNewBlockIndex();
            if(inode->di_addr[12] < 0)
                return -1;
        }
        long index0 = inode->di_addr[12]; //индекс блока с двойными непрямыми адресами (адресами блоков с одинарными непрямыми адресами)
        long count0 = 0;
        block_number = pos / block_size;

        while(count0 < addr_in_block && bytes_wrote < size)
        {
            long index1; //индекс блока с одинарными непрямыми адресами (адресами блоков с прямыми адресами)
            printf("!!!di_size = %ld > %ld\n", inode->di_size, 10 * block_size + count0 * block_size);
            if(10 * block_size + addr_in_block * block_size + addr_in_block * addr_in_block * block_size + count0 * block_size * addr_in_block * addr_in_block < inode->di_size)
            {
                ReadBlock(index0, &index1, count0 * sizeof(long), sizeof(long));
                if(index1 < 0)
                    return -1;
            }
            else
            {
                printf("getting new block\n");
                index1 = GetNewBlockIndex();
                if(index1 < 0)
                    return -1;
                if(WriteBlock(index0, &index1, count0 * sizeof(long), sizeof(long)) < 0)
                    return -1;
            }
            long count1 = 0;

            //printf("!index1 = %ld \n", index1);


            while(count1 < addr_in_block && bytes_wrote < size)
            {
                long index2; //индекс блока с одинарными непрямыми адресами (адресами блоков с прямыми адресами)
                if(10 * block_size + addr_in_block * block_size + addr_in_block * addr_in_block * block_size + count0 * block_size * addr_in_block * addr_in_block + count1 * addr_in_block * block_size < inode->di_size)
                {
                    ReadBlock(index1, &index2, count1 * sizeof(long), sizeof(long));
                    if(index1 < 0)
                        return -1;
                }
                else
                {
                    printf("!getting new block\n");
                    index2 = GetNewBlockIndex();
                    if(index2 < 0)
                        return -1;
                    if(WriteBlock(index1, &index2, count1 * sizeof(long), sizeof(long)) < 0)
                        return -1;
                }

                long count2 = 0;

                while(count2 < addr_in_block && bytes_wrote < size)
                {
                    long index3; //индекс блока с прямыми адресами (адресами блоков с данными)
                    if(10 * block_size + addr_in_block * block_size + addr_in_block * addr_in_block * block_size + count0 * block_size * addr_in_block * addr_in_block + count1 * addr_in_block * block_size + count2 * block_size < inode->di_size)
                    {
                        ReadBlock(index2, &index3, count2 * sizeof(long), sizeof(long));
                        if(index2 < 0)
                            return -1;
                    }
                    else
                    {
                        printf("!getting new block\n");
                        index3 = GetNewBlockIndex();
                        if(index3 < 0)
                            return -1;
                        if(WriteBlock(index2, &index3, count2 * sizeof(long), sizeof(long)) < 0)
                            return -1;
                    }

                    if(pos >= offset || (pos < offset && offset < pos + block_size))
                    {
                        long offs = pos - block_size * block_number;
                        long n = (size - bytes_wrote) < block_size - offs ? (size - bytes_wrote) : block_size - offs;
                        printf("!!Writing: bytes_wrote = %ld, pos = %ld, block = %ld, n = %ld, offs = %ld\n", bytes_wrote, pos, index3, n, offs);
                        if(WriteBlock(index3, buf + bytes_wrote, offs, n) < 0)
                            return -1;
                        bytes_wrote+=n;
                    }
                    pos = (block_number + 1) * block_size;
                    count2++;
                    block_number++;
                }
                count1++;
            }
            count0++;
        }
    }
    inode->di_size = inode->di_size > offset + size ? inode->di_size : offset + size ;
    return 0;
}

long ReadFile(struct dinode *inode, void *buf, long offset, long size)
{
    long bytes_read = 0;
    int block_number = 0;
    //long start_pos =
    long pos = offset;
    long addr_in_block = block_size/sizeof(long);

    //printf("@@@@@@@@@@@@@@@@@@@@@@\n");
    if(pos < 10 * block_size)
    {
        block_number = pos / block_size;
        while(bytes_read < size && block_number < 10)
        {
            if(inode->di_addr[block_number] < 0)
            {
                break;
            }
            long offs = pos - block_number * block_size;

            long n = (size - bytes_read) < block_size - offs ? (size - bytes_read) : block_size - offs;
            if(pos + n > inode->di_size)
                n = inode->di_size - pos;
            printf("Reading: pos = %ld, block = %ld, n = %ld, offs = %ld\n", pos, inode->di_addr[block_number], n, offs);
            if(ReadBlock(inode->di_addr[block_number], buf + bytes_read, offs, n) < 0)
                return -1;
            bytes_read+=n;
            pos = (block_number + 1) * block_size;
            block_number++;
        }
    }
    if(pos >= 10 * block_size && pos < addr_in_block*block_size)
    {
        if(inode->di_addr[10] < 0)
        {
            return -1;
        }
        long index0 = inode->di_addr[10]; //индекс блока с прямыми адресами (адресами блоков с данными)
        long count0 = 0;

        block_number = pos / block_size;

        while(count0 < addr_in_block && bytes_read < size)
        {

            long index1;//индекс блока с данными
            if(10 * block_size + count0 * block_size < inode->di_size)
            {
                ReadBlock(index0, &index1, count0 * sizeof(long), sizeof(long));
                if(index1 < 0)
                    return -1;
            }
            else
            {
                break;
            }

            long offs = pos - block_number * block_size;

            long n = (size - bytes_read) < block_size - offs ? (size - bytes_read) : block_size - offs;
            if(pos + n > inode->di_size)
                n = inode->di_size - pos;
            printf("Reading: !pos = %ld, block = %ld, n = %ld, offs = %ld\n", pos, index1, n, offs);
            if(ReadBlock(index1, buf + bytes_read, offs, n) < 0)
                return -1;
            bytes_read+=n;
            pos = (block_number + 1) * block_size;
            block_number++;
            count0++;
        }
    }
    if(pos >= addr_in_block*block_size && pos < addr_in_block*addr_in_block*block_size)
    {
        if(inode->di_addr[11] < 0)
        {
            return -1;
        }
        long index0 = inode->di_addr[11]; //индекс блока с одинарными непрямыми адресами (адресами блоков с прямыми адресами)
        long count0 = 0;

        block_number = pos / block_size;

        while(count0 < addr_in_block && bytes_read < size)
        {

            long index1; //индекс блока с прямыми адресами (адресами блоков с данными)
            if(10 * block_size + addr_in_block * block_size + count0 * block_size * addr_in_block < inode->di_size)
            {
                ReadBlock(index0, &index1, count0 * sizeof(long), sizeof(long));
                if(index1 < 0)
                    return -1;
            }
            else
            {
                break;
            }
            long count1 = 0;

            while(count1 < addr_in_block && bytes_read < size)
            {

                long index2;//индекс блока с данными
                if(10 * block_size + addr_in_block * block_size + count0 * block_size * addr_in_block + count1 * block_size < inode->di_size)
                {
                    ReadBlock(index1, &index2, count1 * sizeof(long), sizeof(long));
                    if(index2 < 0)
                        return -1;
                }
                else
                {
                    break;
                }

                long offs = pos - block_number * block_size;

                long n = (size - bytes_read) < block_size - offs ? (size - bytes_read) : block_size - offs;
                if(pos + n > inode->di_size)
                    n = inode->di_size - pos;
                printf("Reading: !!pos = %ld, block = %ld, n = %ld, offs = %ld\n", pos, index2, n, offs);
                if(ReadBlock(index2, buf + bytes_read, offs, n) < 0)
                    return -1;
                bytes_read+=n;
                pos = (block_number + 1) * block_size;
                block_number++;
                count1++;
            }
            count0++;
        }
    }
    if(pos >= addr_in_block*addr_in_block*block_size && pos < addr_in_block*addr_in_block*addr_in_block*block_size)
    {
        if(inode->di_addr[12] < 0)
        {
            return -1;
        }
        long index0 = inode->di_addr[12]; //индекс блока с двойными непрямыми адресами (адресами блоков с одинарными непрямыми адресами)
        long count0 = 0;

        block_number = pos / block_size;

        while(count0 < addr_in_block && bytes_read < size)
        {

            long index1; //индекс блока с одинарными непрямыми адресами (адресами блоков с прямыми адресами)
            if(10 * block_size + addr_in_block * block_size + addr_in_block * addr_in_block * block_size + count0 * block_size * addr_in_block * addr_in_block < inode->di_size)
            {
                ReadBlock(index0, &index1, count0 * sizeof(long), sizeof(long));
                if(index1 < 0)
                    return -1;
            }
            else
            {
                break;
            }
            long count1 = 0;

            while(count1 < addr_in_block && bytes_read < size)
            {

                long index2; //индекс блока с прямыми адресами (адресами блоков с данными)
                if(10 * block_size + addr_in_block * addr_in_block * block_size + count0 * block_size * addr_in_block * addr_in_block + count1 * addr_in_block * block_size < inode->di_size)
                {
                    ReadBlock(index1, &index2, count1 * sizeof(long), sizeof(long));
                    if(index2 < 0)
                        return -1;
                }
                else
                {
                    break;
                }

                long count2 = 0;
                while(count2 < addr_in_block && bytes_read < size)
                {

                    long index3; //индекс блока с данными
                    if(10 * block_size + addr_in_block * addr_in_block * block_size + count0 * block_size * addr_in_block * addr_in_block + count1 * addr_in_block * block_size + count1 * block_size < inode->di_size)
                    {
                        ReadBlock(index2, &index3, count2 * sizeof(long), sizeof(long));
                        if(index3 < 0)
                            return -1;
                    }
                    else
                    {
                        break;
                    }

                long offs = pos - block_number * block_size;

                long n = (size - bytes_read) < block_size - offs ? (size - bytes_read) : block_size - offs;
                if(pos + n > inode->di_size)
                    n = inode->di_size - pos;
                printf("Reading: !!!pos = %ld, block = %ld, n = %ld, offs = %ld\n", pos, index3, n, offs);
                if(ReadBlock(index3, buf + bytes_read, offs, n) < 0)
                    return -1;
                bytes_read+=n;
                pos = (block_number + 1) * block_size;
                block_number++;
                count2++;
                }
                count1++;
            }
            count0++;
        }
    }
    if(bytes_read < size)
        memset(buf+bytes_read, 0, size-bytes_read);
    return 0;
}

/*long TruncFile(struct dinode *inode, long offset, long size)
{
    long bytes_read = 0;
    int block_number = 0;
    long pos = offset;
    long addr_in_block = block_size/sizeof(long);

    if(pos < 10 * block_size)
    {
        block_number = pos / block_size;
        while(bytes_read < size && block_number < 10)
        {
            if(inode->di_addr[block_number] < 0)
            {
                break;
            }
            if(pos >= (block_number+1) * block_size)
            {
                FreeBlockIndex(inode->di_addr[block_number]);
                inode->di_addr[block_number];
            }
            bytes_read+=block_size;
            pos = (block_number + 1) * block_size;
            block_number++;
        }
    }
    if(pos >= 10 * block_size && pos < addr_in_block*block_size)
    {
        if(inode->di_addr[10] < 0)
        {
            return -1;
        }
        long index0 = inode->di_addr[10]; //индекс блока с прямыми адресами (адресами блоков с данными)
        long count0 = 0;

        block_number = pos / block_size;

        while(count0 < addr_in_block && bytes_read < size)
        {

            long index1; //индекс блока с данными
            if(10 * block_size + count0 * block_size < inode->di_size)
            {
                ReadBlock(index0, &index1, count0 * sizeof(long), sizeof(long));
                if(index1 < 0)
                    return -1;
            }
            else
            {
                break;
            }

            if(FreeBlockIndex(index1) < 0)
                return -1;
            bytes_read+=block_size;
            pos = (block_number + 1) * block_size;
            block_number++;
            count0++;
        }
    }
    if(pos >= addr_in_block*block_size && pos < addr_in_block*addr_in_block*block_size)
    {
        if(inode->di_addr[11] < 0)
        {
            return -1;
        }
        long index0 = inode->di_addr[11]; //индекс блока с одинарными непрямыми адресами (адресами блоков с прямыми адресами)
        long count0 = 0;

        block_number = pos / block_size;

        while(count0 < addr_in_block && bytes_read < size)
        {

            long index1; //индекс блока с прямыми адресами (адресами блоков с данными)
            if(10 * block_size + addr_in_block * block_size + count0 * block_size * addr_in_block < inode->di_size)
            {
                ReadBlock(index0, &index1, count0 * sizeof(long), sizeof(long));
                if(index1 < 0)
                    return -1;
            }
            else
            {
                break;
            }
            long count1 = 0;

            while(count1 < addr_in_block && bytes_read < size)
            {

                long index2;//индекс блока с данными
                if(10 * block_size + addr_in_block * block_size + count0 * block_size * addr_in_block + count1 * block_size < inode->di_size)
                {
                    ReadBlock(index1, &index2, count1 * sizeof(long), sizeof(long));
                    if(index2 < 0)
                        return -1;
                }
                else
                {
                    break;
                }

                long offs = pos - block_number * block_size;

                long n = (size - bytes_read) < block_size - offs ? (size - bytes_read) : block_size - offs;
                if(pos + n > inode->di_size)
                    n = inode->di_size - pos;
                printf("Reading: !!pos = %ld, block = %ld, n = %ld, offs = %ld\n", pos, index2, n, offs);
                if(ReadBlock(index2, buf + bytes_read, offs, n) < 0)
                    return -1;
                bytes_read+=n;
                pos = (block_number + 1) * block_size;
                block_number++;
                count1++;
            }
            count0++;
        }
    }
    if(pos >= addr_in_block*addr_in_block*block_size && pos < addr_in_block*addr_in_block*addr_in_block*block_size)
    {
        if(inode->di_addr[12] < 0)
        {
            return -1;
        }
        long index0 = inode->di_addr[12]; //индекс блока с двойными непрямыми адресами (адресами блоков с одинарными непрямыми адресами)
        long count0 = 0;

        block_number = pos / block_size;

        while(count0 < addr_in_block && bytes_read < size)
        {

            long index1; //индекс блока с одинарными непрямыми адресами (адресами блоков с прямыми адресами)
            if(10 * block_size + addr_in_block * block_size + addr_in_block * addr_in_block * block_size + count0 * block_size * addr_in_block * addr_in_block < inode->di_size)
            {
                ReadBlock(index0, &index1, count0 * sizeof(long), sizeof(long));
                if(index1 < 0)
                    return -1;
            }
            else
            {
                break;
            }
            long count1 = 0;

            while(count1 < addr_in_block && bytes_read < size)
            {

                long index2; //индекс блока с прямыми адресами (адресами блоков с данными)
                if(10 * block_size + addr_in_block * addr_in_block * block_size + count0 * block_size * addr_in_block * addr_in_block + count1 * addr_in_block * block_size < inode->di_size)
                {
                    ReadBlock(index1, &index2, count1 * sizeof(long), sizeof(long));
                    if(index2 < 0)
                        return -1;
                }
                else
                {
                    break;
                }

                long count2 = 0;
                while(count2 < addr_in_block && bytes_read < size)
                {

                    long index3; //индекс блока с данными
                    if(10 * block_size + addr_in_block * addr_in_block * block_size + count0 * block_size * addr_in_block * addr_in_block + count1 * addr_in_block * block_size + count1 * block_size < inode->di_size)
                    {
                        ReadBlock(index2, &index3, count2 * sizeof(long), sizeof(long));
                        if(index3 < 0)
                            return -1;
                    }
                    else
                    {
                        break;
                    }

                long offs = pos - block_number * block_size;

                long n = (size - bytes_read) < block_size - offs ? (size - bytes_read) : block_size - offs;
                if(pos + n > inode->di_size)
                    n = inode->di_size - pos;
                printf("Reading: !!!pos = %ld, block = %ld, n = %ld, offs = %ld\n", pos, index3, n, offs);
                if(ReadBlock(index3, buf + bytes_read, offs, n) < 0)
                    return -1;
                bytes_read+=n;
                pos = (block_number + 1) * block_size;
                block_number++;
                count2++;
                }
                count1++;
            }
            count0++;
        }
    }
    if(bytes_read < size)
        memset(buf+bytes_read, 0, size-bytes_read);
    return 0;
}*/
long GetInodeIndexByPath(const char *path)
{
    int pos = 0;
    long index = -1;
    if(path[pos] == '/')
        index = 0;

    int l = strlen(path)+1;
    char str[l];
    strncpy(str, path, l);
    char *pch;
    printf ("Splitting string \"%s\" into tokens:\n",str);
    pch = strtok (str,"/");
    while (pch != NULL)
    {
        struct dinode n = ReadInode(index);
        if(n.di_size < 0)
            return -1;
        int count = n.di_size/sizeof(struct dirent);
        struct dirent items[count];
        ReadFile(&n, (void *)items, 0, n.di_size);
        int i;
        index = -1;
        for (i = 0; i<count; i++)
        {
            //printf ("%s\n",items[i].d_name);
            if(strcmp(items[i].d_name, pch) == 0)
            {
                index = items[i].d_ino;
                break;
            }
        }
        //printf ("%s\n",pch);
        pch = strtok (NULL, "/");
    }
    return index;
}

long CreateDirectory(const char *parent_path, char *name)
{
    long parent_index = GetInodeIndexByPath(parent_path);
    struct dinode parent = ReadInode(parent_index);
    if(parent.di_size < 0)
        return -1;
    long index = GetNewInodeIndex();
    if(index < 0)
        return -1;
    struct dinode n;

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
    (parent.di_nlink)++;
    struct dirent items[2];
    long i;
    for(i = 0; i<sizeof(n.di_addr)/sizeof(long); i++)
    {
        n.di_addr[i] = -1;
    }
    strcpy(items[0].d_name,".."); //parent
    items[0].d_ino=parent_index;

    strcpy(items[1].d_name,"."); //this
    items[1].d_ino=index;

    for(i = 0; i<2; i++)
    {
        items[i].d_off = sizeof(struct dirent);       /* offset to the next dirent */
        items[i].d_reclen = sizeof(items);    /* length of this record */
        items[i].d_type = -i;      /* type of file; not supported by all file system types */
    }
    printf("1111");
    struct dirent parent_item;
    parent_item.d_ino = index;       /* inode number */
    parent_item.d_off = sizeof(parent_item);       /* offset to the next dirent */
    parent_item.d_reclen = sizeof(parent_item);    /* length of this record */
    parent_item.d_type = -1;      /* type of file; not supported by all file system types */
    strncpy(parent_item.d_name, name, sizeof(parent_item.d_name));
    if(WriteFile(&n, items, 0, sizeof(items)) < 0)
    {
        return -1;
    }
    if(WriteInode(index, n) < 0)
    {
        return -1;
    }
    printf("parent_size = %ld\n", parent.di_size);
    if(WriteFile(&parent, (void *)&parent_item, parent.di_size, sizeof(parent_item)) < 0)
    {
        return -1;
    }
    printf("parent_size = %ld\n", parent.di_size);
    if(WriteInode(parent_index, parent) < 0)
    {
        return -1;
    }
    return 0;
}
