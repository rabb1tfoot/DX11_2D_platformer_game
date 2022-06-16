#pragma once
#include "Component.h"

class CMesh;
class CMaterial;

#include "ResPtr.h"

class CMeshRender :
	public CComponent
{
private:
	CResPtr<CMesh>		m_pMesh;
	CResPtr<CMaterial>  m_pMeterial;

public:
	void SetMesh(CResPtr<CMesh>	_pMesh) { m_pMesh = _pMesh; }
	void SetMaterial(CResPtr<CMaterial> _pMeterial) { m_pMeterial = _pMeterial; }

	CResPtr<CMesh> GetMesh() { return m_pMesh; }
	CResPtr<CMaterial> GetCloneMaterial();
	CResPtr<CMaterial> GetSharedMaterial() { return m_pMeterial; }

public:
	virtual void Update();
	void Render();

	virtual void SaveToScene(FILE* _pFile);
	virtual CComponent* LoadFromScene(FILE* _pFile);

public:
	CLONE(CMeshRender);

public:
	CMeshRender();
	virtual ~CMeshRender();
};

