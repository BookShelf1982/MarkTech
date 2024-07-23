#include "CVar.h"
#include <Assert.h>
#include <string.h>

namespace MarkTech
{
    CVarF32* pCVars = nullptr;
    U32 cvarCount = 0;
    U32 usedCVars = 0;

    F32 GetCVar(const char* pName)
    {
        MT_ASSERT(pCVars);

        // Loop through the cvar array and find the cvar with the name requested by user and then return it's value
        for (U32 i = 0; i < usedCVars; i++)
        {
            if (strcmp(pCVars[i].name, pName) == 0)
                return pCVars[i].value;
        }

        // Create new cvar since we couldn't find the cvar in the cvar chain
        for (U32 i = 0; i < cvarCount; i++)
        {
            if (strlen(pCVars[i].name) == 0)
            {
                strcpy_s(pCVars[i].name, pName);
                usedCVars++;

                return pCVars[i].value;
            }
        }

        return 0.0f;
    }

    void SetCVar(const char* pName, F32 value)
    {
        // Loop through the cvar array and find the cvar with the name requested by user and then set it's value to the requested value
        for (U32 i = 0; i < usedCVars; i++)
        {
            if (strcmp(pCVars[i].name, pName) == 0)
            {
                pCVars[i].value = value;
            }
        }
    }

    void GiveCVarArray(CVarF32* pCVarArray, U32 numOfCVars)
    {
        pCVars = pCVarArray;
        cvarCount = numOfCVars;
    }
}