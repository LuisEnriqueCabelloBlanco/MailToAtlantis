#pragma once
#include "architecture/Scene.h"

namespace ecs {
	class DeathScene : public ecs::Scene
	{
	public:
		__SC_DECL__(ecs::sc::DEATH_SCENE)

		DeathScene();

		~DeathScene();

		void close() override;
		void init() override;
	private:
	};
}