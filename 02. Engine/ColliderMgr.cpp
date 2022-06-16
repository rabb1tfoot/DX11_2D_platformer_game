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
			//��Ʈ ���������� ��ŷ�� �Ǿ��ִ��� üũ�Ѵ�.
			if (m_arrCheck[i] & (1 << j))
			{
				// �浹 �˻縦 �����Ѵ�.
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
			// �� ������Ʈ �߿��� �浹ü�� ���� ��찡 �ִٸ�
			if (vecLeft[i]->Collider2D() == nullptr || vecRight[j]->Collider2D() == nullptr)
				continue;

			colID.left = vecLeft[i]->Collider2D()->GetID();
			colID.right = vecRight[j]->Collider2D()->GetID();

			map<unsigned long long, bool>::iterator iter = m_mapID2D.find(colID.ID);

			//�浹����
			if (IsCollision(vecLeft[i]->Collider2D(), vecRight[j]->Collider2D()))
			{
				// �浹 �ߴ�.

				if (iter == m_mapID2D.end())
				{
					// �浹 ���� ��ϵ��� ���� ==> �浹���� ����					
					vecLeft[i]->Collider2D()->OnCollisionEnter(vecRight[j]->Collider2D());
					vecRight[j]->Collider2D()->OnCollisionEnter(vecLeft[i]->Collider2D());
					m_mapID2D.insert(make_pair(colID.ID, true));
				}
				else if (false == iter->second)
				{
					// ������ �浹���� �ʰ� �־���.
					vecLeft[i]->Collider2D()->OnCollisionEnter(vecRight[j]->Collider2D());
					vecRight[j]->Collider2D()->OnCollisionEnter(vecLeft[i]->Collider2D());
					iter->second = true;
				}
				else {
					// �浹 ���̴�.
					vecLeft[i]->Collider2D()->OnCollision(vecRight[j]->Collider2D());
					vecRight[j]->Collider2D()->OnCollision(vecLeft[i]->Collider2D());
				}
			}
			else // �浹���� �ʰ� �ִ�.
			{
				if (iter != m_mapID2D.end() && true == iter->second)
				{
					// �������� �浹 ���̾���. ==>�̹��� ������ ��Ȳ
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
		// Rect Rect �浹
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

		// �浹ü �� ���� ������Ų �Ÿ��� ����
		fSum /= 2.f;

		// �߽��� ���� ���͸� ������Ų �Ÿ�
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

	//�浹ü�� ������ �������� ���ϱ����� �� 2������ ���Ѵ�.

	Vec3 vLeftWorldPos[2] = {};
	Vec3 vRightWorldPos[2] = {};

	//������ �浹ü���� ���� ��� ���ϱ�

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

	//�������� �Ÿ� ���ϱ�
	//������ �Ÿ� ���ϱ�
	Vec3 vLeft = (vLeftWorldPos[0] - vLeftWorldPos[1]);
	float fLeftRDist = vLeft.Distance();

	Vec3 vRight = (vRightWorldPos[0] - vRightWorldPos[1]);
	float fRightRDist = vRight.Distance();

	float fSum = fLeftRDist + fRightRDist;

	Vec3 vCenter = vLeftWorldPos[0] - vRightWorldPos[0];
	float fCenterDist = vCenter.Distance();

	//�������� �հ� �������� �Ÿ� ��
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
