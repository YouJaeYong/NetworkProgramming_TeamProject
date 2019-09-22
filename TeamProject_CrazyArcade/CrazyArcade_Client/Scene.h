#pragma once
class CUI;

class CScene
{
public:
	CScene();
	virtual ~CScene();
public:
	virtual void Initialize(void)=0;
	virtual void Update(void)=0;
	virtual void Render(void)=0;

	void AddUI(CUI* pUI, const char* pKey = "");
	void UISortByLevel(void);
	void UIUpdate(void);
	void UIRender(void);

	CUI* FindUI(const char* strKey);
private:
	static bool SortUI(CUI* pDest, CUI* pSrc);
	void Release(void);
private:
	vector<CUI*>						m_vecUI;
	//map<const char*, CUI*>			m_mapUI;
};