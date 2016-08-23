#include <boost/array.hpp>
#include <algorithm>

struct add_1 : public std::unary_function<char, void>
{
    void operator()(char& c) const {
        ++c;
    }
};

typedef boost::array<char, 4> array4_t;

array4_t& vector_advance(array4_t& val)
{
    std::for_each(val.begin(), val.end(), add_1());
    return val;
}

int main() {
    array4_t val = {0, 1, 2, 3};
    array4_t val_ref;
    val_ref = vector_advance(val);
    assert(val.size() == 4);
    assert(1 == val[0]);
    assert(sizeof(val) == sizeof(char) * array4_t::static_size);
    return 0;
}