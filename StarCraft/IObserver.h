#pragma once

class Subject;
class IObserver abstract
{
	friend Subject;
protected:
	virtual void OnNotify(const UnitID& ID, const eObserverMessage& message) abstract;
};

