#pragma once
#include "Object.h"

class CBubbleFlow;

enum E_FLOW_STATE
{
	E_FLOW_CENTER = 0,
	E_FLOW_UP,
	E_FLOW_DOWN,
	E_FLOW_RIGHT,
	E_FLOW_LEFT,

	//E_FLOW_UP =  5 , 
	//E_FLOW_DOWN	= 6 ,
	//E_FLOW_RIGHT = 7,
	//E_FLOW_LEFT = 8 , 

};
class CBubbleFlow_List :
	public CObject
{
private:
	CBubbleFlow_List(VECTOR2 vCenter, int iPower, E_FLOW_STATE eCenterFlow);
public:
	virtual ~CBubbleFlow_List();
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);

	vector<CBubbleFlow*>* GetVecFlow(void) { return &m_vecBubbleFlow; }
	static CBubbleFlow_List* Create(VECTOR2 vCenter, int iPower, E_FLOW_STATE eCenterFlow = E_FLOW_CENTER);
	void AddBubbleFlow(CBubbleFlow* _Flow) { m_vecBubbleFlow.push_back(_Flow); }

	//void DestoryFlow(const char* szKey);
	void DestoryFlow(CBubbleFlow* pBubble, int iOffsetIndex = 0);
private:
	int								m_iPower;
	vector<CBubbleFlow*>			m_vecBubbleFlow;
};

