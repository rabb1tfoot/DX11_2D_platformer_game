#include "stdafx.h"
#include "Sound.h"

#include "ResMgr.h"

FMOD::System* CSound::g_pFMOD = nullptr;

CSound::CSound(RES_TYPE _etype)
	: m_pSound(nullptr)
	, m_pChanel(nullptr)
	, CReSource(_etype)
{
}

CSound::~CSound()
{
	if (nullptr != m_pSound)
	{
		m_pSound->release();
		m_pSound = nullptr;
	}
}

void CSound::Play(int _iRoopCount)
{
	if (0 == _iRoopCount)
	{
		return;
	}
	else if (_iRoopCount < -1)
	{
		assert(nullptr);
	}
	else if (_iRoopCount == -1)
	{
		// 무한반복
	}

	bool bPlaying = false;
	if (m_pChanel)
	{
		m_pChanel->isPlaying(&bPlaying);

		FMOD::Sound* pCurSound = nullptr;
		m_pChanel->getCurrentSound(&pCurSound);

		if (bPlaying && m_pSound == pCurSound)
			m_pChanel->stop();
	}

	_iRoopCount -= 1;

	g_pFMOD->playSound(m_pSound, nullptr, false, &m_pChanel);
	m_pChanel->setMode(FMOD_LOOP_NORMAL);
	m_pChanel->setLoopCount(_iRoopCount);
}

void CSound::Stop()
{
	if (m_pChanel)
	{
		FMOD::Sound* pCurSound = nullptr;
		m_pChanel->getCurrentSound(&pCurSound);

		if (m_pSound == pCurSound)
			m_pChanel->stop();
	}
}

void CSound::Load(const wstring& _strFilePath)
{
	string path(_strFilePath.begin(), _strFilePath.end());

	if (FMOD_OK != g_pFMOD->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &m_pSound))
	{
		assert(nullptr);
	}
}

bool CSound::LoadFromScene(FILE * _pFile)
{
	CReSource::LoadFromScene(_pFile);

	CResPtr<CSound> pTex = CResMgr::GetInst()->FindRes<CSound>(GetName().c_str());
	if (nullptr != pTex)
		return false;

	wstring strPath = CPathMgr::GetResPath();
	strPath += GetPath();
	Load(strPath);

	return true;
}