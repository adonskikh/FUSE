/* Inode structure as it appears on an inode table. */
struct dinode
{ ushort  di_mode;     /* mode and type of file   */
  short   di_nlink;    /* number of links to file */
  ushort  di_uid;      /* owner's user id         */
  ushort  di_gid;      /* owner's group id        */
  long   di_size;     /* number of bytes in file */
  long    di_addr[13]; /* disk block addresses    */
  long    di_gen;      /* file generation number  */
  time_t  di_atime;    /* time last accessed      */
  time_t  di_mtime;    /* time last modified      */
  time_t  di_ctime;    /* time created            */
};
/*
 * The 40 address bytes:
 *   39 used as 13 addresses of 3 bytes each.
 *   40th byte is used as a file generation number.
 */
struct dirent {
    long          d_ino;       /* inode number */
    long          d_off;       /* offset to the next dirent */
    unsigned short d_reclen;    /* length of this record */
    unsigned char  d_type;      /* type of file; not supported
                                   by all file system types */
    char           d_name[256]; /* filename */
  };
