#include <Framework/Event/FREvents.h>
#include <Framework/Event/FREventCore.h>


static SysQueue *g_events = NULL;
static SysList *g_event_actions = NULL;

void fr_events_push_head(FREvent *e) {
  sys_return_if_fail(e != NULL);

  sys_queue_push_head(g_events, e);
}

FREvent *fr_events_get(void) {
  return sys_queue_pop_tail(g_events);
}

void fr_events_dispatch(FREvent *e) {
  sys_return_if_fail(e != NULL);

  FRAction *action;

  for (SysList *item = g_event_actions; item; item = item->next) {
    action = item->data;

    if(fr_action_check(action, e)) {

      fr_action_dispatch(action, e);
    }
  }

  sys_object_unref(e);
}

SysBool fr_events_check(void) {
  return sys_queue_get_length(g_events) > 0;
}

SysList* fr_events_prepend_action(FRAction *action) {
  g_event_actions = sys_list_prepend(g_event_actions, action);
  return g_event_actions;
}

void fr_events_add_action(FRAction *action) {
  g_event_actions = sys_list_prepend(g_event_actions, action);
}

void fr_events_setup(void) {
  fr_awatch_setup();

  g_events = sys_queue_new();
  g_event_actions = NULL;

  fr_events_add_action(FR_AKEY_STATIC);
  fr_events_add_action(FR_AMOUSE_KEY_STATIC);
  fr_events_add_action(FR_ACURSOR_MOVE_STATIC);
  fr_events_add_action(FR_ACTION_STATIC);
}

void fr_events_teardown(void) {
  fr_awatch_teardown();

  sys_queue_free_full(g_events, (SysDestroyFunc)_sys_object_unref);
  sys_list_free_full(g_event_actions, (SysDestroyFunc)_sys_object_unref);
}

