#pragma once
#include "Core.h"
namespace MarkTech
{
	class MARKTECH_API CBaseComponent
	{
	public:
		CBaseComponent();
		CBaseComponent(uint64_t ownerId);
		~CBaseComponent();

		virtual void InitComponent() {};
		virtual void UpdateComponent(float flDeltaTime) {};

		uint64_t GetId() { return m_nId; }
		uint64_t GetOwnerId() { return m_nOwnerId; }
	protected:
		uint64_t m_nId;
		uint64_t m_nOwnerId;
	};

	uint64_t GenerateId();
}