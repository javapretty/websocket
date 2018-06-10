/*
** Copyright (C) 2014 Wang Yaofu
** All rights reserved.
**
**Author:Wang Yaofu voipman@qq.com
**Description: The header file of class Uncopyable.
*/

#ifndef _COMMON_UNCOPYABLE_H
#define _COMMON_UNCOPYABLE_H

class Uncopyable {
protected:
    Uncopyable() {}
    ~Uncopyable() {}

private:
    Uncopyable(const Uncopyable& o);
    Uncopyable& operator=(const Uncopyable& o);
};

#endif // _COMMON_UNCOPYABLE_H
