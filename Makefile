src=$(wildcard src/*.cpp)

server:
	g++ -std=c++17 -pthread -g \
	$(src) \
	main.cpp \
	-o server
	
th:
	g++ -pthread src/ThreadPool.cpp ThreadPoolTest.cpp -o ThreadPoolTest

test:
	g++ src/Util.cpp src/Buffer.cpp src/Socket.cpp src/InetAddress.cpp src/ThreadPool.cpp \
	-pthread \
	test.cpp -o test

clean:
	rm server && rm test