#pragma once

class IObserver;
class Subject
{
private:
	list<IObserver*> mVecObserver;

protected:
	void AddObserver(IObserver* pObserver);
	void RemoveObserver(IObserver* pObserver);
	void Notify(const UnitID& ID, const eObserverMessage& message);
};
