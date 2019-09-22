#pragma once
#include "Scene.h"
class CScene_Test :
	public CScene
{
private:
	CScene_Test();
public:
	virtual ~CScene_Test();
public:
	virtual void Initialize(void) override;
	virtual void Update(void) override;
	virtual void Render(void) override;

	static CScene_Test* Create(void);
	virtual void InitClient(void);
	void CreateCharacter(void);

	void SetChatting(void);
	bool IsChatting(void) { return m_IsChatting; };
private:
	void Release();

	bool m_IsChatting; //Àç¿ë
};
