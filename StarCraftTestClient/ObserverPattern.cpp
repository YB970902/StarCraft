#include "stdafx.h"
#include "ObserverPattern.h"

void ISubject::RegistObserver(IObserver* pObserver)
{
	mVecObserver.push_back(pObserver);
}

void ISubject::ReleaseObserver(IObserver* pObserver)
{
	for (auto it = mVecObserver.begin(); it != mVecObserver.end(); ++it)
	{
		if ((*it) == pObserver)
		{
			mVecObserver.erase(it);
			break;
		}
	}
}
