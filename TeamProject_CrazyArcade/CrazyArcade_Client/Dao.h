#pragma once
#include "Character.h"
class CDao :
	public CCharacter
{
private:
	CDao(const char* szID, int iStartingIndex, IMAGE_INFO _ImageInfo);
public:
	virtual ~CDao();
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);

	static CDao* Create(const char* szID, int iStartingIndex);

private:
	void	ChangeState(void);
};
