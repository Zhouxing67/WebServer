src=$(wildcard src/*.cpp)

server:
	g++ -std=c++17 -pthread -g \
	$(src) \
	main.cpp \
	-o server


clean:
	rm server 