#pragma once
#include "ReSource.h"
#include <FMOD/fmod.h>
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_codec.h>

#ifdef _DEBUG

#ifdef _WIN32
//Debug 32
#pragma comment(lib, "fmodL_vc.lib")
#else
//Debug 64
#pragma comment(lib, "fmodL64_vc.lib")
#endif

#else
#ifdef _WIN32
// Release 32
#pragma comment(lib, "fmod_vc.lib")
#else
// Release 64
#pragma comment(lib, "fmod64_vc.lib")
#endif
#endif

class CSound :
	public CReSource
{
public:
	static FMOD::System* g_pFMOD;

private:
	FMOD::Sound*		m_pSound;
	FMOD::Channel*		m_pChanel;


public:
	// -1(무한 반복)
	void Play(int _iRoopCount);
	void Stop();

public:
	virtual void Load(const wstring& _strFilePath);
	virtual bool LoadFromScene(FILE* _pFile);

public:
	CLONE_DISABLE(CSound);

public:
	CSound(RES_TYPE _etype);
	virtual ~CSound();
};