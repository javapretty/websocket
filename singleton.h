/*
** Copyright (C) 2014 Wang Yaofu
** All rights reserved.
**
**Author:Wang Yaofu voipman@qq.com
**Description: The implement file of class Singleton.
*/
#ifndef _COMMON_SINGLETONG_H_
#define _COMMON_SINGLETONG_H_

//
// singleton class
// Note: template class must 
//       provide default constructor.
//
#include <cstddef>
#include <cstdlib>
#include <pthread.h>
#include "uncopyable.h"

template<class T>
class Singleton : public Uncopyable {
public:
    // global access point
    static T *Instance() {
        if (mInstance == NULL) {
            pthread_once(&mPonce, &Singleton::Init);
        }
        return mInstance;
    }

    static T& InstanceRef() {
        if (mInstance == NULL) {
            pthread_once(&mPonce, &Singleton::Init);
        }
        return *mInstance;
    }

private:
    static void Init() {
        mInstance = new T;
        ::atexit(Singleton::Destroy);
    }
    static void Destroy() {
        if (mInstance != NULL) {
            delete mInstance;
            mInstance = NULL;
        }
    }

private:
    static pthread_once_t mPonce;
    // instance for T
    static T* volatile mInstance;
};
template<typename T>
pthread_once_t Singleton<T>::mPonce = PTHREAD_ONCE_INIT;

template<typename T>
T* volatile Singleton<T>::mInstance = NULL;
#endif // _COMMON_SINGLETONG_H_
