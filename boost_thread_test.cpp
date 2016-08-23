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
            if (!tasks_.empty()) {
                ret = tasks_.front();
                tasks_.pop_front();
            }
            return ret;
        }
};

int main(){

    boost::thread t1(&do_inc);
    boost::thread t2(&do_dec);
    std::cout << shared_i << std::endl;
    t1.join();
    t2.join();
    return 0;
}