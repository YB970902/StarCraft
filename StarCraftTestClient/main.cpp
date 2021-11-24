#include "stdafx.h"
#include "MainGame.h"

int main()
{
	TIME->Init(30);
	boost::asio::io_service ioService;
	NET->Init(ioService);

	boost::thread thread(boost::bind(&boost::asio::io_service::run, &ioService));

	MainGame* pMainGame = new MainGame();
	pMainGame->Init();

	char inputMessage[MAX_TEXT_LEN] = { 0, };
	while (true)
	{
		if (TIME->IsUpdateTime())
		{
			// 업데이트
			pMainGame->Update();
			pMainGame->Render();
		}
	}

	pMainGame->Release();

	NET->Release();

	getchar();

	return 0;
}