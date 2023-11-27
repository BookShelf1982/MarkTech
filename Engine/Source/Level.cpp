#include "Level.h"

namespace MarkTech
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
    
    void CLevel::UpdateLevel(float flDeltaTime)
    {
        for (int i = 0; i < m_Comps.GetSize(); i++)
        {
            m_Comps.c_arr()[i]->UpdateComponent(flDeltaTime);
        }
    }
    
    CBaseEntity* CLevel::GetEntityById(uint64_t id)
    {
        for (int i = 0; i < m_Ents.GetSize(); i++)
        {
            if (m_Ents[i].m_nId == id)
            {
                return &m_Ents.c_arr()[i];
            }
        }
        return nullptr;
    }

    uint64_t CLevel::CreateEntity()
    {
        CBaseEntity ent;
        uint64_t id = ent.m_nId;
        m_Ents.Push(ent);
        return id;
    }

    CLevel::CLevel()
    {
    }
    
    CLevel::~CLevel()
    {
    }
}
