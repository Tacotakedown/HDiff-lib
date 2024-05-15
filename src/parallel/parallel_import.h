#ifndef parallel_import_h
#define parallel_import_h

// select define one for support parallel
//#define _IS_USED_PTHREAD       1
//#define _IS_USED_CPP11THREAD   1
//#define _IS_USED_WIN32THREAD   1

#ifndef _IS_USED_MULTITHREAD
#define _IS_USED_MULTITHREAD 1
#endif

#if ((_IS_USED_PTHREAD > 0) || (_IS_USED_CPP11THREAD > 0) ||                   \
     (_IS_USED_WIN32THREAD > 0))
#//ok have one
#define _IS_USED_MULTITHREAD 1
#else
#if (_IS_USED_MULTITHREAD > 0)
#if ((!(defined _IS_USED_WIN32THREAD)) && (defined _WIN32))
#define _IS_USED_WIN32THREAD 1
#else
#if ((!(defined _IS_USED_CPP11THREAD)) && (__cplusplus >= 201103L))
#define _IS_USED_CPP11THREAD 1
#else
#if (!(defined _IS_USED_PTHREAD))
#define _IS_USED_PTHREAD 1
#endif
#endif
#endif
#endif
#endif

#if (_IS_USED_MULTITHREAD)

//并行临界区锁;
typedef void *HLocker;
HLocker locker_new(void);
void locker_delete(HLocker locker);
void locker_enter(HLocker locker);
void locker_leave(HLocker locker);

//同步变量;
typedef void *HCondvar;
#if (_IS_USED_CPP11THREAD)
#define TLockerBox void /*  used std::unique_lock<std::mutex>  */
#define _TLockerBox_name std::unique_lock<std::mutex>
#else
typedef struct {
  HLocker locker;
} TLockerBox;
#endif
HCondvar condvar_new(void);
void condvar_delete(HCondvar cond);
void condvar_wait(HCondvar cond, TLockerBox *lockerBox);
void condvar_signal(HCondvar cond);
void condvar_broadcast(HCondvar cond);

void this_thread_yield(void);

// parallel run
typedef void (*TThreadRunCallBackProc)(int threadIndex, void *workData);
void thread_parallel(int threadCount, TThreadRunCallBackProc threadProc,
                     void *workData, int isUseThisThread,
                     int threadIndexOffset);

#endif //_IS_USED_MULTITHREAD
#endif // parallel_import_h