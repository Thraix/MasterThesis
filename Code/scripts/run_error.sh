#!/bin/sh
makegen
./scripts/compile_cuda.sh

runX()
{
  echo "p$1p"
  printf "Running $3 "
  printf "rlv "
  vglrun ./bin/Voxelizer.a -r $3 -a rlv -e comp -c ilv -o metrics/$2/rlv_ilv_$3.txt -m $1 > /dev/null
  printf "ilv "
  vglrun ./bin/Voxelizer.a -r $3 -a rlv -e comp -c bre -o metrics/$2/rlv_bre_$3.txt -m $1 > /dev/null
  printf "bresenham"
  vglrun ./bin/Voxelizer.a -r $3 -a ilv -e comp -c bre -o metrics/$2/ilv_bre_$3.txt -m $1 > /dev/null
  printf "\n"
}

get_relative()
{
  tail $1 -n 1 | awk '{print $3}' | awk -F '%' '{print $1 "\\%"}'
}

get_jaccard()
{
  tail $1 -n 2 | head -n 1 | awk '{print $3}' | awk -F '%' '{print $1 "\\%"}'
}

run()
{
  mkdir metrics 2> /dev/null
  mkdir metrics/$2 2> /dev/null
  runX $1 $2 128
  runX $1 $2 256
  runX $1 $2 512
  runX $1 $2 1024
  runX $1 $2 2048


  printf "       & RLV/ILV & %s & %s & %s & %s & %s \\\\\\\\\n" \
    $(get_jaccard metrics/$2/rlv_ilv_128.txt) \
    $(get_jaccard metrics/$2/rlv_ilv_256.txt) \
    $(get_jaccard metrics/$2/rlv_ilv_512.txt) \
    $(get_jaccard metrics/$2/rlv_ilv_1024.txt) \
    $(get_jaccard metrics/$2/rlv_ilv_2048.txt)

  printf "$2 & RLV/Bre & %s & %s & %s & %s & %s \\\\\\\\\n" \
    $(get_jaccard metrics/$2/rlv_bre_128.txt) \
    $(get_jaccard metrics/$2/rlv_bre_256.txt) \
    $(get_jaccard metrics/$2/rlv_bre_512.txt) \
    $(get_jaccard metrics/$2/rlv_bre_1024.txt) \
    $(get_jaccard metrics/$2/rlv_bre_2048.txt)

  printf "       & ILV/Bre & %s & %s & %s & %s & %s \\\\\\\\\n" \
    $(get_jaccard metrics/$2/ilv_bre_128.txt) \
    $(get_jaccard metrics/$2/ilv_bre_256.txt) \
    $(get_jaccard metrics/$2/ilv_bre_512.txt) \
    $(get_jaccard metrics/$2/ilv_bre_1024.txt) \
    $(get_jaccard metrics/$2/ilv_bre_2048.txt)
  echo "\\hline"

}
arg1=$1
arg2=$2
[ -z $arg1 ] && arg1=bunny
[ -z $arg2 ] && arg2=$arg1
run $arg1 $arg2
