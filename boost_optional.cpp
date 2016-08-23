#include <boost/optional.hpp>
#include <iostream>
#include <stdlib.h>

class locked_device {
	explicit locked_device(const char * here) {
		std::cout << "Device is locked" << std::endl;
	}
public:
	~locked_device() {
	}
	void use() {
		std::cout << "Sucess" << std::endl;
	}
	static boost::optional<locked_device> try_lock_device() {
		if (rand() % 2) {
			return boost::none;
		}
		return locked_device("device name");
	}
};
int main()
{
	for (int i = 0; i < 10; ++i) {
		auto t = locked_device::try_lock_device();
		if (t) {
			t->use();
			return 0;
		} else {
			std::cout << "....Trying again" << std::endl;
		}
	}
	std::cout << "Failure" << std::endl;
	return -1;
}
