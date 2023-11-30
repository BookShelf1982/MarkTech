#pragma once
#include "BaseComponent.h"
#include "Level.h"

namespace MarkTech
{
    class CTextureComponent : public CBaseComponent
    {
    public:
        CTextureComponent();
        CTextureComponent(uint64_t id);
        ~CTextureComponent();

        virtual void InitComponent() override;
    private:
        CAssetHandle m_Texture;
    };
}