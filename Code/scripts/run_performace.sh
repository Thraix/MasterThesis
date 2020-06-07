#!/bin/sh
makegen
./scripts/compile_cuda.sh

runX()
{
  printf "Running $3 "
  printf "rlv "
  ./bin/Voxelizer.a -r $3 -a rlv -e perf -o metrics/$2/rlv_$3.txt -m $1 > /dev/null
  printf "ilv "
  ./bin/Voxelizer.a -r $3 -a ilv -e perf -o metrics/$2/ilv_$3.txt -m $1  > /dev/null
  printf "bresenham"
  ./bin/Voxelizer.a -r $3 -a bresenham -e perf -o metrics/$2/bresenham_$3.txt -m $1 > /dev/null
  printf "\n"
}

get_time()
{
  tail $1 -n 1 | awk '{print $8}'
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

  echo "\\\\addplot["
  echo "  color=blue,mark=o,"
  echo "  ]"
  echo "  coordinates {"
  printf "    (128,%s)(256,%s)(512,%s)(1024,%s)(2048,%s)\n" $(get_time metrics/$2/rlv_128.txt) $(get_time metrics/$2/rlv_256.txt) $(get_time metrics/$2/rlv_512.txt) $(get_time metrics/$2/rlv_1024.txt) $(get_time metrics/$2/rlv_2048.txt)
  echo "  };"
   
  echo "\\\\addplot["
  echo "  color=red,mark=square,"
  echo "  ]"
  echo "  coordinates {"
  printf "    (128,%s)(256,%s)(512,%s)(1024,%s)(2048,%s)\n" $(get_time metrics/$2/ilv_128.txt) $(get_time metrics/$2/ilv_256.txt) $(get_time metrics/$2/ilv_512.txt) $(get_time metrics/$2/ilv_1024.txt) $(get_time metrics/$2/ilv_2048.txt)
  echo "  };"

  echo "\\\\addplot["
  echo "  color=green,mark=triangle,"
  echo "  ]"
  echo "  coordinates {"
  printf "    (128,%s)(256,%s)(512,%s)(1024,%s)(2048,%s)\n" $(get_time metrics/$2/bresenham_128.txt) $(get_time metrics/$2/bresenham_256.txt) $(get_time metrics/$2/bresenham_512.txt) $(get_time metrics/$2/bresenham_1024.txt) $(get_time metrics/$2/bresenham_2048.txt)
  echo "  };"

  printf "       & RLV & %s & %s & %s & %s & %s \\\\\n" $(get_time metrics/$2/rlv_128.txt) $(get_time metrics/$2/rlv_256.txt) $(get_time metrics/$2/rlv_512.txt) $(get_time metrics/$2/rlv_1024.txt) $(get_time metrics/$2/rlv_2048.txt)
  printf "$2 & ILV & %s & %s & %s & %s & %s \\\\\n" $(get_time metrics/$2/ilv_128.txt) $(get_time metrics/$2/ilv_256.txt) $(get_time metrics/$2/ilv_512.txt) $(get_time metrics/$2/ilv_1024.txt) $(get_time metrics/$2/ilv_2048.txt)
  printf "       & Bresenham & %s & %s & %s & %s & %s \\\\\n" $(get_time metrics/$2/bresenham_128.txt) $(get_time metrics/$2/bresenham_256.txt) $(get_time metrics/$2/bresenham_512.txt) $(get_time metrics/$2/bresenham_1024.txt) $(get_time metrics/$2/bresenham_2048.txt)
  echo "\\hline"
}

run $1 $2 
