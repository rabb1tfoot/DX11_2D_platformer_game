#include "stdafx.h"
#include "Animator2D.h"

#include "Animation2D.h"


CAnimator2D::CAnimator2D()
	:CComponent(COMPONENT_TYPE::ANIMATION2D)
	, m_bPlayFirst(false)
	, m_bRepeat(false)
{
}

CAnimator2D::CAnimator2D(const CAnimator2D & _other)
	: CComponent(_other)
	, m_pCurAnim(nullptr)
	, m_bPlayFirst(_other.m_bPlayFirst)
	, m_bRepeat(_other.m_bRepeat)
{
	for (auto& pair : _other.m_mapAnim)
	{
		m_mapAnim.insert(make_pair(pair.first, pair.second->Clone()));
	}

	if (nullptr != _other.m_pCurAnim)
	{
		map<wstring, CAnimation2D*>::iterator iter = m_mapAnim.find(_other.m_pCurAnim->GetName());
		if (iter != m_mapAnim.end())
		{
			m_pCurAnim = iter->second;
		}
	}
}

CAnimator2D::~CAnimator2D()
{
	Safe_Delete_Map(m_mapAnim);
}

void CAnimator2D::awake()
{
}

void CAnimator2D::start()
{
	if (nullptr != m_pCurAnim && m_bPlayFirst)
		m_pCurAnim->Play();
	else
		m_pCurAnim->Pause();
}

void CAnimator2D::Update()
{
	if (nullptr == m_pCurAnim)
		return; 

		m_pCurAnim->Update();
}

void CAnimator2D::Lateupdate()
{
	if (nullptr == m_pCurAnim)
		return;

		m_pCurAnim->Lateupdate();
}

void CAnimator2D::Finalupdate()
{
	if (nullptr == m_pCurAnim)
		return;

	m_pCurAnim->Finalupdate();

	if (m_pCurAnim->IsFinish() && m_bRepeat)
	{
		m_pCurAnim->SetFrm(0);
		m_pCurAnim->Play();
	}
}

void CAnimator2D::UpdateData()
{
	if (nullptr != m_pCurAnim)
		m_pCurAnim->UpdateData();
}

void CAnimator2D::AddAnimation(const wstring & _strKey, CResPtr<CTexture> _pTex
	, Vec2 _vLT, Vec2 _vCropSize, int _iFrmCount, float _fTerm)
{
	assert(!FindAnim(_strKey));
	assert(nullptr != _pTex);

	CAnimation2D* pAnim = new CAnimation2D;
	pAnim->Create(_strKey, _pTex, _vLT, _vCropSize, _iFrmCount, _fTerm);

	m_mapAnim.insert(make_pair(_strKey, pAnim));
}

void CAnimator2D::AddAnimation(const wstring & _strKey, const wstring& _strFolder, float _fTerm)
{
	CAnimation2D* pAnim = new CAnimation2D;
	pAnim->Create(_strKey, _strFolder, _fTerm);
	m_mapAnim.insert(make_pair(_strKey, pAnim));
}

void CAnimator2D::PlayAnimation(const wstring & _strKey, bool _bRepeat)
{
	CAnimation2D* pAnim = FindAnim(_strKey);
	assert(pAnim);

	m_pCurAnim = pAnim;
	m_pCurAnim->Play();
	m_bRepeat = _bRepeat;
}


CAnimation2D * CAnimator2D::FindAnim(const wstring & _strKey)
{
	map<wstring, CAnimation2D*>::iterator iter = m_mapAnim.find(_strKey);
	if (iter == m_mapAnim.end())
		return nullptr;
	return iter->second;
}

void CAnimator2D::DeleteAnimation(const wstring & _strKey)
{
	map<wstring, CAnimation2D*>::iterator iter = m_mapAnim.find(_strKey);
	if (iter == m_mapAnim.end())
		assert(nullptr);

	SAFE_DELETE(iter->second);
	m_mapAnim.erase(iter);
}

void CAnimator2D::Pause()
{
	if (nullptr != m_pCurAnim)
	{
		m_pCurAnim->Pause();
	}
}


void CAnimator2D::SaveToScene(FILE * _pFile)
{
	fwrite(&m_bPlayFirst, sizeof(bool), 1, _pFile);
	fwrite(&m_bRepeat, sizeof(bool), 1, _pFile);

	//애니메이션 갯수 저장
	UINT iSize = (UINT)m_mapAnim.size();
	fwrite(&iSize, sizeof(UINT), 1, _pFile);

	//각 애니메이션 저장
	for (const auto& pair : m_mapAnim)
	{
		pair.second->SaveToScene(_pFile);
	}

	//현재 애니메이션 저장
	bool bCurAnim = (bool)m_pCurAnim;
	fwrite(&bCurAnim, sizeof(bool), 1, _pFile);

	if (false == bCurAnim)
		return;

	SaveWString(m_pCurAnim->GetName().c_str(), _pFile);
}

CComponent* CAnimator2D::LoadFromScene(FILE * _pFile)
{
	fread(&m_bPlayFirst, sizeof(bool), 1, _pFile);
	fread(&m_bRepeat, sizeof(bool), 1, _pFile);

	//애니메이션 갯수만큼 로딩
	UINT iSize = 0;
	fread(&iSize, sizeof(UINT), 1, _pFile);

	for (UINT i = 0; i < iSize; ++i)
	{
		CAnimation2D* pAnim2D = new CAnimation2D;
		pAnim2D->LoadFromScene(_pFile);

		m_mapAnim.insert(make_pair(pAnim2D->GetName(), pAnim2D));
	}

	
	//현재 애니메이션 로딩
	bool bCurAnim = false;
	fread(&bCurAnim, sizeof(bool), 1, _pFile);

	if (false == bCurAnim)
		return nullptr;

	wstring strCurAnim = LoadWString(_pFile);
	m_pCurAnim = m_mapAnim.find(strCurAnim)->second;
	return nullptr;
}