#include "stdafx.h"
#include "ColliderMgr.h"
#include "SceneMgr.h"
#include "GameObj.h"
#include "Scene.h"
#include "Layer.h"
#include "Collider2D.h"
#include "ResMgr.h"

CColliderMgr::CColliderMgr()
{
}


CColliderMgr::~CColliderMgr()
{
}

void CColliderMgr::Update()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		for (UINT j = i; j < MAX_LAYER; ++j)
		{
			//비트 곱연산으로 마킹이 되어있는지 체크한다.
			if (m_arrCheck[i] & (1 << j))
			{
				// 충돌 검사를 진행한다.
				CollisionGroup(i, j);
			}
		}
	}

}

void CColliderMgr::CollisionGroup(int _leftIdx, int _rightIdx)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	
	const vector<CGameObj*>& vecLeft = pCurScene->GetLayer(_leftIdx)->GetParentObject();
	const vector<CGameObj*>& vecRight = pCurScene->GetLayer(_rightIdx)->GetParentObject();

	COL_ID colID;

	for (UINT i = 0; i < vecLeft.size(); ++i)
	{
		for (UINT j = 0; j < vecRight.size(); ++j)
		{
			// 두 오브젝트 중에서 충돌체가 없는 경우가 있다면
			if (vecLeft[i]->Collider2D() == nullptr || vecRight[j]->Collider2D() == nullptr)
				continue;

			colID.left = vecLeft[i]->Collider2D()->GetID();
			colID.right = vecRight[j]->Collider2D()->GetID();

			map<unsigned long long, bool>::iterator iter = m_mapID2D.find(colID.ID);

			//충돌판정
			if (IsCollision(vecLeft[i]->Collider2D(), vecRight[j]->Collider2D()))
			{
				// 충돌 했다.

				if (iter == m_mapID2D.end())
				{
					// 충돌 조합 등록된적 없음 ==> 충돌한적 없음					
					vecLeft[i]->Collider2D()->OnCollisionEnter(vecRight[j]->Collider2D());
					vecRight[j]->Collider2D()->OnCollisionEnter(vecLeft[i]->Collider2D());
					m_mapID2D.insert(make_pair(colID.ID, true));
				}
				else if (false == iter->second)
				{
					// 이전에 충돌하지 않고 있었다.
					vecLeft[i]->Collider2D()->OnCollisionEnter(vecRight[j]->Collider2D());
					vecRight[j]->Collider2D()->OnCollisionEnter(vecLeft[i]->Collider2D());
					iter->second = true;
				}
				else {
					// 충돌 중이다.
					vecLeft[i]->Collider2D()->OnCollision(vecRight[j]->Collider2D());
					vecRight[j]->Collider2D()->OnCollision(vecLeft[i]->Collider2D());
				}
			}
			else // 충돌하지 않고 있다.
			{
				if (iter != m_mapID2D.end() && true == iter->second)
				{
					// 이전에는 충돌 중이었다. ==>이번에 떨어진 상황
					vecLeft[i]->Collider2D()->OnCollisionExit(vecRight[j]->Collider2D());
					vecRight[j]->Collider2D()->OnCollisionExit(vecLeft[i]->Collider2D());
					iter->second = false;
				}
			}
		}
	}
}

bool CColliderMgr::IsCollision(CCollider2D * _pLeft, CCollider2D * _pRight)
{
	if (_pLeft->GetCollider2DType() == COLLIDER2D_TYPE::RECT && _pRight->GetCollider2DType() == COLLIDER2D_TYPE::RECT)
	{
		// Rect Rect 충돌
		return CollisionRect(_pLeft, _pRight);
	}
	else if (_pLeft->GetCollider2DType() == COLLIDER2D_TYPE::CIRCLE && _pRight->GetCollider2DType() == COLLIDER2D_TYPE::CIRCLE)
	{
		// Circle Circle
		return CollisionCircle(_pLeft, _pRight);
	}
	else
	{
		// Rect, Circle
	}

	return false;
}

bool CColliderMgr::CollisionRect(CCollider2D * _pLeft, CCollider2D * _pRight)
{
	static CResPtr<CMesh> pRectMesh = CResMgr::GetInst()->FindRes<CMesh>(L"ColliderRectMesh");

	Matrix matLeftWorld = _pLeft->GetWorldMat();
	Matrix matRightWorld = _pRight->GetWorldMat();

	VTX* pVtx = (VTX*)pRectMesh->GetVtxSysMem();

	Vec3 vLeftWorldPos[4] = {};
	Vec3 vRightWorldPos[4] = {};

	for (UINT i = 0; i < 4; ++i)
	{
		vLeftWorldPos[i] = XMVector3TransformCoord(pVtx[i].vPos, matLeftWorld);
		vLeftWorldPos[i].z = 0.f;
	}

	for (UINT i = 0; i < 4; ++i)
	{
		vRightWorldPos[i] = XMVector3TransformCoord(pVtx[i].vPos, matRightWorld);
		vRightWorldPos[i].z = 0.f;
	}

	Vec3 vLeftCenter = (vLeftWorldPos[3] + vLeftWorldPos[0]) / 2.f;
	Vec3 vRightCenter = (vRightWorldPos[3] + vRightWorldPos[0]) / 2.f;

	Vec3 vCenter = vLeftCenter - vRightCenter;

	Vec3 vProj[4] = {};
	Vec3 vAxis[4] = {};

	vProj[0] = vLeftWorldPos[0] - vLeftWorldPos[2];
	vAxis[0] = vProj[0];

	vProj[1] = vLeftWorldPos[3] - vLeftWorldPos[2];
	vAxis[1] = vProj[1];

	vProj[2] = vRightWorldPos[0] - vRightWorldPos[2];
	vAxis[2] = vProj[2];

	vProj[3] = vRightWorldPos[3] - vRightWorldPos[2];
	vAxis[3] = vProj[3];

	for (int i = 0; i < 4; ++i)
	{
		if (vAxis[i].IsZero())
			return false;

		vAxis[i].Normalize();
	}

	float fSum = 0.f;

	for (UINT i = 0; i < 4; ++i)
	{
		fSum = 0.f;
		for (UINT j = 0; j < 4; ++j)
		{
			fSum += abs(vProj[j].Dot(vAxis[i]));
		}

		// 충돌체 각 변을 투영시킨 거리의 절반
		fSum /= 2.f;

		// 중심을 이은 벡터를 투영시킨 거리
		float fDist = abs(vCenter.Dot(vAxis[i]));

		if (fDist > fSum)
			return false;
	}

	return true;
}

bool CColliderMgr::CollisionCircle(CCollider2D * _pLeft, CCollider2D * _pRight)
{
	static CResPtr <CMesh> pCircleMesh = CResMgr::GetInst()->FindRes<CMesh>(L"ColliderCircleMesh");

	Matrix MatLeftWorld = _pLeft->GetWorldMat();
	Matrix MatRightWorld = _pRight->GetWorldMat();

	VTX* pVtx = (VTX*)pCircleMesh->GetVtxSysMem();

	//충돌체의 중점과 반지름을 구하기위한 점 2가지를 구한다.

	Vec3 vLeftWorldPos[2] = {};
	Vec3 vRightWorldPos[2] = {};

	//각각의 충돌체마다 월드 행렬 구하기

	for (UINT i = 0; i < 2; ++i)
	{
		vLeftWorldPos[i] = XMVector3TransformCoord(pVtx[i].vPos, MatLeftWorld);
		vLeftWorldPos[i].z = 0.f;
	}

	for (UINT i = 0; i < 2; ++i)
	{
		vRightWorldPos[i] = XMVector3TransformCoord(pVtx[i].vPos, MatRightWorld);
		vRightWorldPos[i].z = 0.f;
	}

	//중점간의 거리 구하기
	//반지름 거리 구하기
	Vec3 vLeft = (vLeftWorldPos[0] - vLeftWorldPos[1]);
	float fLeftRDist = vLeft.Distance();

	Vec3 vRight = (vRightWorldPos[0] - vRightWorldPos[1]);
	float fRightRDist = vRight.Distance();

	float fSum = fLeftRDist + fRightRDist;

	Vec3 vCenter = vLeftWorldPos[0] - vRightWorldPos[0];
	float fCenterDist = vCenter.Distance();

	//반지름의 합과 중점간의 거리 비교
	if (fSum > fCenterDist)
		return true;

	else
		return false;

}

bool CColliderMgr::IsCollision(CCollider3D * _pLeft, CCollider3D * _pRight)
{
	return false;
}

bool CColliderMgr::CollisionRect(CCollider3D * _pLeft, CCollider3D * _pRight)
{
	return false;
}

bool CColliderMgr::CollisionCircle(CCollider3D * _pLeft, CCollider3D * _pRight)
{
	return false;
}

void CColliderMgr::CollisionCheck(const wstring & _strLayer1, const wstring & _strLayer2)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLeftLayer = pCurScene->FindLayer(_strLayer1);
	CLayer* pRightLayer = pCurScene->FindLayer(_strLayer2);

	int iLayerIdx1 = pLeftLayer->GetLayerIdx();
	int iLayerIdx2 = pRightLayer->GetLayerIdx();

	CollisionCheck(iLayerIdx1, iLayerIdx2);
}

void CColliderMgr::CollisionCheck(int _iLayerIdx1, int _iLayerIdx2)
{
	UINT iIdx = (UINT)(_iLayerIdx1 < _iLayerIdx2 ? _iLayerIdx1 : _iLayerIdx2);
	UINT iBitIdx = iIdx == (UINT)(_iLayerIdx1) ? _iLayerIdx2 : _iLayerIdx1;

	m_arrCheck[iIdx] |= (1 << iBitIdx);
}

void CColliderMgr::DelCollisionCheck(const wstring & _strLayer1, const wstring & _strLayer2)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLeftLayer = pCurScene->FindLayer(_strLayer1);
	CLayer* pRightLayer = pCurScene->FindLayer(_strLayer2);

	int iLayerIdx1 = pLeftLayer->GetLayerIdx();
	int iLayerIdx2 = pRightLayer->GetLayerIdx();

	DelCollisionCheck(iLayerIdx1, iLayerIdx2);
}

void CColliderMgr::DelCollisionCheck(int _iLayerIdx1, int _iLayerIdx2)
{
	UINT iIdx = (UINT)(_iLayerIdx1 < _iLayerIdx2 ? _iLayerIdx1 : _iLayerIdx2);
	UINT iBitIdx = iIdx == (UINT)(_iLayerIdx1) ? _iLayerIdx2 : _iLayerIdx1;

	m_arrCheck[iIdx] &= ~(1 << iBitIdx);
}
