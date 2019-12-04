#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "mypso_regulation.h"

#define W_INI    0.9f
#define W_END    0.4f
#define INF_MAX  FLT_MAX
#define INF_MIN  FLT_MIN

Particle *gParticle;    /* 样本列表 */
Parameter gParameter;   /* 参数 */

static float rand_0to1(void);
static float rand_n1to1(void);
static void _init_parameter(int gk, int gn, int flag);
static void _init_particle(int gn, int m, float (*ptr_fitness)(float * w, int m));
static void _finish_mypso(void);
static void _mypso_gbest(void);
static void _updata_mypso(Particle *p, int g, float (*ptr_fitness)(float * w, int m));

/* func: 粒子群算法回归
 * argu: 粒子向量、粒子维度，优化目标函数，最大迭代次数、种群大小，目标最大flag：0，目标最小flag：1
 * retu: 无
 * date: 2019-12-3
 */
void mypso_regulation(float *w, int m, float (*ptr_fitness)(float * w, int m), int gk, int gn, int flag)
{
  _init_parameter(gk, gn, flag);
  _init_particle(gn, m, ptr_fitness);
  _mypso_gbest();
  // disp_pso();

  /* 迭代 */
  for (int i = 1; i <= gParameter.Gk; ++i)
  {
    for (int j = 0; j < gParameter.Gn; ++j)
    {
      _updata_mypso(gParticle + j, i, ptr_fitness);
    }
    _mypso_gbest();
    // disp_pso();
  }

  memcpy(w, gParameter.gbest, m * sizeof(float));
  _finish_mypso();
}

/* func: 显示种群
 * argu: 无
 * retu: 无
 * date: 2019-12-3
 */
void disp_pso(void)
{
  printf("pso info ==>\n");
  printf("generation=%d\tpopulation=%d\tfitness=%.5f\n", gParameter.Gk, gParameter.Gn, gParameter.gf);
  
  for(int j = 0; j < gParticle[0].m; ++j){
    printf("%.6f\t", gParameter.gbest[j]);
  }
  printf("\n");

  // for(int i = 0; i < gParameter.Gn; ++i){
  //   for(int j = 0; j < gParticle[i].m; ++j){
  //     printf("%.6f\t", gParticle[i].x[j]);
  //   }
  //   printf("\n");
  // }
  // printf("\n");
}

/* func: 生成0-1的随机数
 * argu: 无
 * retu: 生成的随机数
 * date: 2019-12-3
 */
static float rand_0to1(void)
{
  return (float)(rand() % RAND_MAX) / RAND_MAX;
}

/* func: 生成-1-1的随机数
 * argu: argu
 * retu: retu
 * date: date
 */
static float rand_n1to1(void)
{
  return 2.0f * (float)(rand() % RAND_MAX) / RAND_MAX - 1.0f;
}

/* func: 初始化pso参数，种群
 * argu: 迭代代数、种群大小
 * retu: 无
 * date: 2019-12-3
 */
static void _init_parameter(int gk, int gn, int flag)
{
  gParameter.Gk = gk;
  gParameter.Gn = gn;
  gParameter.Gm = flag;
  gParticle = (Particle *)malloc(gn * sizeof(Particle));
}

/* func: 初始化种群
 * argu: 种群大小、粒子维度、优化目标函数，目标最大flag：0，目标最小flag：1
 * retu: 无
 * date: 2019-12-3
 */
static void _init_particle(int gn, int m, float (*ptr_fitness)(float * w, int m))
{
  /* 为粒子种群分配空间 */
  for(int i = 0; i < gn; ++i){
    gParticle[i].v = (float *)malloc(m * sizeof(float));
    gParticle[i].x = (float *)malloc(m * sizeof(float));
    gParticle[i].pbest = (float *)malloc(m * sizeof(float));
    gParticle[i].m = m;
  }

  /* 初始化粒子种群 */
  for(int i = 0; i < gn; ++i){
    for(int j = 0; j < m; ++j){
      gParticle[i].v[j] = 10.0f * rand_n1to1();
      gParticle[i].x[j] = 10.0f * rand_n1to1();
      gParticle[i].pbest[j] = gParticle[i].x[j];
    }
    gParticle[i].pf = ptr_fitness(gParticle[i].x, m);
  }

  gParameter.gbest = (float *)malloc(m * sizeof(float));
  // disp_pso();
}

/* func: 算法结束，释放动态分配内存，防止内存泄露
 * argu: 无
 * retu: 无
 * date: 2019-12-3
 */
static void _finish_mypso(void)
{
  for(int i = 0; i < gParameter.Gn; ++i){
    free(gParticle[i].v);
    free(gParticle[i].x);
    free(gParticle[i].pbest); 
  }

  free(gParticle);
  free(gParameter.gbest);
}

/* func: 计算全局极值
 * argu: 无
 * retu: 无
 * date: 2019-12-3
 */
static void _mypso_gbest(void)
{
  float t = gParticle[0].pf;
  int index = 0;

  for(int i = 1; i < gParameter.Gn; ++i){
    if(gParameter.Gm){                                         /* 最小化目标函数 */
      if(gParticle[i].pf <= t){t = gParticle[0].pf; index = i;}
    }else{                                                     /* 最大化目标函数 */
      if(gParticle[i].pf >= t){t = gParticle[0].pf; index = i;}
    }
  }

  gParameter.gf = t;
  memcpy(gParameter.gbest, gParticle[index].pbest, gParticle[index].m * sizeof(float));
}

/* func: 更新粒子
 * argu: 粒子、当前迭代代数、适应度计算函数
 * retu: 无
 * date: 2019-12-3
 */
static void _updata_mypso(Particle *p, int g, float (*ptr_fitness)(float * w, int m))
{
  float w = (W_INI - W_END) * (gParameter.Gk - g) / gParameter.Gk + W_END;

  /* 更新速度 */
  for(int i = 0; i < p->m; ++i){
    p->v[i] = w * p->v[i]
              + 2.0f * rand_0to1() * (p->pbest[i] - p->x[i])
              + 2.0f * rand_0to1() * (gParameter.gbest[i] - p->x[i]);
  }

  /* 更新位置 */
  for(int i = 0; i < p->m; ++i){
    p->x[i] = p->x[i] + p->v[i];
  }

  /* 更新个体历史极值 */
  float f = ptr_fitness(p->x, p->m);
  if(gParameter.Gm){                                         /* 最小化目标函数 */
    if(f <= p->pf){
      p->pf = f;
      memcpy(p->pbest, p->x, p->m * sizeof(float));
    }
  }else{                                                     /* 最大化目标函数 */
    if(f >= p->pf){
      p->pf = f;
      memcpy(p->pbest, p->x, p->m * sizeof(float));
    }
  }
}
