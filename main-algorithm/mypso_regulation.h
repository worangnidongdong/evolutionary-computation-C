#ifndef _MYPSO_REGULATION_H
#define _MYPSO_REGULATION_H

typedef struct _particle{
  float *v;        /* 粒子速度 */
  float *x;        /* 粒子位置 */
  float *pbest;    /* 历史极值 */
  float pf;        /* 历史极值适应度 */
  int   m;         /* 粒子维度 */
}Particle;

typedef struct _parameter{
  float *gbest;    /* 全局极值 */
  float gf;        /* 全局极值适应度 */
  int   Gk;        /* 最大迭代次数 */
  int   Gn;        /* 种群大小 */
  int   Gm;        /* 适应度优化目标 0：max fitness； 1：min fitness */
}Parameter;

void mypso_regulation(float *w, int m, float (*ptr_fitness)(float * w, int m), int gk, int gn, int flag);
void disp_pso(void);

#endif
