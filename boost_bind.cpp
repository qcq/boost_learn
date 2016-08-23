#include <boost/mem_fn.hpp>
#include <iostream>
#include <algorithm>
#include <boost/array.hpp>
//#include <boost/get_pointer.hpp>
#include <functional>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>




using namespace boost;


int main3()
{
//    std::bind;
    boost::array<int, 12> values = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    std::cout << std::count_if(values.begin(), values.end(), std::bind1st(std::less<int>(), 5)) << std::endl;
    std::cout << std::count_if(values.begin(), values.end(), std::bind(std::less<int>(), 5, std::placeholders::_1)) << std::endl;
    
    boost::array<std::string, 3> str = {"sdadasd", "a", "wd"};
    std::cout << std::count_if(str.begin(), str.end(),
            boost::bind(std::less<std::size_t>(),
                      boost::bind(&std::string::size, std::placeholders::_1), 5));
    return 0;
}