#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include "inode.h"


#define MAX_INODES_COUNT 1000
#define FREE_INODES_COUNT 1000
#define MAX_BLOCKS_COUNT 1000000
#define FREE_BLOCKS_COUNT 1000000
#define BLOCK_SIZE 4096
/*#define LOG_PATH "/home/totzhe/ОС/FUSE/log"
#define FILE_PATH "/home/totzhe/ОС/FUSE/file"*/
#define LOG_PATH "/media/Study/Z/3 курс/Операционные системы/FUSE/log"
#define FILE_PATH "/media/Study/Z/3 курс/Операционные системы/FUSE/file"

char *fsfilename;
long max_inodes_count;
long free_inodes_count;
long max_blocks_count;
long free_blocks_count;
long block_size;

long param_count = 5;

FILE *file;


void WriteToLog(const char *str)
{
    FILE *output;

    if((output=fopen(LOG_PATH, "a+"))==0)
    {
        puts ("Can't open output file.");
        exit(-1);
    }
    fprintf(output, "%s\n", str);
    fclose(output);
}

long ReadFreeInodesCount()
{
    /*FILE *file;
    if((file=fopen(fsfilename,"rb"))==0)
    {
        //!!!printf("Can't open input file.\n");
        return -1;
    }*/
    long result;
    fseek(file, 0, SEEK_SET);
    fread(&result, sizeof(result), 1, file);
    //fclose(file);
    /*char buf[50];
    sprintf(buf, "Read free inodes count: %ld", result);
    //WriteToLog(buf);*/
    return result;
}

long WriteFreeInodesCount(long value)
{
    /*FILE *file;
    if((file=fopen(fsfilename,"r+b"))==0)
    {
        //!!!printf("Can't open input file.\n");
        return -1;
    }*/

    fseek(file, 0, SEEK_SET);
    fwrite(&value, sizeof(value), 1, file);
    fflush(file);
    /*char buf[50];
    sprintf(buf, "Wrote free inodes count: %ld", value);
    //WriteToLog(buf);*/
    return 0;
}

long ReadMaxInodesCount()
{
    /*FILE *file;
    if((file=fopen(fsfilename,"rb"))==0)
    {
        //!!!printf("Can't open input file.\n");
        return -1;
    }*/

    long result;
    fseek(file, sizeof(long), SEEK_SET);
    fread(&result, sizeof(result), 1, file);
    //fclose(file);
    /*char buf[50];
    sprintf(buf, "Read max inodes count: %ld", result);
    //WriteToLog(buf);*/
    return result;
}

long WriteMaxInodesCount(long value)
{
    /*FILE *file;
    if((file=fopen(fsfilename,"r+b"))==0)
    {
        //!!!printf("Can't open input file.\n");
        return -1;
    }*/

    fseek(file, sizeof(long), SEEK_SET);
    fwrite(&value, sizeof(value), 1, file);
    fflush(file);
    /*char buf[50];
    sprintf(buf, "Wrote max inodes count: %ld", value);
    //WriteToLog(buf);*/
    return 0;
}

long ReadFreeBlocksCount()
{
    /*FILE *file;
    if((file=fopen(fsfilename,"rb"))==0)
    {
        //!!!printf("Can't open input file.\n");
        return -1;
    }*/
    long result;
    fseek(file, 2*sizeof(long), SEEK_SET);
    fread(&result, sizeof(result), 1, file);
    //fclose(file);
    /*char buf[50];
    sprintf(buf, "Read free blocks count: %ld", result);
    //WriteToLog(buf);*/
    return result;
}

long WriteFreeBlocksCount(long value)
{
    /*FILE *file;
    if((file=fopen(fsfilename,"r+b"))==0)
    {
        //!!!printf("Can't open input file.\n");
        return -1;
    }*/

    fseek(file, 2*sizeof(long), SEEK_SET);
    fwrite(&value, sizeof(value), 1, file);
    fflush(file);
    /*char buf[50];
    sprintf(buf, "Wrote free blocks count: %ld", value);
    //WriteToLog(buf);*/
    return 0;
}

long ReadMaxBlocksCount()
{
    /*FILE *file;
    if((file=fopen(fsfilename,"rb"))==0)
    {
        //!!!printf("Can't open input file.\n");
        return -1;
    }*/
    long result;
    fseek(file, 3*sizeof(long), SEEK_SET);
    fread(&result, sizeof(result), 1, file);
    //fclose(file);
    /*char buf[50];
    sprintf(buf, "Read max blocks count: %ld", result);
    //WriteToLog(buf);*/
    return result;
}

long WriteMaxBlocksCount(long value)
{
    /*FILE *file;
    if((file=fopen(fsfilename,"r+b"))==0)
    {
        //!!!printf("Can't open input file.\n");
        return -1;
    }*/

    fseek(file, 3*sizeof(long), SEEK_SET);
    fwrite(&value, sizeof(value), 1, file);
    fflush(file);
    /*char buf[50];
    sprintf(buf, "Wrote max blocks count: %ld", value);
    //WriteToLog(buf);*/
    return 0;
}

long ReadBlockSize()
{
    /*FILE *file;
    if((file=fopen(fsfilename,"rb"))==0)
    {
        //!!!printf("Can't open input file.\n");
        return -1;
    }*/

    long result;
    fseek(file, 4*sizeof(long), SEEK_SET);
    fread(&result, sizeof(result), 1, file);
    //fclose(file);
    /*char buf[50];
    sprintf(buf, "Read block size: %ld", result);
    //WriteToLog(buf);*/
    return result;
}

long WriteBlockSize(long value)
{
    /*FILE *file;
    if((file=fopen(fsfilename,"r+b"))==0)
    {
        //!!!printf("Can't open input file.\n");
        return -1;
    }*/

    fseek(file, 4*sizeof(long), SEEK_SET);
    fwrite(&value, sizeof(value), 1, file);
    fflush(file);
    /*char buf[50];
    sprintf(buf, "Wrote block size: %ld", value);
    //WriteToLog(buf);*/
    return 0;
}

long GetNewInodeIndex()
{
    long count = ReadFreeInodesCount();

    if(count == 0)
        return -1;
    /*FILE *file;
    if((file=fopen(fsfilename,"rb"))==0)
    {
        //!!!printf("Can't open input file.\n");
        return -1;
    }*/

    long result;
    long offset = param_count * sizeof(long) + max_inodes_count * sizeof(struct dinode) + (count - 1) * sizeof(long);
    fseek(file, offset, SEEK_SET);
    fread(&result, sizeof(result), 1, file);
    //fclose(file);
    WriteFreeInodesCount(--count);

    return result;
}

long FreeInodeIndex(long index)
{
    if(index < 0 || index >= max_inodes_count)
        return -1;
    long count = ReadFreeInodesCount();

    /*FILE *file;
    if((file=fopen(fsfilename,"r+b"))==0)
    {
        //!!!printf("Can't open input file.\n");
        return -1;
    }*/

    long offset = param_count * sizeof(long) + max_inodes_count * sizeof(struct dinode) + count * sizeof(long);
    fseek(file, offset, SEEK_SET);
    fwrite(&index, sizeof(index), 1, file);
    fflush(file);
    /*char buf[50];
    sprintf(buf, "Inode was freed: %ld", index);
    printf("Inode was freed: %ld\n", index);
    //WriteToLog(buf);*/
    WriteFreeInodesCount(++count);
    return 0;
}

long GetNewBlockIndex()
{
    long count = ReadFreeBlocksCount();

    if(count == 0)
        return -1;
    /*FILE *file;
    if((file=fopen(fsfilename,"rb"))==0)
    {
        //!!!printf("Can't open input file.\n");
        return -1;
    }*/

    long result;
    long offset = param_count * sizeof(long) + max_inodes_count * sizeof(struct dinode) + max_inodes_count * sizeof(long) + (count - 1) * sizeof(long);
    fseek(file, offset, SEEK_SET);
    fread(&result, sizeof(result), 1, file);
    //fclose(file);
    WriteFreeBlocksCount(--count);

    return result;
}

long FreeBlockIndex(long index)
{
    if(index < 0 || index >= max_blocks_count)
        return -1;
    long count = ReadFreeBlocksCount();

    /*FILE *file;
    if((file=fopen(fsfilename,"r+b"))==0)
    {
        //!!!printf("Can't open input file.\n");
        return -1;
    }*/


    long offset = param_count * sizeof(long) + max_inodes_count * sizeof(struct dinode) + max_inodes_count * sizeof(long) + count * sizeof(long);
    fseek(file, offset, SEEK_SET);
    fwrite(&index, sizeof(index), 1, file);
    fflush(file);
    WriteFreeBlocksCount(++count);
    /*char buf[50];
    sprintf(buf, "Block was freed: %ld", index);
    printf("Block was freed: %ld\n", index);
    //WriteToLog(buf);*/
    return 0;
}

struct dinode ReadInode(long index)
{
    struct dinode result;
    result.di_size = -1;
    result.di_gen = -1;
    if(index < 0 || index >= max_inodes_count)
        return result;
    /*FILE *file;
    if((file=fopen(fsfilename,"rb"))==0)
    {
        //!!!printf("Can't open input file.\n");
        return result;
    }*/

    long offset = param_count * sizeof(long) + index * sizeof(result);
    fseek(file, offset, SEEK_SET);
    fread(&result, sizeof(result), 1, file);
    //fclose(file);
    /*char buf[50];
    sprintf(buf, "Read inode: %ld", index);
    //WriteToLog(buf);*/
    return result;
}

long WriteInode(long index, struct dinode value)
{
    if(index < 0 || index >= max_inodes_count)
        return -1;

    /*FILE *file;
    if((file=fopen(fsfilename,"r+b"))==0)
    {
        //!!!printf("Can't open input file.\n");
        return -1;
    }*/

    long offset = param_count * sizeof(long) + index * sizeof(value);
    fseek(file, offset, SEEK_SET);
    fwrite(&value, sizeof(value), 1, file);
    fflush(file);
    /*char buf[50];
    sprintf(buf, "Wrote inode: %ld", index);
    //WriteToLog(buf);*/
    return 0;
}

long ReadBlock(long index, void *buf, long offset, long size)
{
    if(index < 0 || index >= max_blocks_count || offset + size > block_size)
        return -1;

    /*FILE *file;
    if((file=fopen(fsfilename,"rb"))==0)
    {
        //!!!printf("Can't open input file.\n");
        return -1;
    }*/

    long offs = (param_count + max_blocks_count + max_inodes_count) * sizeof(long) + max_inodes_count * sizeof(struct dinode) + index * block_size + offset;
    fseek(file, offs, SEEK_SET);
    fread(buf, size, 1, file);

    //fclose(file);
    /*char buff[50];
    sprintf(buff, "Read %ld bytes from block %ld; offset = %ld", size, index, offset);
    //WriteToLog(buff);*/
    return 0;
}

long WriteBlock(long index, void *buf, long offset, long size)
{
    if(buf == NULL || index < 0 || index >= max_blocks_count || offset + size > block_size)
        return -1;

    /*FILE *file;
    if((file=fopen(fsfilename,"r+b"))==0)
    {
        //!!!printf("Can't open input file.\n");
        return -1;
    }*/

    long offs = (param_count + max_blocks_count + max_inodes_count) * sizeof(long) + max_inodes_count * sizeof(struct dinode) + index * block_size + offset;
    fseek(file, offs, SEEK_SET);
    fwrite(buf, size, 1, file);
    /*char buff[50];
    sprintf(buff, "Wrote %ld bytes to block %ld; offset = %ld", size, index, offset);
    //WriteToLog(buff);*/
    fflush(file);
    return 0;
}

long Load(char *filename)
{
    //FILE *file;
    if((file=fopen(filename,"r+b"))==0)
    {
        //!!!printf("Can't open input file.\n");
        return -1;
    }

    fsfilename = filename;


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
         //!!!printf("%ld\n", node.di_gen);
     }
     for(i = 0; i < max_inodes_count; i++)
     {
         long n;
         fread(&n, sizeof(n), 1, input);
         //!!!printf("%ld\n", n);
     }*/

    fflush(file);
    return 0;
}

void Destroy()
{
    fflush(file);
    fclose(file);
}

long WriteFile(struct dinode *inode, void *buf, long offset, long size)
{
    //!!!printf("SIZE = %ld\n", size);
    long bytes_wrote = 0;
    long block_number = 0;
    long pos = offset < inode->di_size ? offset : inode->di_size;
    //!!!printf("POS = %ld\n", pos);
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
                {
                    //WriteToLog("ERROR1");
                    return -1;
                }
            }
            if(pos >= offset || (pos <= offset && offset < pos + block_size))
            {
                long offs = pos - block_size * block_number;
                long n = (size - bytes_wrote) < block_size - offs ? (size - bytes_wrote) : block_size - offs;
                //!!!printf("Writing: bytes_wrote = %ld, pos = %ld, block = %ld, n = %ld, offs = %ld\n", bytes_wrote, pos, inode->di_addr[block_number], n, offs);
                char message[500];
                sprintf(message, "Writing: bytes_wrote = %ld, pos = %ld, block = %ld, n = %ld, offs = %ld\n", bytes_wrote, pos, inode->di_addr[block_number], n, offs);
                //WriteToLog(message);
                if(WriteBlock(inode->di_addr[block_number], buf + bytes_wrote, offs, n) < 0)
                {
                    //WriteToLog("ERROR2");
                    char message[500];
                    sprintf(message, "bytes_wrote = %ld, pos = %ld, block = %ld, n = %ld, offs = %ld\n", bytes_wrote, pos, inode->di_addr[block_number], n, offs);
                    //WriteToLog(message);
                    return -1;
                }
                bytes_wrote+=n;
            }
            pos = (block_number + 1) * block_size;
            block_number++;
        }
    }
    if(pos >= 10 * block_size && pos < 10 * block_size + addr_in_block*block_size)
    {
        if(inode->di_addr[10] < 0)
        {
            inode->di_addr[10] = GetNewBlockIndex();
            if(inode->di_addr[10] < 0)
                return -1;
        }
        long index0 = inode->di_addr[10]; //индекс блока с прямыми адресами (адресами блоков с данными)
        //pos+=count0*block_size;
        block_number = pos / block_size;
        long count0 = block_number - 10;
        //!!!printf("BLOCK_NUMBER = %ld\n", block_number);

        while(count0 < addr_in_block && bytes_wrote < size)
        {
            long index1;//индекс блока с данными
            //!!!printf("!di_size = %ld > %ld\n", inode->di_size, 10 * block_size + count0 * block_size);
            if(10 * block_size + count0 * block_size < inode->di_size)
            {
                ReadBlock(index0, &index1, count0 * sizeof(long), sizeof(long));
                if(index1 < 0)
                    return -1;
                //!!!printf("!@@index1 = %ld, count0 * sizeof(long) = %ld\n", index1, count0 * sizeof(long));
            }
            else
            {
                //!!!printf("getting new block\n");
                index1 = GetNewBlockIndex();
                if(index1 < 0)
                    return -1;
                if(WriteBlock(index0, &index1, count0 * sizeof(long), sizeof(long)) < 0)
                    return -1;
                //!!!printf("index1 = %ld, count0 * sizeof(long) = %ld\n", index1, count0 * sizeof(long));
            }

            //!!!printf("OFFSET = %ld\n", offset);
            if(pos >= offset || (pos <= offset && offset < pos + block_size))
            {
                long offs = pos - block_size * block_number;
                long n = (size - bytes_wrote) < block_size - offs ? (size - bytes_wrote) : block_size - offs;
                //!!!printf("!Writing: bytes_wrote = %ld, pos = %ld, block = %ld, n = %ld, offs = %ld\n", bytes_wrote, pos, index1, n, offs);
                char message[500];
                sprintf(message, "Writing: bytes_wrote = %ld, pos = %ld, block = %ld, n = %ld, offs = %ld\n", bytes_wrote, pos, index1, n, offs);
                //WriteToLog(message);
                if(WriteBlock(index1, buf + bytes_wrote, offs, n) < 0)
                    return -1;
                bytes_wrote+=n;
            }
            pos = (block_number + 1) * block_size;
            count0++;
            block_number++;
        }
    }
    if(pos >= 10 * block_size + addr_in_block*block_size && pos < 10 * block_size + addr_in_block*block_size + addr_in_block*addr_in_block*block_size)
    {
        if(inode->di_addr[11] < 0)
        {
            inode->di_addr[11] = GetNewBlockIndex();
            if(inode->di_addr[11] < 0)
                return -1;
        }
        long index0 = inode->di_addr[11]; //индекс блока с одинарными непрямыми адресами (адресами блоков с прямыми адресами)

        block_number = pos / block_size;
        long count0 = (block_number-10-addr_in_block)/addr_in_block;

        while(count0 < addr_in_block && bytes_wrote < size)
        {
            long index1; //индекс блока с прямыми адресами (адресами блоков с данными)
            //!!!printf("!di_size = %ld > %ld\n", inode->di_size, 10 * block_size + count0 * block_size);
            if(10 * block_size + addr_in_block * block_size + count0 * block_size * addr_in_block < inode->di_size)
            {
                ReadBlock(index0, &index1, count0 * sizeof(long), sizeof(long));
                if(index1 < 0)
                    return -1;
            }
            else
            {
                //!!!printf("getting new block\n");
                index1 = GetNewBlockIndex();
                if(index1 < 0)
                    return -1;
                if(WriteBlock(index0, &index1, count0 * sizeof(long), sizeof(long)) < 0)
                    return -1;
            }
            long count1 = (block_number-10-addr_in_block) - count0 * addr_in_block;

            //!!!printf("!index1 = %ld \n", index1);


            while(count1 < addr_in_block && bytes_wrote < size)
            {
                long index2; //индекс блока с данными
                //!!!printf("!!di_size = %ld > %ld\n", inode->di_size, 10 * block_size + count0 * block_size);
                if(10 * block_size + addr_in_block * block_size + count0 * block_size * addr_in_block + count1 * block_size < inode->di_size)
                {
                    ReadBlock(index1, &index2, count1 * sizeof(long), sizeof(long));
                    if(index1 < 0)
                        return -1;
                }
                else
                {
                    //!!!printf("!getting new block\n");
                    index2 = GetNewBlockIndex();
                    if(index2 < 0)
                        return -1;
                    if(WriteBlock(index1, &index2, count1 * sizeof(long), sizeof(long)) < 0)
                        return -1;
                }
                //!!!printf("BLOCK_NUMBER = %ld\n", block_number);
                //!!!printf("OFFSET = %ld\n", offset);
                //!!!printf("index0 = %ld\n", index0);
                //!!!printf("index1 = %ld\n", index1);

                if(pos >= offset || (pos <= offset && offset < pos + block_size))
                {
                    long offs = pos - block_size * block_number;
                    long n = (size - bytes_wrote) < block_size - offs ? (size - bytes_wrote) : block_size - offs;
                    //!!!printf("!!Writing: bytes_wrote = %ld, pos = %ld, block = %ld, n = %ld, offs = %ld\n", bytes_wrote, pos, index2, n, offs);
                    char message[500];
                    sprintf(message, "Writing: bytes_wrote = %ld, pos = %ld, block = %ld, n = %ld, offs = %ld\n", bytes_wrote, pos, index2, n, offs);
                    //WriteToLog(message);
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
    if(pos >= 10 * block_size + addr_in_block*block_size + addr_in_block*addr_in_block*block_size && pos < 10 * block_size + addr_in_block*block_size + addr_in_block*addr_in_block*block_size + addr_in_block*addr_in_block*addr_in_block*block_size)
    {
        if(inode->di_addr[12] < 0)
        {
            inode->di_addr[12] = GetNewBlockIndex();
            if(inode->di_addr[12] < 0)
                return -1;
        }
        long index0 = inode->di_addr[12]; //индекс блока с двойными непрямыми адресами (адресами блоков с одинарными непрямыми адресами)
        block_number = pos / block_size;
        long count0 = (block_number-10-addr_in_block-addr_in_block*addr_in_block)/addr_in_block/addr_in_block;

        while(count0 < addr_in_block && bytes_wrote < size)
        {
            long index1; //индекс блока с одинарными непрямыми адресами (адресами блоков с прямыми адресами)
            //!!!printf("!!!di_size = %ld > %ld\n", inode->di_size, 10 * block_size + count0 * block_size);
            if(10 * block_size + addr_in_block * block_size + addr_in_block * addr_in_block * block_size + count0 * block_size * addr_in_block * addr_in_block < inode->di_size)
            {
                ReadBlock(index0, &index1, count0 * sizeof(long), sizeof(long));
                if(index1 < 0)
                    return -1;
            }
            else
            {
                //!!!printf("getting new block\n");
                index1 = GetNewBlockIndex();
                if(index1 < 0)
                    return -1;
                if(WriteBlock(index0, &index1, count0 * sizeof(long), sizeof(long)) < 0)
                    return -1;
            }
            //!!!printf("IND1 = %ld{\n", index1);
            long count1 = (block_number-10-addr_in_block-addr_in_block*addr_in_block)/addr_in_block - count0 * addr_in_block;

            ////!!!printf("!index1 = %ld \n", index1);


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
                    //!!!printf("!getting new block\n");
                    index2 = GetNewBlockIndex();
                    if(index2 < 0)
                        return -1;
                    if(WriteBlock(index1, &index2, count1 * sizeof(long), sizeof(long)) < 0)
                        return -1;
                }
                //!!!printf("IND2 = %ld[\n", index2);

                long count2 = (block_number-10-addr_in_block-addr_in_block*addr_in_block) - count0 * addr_in_block * addr_in_block - count1 * addr_in_block;

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
                        //!!!printf("!getting new block\n");
                        index3 = GetNewBlockIndex();
                        if(index3 < 0)
                            return -1;
                        if(WriteBlock(index2, &index3, count2 * sizeof(long), sizeof(long)) < 0)
                            return -1;
                    }
                    //!!!printf("IND3 = %ld(\n", index3);

                    if(pos >= offset || (pos <= offset && offset < pos + block_size))
                    {
                        long offs = pos - block_size * block_number;
                        long n = (size - bytes_wrote) < block_size - offs ? (size - bytes_wrote) : block_size - offs;
                        //!!!printf("!!Writing: bytes_wrote = %ld, pos = %ld, block = %ld, n = %ld, offs = %ld\n", bytes_wrote, pos, index3, n, offs);
                        char message[500];
                        sprintf(message, "Writing: bytes_wrote = %ld, pos = %ld, block = %ld, n = %ld, offs = %ld\n", bytes_wrote, pos, index3, n, offs);
                        //WriteToLog(message);
                        if(WriteBlock(index3, buf + bytes_wrote, offs, n) < 0)
                            return -1;
                        bytes_wrote+=n;
                    }
                    pos = (block_number + 1) * block_size;
                    count2++;
                    block_number++;
                    //!!!printf(")\n");
                }
                count1++;
                //!!!printf("]\n");
            }
            count0++;
            //!!!printf("}\n");
        }
    }
    inode->di_size = inode->di_size > offset + size ? inode->di_size : offset + size ;
    //!!!printf("inode->di_size = %ld\n",inode->di_size);
    inode->di_atime = time(NULL);
    inode->di_mtime = time(NULL);
    return 0;
}

long ReadFile(struct dinode *inode, void *buf, long offset, long size)
{
    long bytes_read = 0;
    long block_number = 0;
    long pos = offset;
    long addr_in_block = block_size/sizeof(long);

    if(pos < inode->di_size)
    {
    if(pos < 10 * block_size)
    {
        block_number = pos / block_size;
        while(bytes_read < size && block_number < 10 && pos < inode->di_size)
        {
            if(inode->di_addr[block_number] < 0)
            {
                //WriteToLog("ERR1");
                return -1;
            }
            long offs = pos - block_number * block_size;

            long n = (size - bytes_read) < block_size - offs ? (size - bytes_read) : block_size - offs;
            if(pos + n > inode->di_size)
                n = inode->di_size - pos;
            ////!!!printf("Reading: pos = %ld, block = %ld, n = %ld, offs = %ld\n", pos, inode->di_addr[block_number], n, offs);
            char message[500];
            sprintf(message, "Reading: bytes_read = %ld, pos = %ld, block = %ld, n = %ld, offs = %ld\n", bytes_read, pos, inode->di_addr[block_number], n, offs);
            //WriteToLog(message);
            if(ReadBlock(inode->di_addr[block_number], buf + bytes_read, offs, n) < 0)
            {
                //WriteToLog("ERR2");
                return -1;
            }
            bytes_read+=n;
            pos = (block_number + 1) * block_size;
            block_number++;
        }
    }
    if(pos >= 10 * block_size && pos < 10 * block_size + addr_in_block*block_size && pos < inode->di_size)
    {
        if(inode->di_addr[10] < 0)
        {
            return -1;
        }
        long index0 = inode->di_addr[10]; //индекс блока с прямыми адресами (адресами блоков с данными)

        block_number = pos / block_size;
        long count0 = block_number - 10;

        while(count0 < addr_in_block && bytes_read < size && pos < inode->di_size)
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
            ////!!!printf("Reading: !pos = %ld, block = %ld, n = %ld, offs = %ld\n", pos, index1, n, offs);
            char message[500];
            sprintf(message, "Reading: bytes_read = %ld, pos = %ld, block = %ld, n = %ld, offs = %ld\n", bytes_read, pos, index1, n, offs);
            //WriteToLog(message);
            if(ReadBlock(index1, buf + bytes_read, offs, n) < 0)
                return -1;
            bytes_read+=n;
            pos = (block_number + 1) * block_size;
            block_number++;
            count0++;
        }
    }
    if(pos >= 10 * block_size + addr_in_block*block_size && pos < 10 * block_size + addr_in_block*block_size + addr_in_block*addr_in_block*block_size && pos < inode->di_size)
    {
        if(inode->di_addr[11] < 0)
        {
            return -1;
        }
        long index0 = inode->di_addr[11]; //индекс блока с одинарными непрямыми адресами (адресами блоков с прямыми адресами)

        block_number = pos / block_size;
        long count0 = (block_number-10-addr_in_block)/addr_in_block;

        /*//!!!printf("POS = %ld < %ld\n", pos, 10 * block_size + addr_in_block*block_size + addr_in_block*addr_in_block*block_size + addr_in_block*addr_in_block*addr_in_block*block_size);
        //!!!printf("!Reading: block_number = %ld, count0 = %ld\n", block_number, count0);*/
        while(count0 < addr_in_block && bytes_read < size && pos < inode->di_size)
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
            long count1 = (block_number-10-addr_in_block) - count0 * addr_in_block;

            while(count1 < addr_in_block && bytes_read < size && pos < inode->di_size)
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
                ////!!!printf("Reading: !!pos = %ld, block = %ld, n = %ld, offs = %ld\n", pos, index2, n, offs);
                char message[500];
                sprintf(message, "Reading: bytes_read = %ld, pos = %ld, block = %ld, n = %ld, offs = %ld\n", bytes_read, pos, index2, n, offs);
                //WriteToLog(message);
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
    if(pos >= 10 * block_size + addr_in_block*block_size + addr_in_block*addr_in_block*block_size && pos < 10 * block_size + addr_in_block*block_size + addr_in_block*addr_in_block*block_size + addr_in_block*addr_in_block*addr_in_block*block_size && pos < inode->di_size)
    {
        if(inode->di_addr[12] < 0)
        {
            return -1;
        }
        long index0 = inode->di_addr[12]; //индекс блока с двойными непрямыми адресами (адресами блоков с одинарными непрямыми адресами)
        block_number = pos / block_size;
        long count0 = (block_number-10-addr_in_block-addr_in_block*addr_in_block)/addr_in_block/addr_in_block;
        //!!!printf("index0 = %ld, count0 = %ld\n", index0, count0);


        while(count0 < addr_in_block && bytes_read < size && pos < inode->di_size)
        {
            long index1; //индекс блока с одинарными непрямыми адресами (адресами блоков с прямыми адресами)
            //!!!printf("%ld < %ld\n", 10 * block_size + addr_in_block * block_size + addr_in_block * addr_in_block * block_size + count0 * block_size * addr_in_block * addr_in_block, inode->di_size);


            if(10 * block_size + addr_in_block * block_size + addr_in_block * addr_in_block * block_size + count0 * block_size * addr_in_block * addr_in_block < inode->di_size)
            {
                ReadBlock(index0, &index1, count0 * sizeof(long), sizeof(long));
                if(index1 < 0)
                    return -1;
                //!!!printf("!!!!!!!!\n");
            }
            else
            {
                break;
            }
            long count1 = (block_number-10-addr_in_block-addr_in_block*addr_in_block)/addr_in_block - count0 * addr_in_block;

            //!!!printf("block_number = %ld\n", block_number);
            while(count1 < addr_in_block && bytes_read < size && pos < inode->di_size)
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

                long count2 = (block_number-10-addr_in_block-addr_in_block*addr_in_block) - count0 * addr_in_block * addr_in_block - count1 * addr_in_block;
                while(count2 < addr_in_block && bytes_read < size && pos < inode->di_size)
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
                    ////!!!printf("Reading: !!!pos = %ld, block = %ld, n = %ld, offs = %ld\n", pos, index3, n, offs);
                    char message[500];
                    sprintf(message, "Reading: bytes_read = %ld, pos = %ld, block = %ld, n = %ld, offs = %ld\n", bytes_read, pos, index3, n, offs);
                    //WriteToLog(message);
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
    }
    if(bytes_read < size)
        memset(buf+bytes_read, 0, size-bytes_read);
    inode->di_atime = time(NULL);
    return 0;
}
long TruncFile(struct dinode *inode, long offset)
{
    long size = inode->di_size - offset;
    long bytes_removed = 0;
    long block_number = 0;
    long pos = offset;
    long addr_in_block = block_size/sizeof(long);

    if(pos < 10 * block_size)
    {
        block_number = pos / block_size;
        while(bytes_removed < size && block_number < 10)
        {
            if(inode->di_addr[block_number] < 0)
            {
                break;
            }
            long offs = pos - block_number * block_size;

            long n = (size - bytes_removed) < block_size - offs ? (size - bytes_removed) : block_size - offs;
            if(pos + n > inode->di_size)
                n = inode->di_size - pos;
            //!!!printf("Removing: pos = %ld, block = %ld, n = %ld, offs = %ld\n", pos, inode->di_addr[block_number], n, offs);
            if(offs == 0)
            {
                //!!!printf ("FREE BLOCK0 %ld", inode->di_addr[block_number]);
                if(FreeBlockIndex(inode->di_addr[block_number]) < 0)
                    return -1;
                //!!!//!!!printf (" SUCCESS\n");
                inode->di_addr[block_number] = -1;
            }
            bytes_removed+=n;
            pos = (block_number + 1) * block_size;
            block_number++;
        }
    }
    if(pos >= 10 * block_size && pos < 10 * block_size + addr_in_block*block_size)
    {
        if(inode->di_addr[10] >= 0)
        {
            long index0 = inode->di_addr[10]; //индекс блока с прямыми адресами (адресами блоков с данными)

            block_number = pos / block_size;
            long count0 = block_number - 10;
            char free_block0 = count0 == 0? 1 : 0;
            while(count0 < addr_in_block && bytes_removed < size)
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

                long n = (size - bytes_removed) < block_size - offs ? (size - bytes_removed) : block_size - offs;
                if(pos + n > inode->di_size)
                    n = inode->di_size - pos;
                //!!!printf("!Removing: pos = %ld, block = %ld, n = %ld, offs = %ld\n", pos, index1, n, offs);
                if(offs == 0)
                {
                    //!!!printf ("!!FREE BLOCK1 %ld", index1);
                    if(FreeBlockIndex(index1) < 0)
                        return -1;
                    //!!!printf (" !!SUCCESS\n");
                }
                else
                    free_block0 = 0;
                bytes_removed+=n;
                pos = (block_number + 1) * block_size;
                block_number++;
                count0++;
            }
            if(free_block0)
            {
                //!!!printf ("!FREE BLOCK0 %ld", index0);
                if(FreeBlockIndex(index0) < 0)
                    return -1;
                //!!!printf (" !SUCCESS\n"); // index0);
                inode->di_addr[10] = -1;
            }
        }
    }
    if(pos >= 10 * block_size + addr_in_block*block_size && pos < 10 * block_size + addr_in_block*block_size + addr_in_block*addr_in_block*block_size)
    {
        if(inode->di_addr[11] < 0)
        {
            return -1;
        }
        long index0 = inode->di_addr[11]; //индекс блока с одинарными непрямыми адресами (адресами блоков с прямыми адресами)

        block_number = pos / block_size;
        long count0 = (block_number-10-addr_in_block)/addr_in_block;
        char free_block0 = count0 == 0? 1 : 0;

        while(count0 < addr_in_block && bytes_removed < size)
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
            long count1 = (block_number-10-addr_in_block) - count0 * addr_in_block;
            char free_block1 = count1 == 0? 1 : 0;
            free_block0*=free_block1;

            while(count1 < addr_in_block && bytes_removed < size)
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

                long n = (size - bytes_removed) < block_size - offs ? (size - bytes_removed) : block_size - offs;
                if(pos + n > inode->di_size)
                    n = inode->di_size - pos;
                //!!!printf("!!Removing: pos = %ld, block = %ld, n = %ld, offs = %ld\n", pos, index2, n, offs);
                if(offs == 0)
                {
                    printf ("!!FREE BLOCK2 = %ld, count1 = %ld ", index2, count1);
                    if(FreeBlockIndex(index2) < 0)
                        return -1;
                    printf (" !!SUCCESS\n"); // index2);
                }
                else
                {
                    free_block1 = 0;
                    free_block0 = 0;
                }
                bytes_removed+=n;
                pos = (block_number + 1) * block_size;
                block_number++;
                count1++;
            }
            if(free_block1)
            {
                printf ("!!FREE BLOCK1 %ld", index1);
                if(FreeBlockIndex(index1) < 0)
                    return -1;
                printf (" !!SUCCESS\n"); // index1);
            }
            count0++;
        }
        if(free_block0)
        {
            printf ("!!FREE BLOCK0 %ld", index0);
            if(FreeBlockIndex(index0) < 0)
                return -1;
            printf (" !!SUCCESS\n"); // index0);
            inode->di_addr[11] = -1;
        }
    }
    if(pos >= 10 * block_size + addr_in_block*block_size + addr_in_block*addr_in_block*block_size && pos < 10 * block_size + addr_in_block*block_size + addr_in_block*addr_in_block*block_size + addr_in_block*addr_in_block*addr_in_block*block_size)
    {
        if(inode->di_addr[12] < 0)
        {
            return -1;
        }
        long index0 = inode->di_addr[12]; //индекс блока с двойными непрямыми адресами (адресами блоков с одинарными непрямыми адресами)

        block_number = pos / block_size;
        long count0 = (block_number-10-addr_in_block-addr_in_block*addr_in_block)/addr_in_block/addr_in_block;
        char free_block0 = count0 == 0? 1 : 0;


        while(count0 < addr_in_block && bytes_removed < size)
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
            long count1 = (block_number-10-addr_in_block-addr_in_block*addr_in_block)/addr_in_block - count0 * addr_in_block;
            char free_block1 = count1 == 0? 1 : 0;

            while(count1 < addr_in_block && bytes_removed < size)
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

                long count2 = (block_number-10-addr_in_block-addr_in_block*addr_in_block) - count0 * addr_in_block * addr_in_block - count1 * addr_in_block;
                char free_block2 = count2 == 0? 1 : 0;
                free_block1*=free_block2;
                printf("COUNT0 = %ld, COUNT2 = %ld, pos = %ld\n", count0, count2, pos);
                while(count2 < addr_in_block && bytes_removed < size)
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

                    long n = (size - bytes_removed) < block_size - offs ? (size - bytes_removed) : block_size - offs;
                    if(pos + n > inode->di_size)
                        n = inode->di_size - pos;
                    //!!!printf("!!!Removing: pos = %ld, block = %ld, n = %ld, offs = %ld, bytes_removed = %ld\n", pos, index3, n, offs, bytes_removed);
                    if(offs == 0)
                    {
                        printf ("!!!FREE BLOCK3 %ld", index3);
                        if(FreeBlockIndex(index3) < 0)
                            return -1;
                        printf (" !!!SUCCESS\n"); // index3);
                    }
                    else
                    {
                        free_block2 = 0;
                        free_block1 = 0;
                        free_block0 = 0;
                    }
                    bytes_removed+=n;
                    pos = (block_number + 1) * block_size;
                    block_number++;
                    count2++;
                }
                count1++;
                if(free_block2)
                {
                    printf ("!!!FREE BLOCK2 %ld", index2);
                    if(FreeBlockIndex(index2) < 0)
                        return -1;
                    printf (" !!!SUCCESS\n"); // index2);
                }
                else
                    printf ("!!!NOT FREE BLOCK2 %ld\n", index2);
            }
            free_block0*=free_block1;
            count0++;
            if(free_block1)
            {
                printf ("!!!FREE BLOCK1 %ld", index1);
                if(FreeBlockIndex(index1) < 0)
                    return -1;
                printf (" !!!SUCCESS\n"); // index1);
            }
            else
                printf ("!!!NOT FREE BLOCK1 %ld\n", index1);
        }
        if(free_block0)
        {
            printf ("!!!FREE BLOCK0 %ld", index0);
            if(FreeBlockIndex(index0) < 0)
                return -1;
            printf (" !!!SUCCESS\n"); // index0);
            inode->di_addr[12] = -1;
        }
        else
            printf ("!!!NOT FREE BLOCK0 %ld\n", index0);
    }
    inode->di_size = inode->di_size - bytes_removed;
    //!!!printf("inode->di_size = %ld\n",inode->di_size);
    inode->di_atime = time(NULL);
    inode->di_mtime = time(NULL);
    return 0;
}

long GetInodeIndexByPath(const char *path)
{
    int pos = 0;
    long index = -1;
    long start = 0;
    if(path[pos] == '/')
    {
        index = 0;
        start = pos+1;
        pos++;
    }
    else
        return -ENOENT;
    while(pos < strlen(path))
    {
        while(pos < strlen(path) && path[pos] != '/')
            pos++;
        char name[pos - start +1];
        strncpy(name, path+start, pos - start);
        name[pos - start] = '\0';
        start = pos+1;
        pos++;
        struct dinode n = ReadInode(index);
        if(n.di_size < 0)
            return -ENOENT;
        int count = n.di_size/sizeof(struct dirent);
        struct dirent items[count];
        if(ReadFile(&n, (void *)items, 0, n.di_size) < 0)
            return -EIO;
        int i;
        index = -ENOENT;
        for (i = 0; i<count; i++)
        {
            if(strcmp(items[i].d_name, name) == 0)
            {
                index = items[i].d_ino;
                break;
            }
        }
        if(index < 0)
            return -ENOENT;
    }
    return index;
}

/*long GetInodeIndexByPath(const char *path)
{
    int pos = 0;
    long index = -1;
    if(path[pos] == '/')
        index = 0;

    int l = strlen(path)+1;
    char str[l];
    strncpy(str, path, l);
    char *pch;
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
            //printf ("Searching inode index by path: !%s! = !%s!\n",items[i].d_name, pch);
            char message[500];
            //sprintf(message, "Searching inode index by path: !%s! = !%s!\n",items[i].d_name, pch);
            //WriteToLog(message);
            if(strcmp(items[i].d_name, pch) == 0)
            {
                index = items[i].d_ino;
                break;
            }
        }
        pch = strtok (NULL, "/");
    }
    return index;
}*/

long RemoveHardLink(struct dinode *parent, struct dinode *child, long index)
{
    struct dirent parent_items[(parent->di_size)/sizeof(struct dirent)];
    ReadFile(parent, parent_items, 0, (parent->di_size));
    long offs = -1;
    long i;
    for(i = 0; i < (parent->di_size)/sizeof(struct dirent); i++)
    {
        if(parent_items[i].d_ino == index)
        {
            offs = parent_items[i].d_off;
            break;
        }
    }

    if(offs >= 0)
    {
        struct dirent item;
        if(ReadFile(parent, &item, (parent->di_size)-sizeof(item), sizeof(item)) < 0)
            return -EIO;
        item.d_off = offs;
        if(WriteFile(parent, &item, offs, sizeof(item)) < 0)
            return -EIO;
        if(TruncFile(parent, (parent->di_size)-sizeof(item)) < 0)
            return -EIO;
    }
    (child->di_nlink)--;
    parent->di_atime = time(NULL);
    parent->di_mtime = time(NULL);
    return 0;
}

long AddHardLink(struct dinode *parent, struct dinode *child, long index, const char *name)
{
    struct dirent parent_item;
    parent_item.d_ino = index;       /* inode number */
    parent_item.d_off = parent->di_size;       /* offset to this dirent */
    parent_item.d_reclen = sizeof(parent_item);    /* length of this record */
    parent_item.d_type = -1;      /* type of file; not supported by all file system types */
    memset(parent_item.d_name, '\0', sizeof(parent_item.d_name));
    strcpy(parent_item.d_name, name);

    if(WriteFile(parent, (void *)&parent_item, parent->di_size, sizeof(parent_item)) < 0)
    {
        return -EIO;
    }
    (child->di_nlink)++;
    parent->di_atime = time(NULL);
    parent->di_mtime = time(NULL);
    return 0;
}

long CreateDirectory(const char *path, mode_t mode)
{
    if(GetInodeIndexByPath(path) >= 0)
    {
        /*//WriteToLog("ALREADY EXIST");
        //WriteToLog(path);*/
        return -EEXIST; //файл с таким именем уже существует
    }
    int l = strlen(path);
    while(path[l] != '/' && l >= 0)
        l--;
    if(l<0)
        return -ENOENT;

    char parent_path[l+2];
    char name[strlen(path) - l];
    if(l==0)
    {
        strncpy(parent_path, "/", 1);
        parent_path[1]='\0';
    }
    else
    {
        strncpy(parent_path, path, l);
        parent_path[l]='\0';
    }
    strncpy(name, path+l+1, strlen(path) - l-1);
    name[strlen(path) - l-1]='\0';

    ////WriteToLog(name);

    //!!!printf("parent = %s, name = %s\n", parent_path, name);
    //name = p+1;
    long parent_index = GetInodeIndexByPath(parent_path);
    //!!!printf("parent_index = %ld\n", parent_index);
    struct dinode parent = ReadInode(parent_index);
    if(parent.di_size < 0)
        return -ENOENT;
    if(!(parent.di_mode & S_IFDIR))
        return -ENOENT;
    long index = GetNewInodeIndex();
    if(index < 0)
        return -ENFILE;
    struct dinode n;

    n.di_mode = mode;
    n.di_nlink = 1;
    n.di_uid = 0;      /* owner's user id         */
    n.di_gid = 0;      /* owner's group id        */
    n.di_size = 0;     /* number of bytes in file */
    n.di_gen = 0;      /* file generation number  */
    n.di_atime = time(NULL);    /* time last accessed      */
    n.di_mtime = time(NULL);    /* time last modified      */
    n.di_ctime = time(NULL);    /* time created            */
    parent.di_atime = time(NULL);
    parent.di_mtime = time(NULL);
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
        items[i].d_off = n.di_size + i * sizeof(struct dirent);       /* offset to this dirent */
        items[i].d_reclen = sizeof(items);    /* length of this record */
        items[i].d_type = -i;      /* type of file; not supported by all file system types */
    }

    AddHardLink(&parent, &n, index, name);
    (parent.di_nlink)++;

    if(WriteFile(&n, items, 0, sizeof(items)) < 0)
    {
        return -EIO;
    }
    if(WriteInode(index, n) < 0)
    {
        return -EIO;
    }
    if(WriteInode(parent_index, parent) < 0)
    {
        return -EIO;
    }
    return 0;
}

long GetParentIndex(const char *path)
{
    int l = strlen(path);
    while(path[l] != '/' && l >= 0)
        l--;
    if(l<0)
        return -1;

    char parent_path[l+2];
    if(l==0)
    {
        strncpy(parent_path, "/", 1);
        parent_path[1]='\0';
    }
    else
    {
        strncpy(parent_path, path, l);
        parent_path[l]='\0';
    }
    printf("parent_path = %s\n", parent_path);

    return GetInodeIndexByPath(parent_path);
}

long RemoveFile(struct dinode *file, long index, struct dinode *parent)
{
    if(RemoveHardLink(parent, file, index) < 0)
        return -EIO;
    if(FreeInodeIndex(index) < 0)
        return -EIO;
    if(TruncFile(file, 0) < 0)
        return -EIO;
    return 0;
}

long RemoveDirectory(struct dinode *dir, long index, struct dinode *parent)
{
    /*if(dir->di_nlink > 1)
        return 0;*/
    struct dirent items[(dir->di_size)/sizeof(struct dirent)];
    ReadFile(dir, items, 0, (dir->di_size));
    long i;
    long count = (dir->di_size)/sizeof(struct dirent);
    for(i = 0; i < count; i++)
    {
        if(!(strcmp(items[i].d_name, "..")==0) && !(strcmp(items[i].d_name, ".")==0))
        {
            struct dinode child = ReadInode(items[i].d_ino);
            if(child.di_size < 0)
                return -EIO;
            if(child.di_mode & S_IFDIR)
            {
                if(RemoveDirectory(&child, items[i].d_ino, dir)<0)
                    return -EIO;
            }
            else
            {
                if(RemoveFile(&child, items[i].d_ino, dir)<0)
                    return -EIO;
            }
        }
    }
    if(RemoveHardLink(parent, dir, index) < 0)
        return -EIO;
    (parent->di_nlink)--;

    if(TruncFile(dir, 0) < 0)
        return -EIO;
    if(FreeInodeIndex(index) < 0)
        return -EIO;
    return 0;
}

long RemoveByPath(const char *path)
{
    long parent_index = GetParentIndex(path);
    struct dinode parent = ReadInode(parent_index);
    if(parent.di_size < 0)
        return -1;

    long index = GetInodeIndexByPath(path);
    if(index < 0)
        return -1;
    struct dinode n = ReadInode(index);
    if(n.di_size < 0)
        return -1;

    if(n.di_mode & S_IFDIR)
    {
        if(RemoveDirectory(&n, index, &parent) < 0)
            return -EIO;
    }
    else
    {
        if(RemoveFile(&n, index, &parent)<0)
            return -EIO;
    }

    if(WriteInode(parent_index, parent) < 0)
        return -1;
    return 0;
}

long Rename(const char *path, const char *newpath)
{
    int l = strlen(newpath);
    while(newpath[l] != '/' && l >= 0)
        l--;
    if(l<0)
        return -ENOENT;

    char name[strlen(newpath) - l];
    strncpy(name, newpath+l+1, strlen(newpath) - l-1);
    name[strlen(newpath) - l-1]='\0';

    /*if(l > strlen(path) && strncmp(path, newpath, strlen(path)) == 0)
        if(newpath[strlen(path)] == '/')
            return -EPERM;*/
    long index = GetInodeIndexByPath(path);
    struct dinode n = ReadInode(index);
    if(n.di_size < 0)
        return -ENOENT;

    long parent_index = GetParentIndex(path);
    struct dinode parent = ReadInode(parent_index);
    if(parent.di_size < 0)
        return -ENOENT;

    long new_parent_index = GetParentIndex(newpath);
    struct dinode *new_parent;
    if(parent_index == new_parent_index)
    {
        new_parent = &parent;
    }
    else
    {
        struct dinode tmp = ReadInode(new_parent_index);
        new_parent = &tmp;
    }
    if(new_parent->di_size < 0)
        return -ENOENT;

    if(RemoveHardLink(&parent, &n, index) < 0)
        return -EIO;
    if(AddHardLink(new_parent, &n, index, name) < 0)
        return -EIO;
    if(n.di_mode & S_IFDIR)
    {
        (parent.di_nlink)--;
        (new_parent->di_nlink)++;
        struct dirent item;
        item.d_ino = new_parent_index;
        item.d_off = 0;
        item.d_reclen = sizeof(item);
        strcpy(item.d_name, "..");

        if(WriteFile(&n, (void *)&item, 0, sizeof(item)) < 0)
        {
            return -EIO;
        }
    }

    if(WriteInode(new_parent_index, *new_parent) < 0)
    {
        return -EIO;
    }

    if(WriteInode(parent_index, parent) < 0)
    {
        return -EIO;
    }

    n.di_atime = time(NULL);
    n.di_mtime = time(NULL);

    if(WriteInode(index, n) < 0)
    {
        return -EIO;
    }
    return 0;
}

long CreateFile(const char *path, mode_t mode)
{
    if(GetInodeIndexByPath(path) >= 0)
    {
        /*//WriteToLog("ALREADY EXIST");
        //WriteToLog(path);*/
        return -EEXIST; //файл с таким именем уже существует
    }
    int l = strlen(path);
    while(path[l] != '/' && l >= 0)
        l--;
    if(l<0)
        return -ENOENT;

    char parent_path[l+2];
    char name[strlen(path) - l];
    if(l==0)
    {
        strncpy(parent_path, "/", 1);
        parent_path[1]='\0';
    }
    else
    {
        strncpy(parent_path, path, l);
        parent_path[l]='\0';
    }
    strncpy(name, path+l+1, strlen(path) - l-1);
    name[strlen(path) - l-1]='\0';

    long parent_index = GetInodeIndexByPath(parent_path);

    struct dinode parent = ReadInode(parent_index);
    if(!(parent.di_mode & S_IFDIR))
        return -ENOTDIR;
    if(parent.di_size < 0)
        return -ENOENT;
    long index = GetNewInodeIndex();
    if(index < 0)
        return -ENFILE;

    struct dinode n;
    n.di_mode = mode;
    n.di_nlink = 0;
    n.di_uid = 0;      /* owner's user id         */
    n.di_gid = 0;      /* owner's group id        */
    n.di_size = 0;     /* number of bytes in file */
    n.di_gen = 0;      /* file generation number  */
    n.di_atime = time(NULL);    /* time last accessed      */
    n.di_mtime = time(NULL);    /* time last modified      */
    n.di_ctime = time(NULL);    /* time created            */

    int i;
    for(i = 0; i<sizeof(n.di_addr)/sizeof(long); i++)
    {
        n.di_addr[i] = -1;
    }

    parent.di_atime = time(NULL);
    parent.di_mtime = time(NULL);

    AddHardLink(&parent, &n, index, name);
    if(WriteInode(index, n) < 0)
    {
        return -EIO;
    }
    if(WriteInode(parent_index, parent) < 0)
    {
        return -EIO;
    }
    return 0;
}

