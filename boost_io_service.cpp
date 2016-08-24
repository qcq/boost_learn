#include <boost/thread/thread.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/system/error_code.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <boost/asio/deadline_timer.hpp>
#include <boost/make_shared.hpp>


namespace detail {
    template <class T>
    struct task_wrapped
    {
        private:
            T task_unwrapped_;

        public:
            explicit task_wrapped(const T& task_unwrapped) : task_unwrapped_(task_unwrapped)
            {

            }

            void operator()()const {
                try {
                    boost::this_thread::interruption_point();
                } catch (const boost::thread_interrupted&)
                {

                }
                try {
                    task_unwrapped_();
                } catch (const std::exception& e) {
                    std::cerr << "Exception: " << e.what() << '\n';
                } catch (const boost::thread_interrupted&) {
                    std::cerr << "Thread interrupted\n";
                } catch (...) {
                    std::cerr << "Unknown exception\n";
                }
            }
    };

    template <class T>
    task_wrapped<T> make_task_wrapped(const T& task_unwrapped)
    {
        return task_wrapped<T>(task_unwrapped);
    };

    typedef boost::asio::deadline_timer::duration_type duration_type;

    template <class Functor>
    struct timer_task : public task_wrapped<Functor>
    {
    private:
        typedef task_wrapped<Functor> base_t;
        boost::shared_ptr<boost::asio::deadline_timer> timer_;
    public:
        template <class Time>
        explicit timer_task(boost::asio::io_service& ios, const Time& duration_or_time, const Functor& task_unwrapped)
            : base_t(task_unwrapped), timer_(boost::make_shared<boost::asio::deadline_timer>(boost::ref(ios), duration_or_time))
        {

        }

        void push_task() const {
            timer_->async_wait(*this);
        }

        void operator()(const boost::system::error_code& error) const {
            if (!error) {
                base_t::operator()();
            } else {
                std::cerr << error << std::endl;
            }
        }

    };

    template <class Time, class Functor>
    inline timer_task<Functor> make_timer_task(boost::asio::io_service& ios, const Time& duration_or_time, const Functor& task_unwrapped)
    {
        return timer_task<Functor>(ios, duration_or_time, task_unwrapped);

    };


}



class tasks_processor : private boost::noncopyable {
    boost::asio::io_service ios_;
    boost::asio::io_service::work work_;

    tasks_processor() : ios_(), work_(ios_)
    {

    }

public:
    static tasks_processor* get() {
        static tasks_processor* singleton;
        if (!singleton) {
            singleton = new tasks_processor();
        }
        return singleton;
    }

    template <class T>
    inline void push_task(const T& task_unwrapped) {
        ios_.post(detail::make_task_wrapped(task_unwrapped));
    }

    void start() {
        ios_.run();
    }

    void stop() {
        ios_.stop();
    }

    typedef boost::asio::deadline_timer::time_type time_type;
    template <class Functor>
    void run_at(time_type time, const Functor& f) {
        detail::make_timer_task(ios_, time, f).push_task();
    }

    typedef boost::asio::deadline_timer::duration_type duration_type;
    template <class Functor>
    void run_after(duration_type duration_or_time, const Functor& f) {
        detail::make_timer_task(ios_, duration_or_time, f).push_task();
    }
};

int g_val = 0;
void func_test() {
    ++g_val;
    std::cout << g_val << std::endl;
    if(g_val == 3) {
        throw std::logic_error("just checking");
    }
    boost::this_thread::interruption_point();
    if(g_val == 10) {
        throw boost::thread_interrupted();
    }
    if (g_val == 90) {
        tasks_processor::get()->stop();
    }
}

int main() {
    static const std::size_t tasks_count = 100;
    BOOST_STATIC_ASSERT(tasks_count >= 90);
    for (std::size_t i = 0; i < tasks_count; ++i) {
        tasks_processor::get()->push_task(&func_test);
    }
    tasks_processor::get()->push_task(boost::bind(std::plus<int>(), 2, 2));
    assert(g_val == 0);
    tasks_processor::get()->start();
    assert(g_val == 90);
    return 0;
}