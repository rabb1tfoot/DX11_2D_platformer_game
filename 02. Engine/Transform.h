#pragma once
#include "Component.h"
class CTransform :
	public CComponent
{
private:
	Vec3 m_vLocalPos;
	Vec3 m_vLocalScale;
	Vec3 m_vLocalRot;

	Matrix m_matWorld;

public:
	void SetLocalPos(const Vec3& _pos) { m_vLocalPos = _pos; }
	void SetLocalScale(const Vec3& _scale) { m_vLocalScale = _scale; }
	void SetLocalRot(const Vec3& _rot) { m_vLocalRot = _rot; }

	const Vec3& GetLocalPos() { return m_vLocalPos; }
	const Vec3& GetLocalScale() { return m_vLocalScale; }
	const Vec3& GetLocalRot() { return m_vLocalRot; }

	const Matrix& GetWorldMat() { return m_matWorld; }

public:
	virtual void Update() {};
	virtual void Finalupdate();

	void UpdateData();

	virtual void SaveToScene(FILE* _pFile);
	virtual CComponent* LoadFromScene(FILE* _pFile);

public:
	CLONE(CTransform);

public:
	CTransform();
	virtual ~CTransform();
};

