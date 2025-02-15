TCP=$(wildcard tcp/*.cpp)

server:
	g++ -std=c++17 -pthread -g \
	$(TCP) \
	main.cpp \
	-o ./bin/server-release
	

clean:
	rm ./bin/server* 