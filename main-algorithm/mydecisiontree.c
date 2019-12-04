#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mydecisiontree.h"
#include "mypso_regulation.h"

Data gData;                 /* 样本集合 */
int  gMaxLevel;             /* 决策树最大层数 */
Sample *gRoot = NULL;       /* 父节点集合 */

static void _establish_decisiontree(Sample *s, int l, int parent, int node);
static int _type_count(Sample *s);
static int _type_n_count(Sample *s, int n);
static int _number_in_arry(int num, int *arry, int n);
static float _impurity(Sample *s);
static void _divide_set(Sample *root, Sample *left, Sample *right, float *w, int n);
static float _cal_fitness(float *w, int n);
static float vector_multiplication(float *a, float *b, int n);
static void disp_node(Sample *s, int parent, int node);
static void disp_coefficient(float *w, int n, int node);

/* func: 建立样本，单个样本属性最后加入一列常数项，向量维度为 d+1。 超平面为 sum(wi*xi)+wd 形式
 * argu: 样本元素、样本类型、样本个数、属性个数、类别个数
 * retu: 空间分配是否成功 0：成功 else:失败
 * date: 2019-12-1
 */
int set_gData(float *f, int *type, int n, int d, int t)
{
  gData.N = n;
  gData.T = t;
  gData.D = d + 1;  /* 向量维度，比属性个数多1，为wd系数 */
  gData.e = (Element *)malloc(n * sizeof(Element));
  if(!gData.e) return -1;
  
  for(int i = 0; i < n; ++i){
    gData.e[i].D = gData.D;
    gData.e[i].type = type[i];
    gData.e[i].f = (float *)malloc(gData.D * sizeof(float));
    if(!gData.e[i].f) return -1;
    
    for(int j = 0; j < d; ++j){
      gData.e[i].f[j] = *(f + i * d + j);
    }
    gData.e[i].f[d] = 1;
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
  printf("sample set ==>\n");
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
    s.s[i] = i;
  }

  printf("decision tree ==>\n");
  _establish_decisiontree(&s, 0, 0, 0);

  free(s.s);
}

/* func: 计算适应度，若样本集合只剩一个种类，不应再调用此函数
 * argu: 划分数组，数组大小
 * retu: 适应度
 * date: 2019-12-2
 */
static float _cal_fitness(float *w, int n)
{
  if(!gRoot) return 0.0f;
  if(gRoot->n <= 0) return 0.0f;

  Sample left, right;
  left.s = (int *)malloc(gRoot->n * sizeof(int));
  right.s = (int *)malloc(gRoot->n * sizeof(int));

  _divide_set(gRoot, &left, &right, w, n);

  float fitness = 0.0f;
  float fitleft = (float)left.n / (float)gRoot->n * _impurity(&left);
  float fitright = (float)right.n / (float)gRoot->n * _impurity(&right);

  fitness = _impurity(gRoot) - (fitleft + fitright);
  // printf("[fitness]\t%.5f  %.5f  %.5f\n", fitleft, fitright, fitness);

  free(left.s);
  free(right.s);

  return fitness;
}

/* func: 划分集合
 * argu: argu
 * retu: retu
 * date: date
 */
static void _divide_set(Sample *root, Sample *left, Sample *right, float *w, int n)
{
  left->n = right->n = 0;
  for(int i = 0; i < root->n; ++i){
    if(vector_multiplication(gData.e[root->s[i]].f, w, n) < 0){
      left->s[(left->n)++] = root->s[i];
      // printf("left:%d\n", left->s[(left->n) - 1]);
    }else{
      right->s[(right->n)++] = root->s[i];
      // printf("right:%d\n", right->s[(right->n) - 1]);
    }
  }
}

/* func: 打印结点样本
 * argu: 样本集合、父节点、当前结点
 * retu: 无
 * date: 2019-12-2
 */
static void disp_node(Sample *s, int parent, int node)
{
  printf("[node][%d]-[%d]\t", parent, node);
  for(int i = 0; i < s->n; ++i)
  {
    printf("%d  ", s->s[i] + 1);
  }
  printf("\n");
}

/* func: 打印结点分类超平面
 * argu: 超平面数组、数组大小、结点
 * retu: 无
 * date: 2019-12-2
 */
static void disp_coefficient(float *w, int n, int node)
{
  printf("[coef][%d]\t", node);
  for(int i = 0; i < n; ++i)
  {
    printf("%.5f  ", w[i]);
  }
  printf("\n");
}

/* func: 建立决策树递归函数
 * argu: 样本集合，决策树最大层数、父节点、当前结点
 * retu: 无
 * date: 2019-12-1
 */
static void _establish_decisiontree(Sample *s, int l, int parent, int node)
{
  disp_node(s, parent, node);

  if(l >= gMaxLevel) return; 
  if(_type_count(s) <= 1) return;

  float *w = (float *)malloc(gData.D * sizeof(float));

  gRoot = s;
  mypso_regulation(w, gData.D, _cal_fitness, 1000, 100, 0); /* max fitness */
  gRoot = NULL;

  disp_coefficient(w, gData.D, node);

  Sample left, right;
  left.s = (int *)malloc(s->n * sizeof(int));
  right.s = (int *)malloc(s->n * sizeof(int));

  _divide_set(s, &left, &right, w, gData.D);

  _establish_decisiontree(&left, l + 1, node, node + 1);
  _establish_decisiontree(&right, l + 1, node, node + 2);

  free(w);
  free(left.s);
  free(right.s);
}

/* func: 判断样本中类型个数
 * argu: 样本集合
 * retu: 样本类型个数
 * date: 2019-12-1
 */
static int _type_count(Sample *s)
{
  int count = 0;
  struct{
    int *arry;
    int n;
  }t;

  t.arry = (int *)malloc(s->n * sizeof(int));
  t.n = 0;

  for(int i = 0; i < s->n; ++i){
    if(!_number_in_arry(gData.e[s->s[i]].type, t.arry, t.n)){
      count++;
      t.arry[(t.n)++] = gData.e[s->s[i]].type;
    }
  }

  free(t.arry);

  return count;
}

/* func: 获取样本集合中第n类的个数
 * argu: 样本集合s，类别n
 * retu: 类别n的个数
 * date: 2019-12-2
 */
static int _type_n_count(Sample *s, int n)
{
  int count = 0;

  for(int i = 0; i < s->n; ++i){
    if(n == gData.e[s->s[i]].type)
      count++;
  }

  return count;
}

/* func: 计算不纯度
 * argu: argu
 * retu: retu
 * date: date
 */
static float _impurity(Sample *s)
{
  int ncnt = _type_count(s);
  int *narry = (int *)malloc(ncnt * sizeof(int));

  for(int i = 0; i < ncnt; ++i){
    narry[i] = _type_n_count(s, i);
  }

  float imp = 0.0f;
  for(int i = 0; i < ncnt; ++i){
    if(narry[i] != 0){
      float p = (float)narry[i] / s->n;
      imp += p * log(p) / log(2);
    }
  }
  imp = -imp;
  // printf("%.5f\n", imp);

  free(narry);
  return imp;
}

/* func: number是否在n维数组arry中
 * argu: 数字，数组，数组大小
 * retu: 1：在 0：不在
 * date: 2019-12-2
 */
static int _number_in_arry(int num, int *arry, int n)
{
  for(int i = 0; i < n; ++i){
    if(num == arry[i])
      return 1;
  }
  return 0;
}

/* func: 向量乘法
 * argu: 向量a, 向量b
 * retu: 向量乘积
 * date: 2019-12-2
 */
static float vector_multiplication(float *a, float *b, int n)
{
  float ret = 0.0f;

  for(int i = 0; i < n; ++i){
    ret += a[i] * b[i];
  }

  return ret;
}
