# Cst

## 这是什么?

`Cst` 是一个基于组件这一概念, 使用c语言开发的，在LGPL下发布的GUI框架。

## 特性
1. 使用原生编译器c语言编译代码。
2. 能使用css进行简单的布局
3. 使用组件(Component)的组装代码．

## Build && Debug

### On Windows

1. 安装工具.
```text
cmake
Visual Studio 2017
flex(win_flex_bison-2.5.25.zip)
bison(win_flex_bison-2.5.25.zip)
```

2. 克隆项目

```shell
$ git clone 'https://github.com/yuanronghhh/Cst'
```

3. 下载依赖

核心依赖
```text
cairo
glfw
```

可以直接到release下载 `windows-zip.zip`文件夹，然后在项目外一层解压，比如：
```text
  Cst/
  lib/win64_vc14/cairo
  lib/win64_vc14/cairo/include
  lib/win64_vc14/cairo/bin
  lib/win64_vc14/cairo/lib
```

4. 使用cmake生成项目.

```shell
$ bison -o ./Cst/CstCore/Parser/CstCli.yy.c -d ./Cst/CstCore/Parser/CstCli.y
$ flex -o ./Cst/CstCore/Parser/lex.yy.c ./Cst/CstCore/Parser/CstCli.l
$ cmake -B build -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 15 2017 Win64"
$ /usr/bin/sed -i '1i#include "Parser/CstParserCommon.h"' './Cst/CstCore/Parser/lex.yy.c'
```

5. 编译

编译 `Mini` 示例项目

## LICENSE

LGPL
