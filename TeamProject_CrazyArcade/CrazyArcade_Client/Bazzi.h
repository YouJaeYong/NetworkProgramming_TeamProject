#pragma once
#include "Character.h"

class CBazzi :
	public CCharacter
{
private:
	CBazzi(const char* szID, int iStartingIndex, IMAGE_INFO _ImageInfo);
public:
	virtual ~CBazzi();

public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
private:
	void	ChangeState(void);
public:
	static CBazzi* Create(const char* szID, int iStartingIndex);
};

