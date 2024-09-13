#include <WinSock2.h>

#include <thread>
#include <iostream>

#include <net.hpp>

int main(int argc, char** argv) {
	/*send();
	recv();
	accept();
	bind();
	closesocket();
	socket();
	connect();
	listen();
	shutdown();
	*
	* 
	* создать
	* отправить
	* принять
	* 
	*/

	std::string buff;
	net::tcp_client tcp_cli{ "localhost", "50000" };

	try {
		tcp_cli.connect();
		tcp_cli.send("aaaaaaaaaaaaaaaaaaaaaaaaaa");
		tcp_cli.recv(buff);
	}
	catch (std::exception& ex) {
		std::cout << ex.what();
	}
	return 0;
}