#include "stdafx.h"
#include "Transform.h"

#include "Device.h"
#include "ConstBuffer.h"

tTransform g_transform;

CTransform::CTransform()
	:CComponent(COMPONENT_TYPE::TRANSFORM)
{
}


CTransform::~CTransform()
{
}


void CTransform::Finalupdate()
{
	Matrix matScale = XMMatrixScaling(m_vLocalScale.x, m_vLocalScale.y, m_vLocalScale.z);
	Matrix matRotX = XMMatrixRotationX(m_vLocalRot.x);
	Matrix matRotY = XMMatrixRotationY(m_vLocalRot.y);
	Matrix matRotZ = XMMatrixRotationZ(m_vLocalRot.z);
	Matrix matPos = XMMatrixTranslation(m_vLocalPos.x, m_vLocalPos.y, m_vLocalPos.z);

	m_matWorld = matScale * matRotX * matRotY * matRotZ * matPos;

	//부모가 있으면 부모에 영향을 받는다.
	if (nullptr != ParentObject())
	{
		const Matrix& matParentWorld = ParentObject()->Transform()->GetWorldMat();
		m_matWorld *= matParentWorld;
	}
}

void CTransform::UpdateData()
{
	g_transform.matWorld = m_matWorld;

	static CConstBuffer* pConstBuffer = CDevice::GetInst()->FindConstBuffer(L"Transform");

	D3D11_MAPPED_SUBRESOURCE tSub = {};

	pConstBuffer->AddData(&g_transform, sizeof(tTransform));
	pConstBuffer->UpdateData();
	pConstBuffer->SetRegisterAll();
}

void CTransform::SaveToScene(FILE * _pFile)
{
	fwrite(&m_vLocalPos, sizeof(Vec3), 1, _pFile);
	fwrite(&m_vLocalScale, sizeof(Vec3), 1, _pFile);
	fwrite(&m_vLocalRot, sizeof(Vec3), 1, _pFile);
}

CComponent* CTransform::LoadFromScene(FILE * _pFile)
{
	fread(&m_vLocalPos, sizeof(Vec3), 1, _pFile);
	fread(&m_vLocalScale, sizeof(Vec3), 1, _pFile);
	fread(&m_vLocalRot, sizeof(Vec3), 1, _pFile);
	return nullptr;
}
