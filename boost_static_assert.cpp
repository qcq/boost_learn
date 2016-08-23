#include <boost/static_assert.hpp>
#include <iostream>
using namespace std;

int main()
{
    BOOST_STATIC_ASSERT_MSG(1, "what happened!", "jajjsajj");
    return 0;
}