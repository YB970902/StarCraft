#pragma once

class IObserver abstract
{
public:
	virtual void Notice(Message* pMsg) = 0;
};

class ISubject abstract
{
private:
	std::vector<IObserver*> mVecObserver;
protected:
	void RegistObserver(IObserver* pObserver);
	void ReleaseObserver(IObserver* pObserver);
	virtual void Execute(Message* pMsg) = 0;
};