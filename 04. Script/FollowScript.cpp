#include "stdafx.h"
#include "FollowScript.h"
#include <Layer.h>
#include <Scene.h>
#include <SceneMgr.h>


CFollowScript::CFollowScript()
	:CScript((UINT)SCRIPT_TYPE::FOLLOWSCRIPT)
{
}


CFollowScript::~CFollowScript()
{
}

void CFollowScript::awake()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer** pLayer = pCurScene->GetAllLayer();
	vector<CGameObj*> vecObj = pLayer[3]->GetParentObject();
	for (int i = 0; i < vecObj.size(); ++i)
	{
		if (!wcscmp(m_wFollowedName.c_str(), vecObj[i]->GetName().c_str()))
		{
			m_pObject = vecObj[i];
		}
	}
}

void CFollowScript::Update()
{
	if (nullptr == m_pObject)
		return;
	Vec3 FollowedPos  = m_pObject->Transform()->GetLocalPos();

	Transform()->SetLocalPos(FollowedPos);
}