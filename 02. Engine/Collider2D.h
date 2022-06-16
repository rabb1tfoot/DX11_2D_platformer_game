#pragma once
#include "Component.h"

#include "Mesh.h"
#include "Material.h"
#include "ResPtr.h"

class CCollider2D :
	public CComponent
{
private:
	static vector<CResPtr<CMaterial>>   g_vecColMtrl;
	static CResPtr<CMaterial>			g_pOriginMtrl;

public:
	static void CreateMaterial();
	static CResPtr<CMaterial> GetCloneMtrl();
	static void ClearMaterial();

private:
	Vec3 m_vOffset;
	Vec3 m_vScale;

	Vec3 m_vFinalPos;
	Vec3 m_vFinalScale;
	Vec3 m_vLTPos;
	Vec3 m_vRBPos;

	Matrix m_matWorld;

	COLLIDER2D_TYPE m_eType;

	CResPtr<CMesh> m_pMesh;
	CResPtr<CMaterial> m_pCurMtrl;

	UINT m_iColCount;


public:
	virtual void Update();
	virtual void Finalupdate();

public:
	void SetOffset(const Vec3& _vOffset) { m_vOffset = _vOffset; }
	const Vec3& GetOffset() { return m_vOffset; }

	void SetScale(const Vec3& _vScale) { m_vScale = _vScale; }
	const Vec3& GetScale() { return m_vScale; }

	void SetColliderType(COLLIDER2D_TYPE _eType);
	COLLIDER2D_TYPE GetCollider2DType() { return m_eType; }

	const Matrix& GetWorldMat() { return m_matWorld; }

	const Vec3& GetFinalPos() { return m_vFinalPos; }
	const Vec3& GetFinalScale() { return m_vFinalScale; }
	const Vec3& GetLT() { return m_vLTPos; }
	const Vec3& GetRB() { return m_vRBPos; }

	void OnCollisionEnter(CCollider2D* _pOther);
	void OnCollision(CCollider2D* _pOther);
	void OnCollisionExit(CCollider2D* _pOther);

	void UpdateData();
	void render();

	virtual void SaveToScene(FILE* _pFile);
	virtual CComponent* LoadFromScene(FILE* _pFile);

	CLONE(CCollider2D);

public:
	CCollider2D();
	virtual ~CCollider2D();
};

