#ifndef _MYDECISION_H
#define _MYDECISION_H

typedef struct _element{
  float *f;    /* 属性列表 */
  int D;       /* 属性个数 */
  int type;    /* 元素类别 */
}Element;

typedef struct _data{
  Element *e;  /* 样本列表 */
  int N;       /* 样本个数 */
  int T;       /* 类别总数 */
  int D;       /* 属性个数 */
}Data;

typedef struct _sample{
  int *s;      /* 样本集合 */
  int n;       /* 样本个数 */
}Sample;

int set_gData(float *f, int *type, int n, int d, int t);
void free_gData(void);
void disp_gData(void);
void establish_decisiontree(int l);

#endif
