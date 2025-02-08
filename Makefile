TCP=$(wildcard tcp/*.cpp)

server:
	g++ -std=c++17 -pthread -g \
	$(TCP) \
	main.cpp \
	-o server
	

clean:
	rm server 