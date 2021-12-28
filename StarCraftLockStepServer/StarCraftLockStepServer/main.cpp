#include "stdafx.h"
#include "Server.h"

int main()
{
	boost::asio::io_service ioService;
	Server server(ioService);
	server.Start();
	ioService.run();

	std::wcout << "匙飘况农 立加 辆丰\n";

	getchar();

	return 0;
}