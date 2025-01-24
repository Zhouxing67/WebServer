server:
	g++ \
		src/Util.cpp \
		main.cpp \
		src/InetAddress.cpp \
		src/Socket.cpp \
		src/Channel.cpp \
		src/EventLoop.cpp \
		src/Server.cpp \
		src/Acceptor.cpp \
		src/Connection.cpp \
		src/Buffer.cpp \
		-o server

clean:
	rm server
