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
		//���̵�ƿ� �̹����� ã�´�.
		vector<CGameObj*> vObjDefault = pLayer[0]->GetAllObject();
		for (int i = 0; i < vObjDefault.size(); ++i)
		{
			if (!wcscmp(vObjDefault[i]->GetName().c_str(), L"BlackImage2"))
			{
				pObjGround = vObjDefault[i];
			}
		}
		//���̵� �ƿ���Ų��.
		CScript* vBlackImageScript = pObjGround->GetScript<CBlackoutScript>();
		((CBlackoutScript*)vBlackImageScript)->SetFadeOff();

		//���� ����ķ�� ã�´�.
		CGameObj* pObjCam = nullptr;
		for (int i = 0; i < vObjDefault.size(); ++i)
		{
			if (!wcscmp(vObjDefault[i]->GetName().c_str(), L"MainCam"))
			{
				pObjCam = vObjDefault[i];
			}
		}
		//���̵��� ����ķ���� ȣ���Ѵ�.
		CScript* vCamScript = pObjCam->GetScript<CCameraScipt>();

		((CCameraScipt*)vCamScript)->SetSceneMove(m_vMovePoint, m_wSceneName);

		//UI��  �����.
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
