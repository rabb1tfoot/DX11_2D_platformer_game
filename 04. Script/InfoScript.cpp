#include "stdafx.h"
#include "InfoScript.h"
#include <ResPtr.h>
#include <Sound.h>
#include <SceneMgr.h>


CInfoScript::CInfoScript()
	: CScript((UINT)SCRIPT_TYPE::INFOSCRIPT)
	, m_bImmune(false)
	, m_iPlayerHp(10)
	, m_iPlayerWeapon(-1)
	, m_iActiveWeapon(-1)
	, m_pSound(nullptr)
{
}


CInfoScript::~CInfoScript()
{
}

void CInfoScript::Update()
{ 
	if (!m_bSound)
	{
		m_pSound = CSceneMgr::GetInst()->GetSound();

		if (m_pSound == nullptr)
		{
			m_pSound = CResMgr::GetInst()->Load<CSound>(L"Sound\\The Axiom.mp3", L"Sound\\The Axiom.mp3");
			m_pSound->Play(-1);
			m_pSound->SetName(L"title");
			CSceneMgr::GetInst()->SetSound(m_pSound);
		}
	}
	m_bSound = true;
}

void CInfoScript::SetPlayerHpdown()
{
	 if (!m_bImmune)
		 --m_iPlayerHp; 
}
