#include "util.h"
#include <stdio.h>
#include <stdlib.h>


void errif(bool condition, const char* errmsg){
// 第一个参数是是否发生错误，如果为真，则表示有错误发生，会调用`<stdio.h>`头文件中的`perror`，
// 这个函数会打印出`errno`的实际意义，还会打印出我们传入的字符串，
// 也就是第函数第二个参数，让我们很方便定位到程序出现错误的地方。
// 然后使用`<stdlib.h>`中的`exit`函数让程序退出并返回一个预定义常量`EXIT_FAILURE`。
    if(condition){
        perror(errmsg);
        exit(EXIT_FAILURE);
    }
}