#include "stdafx.h"
#include "Subject.h"
#include "IObserver.h"

void Subject::AddObserver(IObserver* pObserver)
{
	mVecObserver.push_back(pObserver);
}

void Subject::RemoveObserver(IObserver* pObserver)
{
	auto it = find(mVecObserver.begin(), mVecObserver.end(), pObserver);
	if (it != mVecObserver.end())
	{
		mVecObserver.erase(it);
	}
	else
	{
		cout << "그럴수가 있나? " << endl;
	}
}

void Subject::Notify(const UnitID& ID, const eObserverMessage& message)
{
	for (auto it = mVecObserver.begin(); it != mVecObserver.end(); ++it)
	{
		(*it)->OnNotify(ID, message);
	}
}
