#include "stdafx.h"
#include "Collider2D.h"
#include "ResMgr.h"
#include "Transform.h"
#include "ResMgr.h"
#include "Device.h"
#include "ConstBuffer.h"
#include "Material.h"
#include "Script.h"

CResPtr<CMaterial> CCollider2D::g_pOriginMtrl = nullptr;
vector<CResPtr<CMaterial>> CCollider2D::g_vecColMtrl;

void CCollider2D::CreateMaterial()
{
	g_pOriginMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"Collider2DMaterial");

	//각각의 충돌체에 쓸 메모리풀 클론 메테리얼 만들기
	for (UINT i = 0; i < 10; ++i)
	{
		CResPtr<CMaterial> pCloneMtrl = g_pOriginMtrl->Clone();
		CResMgr::GetInst()->AddCloneRes<CMaterial>(pCloneMtrl);
		g_vecColMtrl.push_back(pCloneMtrl);

	}
}

CResPtr<CMaterial> CCollider2D::GetCloneMtrl()
{
	if (g_vecColMtrl.empty())
	{
		CResPtr<CMaterial> pCloneMtrl = g_pOriginMtrl->Clone();
		CResMgr::GetInst()->AddCloneRes<CMaterial>(pCloneMtrl);
		return pCloneMtrl;
	}
	CResPtr<CMaterial> pMtrl = g_vecColMtrl.back();
	g_vecColMtrl.pop_back();

	return pMtrl;
}

void CCollider2D::ClearMaterial()
{
	g_pOriginMtrl = nullptr;
	for (UINT i = 0; i < g_vecColMtrl.size(); ++i)
	{
		g_vecColMtrl[i] = nullptr;
	}
}


CCollider2D::CCollider2D()
	: CComponent(COMPONENT_TYPE::COLLIDER2D)
	, m_pMesh(nullptr)
	, m_pCurMtrl(nullptr)
	, m_eType(COLLIDER2D_TYPE::END)
	, m_vOffset(0.f, 0.f, 0.f)
	, m_vScale(1.f, 1.f, 0.f)
	, m_iColCount(0)
{
	SetColliderType(COLLIDER2D_TYPE::RECT);
	m_pCurMtrl = g_pOriginMtrl;
}


CCollider2D::~CCollider2D()
{
}

void CCollider2D::Update()
{
}

void CCollider2D::Finalupdate()
{
	Vec3 vPos = Transform()->GetLocalPos();
	Vec3 vScale = Transform()->GetLocalScale();
	Vec3 vRot = Transform()->GetLocalRot();

	m_vFinalPos = vPos + m_vOffset;
	m_vFinalScale = vScale * m_vScale;

	m_vLTPos = Vec3(m_vFinalPos.x - m_vFinalScale.x / 2, m_vFinalPos.y + m_vFinalScale.y / 2, m_vFinalPos.z);
	m_vRBPos = Vec3(m_vFinalPos.x + m_vFinalScale.x / 2, m_vFinalPos.y - m_vFinalScale.y / 2, m_vFinalPos.z);

	Matrix matScale = XMMatrixScaling(m_vFinalScale.x, m_vFinalScale.y, m_vFinalScale.z);
	Matrix matRotationX = XMMatrixRotationX(vRot.x);
	Matrix matRotationY = XMMatrixRotationY(vRot.y);
	Matrix matRotationZ = XMMatrixRotationZ(vRot.z);
	Matrix matTrans = XMMatrixTranslation(m_vFinalPos.x, m_vFinalPos.y, m_vFinalPos.z);

	m_matWorld = matScale * matRotationX * matRotationY * matRotationZ * matTrans;

	if (nullptr != ParentObject())
	{
		m_matWorld *= GetWorldMat();
	}

}

void CCollider2D::SetColliderType(COLLIDER2D_TYPE _eType)
{
	m_eType = _eType;

	if (COLLIDER2D_TYPE::RECT == m_eType)
	{
		m_pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"ColliderRectMesh");
	}
	else if (COLLIDER2D_TYPE::CIRCLE == m_eType)
	{
		m_pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"ColliderCircleMesh");
	}
	else
		assert(nullptr);
}


void CCollider2D::UpdateData()
{
	g_transform.matWorld = m_matWorld;

	static CConstBuffer* pCB = CDevice::GetInst()->FindConstBuffer(L"Transform");

	pCB->AddData(&g_transform, sizeof(tTransform));
	pCB->UpdateData();
	pCB->SetRegister((UINT)SHADER_TYPE::VERTEX_SHADER);
}

void CCollider2D::render()
{
	UpdateData();
	m_pMesh->SetLayout(m_pCurMtrl->GetShader());
	m_pCurMtrl->UpdateData();
	m_pMesh->Render();
}

void CCollider2D::SaveToScene(FILE * _pFile)
{
	fwrite(&m_vOffset, sizeof(Vec3), 1, _pFile);
	fwrite(&m_vScale, sizeof(Vec3), 1, _pFile);
	fwrite(&m_eType, sizeof(UINT), 1, _pFile);
}

CComponent* CCollider2D::LoadFromScene(FILE * _pFile)
{
	fread(&m_vOffset, sizeof(Vec3), 1, _pFile);
	fread(&m_vScale, sizeof(Vec3), 1, _pFile);
	fread(&m_eType, sizeof(UINT), 1, _pFile);

	SetColliderType(m_eType);

	return nullptr;
}

void CCollider2D::OnCollisionEnter(CCollider2D * _pOther)
{
	if (g_pOriginMtrl == m_pCurMtrl)
		m_pCurMtrl = GetCloneMtrl();

	// 충돌시 상수변수를 1로 바꿔서 충돌중으로 바꾼다.
	int iCollision = 1;
	m_pCurMtrl->SetData(SHADER_PARAM::INT_0, &iCollision);

	++m_iColCount;

	vector<CScript*>& vecScript = Object()->GetScripts();
	for (UINT i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->OnCollisionEnter(_pOther);
	}
}

void CCollider2D::OnCollision(CCollider2D * _pOther)
{
	vector<CScript*>& vecScript = Object()->GetScripts();
	for (UINT i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->OnCollision(_pOther);
	}
}

void CCollider2D::OnCollisionExit(CCollider2D * _pOther)
{
	--m_iColCount;

	if (0 == m_iColCount)
	{
		g_vecColMtrl.push_back(m_pCurMtrl);
		m_pCurMtrl = g_pOriginMtrl;
	}


	vector<CScript*>& vecScript = Object()->GetScripts();
	for (UINT i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->OnCollisionExit(_pOther);
	}
}