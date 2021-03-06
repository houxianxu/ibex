cimport cython
cimport numpy as np
import numpy as np
import ctypes

cdef extern from 'cpp-seg2gold.h':
    long *CppMapping(long *segmentation, int *gold, long nentries, double match_threshold, double nonzero_threshold)

def Mapping(segmentation, gold, match_threshold=0.70, nonzero_threshold=0.40):
    cdef np.ndarray[long, ndim=3, mode='c'] cpp_segmentation
    cpp_segmentation = np.ascontiguousarray(segmentation, dtype=ctypes.c_int64)
    cdef np.ndarray[int, ndim=3, mode='c'] cpp_gold
    cpp_gold = np.ascontiguousarray(gold, dtype=ctypes.c_int32)

    max_segmentation = np.amax(segmentation) + 1

    cdef long *mapping = CppMapping(&(cpp_segmentation[0,0,0]), &(cpp_gold[0,0,0]), segmentation.size, match_threshold, nonzero_threshold)

    cdef long[:] tmp_mapping = <long[:max_segmentation]> mapping;

    return np.asarray(tmp_mapping)