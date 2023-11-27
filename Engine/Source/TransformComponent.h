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

        void SetPosition(MVector3 pos);
        void SetRotation(MRotator rot);
        void SetScale(MVector3 scale);

        const MVector3& GetPosition() { return m_Transform.Position; }
        const MRotator& GetRotation() { return m_Transform.Rotation; }
        const MVector3& GetScale() { return m_Transform.Scale; }

        const MVector3& GetForwardVector();
        const MVector3& GetRightVector();

    protected:
        MTransform m_Transform;
    };
}