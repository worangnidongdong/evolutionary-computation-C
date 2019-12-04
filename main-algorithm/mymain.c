#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "mydecisiontree.h"

/* func: 平台初始化
 * argu: 0：随机初始化种子，结果不可复现；1：使用r初始化种子，结果可复现
 * retu: 无
 * date: 2019-11-29
 */
void platform_init(void)
{
  /* 随机化种子 */
  int r = 0;
  scanf("seed=%d,", &r);
  if(r)
    srand(r);
  else
    srand((unsigned int)time(NULL));
}

/* func: 输入样本
 * argu: 样本数组、样本类型、数组大小、属性大小、样本种类
 * retu: 无
 * date: 2019-12-3
 */
void input_data(float *f, int *type, int n, int d, int t)
{
  for(int i = 0; i < n; ++i){
    for(int j = 0; j < d; ++j){
      scanf("%f", (f + i * d + j));
    }
    scanf("%d", (type + i));
  }
}

/* func: 决策树测试
 * argu: 无
 * retu: 无
 * date: 2019-12-3
 */
void decision_tree_test(void)
{
  /* 样本数量、样本种类、属性个数、决策树最大层数 */
  int n = 6, t = 2, d = 2, l = 3;

  scanf("sample=%d,type=%d,attribute=%d,level=%d", &n, &t, &d, &l);

  float *f = (float *)malloc(n * d * sizeof(float));
  int *type = (int *)malloc(n * sizeof(int));

  input_data(f, type, n, d, t);
  set_gData(f, type, n, d, t);
  disp_gData();

  establish_decisiontree(l);

  free(f);
  free(type);
}

/* func: main
 * argu: cmdline
 * retu: code
 * date: 2019-12-1
 */
int main(int argc, char const *argv[])
{
  platform_init();

  decision_tree_test();

  return 0;
}
