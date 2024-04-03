# Cst

## What is this?

`Cst` is a component-based gui library for exploring the implementation of vue in c.
only avaliable for study now.

`Cst` is will parse the file like below:
```
import MenuBar from "./MiniMenuBar.cst";

Component<id: "mini-component">: {
  Style: {
    menu-text: {
      color: "#0000ff"
    }
  },

  Layout: {
    LBox<id: "menu">: {
      MenuBar<value: "文件(F)", "@press1": "mini_btn_press2">: null,
      MenuBar<value: "编辑(E)">: null,
      MenuBar<value: "视图(V)">: null,
      MenuBar<value: "图像(I)">: null,
      MenuBar<value: "队列(L)">: null,
      MenuBar<value: "显示(V)">: null,
      MenuBar<value: "其他(O)">: null
    },
    LGrid<id:"image-body">: {
    }
  }
}
```

the event function will map to c code.
the code like this:
```c
/***
 * MiniComponent will be generated from ComponentCode.code template
 * now CstCli not finished yet.
 */

#include <Mini/Front/MiniComponent.h>

struct _MiniComponentPrivate {
  SysInt reserved;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(MiniComponent, mini_component, CST_TYPE_COMPONENT);

/**
 * user functions
 */
FR_FUNC_DEFINE_EVENT(mini_quit_key) {
  FREventKey *ekey = FR_EVENT_KEY(e);
  CstApplication *app = cst_application_acquire();

  sys_debug_N("key press: %d,%d", fr_event_key_scancode(ekey), fr_event_key_action(ekey));

  cst_application_stop(app);

  return 0;
}

FR_FUNC_DEFINE_EVENT(mini_component_press2) {
  return 0;
}

MiniComponent* mini_component_new(void) {
  return sys_object_new(MINI_TYPE_COMPONENT, NULL);
}

static void mini_component_construct(CstComponent *comp, CstModule *v_module, CstComponent *v_parent) {
  CST_COMPONENT_CLASS(mini_component_parent_class)->construct(comp, v_module, v_parent);

  FRAWatchProps props = { 0 };
  props.key = FR_KEY_Q;

  cst_module_add_awatch(v_module, comp, "key", "mini_quit_key", mini_quit_key, &props);
  cst_module_set_function(v_module, FR_FUNC_ANY(mini_component_press2));
}

static void mini_component_init(MiniComponent *self) {
  self->priv = mini_component_get_private(self);
}

static void mini_component_dispose(SysObject *o) {

  SYS_OBJECT_CLASS(mini_component_parent_class)->dispose(o);
}

static void mini_component_class_init(MiniComponentClass *cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  CstComponentClass *ccls = CST_COMPONENT_CLASS(cls);

  ocls->dispose = mini_component_dispose;
  ccls->construct = mini_component_construct;
}
```

## Build && Debug

### On Windows

1. Install tools.

Download and install tools:
```text
cmake
visual studio 2017
flex
bison
```

2. Clone project

```shell
$ git clone 'https://github.com/yuanronghhh/Cst'
```

3. Download dependencies.

```text
cairo
glfw
```

Download `windows-deps.zip` in releases. and unzip it outside of this project.

like below:
```text
  Cst/
  lib/win64_vc14/cairo
  lib/win64_vc14/cairo/include
  lib/win64_vc14/cairo/bin
  lib/win64_vc14/cairo/lib
```
add `make.exe` and `tools/win_flex_bison-2.5.25/` to your system path enviroment.

4. generate project use cmake.
```shell
$ bison -o ./Cst/CstCore/Parser/CstCli.yy.c -d ./Cst/CstCore/Parser/CstCli.y
$ flex -o ./Cst/CstCore/Parser/lex.yy.c ./Cst/CstCore/Parser/CstCli.l
$ /usr/bin/sed -i '1i#include <CstCore/Parser/CstParserCommon.h>' './Cst/CstCore/Parser/lex.yy.c'
$ cmake -B build -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 15 2017 Win64"
```

5. build

see `CstCoreTest` project

## License

LGPL
