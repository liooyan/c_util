#ifndef MEM_INCLUDED
#define MEM_INCLUDED

#include "../except/except.h"

extern Exception Mem_Failed;

extern void *mem_alloc(long  nbytes, const char *file, int line);
extern  void *mem_calloc(long  count,long nbytes,const char *file,int line);
extern  void mem_free(void *ptr,const char *file,int line);
extern  void mem_resize(void *ptr,long  count,const char *file,int line);

#define ALLOC(nbytes) \
mem_alloc(nbytes,__FILE__,__LINE__)

#define CALLOC(count,nbytes) \
mem_calloc(count,nbytes,__FILE__,__LINE__)


#define FREE(ptr) \
mem_free(ptr,__FILE__,__LINE__)
#define RESIZE(ptr,count) \
mem_resize(ptr,count,__FILE__,__LINE__)

#define NEW(p) ((p) = ALLOC((long) sizeof *(p)))
#define NEW0(p) ((p) = CALLOC(1,(long) sizeof *(p)))


#endif