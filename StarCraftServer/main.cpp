#include "stdafx.h"
#include "Server.h"

int main()
{
	boost::asio::io_service ioService;
	Server server(ioService);
	server.Start();
	ioService.run();

	std::cout << "��Ʈ��ũ ���� ����" << std::endl;

	getchar();

	return 0;
}