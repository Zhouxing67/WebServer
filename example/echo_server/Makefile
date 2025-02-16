TCP=$(wildcard ../../tcp/*.cpp)
server:
	g++ -std=c++17 -pthread -g \
	$(TCP) \
	echo_server.cpp \
	main.cpp \
	-o ../../bin/echo_server

clean:
	rm ../../bin/echo_server