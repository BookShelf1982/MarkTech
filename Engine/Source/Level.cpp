#include "Level.h"

namespace MarkTech
{
    namespace Level
    {
        CLevel* CLevel::g_pLevel = new CLevel();

        bool CLevel::InitLevel()
        {
            return true;
        }

        void CLevel::DestroyLevel()
        {
            delete g_pLevel;
        }
    }
}
