#include <Framework/DataType/FRPQueue.h>

/* pqueue api */
static FRPrioLink *prio_list_new(SysInt prio, SysPointer data) {
  FRPrioLink *plink = sys_new0_N(FRPrioLink, 1);

  plink->parent.data = data;
  plink->prio = prio;

  return plink;
}

static void prio_list_free(FRPrioLink *plink) {
  sys_free_N(plink);
}

FRPQueue *fr_pqueue_new(void) {
  return sys_queue_new();
}

static SysBool _pqueue_push_tail(SysList *tail, FRPrioLink *plink) {
  SysList *nlist = SYS_LIST(plink);

  for (SysList *list = tail; list; list = list->prev) {
    FRPrioLink *plist = FR_PRIO_LIST(list);

    if (plink->prio > plist->prio) {
      if (list->next) {
        nlist->next = list->next;
        nlist->next->prev = nlist;
      }

      list->next = nlist;
      nlist->prev = list;

      return true;
    }
  }

  return false;
}

static SysBool _pqueue_push_head(SysList *head, FRPrioLink *plink) {
  SysList *nlist = SYS_LIST(plink);

  for (SysList *list = head; list; list = list->next) {
    FRPrioLink *plist = FR_PRIO_LIST(list);

    if (plink->prio <= plist->prio) {
      if (list->prev) {
        nlist->prev = list->prev;
        nlist->prev->next = nlist;
      }

      list->prev = nlist;
      nlist->next = list;

      return true;
    }
  }

  return false;
}

void fr_pqueue_push_head_link(FRPQueue *queue, FRPrioLink *plink) {
  sys_return_if_fail(queue != NULL);
  sys_return_if_fail(plink != NULL);
  sys_return_if_fail(plink->prio > 0);

  SysList *nlist = SYS_LIST(plink);

  sys_return_if_fail(nlist->prev == NULL);
  sys_return_if_fail(nlist->next == NULL);

  if (!_pqueue_push_head(queue->head, plink)) {
    if (queue->head) {
      nlist->next = queue->head;
      nlist->next->prev = nlist;
    }
    
    queue->head = nlist;
  }

  if (!queue->tail) {
    queue->tail = queue->head;
  }
}

void fr_pqueue_push_tail_link(FRPQueue *queue, FRPrioLink *plink) {
  sys_return_if_fail(queue != NULL);
  sys_return_if_fail(plink != NULL);
  sys_return_if_fail(plink->prio > 0);

  SysList *nlist = SYS_LIST(plink);

  sys_return_if_fail(nlist->prev == NULL);
  sys_return_if_fail(nlist->next == NULL);

  if (!_pqueue_push_tail(queue->tail, plink)) {
    if (queue->tail) {
      nlist->prev = queue->tail;
      nlist->prev->next = nlist;
    }

    queue->tail = nlist;
  }

  if (!queue->head) {
    queue->head = queue->tail;
  }
}

FRPrioLink *fr_pqueue_push_tail(FRPQueue *queue, SysInt prio, SysPointer data) {
  sys_return_val_if_fail(queue != NULL, NULL);
  sys_return_val_if_fail(prio > 0, NULL);

  FRPrioLink *plink = prio_list_new(prio, data);

  fr_pqueue_push_tail_link(queue, plink);

  if (queue->tail->next)
    queue->tail = queue->tail->next;
  else
    queue->head = queue->tail;
  queue->length++;

  return plink;
}

FRPrioLink *fr_pqueue_push_head(FRPQueue *queue, SysInt prio, SysPointer data) {
  sys_return_val_if_fail(queue != NULL, NULL);
  sys_return_val_if_fail(prio > 0, NULL);

  FRPrioLink *plink = prio_list_new(prio,  data);

  fr_pqueue_push_head_link(queue, plink);

  if (!queue->tail)
    queue->tail = queue->head;
  queue->length++;

  return plink;
}

void fr_pqueue_unlink(FRPQueue *queue, FRPrioLink *plink) {
  sys_return_if_fail(queue != NULL);
  sys_return_if_fail(plink != NULL);
  sys_return_if_fail(plink->prio > 0);

  SysList *nlist = SYS_LIST(plink);

  if (nlist == queue->tail) {
    queue->tail = queue->tail->prev;
  }

  SysList *nlink = sys_list_remove_link(queue->head, nlist);

  queue->head = nlink;
  queue->length--;
}

void fr_pqueue_free(FRPQueue *queue) {
  sys_return_if_fail(queue != NULL);

  if (queue->head) {
    prio_list_free(FR_PRIO_LIST(queue->head));
    queue->head = NULL;
  }
  sys_slice_free(FRPQueue, queue);
}

void fr_pqueue_free_full(FRPQueue *queue, SysDestroyFunc free_func) {
  for (SysList *node = queue->head; node; node = node->next) {
    free_func(node->data);
  }

  fr_pqueue_free(queue);
}

