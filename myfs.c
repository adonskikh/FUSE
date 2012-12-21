#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include "fsfileops.h"

static const char *hello_str = "Hello World!\n";
static const char *hello_path = "/hello";
long last_index;
struct dinode last_inode;


/** Get file attributes.
 *
 * Similar to stat().  The 'st_dev' and 'st_blksize' fields are
 * ignored.  The 'st_ino' field is ignored except if the 'use_ino'
 * mount option is given.
 */
static int my_getattr(const char *path, struct stat *stbuf)
{
    int res = 0;
    //WriteToLog("getattr: ");
    //WriteToLog(path);
    memset(stbuf, 0, sizeof(struct stat));

    long index = GetInodeIndexByPath(path);
    struct dinode n = ReadInode(index);
    if(n.di_size < 0)
        return -ENOENT;

    stbuf->st_ino = index;     /* inode number */
    /*if(!(n.di_mode & S_IFDIR)&& !(n.di_mode & S_IFREG))
    {
        return -ENOENT;
    }*/
    stbuf->st_mode = n.di_mode;    /* protection */
    stbuf->st_nlink = n.di_nlink;   /* number of hard links */
    stbuf->st_uid = n.di_uid;     /* user ID of owner */
    stbuf->st_gid = n.di_gid;     /* group ID of owner */
    stbuf->st_size = n.di_size;    /* total size, in bytes */
    stbuf->st_atime = n.di_atime;   /* time of last access */
    stbuf->st_mtime = n.di_mtime;   /* time of last modification */
    stbuf->st_ctime = n.di_ctime;   /* time of last status change */
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

    if(n.di_mode & S_IFDIR)
    {
        struct dirent items[n.di_size/sizeof(struct dirent)];
        ReadFile(&n, items, 0, n.di_size);
        long i;
        for(i = 0; i < n.di_size/sizeof(struct dirent); i++)
        {
            struct stat stbuf;
            struct dinode item = ReadInode(items[i].d_ino);
            if(item.di_size < 0)
                return -ENOENT;
            stbuf.st_ino = index;     /* inode number */
            stbuf.st_mode = item.di_mode;    /* protection */
            stbuf.st_nlink = item.di_nlink;   /* number of hard links */
            stbuf.st_uid = item.di_uid;     /* user ID of owner */
            stbuf.st_gid = item.di_gid;     /* group ID of owner */
            stbuf.st_size = item.di_size;    /* total size, in bytes */
            stbuf.st_atime = item.di_atime;   /* time of last access */
            stbuf.st_mtime = item.di_mtime;   /* time of last modification */
            stbuf.st_ctime = item.di_ctime;   /* time of last status change */
            filler(buf, items[i].d_name, &stbuf, 0); //TODO: stat вместо NULL
        }
    }
    else
        return -ENOENT;

    return 0;
}

/** Create a file node
 *
 * There is no create() operation, mknod() will be called for
 * creation of all non-directory, non-symlink nodes.
 */
int my_mknod(const char *path, mode_t mode, dev_t dev)
{
    if (S_ISREG(mode))
        return CreateFile(path, mode | S_IFREG);
    else
    if (S_ISFIFO(mode))
        return CreateFile(path, mode | S_IFIFO);

    return -EINVAL;
}

/** Create a directory */
int my_mkdir(const char *path, mode_t mode)
{
    //WriteToLog("mkdir: ");
    //WriteToLog(path);

    return(CreateDirectory(path, mode | S_IFDIR));
}

/* указатель на эту функцию будет передан модулю ядра FUSE в качестве
поля open структуры типа   fuse_operations  - эта функция определяет
имеет ли право пользователь открыть файл /hello нашей файловой системы -
путём анализа данных структуры типа fuse_file_info (читайте о ней ниже)*/
static int my_open(const char *path, struct fuse_file_info *fi)
{
    fi->fh = GetInodeIndexByPath(path);
    if((fi->fh) < 0)
        return -ENOENT;
    /*if((fi->flags & 3) != O_RDONLY)
        return -EACCES;*/

    return 0;
}

/** Open directory
 *
 * This method should check if the open operation is permitted for
 * this  directory
 *
 * Introduced in version 2.3
 */
int my_opendir(const char *path, struct fuse_file_info *fi)
{
    long index = GetInodeIndexByPath(path);
    struct dinode n = ReadInode(index);
    if(n.di_size < 0)
        return -ENOENT;

    /*char message[50];
    sprintf(message, "opendir flags: %o", fi->flags);
    WriteToLog(message);*/

    /*if((fi->flags & 3) != O_RDONLY)
        return -EACCES;*/

    return 0;
}

/*определяет, как именно будет считываться информация из файла для передачи пользователю*/
/*static int my_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    //WriteToLog("read: ");
    //WriteToLog(path);
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
    /*if(!(n.di_mode & 0444))
        return -EACCES;*/

    /*return size;
}*/

/** Read data from an open file
 *
 * Read should return exactly the number of bytes requested except
 * on EOF or error, otherwise the rest of the data will be
 * substituted with zeroes.  An exception to this is when the
 * 'direct_io' mount option is specified, in which case the return
 * value of the read system call will reflect the return value of
 * this operation.
 *
 * Changed in version 2.2
 */
// I don't fully understand the documentation above -- it doesn't
// match the documentation for the read() system call which says it
// can return with anything up to the amount of data requested. nor
// with the fusexmp code which returns the amount of data also
// returned by read.
int my_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    long index = fi->fh;//GetInodeIndexByPath(path);
    struct dinode n;
    if(last_index == index)
    {
        n = last_inode;
    }
    else
        n = ReadInode(index);
    /*char message[500];
    sprintf(message, "read: index = %ld, path = %s, size = %ld, offset = %ld, real size = %o", index, path, size, (long)offset, n.di_size);
    WriteToLog(message);*/
    if(n.di_size < 0)
        return -ENOENT;
    if(ReadFile(&n, buf, (long)offset, size) < 0)
        return -EIO;
    return size;
}

/** Write data to an open file
 *
 * Write should return exactly the number of bytes requested
 * except on error.  An exception to this is when the 'direct_io'
 * mount option is specified (see read operation).
 *
 * Changed in version 2.2
 */
int my_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    long index = fi->fh;//GetInodeIndexByPath(path);
    struct dinode n;
    if(last_index == index)
    {
        n = last_inode;
    }
    else
        n = ReadInode(index);
    if(n.di_size < 0)
        return -ENOENT;
    /*char message[500];
    sprintf(message, "write: index = %ld, path = %s, size = %ld, offset = %ld, real size = %ld, fi->fh = %ld", index, path, size, (long)offset, n.di_size, fi->fh);
    WriteToLog(message);*/
    if(WriteFile(&n, (void *)buf, (long)offset, size) < 0)
    {
        WriteInode(index, n);
        return -EIO;
    }
    if(WriteInode(index, n) < 0)
        return -EIO;
    WriteToLog("SUCCESS");
    return size;
}

/** Remove a directory */
int my_rmdir(const char *path)
{
    return RemoveByPath(path);
}

/** Remove a file */
int my_unlink(const char *path)
{
    return RemoveByPath(path);
}

/** Get file system statistics
 *
 * The 'f_frsize', 'f_favail', 'f_fsid' and 'f_flag' fields are ignored
 *
 * Replaced 'struct statfs' parameter with 'struct statvfs' in
 * version 2.5
 */
int my_statfs(const char *path, struct statvfs *statv)
{
    int retstat = 0;

    statv->f_bsize = ReadBlockSize();
    statv->f_bsize = ReadBlockSize();
    statv->f_blocks = ReadMaxBlocksCount();
    statv->f_bfree = ReadFreeBlocksCount();
    statv->f_bavail = statv->f_bfree;
    statv->f_files = ReadMaxInodesCount();
    statv->f_ffree = ReadFreeInodesCount();
    statv->f_favail = statv->f_ffree;

    return retstat;
}

/** Change the access and/or modification times of a file */
int my_utime(const char *path, struct utimbuf *ubuf)
{
    long index = GetInodeIndexByPath(path);
    struct dinode n = ReadInode(index);
    if(n.di_size < 0)
        return -ENOENT;
    n.di_atime = ubuf->actime;
    n.di_mtime = ubuf->modtime;
    if(WriteInode(index, n) < 0)
        return -EIO;
    return 0;
}

/** Rename a file */
// both path and newpath are fs-relative
int my_rename(const char *path, const char *newpath)
{
    return Rename(path, newpath);
}

/** Change the size of a file */
int my_truncate(const char *path, off_t newsize)
{
    long index = GetInodeIndexByPath(path);
    struct dinode n = ReadInode(index);
    if(n.di_size < 0)
        return -ENOENT;
    if(TruncFile(&n, (long)newsize) < 0)
    {
        WriteInode(index, n);
        return -EIO;
    }
    if((long)newsize == 0)
        return FreeInodeIndex(index);
    return(WriteInode(index, n));
}

static struct fuse_operations my_operations;  /* в этой структуре будут храниться ссылки на функции, которые  реализуют операции, определённые в рамках нашей файловой системы */

int main(int argc, char *argv[])
{
    //Create();
    if(Load(FILE_PATH) < 0)
        printf("AAAAAAAAAAAAAAAAAAAAAAAAAa\n");
    last_index = -1;
    //WriteToFile();
    // начало заполнения полей структуры
    my_operations.getattr = my_getattr;
    my_operations.readdir = my_readdir;
    my_operations.open = my_open;
    my_operations.opendir = my_opendir;
    my_operations.read = my_read;
    my_operations.mkdir = my_mkdir;
    my_operations.rmdir = my_rmdir;
    my_operations.statfs = my_statfs;
    my_operations.utime = my_utime;
    my_operations.rename = my_rename;
    my_operations.mknod = my_mknod;
    my_operations.unlink = my_unlink;
    my_operations.truncate = my_truncate;
    my_operations.read = my_read;
    my_operations.write = my_write;
    // окончание заполнения полей структуры
    return fuse_main(argc, argv, &my_operations, 0); // передаём структуру с инф. об операциях модулю FUSE
}
