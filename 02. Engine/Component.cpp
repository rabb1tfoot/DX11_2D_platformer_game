#include "stdafx.h"
#include "Component.h"



CComponent::~CComponent()
{
}


CComponent::CComponent(COMPONENT_TYPE _etype)
	: CEntity()
	, m_eComType(_etype)
	, m_pObj(nullptr)
{
}

CComponent::CComponent(const CComponent & _com)
	: CEntity(_com)
	, m_eComType(_com.m_eComType)
	, m_pObj(nullptr)
{
}
