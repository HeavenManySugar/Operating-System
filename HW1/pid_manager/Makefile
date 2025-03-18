all:
	g++ -std=c++14 -isystem /usr/include/gtest -pthread pid_manager.c test_pid_manager.cpp -o test_pid_manager -lgtest -lgtest_main
	./test_pid_manager

clean:
	rm test_pid_manager