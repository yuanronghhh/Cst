# Cst

## What is this?

`Cst` is a component-based gui library for exploring the implementation of vue in c.
only avaliable for study now.

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

Download `windows-zip` in releases. and unzip it outside of this project.

like below:
```text
  Cst/
  lib/win64_vc14/cairo
  lib/win64_vc14/cairo/include
  lib/win64_vc14/cairo/bin
  lib/win64_vc14/cairo/lib
```

4. generate project use cmake.
```shell
$ bison -o ./Cst/CstCore/Parser/CstCli.yy.c -d ./Cst/CstCore/Parser/CstCli.y
$ flex -o ./Cst/CstCore/Parser/lex.yy.c ./Cst/CstCore/Parser/CstCli.l
$ /usr/bin/sed -i '1i#include <CstCore/Parser/CstParserCommon.h>' './Cst/CstCore/Parser/lex.yy.c'
$ cmake -B build -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 15 2017 Win64"
```

5. build

see `CstCoreTest` project
