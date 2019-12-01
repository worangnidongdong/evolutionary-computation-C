#include <stdio.h>
#include <stdlib.h>
#include "mydecision.h"

Data gData;
int  gMaxLevel;

int set_gData(float *f, int *type, int n, int d, int t)
{
  gData.N = n;
  gData.T = t;
  gData.e = (Element *)malloc(n * sizeof(Element));
  if(!gData.e) return -1;
  
  for(int i = 0; i < n; ++i){
    gData.D = d;
    gData.type = type[i];
    gData.e[i]->f = (float *)malloc(d * sizeof(float));
    if(!gData.e[i]->f) return -1;
    
    for(int j = 0; j < d; ++j){
      gData.e[i]->f[j] = f[i][j];
    }
  }
  
  return 0;
}

void free_gData(void)
{
  for(int i = 0; i < gData.N; ++i){
    free(gData.e[i]->f);
  }
  free(gData.e);
}

void disp_gData(void)
{
  printf("样本-->>:\n");
  for(int i = 0; i < gData.N; ++i){
    printf("[%d]\t", i);
    for(int j = 0; j < gData.e[i]->D; ++j){
      printf("%.2f\t", gData.e[i]->f[j]);
    }
    printf("\n");
  }
}

void _establish_decisiontree(int *s, int n, int l)
{
  if(l >= gMaxLevel) return;
  
}

void establish_decisiontree(int l)
{
  gMaxLevel = l;
  Sample s;
  s.N = gData.N;
  s.s = (int *)malloc(s.N * sizeof(int));
  _establish_decisiontree(0);
  free(s.s);
}

