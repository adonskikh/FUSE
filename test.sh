#!/bin/bash
echo "MOUNTING FILE SYSTEM"
./myfs fs
directory="fs/teeeeest"
echo "Making directory $directory"
mkdir $directory
sleep 1
echo "UNMOUNTING FILE SYSTEM"
fusermount -u fs
echo "MOUNTING FILE SYSTEM"
./myfs fs
if [ -d $directory ]; then
    echo "SUCCESS: Directory was created."
else
    echo "ERROR: Directory does not exist."
fi
i=0
while [ $i -lt 10 ]
do
  #sleep 1
  addr="$directory/file$i.htm"
  touch $addr
  cp FUSE.htm $addr
  echo "File $addr was created." 
  i=$(( $i + 1 )) 
done
./stat
sleep 1
echo "UNMOUNTING FILE SYSTEM"
fusermount -u fs
echo "MOUNTING FILE SYSTEM"
./myfs fs
echo "Checking files:"
i=0
sleep 1
while [ $i -lt 10 ]
do
  #sleep 1
  addr="$directory/file$i.htm"
  echo "Checking $addr:"
  if ((diff FUSE.htm $addr) &> /dev/null) && ((diff $addr FUSE.htm) &> /dev/null); then
    echo "OK"
  else
    echo "FAIL"
  fi
diff FUSE.htm $addr
diff $addr FUSE.htm
  i=$(( $i + 1 )) 
done

echo "Removing directory $directory"
rmdir $directory
sleep 1
echo "UNMOUNTING FILE SYSTEM"
fusermount -u fs
echo "MOUNTING FILE SYSTEM"
./myfs fs
# проверяем удаление директории
if [ -d $directory ]; then
    echo "ERROR: Directory still exists."
else
    echo "SUCCESS: Directory was removed."
fi
./stat
echo "UNMOUNTING FILE SYSTEM"
sleep 1
fusermount -u fs

