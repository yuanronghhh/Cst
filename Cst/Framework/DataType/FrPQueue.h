#ifndef __FR_PQUEUE_H__
#define __FR_PQUEUE_H__

#include <System/SysCore.h>

#define FR_PRIO_LIST(o) ((FRPrioLink *)o)

SYS_BEGIN_DECLS


/**
 * deprecated api
 */
typedef struct _SysQueue FrPQueue;
typedef struct _FRPrioLink FRPrioLink;

struct _FRPrioLink {
  SysList parent;
  SysInt prio;
};

/* priority api */
FrPQueue *fr_pqueue_new(void);
FRPrioLink *fr_pqueue_push_tail(FrPQueue *queue, SysInt prio, SysPointer data);
FRPrioLink *fr_pqueue_push_head(FrPQueue *queue, SysInt prio, SysPointer data);

void fr_pqueue_unlink(FrPQueue *queue, FRPrioLink *plink);
void fr_pqueue_push_tail_link(FrPQueue *queue, FRPrioLink *plink);
void fr_pqueue_push_head_link(FrPQueue *queue, FRPrioLink *plink);
void fr_pqueue_free_full(FrPQueue *queue, SysDestroyFunc free_func);

SYS_END_DECLS

#endif
