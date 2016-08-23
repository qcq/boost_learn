#include <boost/variant.hpp>
#include <iostream>
#include <vector>
#include <string>

int main()
{
	typedef boost::variant<int, const char*, std::string> my_var_t;
	std::vector<my_var_t> some_values;
	some_values.push_back(10);
	some_values.push_back("Hello There");
	some_values.push_back(std::string("WoW!"));
	std::string& s = boost::get<std::string>(some_values.back());
	s += " That is niubi";
	std::cout << s << std::endl;
	return 0;
}
