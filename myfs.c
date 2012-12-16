#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include "fsfileops.h"

static const char *hello_str = "Hello World!\n";
static const char *hello_path = "/hello";

/*static const char *fsfilename = "file";

#define BLOCK_SIZE 4096;
#define MAX_FILES_COUNT 100;
#define INODES_AREA_SIZE MAX_FILES_COUNT * sizeof(struct dinode);
#define FREE_INODES_AREA_SIZE MAX_FILES_COUNT * sizeof(long);*/

void WriteToFile()
{
    struct dinode node;
    FILE *output;
    int i;
    int count = 5, range = 10;
    /*printf("count = ");
    scanf("%d", &count);
    printf("range = ");
    scanf("%d", &range);*/
    if((output=fopen(fsfilename,"wb"))==0)
    {
        puts ("Can't open output file.");
        exit(-1);
    }

    for (i=0; i<count; i++)
    {
        int n = rand()%range;
        fwrite(&n, sizeof(int), 1, output);
        //fprintf(output, "%d\n", (int)rand()%range);
    }
    fclose(output);
}
/* Далее мы определим действия функций,
которые требуются для FUSE  при описании файловой системы*/

/*указатель на эту функцию будет передан модулю ядра FUSE в качестве
поля getattr  структуры типа   fuse_operations  - эта функция определяет
метаинформацию о файле, путь к которому указан в переменной *path
метаиноформация возвращается в виде специальной структуры
 stat (читайте о ней ниже)
*/
static int my_getattr(const char *path, struct stat *stbuf)
{
    int res = 0;
    WriteToLog("getattr: ");
    WriteToLog(path);
    memset(stbuf, 0, sizeof(struct stat));

    long index = GetInodeIndexByPath(path);
    struct dinode n = ReadInode(index);
    if(n.di_size < 0)
        return -ENOENT;
    char message[50];
    sprintf(message, "Inode size: %ld", n.di_size);
    WriteToLog(message);
    if(n.di_size < 0)
        return -ENOENT;

    //dev_t     stbuf->st_dev;     /* ID of device containing file */
    stbuf->st_ino = index;     /* inode number */
    if(!(n.di_mode | S_IFDIR)&& !(n.di_mode | S_IFREG))
    {
        return -ENOENT;
    }
    stbuf->st_mode = n.di_mode;    /* protection */
    stbuf->st_nlink = n.di_nlink;   /* number of hard links */
    stbuf->st_uid = n.di_uid;     /* user ID of owner */
    stbuf->st_gid = n.di_gid;     /* group ID of owner */
    stbuf->st_rdev;    /* device ID (if special file) */
    stbuf->st_size = n.di_size;    /* total size, in bytes */
    //blksize_t stbuf->st_blksize; /* blocksize for file system I/O */
    //blkcnt_t  stbuf->st_blocks;  /* number of 512B blocks allocated */
    stbuf->st_atime = n.di_atime;   /* time of last access */
    stbuf->st_mtime = n.di_mtime;   /* time of last modification */
    stbuf->st_ctime = n.di_ctime;   /* time of last status change */
    //return res;




    /*memset(stbuf, 0, sizeof(struct stat));
    if(strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    }
    else if(strcmp(path, hello_path) == 0) {
        stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_size = strlen(hello_str);
    }
    else
        res = -ENOENT;*/
    return res;
}

/* указатель на эту функцию будет передан модулю ядра FUSE в качестве
поля readdir структуры типа   fuse_operations  - эта функция определяет
порядок чтения данных из директория*/
static int my_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    long index = GetInodeIndexByPath(path);
    struct dinode n = ReadInode(index);
    if(n.di_size < 0)
        return -ENOENT;

    if(n.di_mode | S_IFDIR)
    {
        struct dirent items[n.di_size/sizeof(struct dirent)];
        ReadFile(&n, items, 0, n.di_size);
        long i;
        for(i = 0; i < n.di_size/sizeof(struct dirent); i++)
        {
            filler(buf, items[i].d_name, NULL, 0); //TODO: stat вместо NULL
        }
    }
    else
        return -ENOENT;




    /*(void) offset;
    (void) fi;
    WriteToLog("readdir: ");
    WriteToLog(path);

    if(strcmp(path, "/") != 0)
        return -ENOENT;

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);
    filler(buf, hello_path + 1, NULL, 0);*/

    return 0;
}

/* указатель на эту функцию будет передан модулю ядра FUSE в качестве
поля open структуры типа   fuse_operations  - эта функция определяет
имеет ли право пользователь открыть файл /hello нашей файловой системы -
путём анализа данных структуры типа fuse_file_info (читайте о ней ниже)*/
static int my_open(const char *path, struct fuse_file_info *fi)
{
    WriteToLog("open: ");
    WriteToLog(path);
    if(strcmp(path, hello_path) != 0)
        return -ENOENT;

    if((fi->flags & 3) != O_RDONLY)
        return -EACCES;

    return 0;
}

/*определяет, как именно будет считываться информация из файла для передачи пользователю*/
static int my_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    WriteToLog("read: ");
    WriteToLog(path);
    size_t len;
    (void) fi;
    if(strcmp(path, hello_path) != 0)
        return -ENOENT;

    len = strlen(hello_str);
    if (offset < len) {
        if (offset + size > len)
            size = len - offset;
        memcpy(buf, hello_str + offset, size);
    } else
        size = 0;

    return size;
}

static struct fuse_operations my_operations;  /* в этой структуре будут храниться ссылки на функции, которые  реализуют операции, определённые в рамках нашей файловой системы */

int main(int argc, char *argv[])
{
    //Create();
    Load("/media/Study/Z/3 курс/Операционные системы/FUSE/file");
    //WriteToFile();
    // начало заполнения полей структуры
    my_operations.getattr = my_getattr;
    my_operations.readdir = my_readdir;
    my_operations.open = my_open;
    my_operations.read = my_read;
    // окончание заполнения полей структуры
    return fuse_main(argc, argv, &my_operations, 0); // передаём структуру с инф. об операциях модулю FUSE
}
