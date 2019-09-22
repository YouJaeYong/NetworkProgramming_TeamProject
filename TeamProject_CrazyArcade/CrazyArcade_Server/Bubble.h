#pragma once
#include "Object.h"
class CBubble :
	public CObject
{
public:
	CBubble();
	virtual ~CBubble();
public:
	virtual void Initiailze();
	virtual int Update();
};

