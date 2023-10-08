#!/bin/bash

find ./Cst/CstCore/ -type f -name '*.h' -or -name '*.c' | xargs -d '\n' sed -i 's@CssPosition@CssArray@g'
