#include "stdafx.h"
#include "CameraScipt.h"
#include "TimeMgr.h"
#include <SceneMgr.h>
#include <Scene.h>
#include <Layer.h>
#include <GameObj.h>
#include <SaveLoadMgr.h>

CCameraScipt::CCameraScipt()
	: CScript((UINT)SCRIPT_TYPE::CAMERASCIPT)
	, m_fSpeed(200.f)
	, m_bChasePlayer(true)
	, m_bSceneMove(false)
	, m_fAccTime(0.f)
	, m_vCamBoundLT{}
	, m_vCamBoundRB{}
{
}


CCameraScipt::~CCameraScipt()
{
}

void CCameraScipt::Update()
{
	if (!m_bChasePlayer || !m_bSceneMove)
	{
		Vec2 vPos = { Transform()->GetLocalPos().x, Transform()->GetLocalPos().y };

		if (KEYHOLD(KEY_TYPE::KEY_A))
		{
			vPos.x -= 600.f * DT;
		}
		if (KEYHOLD(KEY_TYPE::KEY_D))
		{
			vPos.x += 600.f * DT;

		}
		if (KEYHOLD(KEY_TYPE::KEY_W))
		{
			vPos.y += 600.f * DT;

		}
		if (KEYHOLD(KEY_TYPE::KEY_S))
		{
			vPos.y -= 600.f * DT;
		}

		Transform()->SetLocalPos(Vec3(vPos.x, vPos.y, 0.f));

	}
	if (m_bChasePlayer)
	{
		ChasingPlayer();
	}

	//씬 변경시 효과
	if (m_bSceneMove)
	{
		SceneMove();
	}
}

void CCameraScipt::SaveToScene(FILE * _pFile)
{
	fwrite(&m_vCamBoundLT, sizeof(XMFLOAT2), 1, _pFile);
	fwrite(&m_vCamBoundRB, sizeof(XMFLOAT2), 1, _pFile);
}

CComponent * CCameraScipt::LoadFromScene(FILE * _pFile)
{
	fread(&m_vCamBoundLT, sizeof(XMFLOAT2), 1, _pFile);
	fread(&m_vCamBoundRB, sizeof(XMFLOAT2), 1, _pFile);
	return nullptr;
}

void CCameraScipt::SetSceneMove(Vec2 _vDirect, wstring _wSceneName)
{
	m_bSceneMove = true;
	m_vSceneMovePoint = _vDirect;
	m_vChangeSceneName = _wSceneName;
}

void CCameraScipt::SetBoundVec(Vec2 * _vBound)
{
	m_vCamBoundLT = _vBound[0];
	m_vCamBoundRB = _vBound[1];
}


void CCameraScipt::ChasingPlayer()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer** pLayer = pCurScene->GetAllLayer();
	Vec3 vMyPos = Transform()->GetLocalPos();
	Vec3 vMySize = Transform()->GetLocalScale();
	
	
	//플레이어위치에 카메라를 맞춘다.
	const vector<CGameObj*>& vObjPlayer = pLayer[1]->GetParentObject();
	Vec3 vPlayerPos = vObjPlayer[0]->Transform()->GetLocalPos();
	CTransform* CameraTrans = Object()->Transform();
	Vec3 vCamPos = CameraTrans->GetLocalPos();

	if (m_vCamBoundLT.x <= vCamPos.x  && m_vCamBoundRB.x >= vCamPos.x
		&& m_vCamBoundLT.y <= vCamPos.y  && m_vCamBoundRB.y >= vCamPos.y)
	{
		if (vPlayerPos.x >= vCamPos.x || vPlayerPos.x <= vCamPos.x
			|| vPlayerPos.y >= vCamPos.y || vPlayerPos.y  <= vCamPos.y)
		{
			if (vPlayerPos.x >= vCamPos.x)
			{
				if(m_vCamBoundLT.x <= vCamPos.x + 300.f * DT &&
					m_vCamBoundRB.x >= vCamPos.x + 300.f * DT)
					vCamPos.x += 300.f * DT;
			}
			if (vPlayerPos.x <= vCamPos.x)
			{
				if (m_vCamBoundLT.x <= vCamPos.x - 300.f * DT &&
					m_vCamBoundRB.x >= vCamPos.x - 300.f * DT)
					vCamPos.x -= 300.f * DT;
			}
			if (vPlayerPos.y >= vCamPos.y)
			{
				if (m_vCamBoundLT.y <= vCamPos.y + 300.f * DT &&
					m_vCamBoundRB.y >= vCamPos.y + 300.f * DT)
					vCamPos.y += 300.f * DT;
			}
			if (vPlayerPos.y <= vCamPos.y)
			{
				if (m_vCamBoundLT.y <= vCamPos.y - 300.f * DT &&
					m_vCamBoundRB.y >= vCamPos.y - 300.f * DT)
					vCamPos.y -= 300.f * DT;
			}
		}
	}

	CameraTrans->SetLocalPos(Vec3(vCamPos.x, vCamPos.y, 0.f));
}

void CCameraScipt::SceneMove()
{
	if (m_fAccTime < 1.2f)
	{
		Vec2 vPos = { Transform()->GetLocalPos().x, Transform()->GetLocalPos().y };

		vPos.x += m_vSceneMovePoint.x *DT;
		vPos.y += m_vSceneMovePoint.y *DT;
		m_fAccTime += DT;
		Transform()->SetLocalPos(Vec3(vPos.x, vPos.y, 0.f));
	}
	if (m_fAccTime > 1.2f)
	{
		wstring wPath = CPathMgr::GetResPath();
		wPath += L"Scene\\";
		wPath += m_vChangeSceneName;
		CSaveLoadMgr::LoadScene(wPath.c_str());
	}
}

