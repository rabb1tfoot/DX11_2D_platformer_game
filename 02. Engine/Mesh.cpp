#include "stdafx.h"
#include "Mesh.h"
#include "Device.h"
#include "ResMgr.h"


CMesh::CMesh(RES_TYPE _etype)
	:CReSource(_etype)
{

}


CMesh::~CMesh()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);

	SAFE_DELETE(m_pVtxSysMem);
	SAFE_DELETE(m_pIdxSysMem);
	SAFE_RELEASE(m_pLayout);
}


void CMesh::CreateMesh(UINT _iVtxSize, UINT _iVtxCount, D3D11_USAGE _eVtxUsage, void * _pVtxSysMem, UINT _iIdxSize, UINT _iIdxCount, void * _pIdxSysMem, D3D11_PRIMITIVE_TOPOLOGY _etopology)
{
	//버택스 버퍼 만들기
	D3D11_BUFFER_DESC tBufferDesc = {};

	tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tBufferDesc.ByteWidth = _iVtxSize * _iVtxCount;
	tBufferDesc.Usage = _eVtxUsage;

	if (_etopology == D3D11_USAGE_DYNAMIC)
		tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA tSub = {};
	tSub.pSysMem = _pVtxSysMem;

	DEVICE->CreateBuffer(&tBufferDesc, &tSub, &m_pVB);

	//인덱스 버퍼만들기
	tBufferDesc = {};

	tBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	tBufferDesc.ByteWidth = _iIdxSize * _iIdxCount;
	tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	tBufferDesc.CPUAccessFlags = 0;

	tSub = {};
	tSub.pSysMem = _pIdxSysMem;
	DEVICE->CreateBuffer(&tBufferDesc, &tSub, &m_pIB);

	m_iVtxCount = _iVtxCount;
	m_iVtxSize = _iVtxSize;
	m_iIdxCount = _iIdxCount;
	m_iIdxSize = _iIdxSize;
	m_eTopology = _etopology;

	m_pVtxSysMem = malloc(m_iVtxCount * m_iVtxSize);
	m_pIdxSysMem = malloc(m_iIdxCount * m_iIdxSize);

	memcpy(m_pVtxSysMem, _pVtxSysMem, m_iVtxCount * m_iVtxSize);
	memcpy(m_pIdxSysMem, _pIdxSysMem, m_iIdxCount * m_iIdxSize);

	//레이아웃 만들기
	UINT iByteOffset = 0;
	D3D11_INPUT_ELEMENT_DESC tLayoutDesc;

	tLayoutDesc.AlignedByteOffset = iByteOffset;
	tLayoutDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	tLayoutDesc.InputSlot = 0;
	tLayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	tLayoutDesc.InstanceDataStepRate = 0;
	tLayoutDesc.SemanticIndex = 0;
	tLayoutDesc.SemanticName = "POSITION";

	m_vecLayout.push_back(tLayoutDesc);

	iByteOffset += sizeof(Vec3);

	tLayoutDesc.AlignedByteOffset = iByteOffset;
	tLayoutDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	tLayoutDesc.InputSlot = 0;
	tLayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	tLayoutDesc.InstanceDataStepRate = 0;
	tLayoutDesc.SemanticIndex = 0;
	tLayoutDesc.SemanticName = "COLOR";

	m_vecLayout.push_back(tLayoutDesc);

	iByteOffset += sizeof(Vec4);

	tLayoutDesc.AlignedByteOffset = iByteOffset;
	tLayoutDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
	tLayoutDesc.InputSlot = 0;
	tLayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	tLayoutDesc.InstanceDataStepRate = 0;
	tLayoutDesc.SemanticName = "TEXCOORD";
	tLayoutDesc.SemanticIndex = 0;

	m_vecLayout.push_back(tLayoutDesc);

	iByteOffset += sizeof(Vec2);

}

void CMesh::SetLayout(CResPtr<CShader> _pShader)
{
	ID3DBlob* pVtxVlob = _pShader->GetVSBlob();

	if (nullptr != m_pLayout)
	{
		SAFE_RELEASE(m_pLayout);
		m_pLayout = nullptr;
	}
	DEVICE->CreateInputLayout(&m_vecLayout[0], m_vecLayout.size(), pVtxVlob->GetBufferPointer()
		, pVtxVlob->GetBufferSize(), &m_pLayout);
}

void CMesh::Render()
{
	UINT iStride = m_iVtxSize;
	UINT iOffset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, &m_pVB, &iStride, &iOffset);
	CONTEXT->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R16_UINT, 0);
	CONTEXT->IASetPrimitiveTopology(m_eTopology);
	CONTEXT->IASetInputLayout(m_pLayout);

	CONTEXT->DrawIndexed(m_iIdxCount, 0, 0);
}

bool CMesh::LoadFromScene(FILE * _pFile)
{
	CReSource::LoadFromScene(_pFile);

	CResPtr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(GetName());

	if (nullptr != pMesh)
		return false;

	wstring strPath = CPathMgr::GetResPath();
	strPath += GetPath();
	Load(strPath);

}
