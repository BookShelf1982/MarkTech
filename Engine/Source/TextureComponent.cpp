#include "TextureComponent.h"

namespace MarkTech
{
	CTextureComponent::CTextureComponent()
	{
	}

	CTextureComponent::CTextureComponent(uint64_t id)
		:CBaseComponent::CBaseComponent(id)
	{
	}

	CTextureComponent::~CTextureComponent()
	{
	}

	void CTextureComponent::InitComponent()
	{
		m_Texture = GetLevel()->LoadAsset("Textures/grass.mtex", MTexture);
	}
}