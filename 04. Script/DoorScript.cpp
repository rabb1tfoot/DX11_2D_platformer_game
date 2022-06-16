#include "stdafx.h"
#include "DoorScript.h"
#include <Layer.h>
#include <Scene.h>
#include <SceneMgr.h>
#include <GameObj.h>
#include <PathMgr.h>
#include <func.h>
#include "CameraScipt.h"
#include "BlackoutScript.h"

void CDoorScript::SetMoveScenePoint(Vec2 _vPoint, wstring _wSceneName)
{
	m_vMovePoint = _vPoint;
	m_wSceneName = _wSceneName;
}

void CDoorScript::OnCollisionEnter(CCollider2D * _pOther)
{
	wstring strName = _pOther->Object()->GetName();

	if (!wcscmp(strName.c_str(), L"Player"))
	{
		CLayer** pLayer = CSceneMgr::GetInst()->GetCurScene()->GetAllLayer();
		CGameObj* pObjGround = nullptr;
		//페이드아웃 이미지를 찾는다.
		vector<CGameObj*> vObjDefault = pLayer[0]->GetAllObject();
		for (int i = 0; i < vObjDefault.size(); ++i)
		{
			if (!wcscmp(vObjDefault[i]->GetName().c_str(), L"BlackImage2"))
			{
				pObjGround = vObjDefault[i];
			}
		}
		//페이드 아웃시킨다.
		CScript* vBlackImageScript = pObjGround->GetScript<CBlackoutScript>();
		((CBlackoutScript*)vBlackImageScript)->SetFadeOff();

		//현재 매인캠을 찾는다.
		CGameObj* pObjCam = nullptr;
		for (int i = 0; i < vObjDefault.size(); ++i)
		{
			if (!wcscmp(vObjDefault[i]->GetName().c_str(), L"MainCam"))
			{
				pObjCam = vObjDefault[i];
			}
		}
		//씬이동을 매인캠에서 호출한다.
		CScript* vCamScript = pObjCam->GetScript<CCameraScipt>();

		((CCameraScipt*)vCamScript)->SetSceneMove(m_vMovePoint, m_wSceneName);

		//UI를  지운다.
		vector<CGameObj*> vObjUI = pLayer[5]->GetParentObject();
		for (int i = 0; i < vObjUI.size(); ++i)
		{
			if (vObjUI.size() == 0)
				return;
			vObjUI[i]->SetActive(false);
		}

	}
	return;
}

void CDoorScript::SaveToScene(FILE * _pFile)
{
	fwrite(&m_vMovePoint, sizeof(XMFLOAT2), 1, _pFile);
	SaveWString(m_wSceneName.c_str(), _pFile);
}

CComponent * CDoorScript::LoadFromScene(FILE * _pFile)
{
	fread(&m_vMovePoint, sizeof(XMFLOAT2), 1, _pFile);
	m_wSceneName = LoadWString(_pFile);
	return nullptr;
}


CDoorScript::CDoorScript()
	: CScript((UINT)SCRIPT_TYPE::DOORSCRIPT)
{
}


CDoorScript::~CDoorScript()
{
}
