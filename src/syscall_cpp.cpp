
#include "../h/syscall_cpp.hpp"

void* operator new (size_t s){
    return mem_alloc(s);
}

void operator delete (void* adr) noexcept{
    mem_free( adr);
}

Thread::Thread(void (*body)(void *), void *arg) {
    thread_create_cpp(&myHandle, body, arg);
}

Thread::~Thread() {
    mem_free(myHandle);
}

int Thread::start() {
    Scheduler::put((TCB*)myHandle);
    return 0;
}

void Thread::join() {
    thread_join(myHandle);
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t) {
    return 0;
}

Thread::Thread() {
    thread_create_cpp(&myHandle, wrapperRun, this);
}

void Thread::wrapperRun(void * t) {
    Thread* thr=(Thread*) t;
    thr->run();
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}
/*
void PeriodicThread::terminate() {

}

PeriodicThread::PeriodicThread(time_t period) {

}
*/
char Console::getc() {
    return __getc();
}

void Console::putc(char c) {
    __putc(c);
}
