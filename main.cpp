//
//  main.cpp
//  C++
//
//  Created by 秦传庆 on 8/14/16.
//  Copyright © 2016 秦传庆. All rights reserved.
//

#include <iostream>
#include <boost/program_options.hpp>
#include <boost/program_options/errors.hpp>

namespace opt = boost::program_options;

int main1(int argc, const char * argv[]) {
    // insert code here...
    opt::options_description desc("All options");
    desc.add_options()
        ("apples,a", opt::value<int>()->default_value(10), "apples that you have")
        ("oranges,o", opt::value<int>(), "organges that you have")
        ("name", opt::value<std::string>(), "your name")
        ("help", "produce help message");
    opt::variables_map vm;
    opt::store(opt::parse_command_line(argc, argv, desc), vm);
    opt::notify(vm);
    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 1;
    }
    try {
        opt::store(opt::parse_config_file<char>("apples_oranges.cfg", desc), vm);
    } catch (const opt::reading_file& e) {
        std::cout << "Failed to open file" << e.what() << std::endl;
    }
    opt::notify(vm);
    if (vm.count("name")) {
        std::cout << "Hi, " << vm["name"].as<std::string>() << std::endl;
    }
    
    std::cout << "Fruits count:" << vm["apples"].as<int>() + vm["oranges"].as<int>() << std::endl;
    return 0;
}
