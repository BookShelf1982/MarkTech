#pragma once
#include "Core.h"
#include "Pool.h"
#include "Vessel.h"
#include "ModelComponent.h"
#include "TransformComponent.h"

#define MAX_VESSELS 2048
#define MAX_COMPONENTS 1024

class CLevel
{
public:
	CLevel();
	~CLevel();

	bool InitLevel();
	void UpdateLevel(float flDeltaTime);
	void DestroyLevel();

	uint64_t CreateVessel();
	void DestroyVessel(uint64_t vesselId);

	template<class T>
	T CreateComponent();

	template<class T> 
	void AttachComponentToVessel(uint64_t vesselId, T component);

	template<class T>
	void DestroyComponent(uint64_t compId);

	void DestroyAllComponentsFromVessel(uint64_t vesselId);

	template<class T>
	size_t GetIndexOfComponentFromVesselId(uint64_t vesselId);

	void GiveSceneBuffer(IRenderer* pRenderer);
private:
	CTPool<MVessel> m_Vessels;
	CTPool<MModelComponent> m_ModelComponents;
	CTPool<MTransformComponent> m_TransformComponents;
};