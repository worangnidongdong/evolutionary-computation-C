#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>

struct point
{
	float x1;
	float x2;
};

struct _c
{
	float c1;
	float c2;
};

float rand_0to1(void)
{
	return (float)(rand() % 100) / 100.0f;
}

#define W_INI    0.9f
#define W_END    0.4f
#define INF      FLT_MAX

typedef struct point vtype;
typedef struct point xtype;
typedef struct _c    ctype;
typedef float        wtype;
typedef float        fitness;
typedef xtype        pbest;
typedef pbest        gbest;

struct particle{
	vtype v;
	xtype x;
	pbest p;  /* 个体极值 */
	fitness f;/* 个体历史最优适应度 */
};

struct psopara{
	wtype w;  /* 惯性因子 */
	ctype c;  /* 学习因子 */
	gbest g;  /* 全局极值 */
	int   Gk; /* 最大迭代次数 */
	int   N;  /* 种群大小 */
	fitness f;/* 最优适应度 */
};

/* func: 平台初始化
 * argu: 0：随机初始化种子，结果不可复现；1：使用r初始化种子，结果可复现
 * retu: 无
 * date: 2019-11-29
 */
void platform_init(unsigned int r)
{
	if(r)
		srand(r);
	else
		srand((unsigned int)time(NULL));
}

/* func: 算法参数初始化，全局极值需要在种群初始化后更新一次
 * argu: 算法参数
 * retu: 无
 * date: 2019-11-29
 */
void mypso_init(struct psopara *pp)
{
	pp->w = W_INI;
	pp->c.c1 = 2.0f;
	pp->c.c2 = 2.0f;
	pp->Gk = 100;
	pp->N = 10;
	pp->f = INF;
}

/* func: 算法结束释放内存
 * argu: 种群数组
 * retu: 无
 * date: 2019-11-29
 */
void mypso_end(struct particle *pt)
{
	free(pt);
}

void mypso_display_fitness(xtype x, fitness f, int i)
{
	printf("[%d]: gbest is (%.2f, %.2f), fitness is %.2f\n", i, x.x1, x.x2, f);
}
/* func: 计算适应度函数
 * argu: 位置
 * retu: 适应度
 * date: 2019-11-29
 */
fitness mypso_cal_fitness(xtype x)
{
	return x.x1 * x.x1 + x.x2 * x.x2;
}

/* func: 初始化种群
 * argu: 种群大小
 * retu: 种群数组
 * date: 2019-11-29
 */
struct particle *particle_init(int n)
{
	struct particle *p = (struct particle*)malloc(n * sizeof(struct particle));

	for(int i = 0; i < n; ++i){
		p[i].v.x1 = (float)(rand()%10000) / 100.0f;
		p[i].v.x2 = (float)(rand()%10000) / 100.0f;
		p[i].x.x1 = (float)(rand()%10000) / 100.0f;
		p[i].x.x2 = (float)(rand()%10000) / 100.0f;
		p[i].p = p[i].x;
		p[i].f = mypso_cal_fitness(p[i].p);
		// mypso_display_fitness(p[i].p);
	}

	return p;
}

/* func: 更新全局极值
 * argu: 种群数组
 * retu: 无，以传地址的方式改变全局极值
 * date: 2019-11-29
 */
void mypso_gbest(struct psopara *pp, struct particle *pt)
{
	for (int i = 0; i < pp->N; ++i){
		if(pp->f > pt[i].f){
			pp->f = pt[i].f;
			pp->g = pt[i].p;
		}
	}

	// mypso_display_fitness(pp->g, pp->f);
}

/* func: 粒子更新
 * argu: 参数，粒子，迭代代数
 * retu: 无，以传地址的方式对粒子进行更新
 * date: 2019-11-29
 */
void mypso_x_v_type(struct psopara *pp, struct particle *pt, int g)
{
	wtype w = (W_INI - W_END) * (pp->Gk - g) / pp->Gk + W_END;

	/* 更新速度 */
	pt->v.x1 = w * pt->v.x1 
	           + pp->c.c1 * rand_0to1() * (pt->p.x1 - pt->x.x1) 
	           + pp->c.c2 * rand_0to1() * (pp->g.x1 - pt->x.x1);
	pt->v.x2 = w * pt->v.x2 
	           + pp->c.c1 * rand_0to1() * (pt->p.x2 - pt->x.x2) 
	           + pp->c.c2 * rand_0to1() * (pp->g.x2 - pt->x.x2);

	/* 更新位置 */
	pt->x.x1 = pt->x.x1 + pt->v.x1;
	pt->x.x2 = pt->x.x2 + pt->v.x2;

	/* 更新个体历史极值 */
	fitness f = mypso_cal_fitness(pt->x);
	if(f < pt->f){
		pt->f = f;
		pt->p = pt->x;
	}
}

int main(int argc, char const *argv[])
{
	struct psopara pp;
	struct particle *pt;

	platform_init(0);
	mypso_init(&pp);
	pt = particle_init(pp.N);
	mypso_gbest(&pp, pt);

	for (int i = 1; i <= pp.Gk; ++i)
	{
		for (int j = 0; j < pp.N; ++j)
		{
			mypso_x_v_type(&pp, &pt[j], i);
		}
		mypso_gbest(&pp, pt);
		mypso_display_fitness(pp.g, pp.f, i);
	}

	mypso_end(pt);
	return 0;
}