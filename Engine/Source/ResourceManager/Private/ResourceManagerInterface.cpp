#include "ResourceManagerInterface.h"
#include "Manager.h"

IResourceManager* CreateResourceManager()
{
    return new CResourceManager();
}

void InitModule()
{
}

void UpdateModule()
{
}

void ShutdownModule()
{
}