#include <iostream>
#include <boost/thread.hpp>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <functional>

std::atomic<int> shared_i(0);
std::mutex i_mutex;
void do_inc() {
    for(std::size_t i = 0; i < 300000; i++) {
        ++shared_i;
         std::cout << shared_i << std::endl;
    }
}

void do_dec() {
    for(std::size_t i = 0; i < 300000; i++) {
        --shared_i;
         std::cout << shared_i << std::endl;
    }
}

class work_queue {
    public:
        typedef std::function<void()> task_type;
    private:
        std::deque<task_type> tasks_;
        std::mutex tasks_mutex_;
        std::condition_variable cond_;
    public:
        void push_task(task_type const & task) {
            std::unique_lock<std::mutex> lock(tasks_mutex_);
            tasks_.push_back(task);
            lock.unlock();
            cond_.notify_one();
        }

        task_type try_pop_task() {
            task_type ret;
            std::lock_guard<std::mutex> lock(tasks_mutex_);
            if (!tasks_.empty()) {
                ret = tasks_.front();
                tasks_.pop_front();
            }
            return ret;
        }

        task_type pop_task() {
            std::unique_lock<std::mutex> lock(tasks_mutex_);
            while (tasks_.empty()) {
                cond_.wait(lock);
            }
            task_type ret = tasks_.front();
            tasks_.pop_front();
            return ret;
        }
};

work_queue g_queue;

void do_nothing(){
    std::cout << "hello world" << std::endl;
}

const std::size_t tests_tasks_count = 3000;

void pusher() {
    for(std::size_t i = 0; i < tests_tasks_count; ++i) {
        g_queue.push_task(&do_nothing);
    }
}

void popper_sync() {
    for (std::size_t i = 0; i < tests_tasks_count; ++i) {
        g_queue.pop_task()();
    }
}

int main(){
    boost::thread pop_sync1(&popper_sync);
    boost::thread pop_sync2(&popper_sync);
    boost::thread pop_sync3(&popper_sync);
    
    boost::thread push1(&pusher);
    boost::thread push2(&pusher);
    boost::thread push3(&pusher);
    pop_sync1.join();
    pop_sync2.join();
    pop_sync3.join();

    push1.join();
    push2.join();
    push3.join();
    return 0;
}