#!/bin/sh

makegen
./scripts/compile_cuda.sh
screenshot()
{
  vglrun ./bin/Voxelizer.a -r 16  -a $3 -m $1 -s screenshots/$2_$3_16.png
  vglrun ./bin/Voxelizer.a -r 32  -a $3 -m $1 -s screenshots/$2_$3_32.png
  vglrun ./bin/Voxelizer.a -r 64  -a $3 -m $1 -s screenshots/$2_$3_64.png
  vglrun ./bin/Voxelizer.a -r 128 -a $3 -m $1 -s screenshots/$2_$3_128.png
  vglrun ./bin/Voxelizer.a -r 256 -a $3 -m $1 -s screenshots/$2_$3_256.png
  vglrun ./bin/Voxelizer.a -r 512 -a $3 -m $1 -s screenshots/$2_$3_512.png
}

arg1=$1
arg2=$2
[ -z $arg1 ] && arg1=bunny
[ -z $arg2 ] && arg2=$arg1

screenshot $arg1 $arg2 rlv
screenshot $arg1 $arg2 ilv 
screenshot $arg1 $arg2 bre 
