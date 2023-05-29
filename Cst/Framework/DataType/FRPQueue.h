#ifndef __FR_PQUEUE_H__
#define __FR_PQUEUE_H__

#include <System/SysCore.h>

#define FR_PRIO_LIST(o) ((FRPrioList *)o)

SYS_BEGIN_DECLS

typedef struct _SysQueue FRPQueue;
typedef struct _FRPrioList FRPrioList;

struct _FRPrioList {
  SysList parent;
  SysInt prio;
};

/* priority api */
FRPQueue *fr_pqueue_new(void);
FRPrioList *fr_pqueue_push_tail(FRPQueue *queue, SysInt prio, SysPointer data);
FRPrioList *fr_pqueue_push_head(FRPQueue *queue, SysInt prio, SysPointer data);

void fr_pqueue_unlink(FRPQueue *queue, FRPrioList *plink);
void fr_pqueue_push_tail_link(FRPQueue *queue, FRPrioList *plink);
void fr_pqueue_push_head_link(FRPQueue *queue, FRPrioList *plink);
void fr_pqueue_free_full(FRPQueue *queue, SysDestroyFunc free_func);

SYS_END_DECLS

#endif
