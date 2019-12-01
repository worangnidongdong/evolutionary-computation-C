import numpy as np
cimport numpy as np

# ctypedef np.float64 as FLOAT
# ctypedef np.int64 as INT

cdef struct Element:
  float   *f  #属性列表
  int D       #属性个数
  int type    #元素类别
  
cdef struct Data:
  Element *e  #样本列表
  int N       #样本个数
  int T       #类别总数
  
cdef struct Sample:
  int *s      #样本集合
  int n       #样本个数
