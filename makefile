CC=g++
INCLUDE=-I /usr/local/include -std=c++11
LIB=-L /usr/local/lib
ALLSOURCE=program_options.cpp boost_any.cpp

program_options : program_options.cpp
	$(CC) -o program_options program_options.cpp $(INCLUDE) $(LIB) -lboost_program_options
boost_any : boost_any.cpp
	$(CC) -o boost_any boost_any.cpp $(INCLUDE)
boost_optional : boost_optional.cpp
	$(CC) -o boost_optional boost_optional.cpp $(INCLUDE)
boost_variant : boost_variant.cpp
	$(CC) -o boost_variant boost_variant.cpp $(INCLUDE)
boost_array : boost_array.cpp
	$(CC) -o boost_array boost_array.cpp $(INCLUDE)
boost_bind : boost_bind.cpp
	$(CC) -o boost_bind boost_bind.cpp $(INCLUDE)
boost_spirit : boost_spirit.cpp
	$(CC) -o boost_spirit boost_spirit.cpp $(INCLUDE)
boost_ptr : boost_ptr.cpp
	$(CC) -o boost_ptr boost_ptr.cpp $(INCLUDE) -lboost_thread -l boost_system
boost_static_assert : boost_static_assert
	$(CC) -o boost_static_assert boost_static_assert.cpp $(INCLUDE)
boost_thread_test : boost_thread_test.cpp
	$(CC) -o boost_thread_test boost_thread_test.cpp $(INCLUDE) $(LIB) -lboost_thread-mt -lboost_system
clean :
	rm ./hello ./boost_any ./boost_optional ./boost_variant ./boost_array ./boost_spirit
