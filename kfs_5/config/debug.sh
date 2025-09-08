#!/bin/sh

cd /home/rertzer/Projects/kfs/kfs_github/kfs_4/
sleep 2 && gdb ./jros.bin -ex 'target remote localhost:1234' 
