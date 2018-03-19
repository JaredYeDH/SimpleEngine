#include "defines.h"

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include "asio.hpp"
#include "DebugServer.h"




int main(int argc, char* argv[])
{
	int port = 4711;
	try
	{
		if (argc < 2)
		{
			std::cerr << "Usage: chat_server <port> [<port> ...]\n";
			return 1;
		}
		
		DebugServer server(port);
		server.listen();
		
		
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}
