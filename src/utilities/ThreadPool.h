//
// Created by esmayl on 12/08/2024.
//

#ifndef THREADPOOL_H
#define THREADPOOL_H

#pragma once
#include <condition_variable>
#include <functional>
#include <queue>
#include <thread>


class ThreadPool
{
    public:
        explicit ThreadPool(size_t numThreads);
        ~ThreadPool();

        void Enqueue(std::function<void()> task);
    private:
        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;

        std::mutex queueMutex;
        std::condition_variable condition;
        bool stop;
};



#endif //THREADPOOL_H
