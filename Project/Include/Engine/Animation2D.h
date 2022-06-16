#pragma once
#include "Entity.h"
#include "ResPtr.h"
#include "Texture.h"


// Animation Frame
struct tAnim2DFrm
{
	CResPtr<CTexture>   pTex;
	Vec2				vLT;
	Vec2				vSize;
	float				vTerm;
};

class CAnimation2D :
	public CEntity
{
private:
	vector<tAnim2DFrm>  m_vecFrm;
	int					m_iCurFrm;
	float				m_fAccTime;

	bool				m_bPlay;
	bool				m_bFinish;

public:
	 void Update();
	 void Lateupdate();
	 void Finalupdate();
	void UpdateData();

public:
	void Create(const wstring& _strName, CResPtr<CTexture> _pTex, Vec2 _vLT, Vec2 _vChopSize
		, int _iFrmCount, float _fTerm);
	void Create(const wstring& _strName, const wstring& _strFolderPath, float _fTerm);
	void Play();
	void Pause() { m_bPlay = false; }
	bool IsFinish() { return m_bFinish; }
	bool IsPlay() { return m_bPlay; }
	void SetFrm(UINT _iFrm) {
		m_iCurFrm = 0; m_fAccTime = 0.f;
		if (m_vecFrm.size() - 1 != m_iCurFrm)
			m_bFinish = false;
		else
			m_bFinish = true;
	}

	void SaveToScene(FILE* _pFile);
	void LoadFromScene(FILE* _pFile);

public:
	static void ClearRegister();

public:
	CLONE(CAnimation2D);
public:
	CAnimation2D();
	virtual ~CAnimation2D();
};

