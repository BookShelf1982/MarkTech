#pragma once
#include "DllExport.h"
#include "Core.h"
#include "BaseEntity.h"

namespace MarkTech
{
	namespace Level
	{
		class MARKTECH_API CLevel
		{
		public:
			bool InitLevel();

			void DestroyLevel();

			static CLevel* GetLevel() { return g_pLevel; }

		private:
			CLevel() {}
			~CLevel() {}

			static CLevel* g_pLevel;

			CTArray<CBaseEntity> entities;

		};
	}
}