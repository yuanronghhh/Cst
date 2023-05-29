#include <Framework/FRCommon.h>


SysInt fr_get_type_by_name(const SysChar *names[], SysInt len, const SysChar *name) {
  sys_return_val_if_fail(name != NULL, -1);
  sys_return_val_if_fail(names != NULL, -1);
  sys_return_val_if_fail(len > 0, -1);

  const SysChar* item;
  for (SysInt i = 0; i < len; i++) {
    item = names[i];

    if (sys_str_equal(name, item)) {
      return i;
    }
  }

  return -1;
}