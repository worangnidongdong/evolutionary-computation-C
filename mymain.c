#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mydecisiontree.h"

void input_data(float *f, int *type, int n, int d, int t)
{
  for(int i = 0; i < n; ++i){
    for(int j = 0; j < d; ++j){
      scanf("%f", (f + i * d + j));
    }
    scanf("%d", (type + i));
  }
}

/* func: main
 * argu: cmdline
 * retu: code
 * date: 2019-12-1
 */
int main(int argc, char const *argv[])
{
  int n = 5, t = 2, d = 2;
  float *f = (float *)malloc(n * d * sizeof(float));
  int *type = (int *)malloc(n * sizeof(int));

  input_data(f, type, n, d, t);
  set_gData(f, type, n, d, t);
  disp_gData();
  free_gData();

  return 0;
}
