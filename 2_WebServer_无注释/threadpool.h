#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <list>
#include "locker.h"

template<typename T>
class threadpool{
private:
    int m_thread_number;
    pthread_t* m_threads;
 
    int m_max_requests;
    std::list<T*> m_workqueue;
   
    locker m_queuelocker;
    sem m_queuestat;

    bool m_stop;

public:
    threadpool(int thread_number = 6, int max_requests = 1000);
    ~threadpool();
    bool append(T* request);
private:
    static void* work(void* arg);
    void run();
};

template<typename T>
threadpool<T>::threadpool(int thread_number, int max_requests)
    : m_thread_number(thread_number), m_max_requests(max_requests), m_threads(NULL), m_stop(false){
    if((thread_number <= 0) || (max_requests <= 0)){
        throw std::exception();
    }
    m_threads = new pthread_t[m_thread_number];
    if(!m_threads){
        throw std::exception();
    }
    for(int i = 0; i < thread_number; ++i){
        printf("create the %dth thread\n", i);
        if(pthread_create(m_threads + i, NULL, work, this) != 0){
            delete[] m_threads;
            throw std::exception();
        }
        if(pthread_detach(m_threads[i])){
            delete[] m_threads;
            throw std::exception();
        }
    }
}

template<typename T>
threadpool<T>::~threadpool(){
    delete[] m_threads;
    m_stop = true;
}

template<typename T>
bool threadpool<T>::append(T* request){
    m_queuelocker.lock();
    if(m_workqueue.size() > m_max_requests){
        m_queuelocker.unlock();
        return false;
    }
    m_workqueue.push_back(request);
    m_queuestat.post();
    m_queuelocker.unlock();
    return true;
}

template<typename T>
void* threadpool<T>::work(void* arg){
    threadpool* pool = (threadpool*)arg;
    pool->run();
    return pool;
}

template<typename T>
void threadpool<T>::run(){
    while(!m_stop){
        m_queuestat.wait();
        m_queuelocker.lock();
        if(m_workqueue.empty()){
            m_queuelocker.unlock();
            continue;
        }
        T* request = m_workqueue.front();
        m_workqueue.pop_front();
        m_queuelocker.unlock();
        if(!request){
            continue;
        }
        request->process();
    }
}

#endif