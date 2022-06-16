#include "stdafx.h"
#include "Material.h"

#include "Device.h"
#include "ConstBuffer.h"
#include "PathMgr.h"
#include "ResMgr.h"

CMaterial::CMaterial(RES_TYPE _etype)
	: CReSource(_etype)
	, m_pShader(nullptr)
	, m_arrTex{}
	, m_tParam{}
	, m_tTileParam{}
	, m_bTileRender(false)
{
}


CMaterial::~CMaterial()
{
}


void CMaterial::SetShader(CResPtr<CShader> _pShader)
{
	m_pShader = _pShader;

	Save();

}

void CMaterial::SetData(SHADER_PARAM _eType, void * _pSrc)
{
	int iIdx = 0;

	switch (_eType)
	{
	case SHADER_PARAM::INT_0:
	case SHADER_PARAM::INT_1:
	case SHADER_PARAM::INT_2:
	case SHADER_PARAM::INT_3:
		iIdx = (UINT)_eType - (UINT)SHADER_PARAM::INT_0;
		m_tParam.arrInt[iIdx] = *((int*)_pSrc);
		break;
	case SHADER_PARAM::FLOAT_0:
	case SHADER_PARAM::FLOAT_1:
	case SHADER_PARAM::FLOAT_2:
	case SHADER_PARAM::FLOAT_3:
		iIdx = (UINT)_eType - (UINT)SHADER_PARAM::FLOAT_0;
		m_tParam.arrFloat[iIdx] = *((float*)_pSrc);

		break;
	case SHADER_PARAM::VEC2_0:
	case SHADER_PARAM::VEC2_1:
	case SHADER_PARAM::VEC2_2:
	case SHADER_PARAM::VEC2_3:
		iIdx = (UINT)_eType - (UINT)SHADER_PARAM::VEC2_0;
		m_tParam.arrVec2[iIdx] = *((Vec2*)_pSrc);
		break;
	case SHADER_PARAM::VEC4_0:
	case SHADER_PARAM::VEC4_1:
	case SHADER_PARAM::VEC4_2:
	case SHADER_PARAM::VEC4_3:
		iIdx = (UINT)_eType - (UINT)SHADER_PARAM::VEC4_0;
		m_tParam.arrVec4[iIdx] = *((Vec4*)_pSrc);
		break;
	case SHADER_PARAM::MAT_0:
	case SHADER_PARAM::MAT_1:
	case SHADER_PARAM::MAT_2:
	case SHADER_PARAM::MAT_3:
		iIdx = (UINT)_eType - (UINT)SHADER_PARAM::MAT_0;
		m_tParam.arrMat[iIdx] = *((Matrix*)_pSrc);
		break;
	case SHADER_PARAM::TEX_0:
	case SHADER_PARAM::TEX_1:
	case SHADER_PARAM::TEX_2:
	case SHADER_PARAM::TEX_3:
		iIdx = (UINT)_eType - (UINT)SHADER_PARAM::TEX_0;
		m_arrTex[iIdx] = *((CResPtr<CTexture>*)_pSrc);
		break;
	case SHADER_PARAM::INT_ARRAY:
		for (UINT i = 0; i < 900; ++i)
		{
			if (-1 == ((int*)_pSrc)[i])
				break;
		
			m_tTileParam.arrIntTile[i] = ((int*)_pSrc)[i];
		}
		break;
	case SHADER_PARAM::VEC2_ARRAY:
		for (UINT i = 0; i < 900; ++i)
		{
			if (-1 == ((Vec2*)_pSrc)[i].x)
				break;
			
			m_tTileParam.arrVec2TileX[i] = ((Vec2*)_pSrc)[i].x;
			m_tTileParam.arrVec2TileY[i] = ((Vec2*)_pSrc)[i].y;
		
		}
		break;
	case SHADER_PARAM::VEC2_1ARRAY:
		for (UINT i = 0; i < 900; ++i)
		{
			if (-1 == ((Vec2*)_pSrc)[i].x)
				break;

			m_tTileParam.arrVec2TileTexX[i] = ((Vec2*)_pSrc)[i].x;
			m_tTileParam.arrVec2TileTexY[i] = ((Vec2*)_pSrc)[i].y;

		}
		break;
	default:
		assert(nullptr);
		break;
	}

	//Save();
}

void CMaterial::Save()
{
	wstring strFileName = CPathMgr::GetResPath();
	strFileName += GetName();

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFileName.c_str(), L"wb");

	//쉐이더의 태크명 저장
	wstring strKey;
	if (nullptr != m_pShader)
		strKey = m_pShader->GetName();

	SaveWString(strKey.c_str(), pFile);

	//쉐이더 파라미터 저장
	fwrite(&m_tParam, sizeof(tShaderParam), 1, pFile);

	//타일 파라미터 저장
	fwrite(&m_tTileParam, sizeof(m_tTileParam), 1, pFile);

	//텍스처 파라미터 저장
	for (UINT i = 0; i < (UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0; ++i)
	{
		if (nullptr != m_arrTex[i])
		{
			fwrite(&i, sizeof(int), i, pFile);
			SaveWString(m_arrTex[i]->GetName().c_str(), pFile);
			SaveWString(m_arrTex[i]->GetPath().c_str(), pFile);
		}
	}

	//텍스처 마감데이터 저장
	int iTexEnd = (UINT)SHADER_PARAM::TEX_END;
	fwrite(&iTexEnd, sizeof(int), 1, pFile);

	fclose(pFile);
}

bool CMaterial::LoadFromScene(FILE * _pFile)
{
	CReSource::LoadFromScene(_pFile);

	CResPtr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(GetName());

	if (nullptr != pMtrl)
		return false;

	wstring strPath = CPathMgr::GetResPath();
	strPath += GetPath();

	Load(strPath);
}

void CMaterial::Load(const wstring & _strFilePath)
{
	FILE *pFile = nullptr;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

	//쉐이더 키 읽기
	wstring strShaderName = LoadWString(pFile);
	m_pShader = CResMgr::GetInst()->FindRes<CShader>(strShaderName);

	//쉐이더 파라미터 읽기
	fread(&m_tParam, sizeof(tShaderParam), 1, pFile);

	//타일 파라미터 읽기
	fread(&m_tTileParam, sizeof(m_tTileParam), 1, pFile);

	//텍스처 파라미터 읽기
	while (true)
	{
		int iIdx = 0;
		fread(&iIdx, sizeof(int), 1, pFile);

		if (iIdx == (UINT)SHADER_PARAM::TEX_END)
			break;

		wstring strKey = LoadWString(pFile);
		wstring strPath = LoadWString(pFile);

		m_arrTex[iIdx] = CResMgr::GetInst()->FindRes<CTexture>(strKey);
		if (nullptr == m_arrTex[iIdx])
		{
			m_arrTex[iIdx] = CResMgr::GetInst()->Load<CTexture>(strKey, strPath);
		}
	}

	fclose(pFile);
}

void CMaterial::UpdateData()
{
	m_pShader->UpdateData();

	static CConstBuffer* pConstBuffer = CDevice::GetInst()->FindConstBuffer(L"ShaderParam");

	pConstBuffer->AddData(&m_tParam, sizeof(tShaderParam));
	pConstBuffer->UpdateData();
	pConstBuffer->SetRegisterAll();

	UINT iCount = (UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0;

	for (UINT i = 0; i < iCount; ++i)
	{
		if (nullptr == m_arrTex[i])
			continue;

		m_arrTex[i]->SetRegister(i, (UINT)SHADER_TYPE::PIXEL_SHADER);
	}

	static CConstBuffer* pConstBufferTile = CDevice::GetInst()->FindConstBuffer(L"Tile");
	pConstBufferTile->AddData(&m_tTileParam, sizeof(tTileParam));
	pConstBufferTile->UpdateData();
	pConstBufferTile->SetRegister((UINT)SHADER_TYPE::PIXEL_SHADER);

}