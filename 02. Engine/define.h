#pragma once

#define SINGLE(T) public: static T* GetInst() \
{\
	static T mgr;\
	return &mgr;\
}\
private:\
T();\
~T();

#define DT CTimeMgr::GetInst()->GetDeltaTime()
#define KEY(etype, eState) (eState == CKeyMgr::GetInst()->GetKeyState(etype))
#define KEYHOLD(etype) KEY(etype, KEY_STATE::STATE_HOLD)
#define KEYENTER(etype) KEY(etype, KEY_STATE::STATE_ENTER)
#define KEYAWAY(etype) KEY(etype, KEY_STATE::STATE_AWAY)
#define KEYNONE(etype) KEY(etype, KEY_STATE::STATE_NONE)
#define MOUSEPOS CKeyMgr::GetInst()->GetMousePos()

#define DEVICE CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetContext()

#define CLONE(type) type* Clone() { return new type(*this); }
#define CLONE_DISABLE(type) private: type* Clone(){return nullptr;}

#define SAFE_RELEASE(p) if(nullptr != p ) p->Release();
#define SAFE_DELETE(p) if(nullptr != p) delete p; p = nullptr;

#define MAX_LAYER 32

enum class COMPONENT_TYPE
{
	TRANSFORM,
	MESHRENDER,
	CAMERA,
	COLLIDER2D,
	COLLIDER3D,
	ANIMATION2D,
	ANIMATION3D,
	END,

	SCRIPT,
};

enum class RES_TYPE
{
	MATERIAL,
	PREFAB,
	MESH,
	SHADER,
	TEXTURE,
	SOUND,
	END

};

enum class PLAYER_DIR
{
	LEFT,
	RIGHT,
	TOP,
	BOTTOM,
	LT,
	LB,
	RT,
	RB,
	END,
};

enum class PLAYER_KEY
{
	LEFT,
	RIGHT,
	TOP,
	BOTTOM,
	END,
};

enum class BOSS_DIR
{
	LEFT,
	RIGHT,
	END,
};

enum class SAMPLER_TYPE
{
	DEFAULT,
	END
};

enum class BLEND_TYPE
{
	DEFAULT,
	ALPHABLEND,
	END,
};

enum class COLLIDER2D_TYPE
{
	RECT,
	CIRCLE,
	END
};

enum class SHADER_TYPE
{
	VERTEX_SHADER = 0x01,
	HULL_SHADER = 0x02,
	DOMAIN_SHADER = 0x04,
	GEOMETRY_SHADER = 0x08,
	COMPUTE_SHADER = 0x10,
	PIXEL_SHADER = 0x20,
	END,

};

enum class SHADER_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,
	INT_END,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,
	FLOAT_END,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,
	VEC2_END,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,
	VEC4_END,

	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_END,

	MAT_0,
	MAT_1,
	MAT_2,
	MAT_3,
	MAT_END,

	INT_ARRAY,
	VEC2_ARRAY,
	VEC2_1ARRAY,


	END,
};

enum class EVENT_TYPE
{
	CREATE_OBJECT,
	DELETE_OBJECT,
	ADD_CHILD,
	CLEAR_PARANT,
	CLEAR_CHILD,
	ON_ENABLE,
	ON_DISABLE,
	CHANGE_LAYER,
	DEL_SCRIPT,
	END,
};