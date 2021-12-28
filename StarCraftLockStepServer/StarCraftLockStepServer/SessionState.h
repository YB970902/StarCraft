#pragma once

enum class eSessionState
{
	Connecting,
	Lobby,
	Room,
	InGame,
};

class Session;
class IState
{
protected:
	Session* mpSession = nullptr;
public:
	IState(Session* pSession) :mpSession(pSession) {}

	virtual void Enter() = 0;
	virtual void Exit() = 0;
	virtual void ProcessMessage(Message* pMsg) = 0;
};

class SessionState
{
private:
	Session* mpSession = nullptr;

	std::unordered_map<eSessionState, IState*> mMapState;
	IState* mpCurState = nullptr;
public:
	SessionState() = delete;
	SessionState(Session* pSession);

	void Init();
	void Release();

	void AddState(eSessionState tag, IState* pState);
	void ChangeState(eSessionState tag);

	void Process(Message* pMsg);
};

class ConnectingState : public IState
{
public:
	ConnectingState(Session* pSession) : IState(pSession) { }
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void ProcessMessage(Message* pMsg) override;
};

class LobbyState : public IState
{
public:
	LobbyState(Session* pSession) : IState(pSession) { }
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void ProcessMessage(Message* pMsg) override;
};

class RoomState : public IState
{
public:
	RoomState(Session* pSession) : IState(pSession) { }
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void ProcessMessage(Message* pMsg) override;
};

class InGameState : public IState
{
public:
	InGameState(Session* pSession) : IState(pSession) { }
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void ProcessMessage(Message* pMsg) override;
};