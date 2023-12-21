#pragma once
#include "Core.h"
#include "ComponentPool.h"


namespace MarkTech
{
	class MARKTECH_API CComponentRegistry
	{
	public:
		CComponentRegistry();
		~CComponentRegistry();

		void InitializeComponents();
		void UpdateComponents(float flDeltaTime);

		/////////////////////////////////////////////////////////////////////
		/// Func: CreateComponent()										/////
		/// Desc: Creates a component with specified owner				/////
		/// Params: uint64_t component owner id							/////
		/////////////////////////////////////////////////////////////////////

		template<class T>
		uint64_t CreateComponent(uint64_t ownerId) {};

		/////////////////////////////////////////////////////////////////////
		/// Func: ComponentExists()										/////
		/// Desc: Checks to see if a component of specific id exists	/////
		/// Params: uint64_t component id								/////
		/////////////////////////////////////////////////////////////////////

		template<class T>
		bool ComponentExists(uint64_t id) {};

		/////////////////////////////////////////////////////////////////////////
		/// Func: ComponentExists()											/////
		/// Desc: Gives a reference	that points to the specified component	/////
		/// Params: uint64_t component id									/////
		/////////////////////////////////////////////////////////////////////////

		template<class T>
		T* GetComponentFromOwner(uint64_t id) {};

	private:
	};
}