//
//  boost_any.cpp
//  C++
//
//  Created by 秦传庆 on 8/15/16.
//  Copyright © 2016 秦传庆. All rights reserved.
//

#include <boost/any.hpp>
#include <iostream>
#include <vector>
#include <string>

int main2()
{
    std::vector<boost::any> some_values;
    some_values.push_back(10);
    const char * c_str = "Hello World!";
    some_values.push_back(c_str);
    some_values.push_back(std::string("wow"));
    std::string& s = boost::any_cast<std::string&>(some_values.back());
    
    s += " That is great!\n";
    std::cout << s;
    return 0;
}
