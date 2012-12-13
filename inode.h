/* Inode structure as it appears on an inode table. */
struct dinode
{ ushort  di_mode;     /* mode and type of file   */
  short   di_nlink;    /* number of links to file */
  ushort  di_uid;      /* owner's user id         */
  ushort  di_gid;      /* owner's group id        */
  off_t   di_size;     /* number of bytes in file */
  long    di_addr[39]; /* disk block addresses    */
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
