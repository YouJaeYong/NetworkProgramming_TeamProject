#pragma once
#include "Scene.h"

class CScene_Login :
	public CScene
{
private:
	CScene_Login();
public:
	virtual ~CScene_Login();
public:
	virtual void Initialize(void) override;
	virtual void Update(void) override;
	virtual void Render(void) override;


public:
	static CScene_Login* Create(void);
};

