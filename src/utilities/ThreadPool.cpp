//
// Created by esmayl on 12/08/2024.
//

#include "ThreadPool.h"
ThreadPool::ThreadPool(size_t numThreads)
{
    stop = false;

    for(size_t i =0;i<numThreads;i++)
    {
        workers.emplace_back([this]
        {
            // internal thread loop, checks to see if it can execute a task based on lock and tasks list
            while(true)
            {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->queueMutex);

                    this->condition.wait(lock,[this]
                    {
                        return this->stop || !this->tasks.empty();
                    });

                    if(this->stop && this->tasks.empty())
                    {
                        return;
                    }

                    task = std::move(this->tasks.front());

                    this->tasks.pop();
                }

                task();
            }
        });
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }

    condition.notify_all();

    // C++ foreach loop
    for(std::thread &worker : workers)
    {
        worker.join();
    }
}

void ThreadPool::Enqueue(std::function<void()> task)
{
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        tasks.emplace(std::move(task));
    }

    condition.notify_one();
}




