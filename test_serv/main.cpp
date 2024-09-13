#include <net.hpp>

#include <iostream>
#include <list>
#include <conio.h>

int main(int argc, char** argv) {
	net::tcp_server tcp_serv{ "50000" };
	
	try {
		tcp_serv.listen();

		while (true) {
			int symbol = _getch();
			if (symbol == 27)
				break;
		}
		std::cout << "Hella!\n";
	}
	catch (std::exception& ex) {
		std::cout << ex.what();
	}

	return 0;
}