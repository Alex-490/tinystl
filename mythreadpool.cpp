#include <mutex>
#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <condition_variable>
#include <stdexcept>
#include <future>
#include <atomic>

class mythreadpool{
public:
    explicit mythreadpool(size_t thread_count):stop_(false){
        if(thread_count<=0){
            throw std::invalid_argument("thread count must be >0\n");
        }
        
        for(size_t i=0;i<thread_count;i++){
            workers_.emplace_back([this]{
                worker_loop();
            });
        }
    }

    ~mythreadpool(){
        stop();
    }

    mythreadpool(const mythreadpool&)=delete;
    mythreadpool& operator=(const mythreadpool&)=delete;

    template <class F,class... Args>
    auto submit(F&& f,Args&&... args)
        ->std::future<std::invoke_result_t<F,Args...>>{
        using result_type=std::invoke_result_t<F,Args...>;

        auto task=std::make_shared<std::packaged_task<result_type()>>(
            std::bind(std::forward<F>(f),std::forward<Args>(args)...)
        );

        std::future<result_type> result=task->get_future();

        {
            std::unique_lock<std::mutex> lock(mutex_);

            if(stop_){
                throw std::runtime_error("submit on stopped threadpool\n");
            }

            tasks_.emplace([task]{
                (*task)();
            });
        }

        cv_.notify_one();
        return result;
    }
    
    
    void stop() noexcept{
        {
            std::unique_lock<std::mutex> lock(mutex_);
            stop_=true;
        }

        cv_.notify_all();

        for(auto& worker:workers_){
            if(worker.joinable()){
                worker.join();
            }
        }

        workers_.clear();
    }
    
    
    void worker_loop(){
        std::function<void()> task;

        while(true){
            std::unique_lock<std::mutex> lock(mutex_);

            cv_.wait(lock,[this]{
                return stop_||!tasks_.empty();
            });

            if(stop_&&tasks_.empty()){
                return;
            }

            task=tasks_.front();
            tasks_.pop();

            task();
        }
    }

private:
    bool stop_;
    std::mutex mutex_;
    std::vector<std::thread> workers_;
    std::condition_variable cv_;
    std::queue<std::function<void()>> tasks_;
};

















// #include <vector>
// #include <thread>
// #include <queue>
// #include <mutex>
// #include <condition_variable>
// #include <future>
// #include <functional>
// #include <stdexcept>
// #include <atomic>

// class mythreadpool {
// public:
//     explicit mythreadpool(size_t thread_count)
//         : stop_(false)
//     {
//         if (thread_count == 0) {
//             throw std::invalid_argument("thread_count must be > 0");
//         }

//         for (size_t i = 0; i < thread_count; ++i) {
//             workers_.emplace_back([this] {
//                 worker_loop();
//             });
//         }
//     }

//     ~mythreadpool() {
//         stop();
//     }

//     mythreadpool(const mythreadpool&) = delete;
//     mythreadpool& operator=(const mythreadpool&) = delete;

//     // =========================
//     // submit：返回 future
//     // =========================
//     template <class F, class... Args>
//     auto submit(F&& f, Args&&... args)
//         -> std::future<std::invoke_result_t<F, Args...>>
//     {
//         using result_type = std::invoke_result_t<F, Args...>;

//         auto task = std::make_shared<std::packaged_task<result_type()>>(
//             std::bind(std::forward<F>(f), std::forward<Args>(args)...)
//         );

//         std::future<result_type> result = task->get_future();

//         {
//             std::lock_guard<std::mutex> lock(mutex_);
//             if (stop_) {
//                 throw std::runtime_error("submit on stopped threadpool");
//             }

//             tasks_.emplace([task]() {
//                 (*task)();
//             });
//         }

//         cv_.notify_one();
//         return result;
//     }

//     void stop() noexcept {
//         {
//             std::lock_guard<std::mutex> lock(mutex_);
//             stop_ = true;
//         }

//         cv_.notify_all();

//         for (auto& t : workers_) {
//             if (t.joinable()) {
//                 t.join();
//             }
//         }

//         workers_.clear();
//     }

// private:
//     void worker_loop() {
//         while (true) {
//             std::function<void()> task;

//             {
//                 std::unique_lock<std::mutex> lock(mutex_);
//                 cv_.wait(lock, [this] {
//                     return stop_ || !tasks_.empty();
//                 });

//                 if (stop_ && tasks_.empty()) {
//                     return;
//                 }

//                 task = std::move(tasks_.front());
//                 tasks_.pop();
//             }

//             task();
//         }
//     }

// private:
//     std::vector<std::thread> workers_;
//     std::queue<std::function<void()>> tasks_;

//     std::mutex mutex_;
//     std::condition_variable cv_;
//     bool stop_;
// };
