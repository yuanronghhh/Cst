# CST PARSER

a component parser base on gtk.

# quick start

```mini.cst
#include "SubNav"

Component ComponetName {
  Layout {
    box["box1"][color=#020202]: {
      div["div1"]: {
        text["txt1"][value="text1"]: null,
        button["btn1"][value="click2";@clicked=widgetPrint(this)]: null
      },
      box["box2"]: {
        text["txt1"][value="t1"]: null
      }
    }
  }

  /*
  Style {
    box1 {
      border: "solid 1px white";
      color: "#ffffff";
    }
  }
  */

  Function {%
    def widgetPrint(this):
        print("fine %s", this.value)
  %}
}
```
