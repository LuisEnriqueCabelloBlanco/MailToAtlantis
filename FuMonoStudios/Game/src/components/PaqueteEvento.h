#pragma once
#include "../architecture/Component.h"
#include <functional>
#include <vector>

using Condition = std::function<bool(Paquete*)>;
using namespace pq;

class GeneralData;

class PaqueteEvento : public ecs::Component
{
public:
	__CMP_DECL__(ecs::cmp::PAQUETEEVENTO)
	PaqueteEvento(GeneralData::NPCevent* event);
	~PaqueteEvento();

	void paqueteSent(Distrito tubo);

	void addCondition(Condition c) { conditions.push_back(c); }
private:
	std::vector<Condition> conditions;
	GeneralData::NPCevent* event_;
};

