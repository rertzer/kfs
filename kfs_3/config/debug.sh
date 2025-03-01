#!/bin/sh

sleep 2 && gdb jros.bin -ex 'target remote localhost:1234' 
