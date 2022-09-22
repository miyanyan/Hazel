#pragma once

#include "Hazel/Scene/Entity.h"

namespace Hazel {

	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() = default;

		template<typename T>
		T& getComponent()
		{
			return m_entity.getComponent<T>();
		}

	protected:
		virtual void onCreate() {}
		virtual void onDestory() {}
		virtual void onUpdate(Timestep ts) {}

	private:
		Entity m_entity;
		friend class Scene;
	};

}