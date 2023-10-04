#ifndef __FR_PQUEUE_H__
#define __FR_PQUEUE_H__

#include <System/SysCore.h>

#define FR_PRIO_LIST(o) ((FRPrioLink *)o)

SYS_BEGIN_DECLS

typedef struct _SysQueue FRPQueue;
typedef struct _FRPrioLink FRPrioLink;

struct _FRPrioLink {
  SysList parent;
  SysInt prio;
};

/* priority api */
FRPQueue *fr_pqueue_new(void);
FRPrioLink *fr_pqueue_push_tail(FRPQueue *queue, SysInt prio, SysPointer data);
FRPrioLink *fr_pqueue_push_head(FRPQueue *queue, SysInt prio, SysPointer data);

void fr_pqueue_unlink(FRPQueue *queue, FRPrioLink *plink);
void fr_pqueue_push_tail_link(FRPQueue *queue, FRPrioLink *plink);
void fr_pqueue_push_head_link(FRPQueue *queue, FRPrioLink *plink);
void fr_pqueue_free_full(FRPQueue *queue, SysDestroyFunc free_func);

SYS_END_DECLS

#endif
