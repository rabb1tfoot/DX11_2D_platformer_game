#pragma once

class CCollider;

class CObj
{
public:
	CObj();
	CObj(SHAPE _etype);
	~CObj();

private:
	UINT m_iVtxAmount;
	UINT m_iIndexAmount;
	ID3D11Buffer * m_pVB;
	ID3D11Buffer * m_pIB;
	ID3D11Buffer * m_pCB;
	VTX m_arrVtx[60];
	OBJ_TYPE m_eType;
protected:
	Vec3 m_vWorldPos;
	Vec3 m_vWorldScale;
	Vec3 m_vWorldRotation;
	CCollider* m_col;

public:
	void SetTranslate(const Vec3& _pos);
	void SetRotation(const Vec3& _rot);
	void SetScale(const Vec3& _size);
	void SetColor(const Vec4& _color);
	void SetType(OBJ_TYPE _etype);
	void CreateCollider();

	OBJ_TYPE GetType() { return m_eType; }
	Vec3 GetTranslate() { return m_vWorldPos; }
	Vec3 GetScale() { return m_vWorldScale; }
	CCollider* GetCollider() { return m_col; }
public:
	virtual void Init();
	virtual void Update();
	virtual void Render();
};

