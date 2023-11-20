#pragma once
#include "BaseComponent.h"
#include "Vectors.h"

namespace MarkTech
{
    class CTransformComponent : public CBaseComponent
    {
    public:
        CTransformComponent();
        CTransformComponent(uint64_t ownerId);
        ~CTransformComponent();

        virtual void InitComponent() override;
        virtual void UpdateComponent(float flDeltaTime) override;

    protected:
        MTransform m_Transform;
    };
}