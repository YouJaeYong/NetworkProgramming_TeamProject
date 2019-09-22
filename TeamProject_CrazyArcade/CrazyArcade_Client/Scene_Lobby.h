#pragma once
#include "Scene.h"

class CClient;

class CScene_Lobby :
	public CScene
{
private:
	CScene_Lobby();
public:
	virtual ~CScene_Lobby();
public:
	virtual void Initialize(void) override;
	virtual void Update(void) override;
	virtual void Render(void) override;

	static CScene_Lobby* Create(void);

	void AddClient(CClient * pClient, int iIndex);
	virtual void InitClient(void);

};

