#include <stdio.h>
#include <cstdio>
#include <string.h>
#include <cstdlib>
#define NUM 512


class Potato{
 public:
  int num_hops;
  int count;
  int path[NUM];
 Potato(): num_hops(0), count(0) {
   memset(path, -1, sizeof(path));
 }
};
