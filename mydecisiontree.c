#include <stdio.h>
#include <stdlib.h>
#include "mydecisiontree.h"

Data gData;
int  gMaxLevel;

static void _establish_decisiontree(int *s, int n, int l);
static int _is_purity(int *s, int n);

/* func: 建立样本
 * argu: 样本元素、样本类型、样本个数、属性个数、类别个数
 * retu: 空间分配是否成功 0：成功 else:失败
 * date: 2019-12-1
 */
int set_gData(float *f, int *type, int n, int d, int t)
{
  gData.N = n;
  gData.T = t;
  gData.e = (Element *)malloc(n * sizeof(Element));
  if(!gData.e) return -1;
  
  for(int i = 0; i < n; ++i){
    gData.e[i].D = d;
    gData.e[i].type = type[i];
    gData.e[i].f = (float *)malloc(d * sizeof(float));
    if(!gData.e[i].f) return -1;
    
    for(int j = 0; j < d; ++j){
      gData.e[i].f[j] = *(f + i * d + j);
    }
  }
  
  return 0;
}

/* func: 释放样本空间
 * argu: 无
 * retu: 无
 * date: 2019-12-1
 */
void free_gData(void)
{
  for(int i = 0; i < gData.N; ++i){
    free(gData.e[i].f);
  }
  free(gData.e);
}

/* func: 显示所有样本
 * argu: 无
 * retu: 无
 * date: 2019-12-1
 */
void disp_gData(void)
{
  printf("sample set -->>\n");
  for(int i = 0; i < gData.N; ++i){
    printf("[%d]\t", i + 1);
    for(int j = 0; j < gData.e[i].D; ++j){
      printf("%.2f\t", gData.e[i].f[j]);
    }
    printf("%d\n", gData.e[i].type);
  }
}

/* func: 建立决策树，用户调用接口
 * argu: 决策树最大层数
 * retu: 无
 * date: 2019-12-1
 */
void establish_decisiontree(int l)
{
  gMaxLevel = l;
  Sample s;

  s.n = gData.N;
  s.s = (int *)malloc(s.n * sizeof(int));

  for(int i = 0; i < s.n; ++i){
    s.s[i] = i + 1;
  }

  _establish_decisiontree(s.s, s.n, 0);

  free(s.s);
}

/* func: 建立决策树递归函数
 * argu: 样本列表，样本个数，决策树最大层数
 * retu: 无
 * date: 2019-12-1
 */
static void _establish_decisiontree(int *s, int n, int l)
{
  if(l >= gMaxLevel) return;
  
  if(_is_purity(s, n)) return;


}

/* func: 判断结点纯度
 * argu: 样本列表，样本个数
 * retu: 结点集合是否纯 1：纯 else：不纯
 * date: 2019-12-1
 */
static int _is_purity(int *s, int n)
{
  return 0;
}
