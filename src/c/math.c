#include "header/math.h"


int div(int a, int b){
  return a/b;
}

int mod(int a, int n){
  return a-n*div(a, n);
}
