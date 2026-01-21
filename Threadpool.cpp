#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <queue>
#include <time>
#include <functional>

enum Status{
    idle,
    running,
    done
};

class Task{
public:
    Task();
    ~Task();
    Status getStatus(){return status_;}
    void setStatus(Status s){
        mu_.lock();
        status_=s;
        mu_.unlock();
    }
private:
    Status status_;
    ((void)*()) func_;
    std::mutex mu_;
};

class ThreadPool{
public:
    ThreadPool(int n);
    ~ThreadPool();

    void addTask(Task* task);
    void work();

private:
    int workers_;
    std::queue<Task*> tasks_;
    std::mutex mu_;
    std:: cond_;
};


ThreadPool::ThreadPool(int n){
    workers_=n;

}


//添加任务，唤醒
void ThreadPool::addTask(Task* task){
    tasks_.emplace_back(task);
    notify_all();//?
}

//无尽循环，只要满足task列表不为空以及线程池还有余量，就执行
void ThreadPool::work(){
    while(1){
        while(tasks_.size()&&workers_){
            mu_.lock();
            auto task=tasks_.front();
            tasks_.pop();
            mu_.unlock();

            if(task->getStatus()!=idle) continue;
            task->setStatus(running);
            workers_--;
            int id=pthread_create(task,0,0,);
            printf("%d is running\n",id);
        }
        time.sleep(time.millsecond*100);
    }
}

