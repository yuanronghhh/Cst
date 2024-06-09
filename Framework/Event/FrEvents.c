#include <Framework/Event/FrEvents.h>
#include <Framework/Event/FrEventCore.h>


static SysQueue *g_events = NULL;
static SysList *g_event_actions = NULL;
static SysMutex g_event_lock;

void fr_events_lock(void) {

  sys_mutex_lock(&g_event_lock);
}

void fr_events_unlock(void) {

  sys_mutex_unlock(&g_event_lock);
}

void fr_events_push_head(FrEvent *e) {
  sys_return_if_fail(e != NULL);

  sys_queue_push_head(g_events, e);
}

FrEvent *fr_events_get(void) {
  return sys_queue_pop_tail(g_events);
}

void fr_events_dispatch(FrEvent *e) {
  sys_return_if_fail(e != NULL);

  FrAction *action;

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

SysList* fr_events_prepend_action(FrAction *action) {
  g_event_actions = sys_list_prepend(g_event_actions, action);
  return g_event_actions;
}

static void events_add_action(FrAction *action) {
  g_event_actions = sys_list_prepend(g_event_actions, action);
}

void fr_events_setup(void) {
  fr_awatch_setup();

  g_events = sys_queue_new();
  g_event_actions = NULL;

  sys_mutex_init(&g_event_lock);

  events_add_action(FR_AKEY_STATIC);
  events_add_action(FR_AMOUSE_KEY_STATIC);
  events_add_action(FR_ACURSOR_MOVE_STATIC);
  events_add_action(FR_ACTION_STATIC);
}

void fr_events_teardown(void) {
  fr_awatch_teardown();

  sys_queue_free_full(g_events, (SysDestroyFunc)_sys_object_unref);
  sys_list_free_full(g_event_actions, (SysDestroyFunc)_sys_object_unref);
  sys_mutex_clear(&g_event_lock);
}

