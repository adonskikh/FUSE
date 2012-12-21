#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

static const char *hello_str = "Hello World!\n";
static const char *hello_path = "/hello";
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

    memset(stbuf, 0, sizeof(struct stat));
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
        res = -ENOENT;

    return res;
}

/* указатель на эту функцию будет передан модулю ядра FUSE в качестве
поля readdir структуры типа   fuse_operations  - эта функция определяет
порядок чтения данных из директория*/
static int my_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi)
{
    (void) offset;
    (void) fi;

    if(strcmp(path, "/") != 0)
        return -ENOENT;

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);
    filler(buf, hello_path + 1, NULL, 0);

    return 0;
}

/* указатель на эту функцию будет передан модулю ядра FUSE в качестве
поля open структуры типа   fuse_operations  - эта функция определяет
имеет ли право пользователь открыть файл /hello нашей файловой системы -
путём анализа данных структуры типа fuse_file_info (читайте о ней ниже)*/
static int my_open(const char *path, struct fuse_file_info *fi)
{
    if(strcmp(path, hello_path) != 0)
        return -ENOENT;

    if((fi->flags & 3) != O_RDONLY)
        return -EACCES;

    return 0;
}

/*определяет, как именно будет считываться информация из файла для передачи пользователю*/
static int my_read(const char *path, char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi)
{
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
    // начало заполнения полей структуры
    my_operations.getattr = my_getattr;
    my_operations.readdir = my_readdir;
    my_operations.open = my_open;
    my_operations.read = my_read;
    // окончание заполнения полей структуры
    return fuse_main(argc, argv, &my_operations, 0); // передаём структуру с инф. об операциях модулю FUSE
}
