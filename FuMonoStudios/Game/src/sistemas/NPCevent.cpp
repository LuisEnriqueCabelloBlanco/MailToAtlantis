#include <sistemas/NPCevent.h>


//NPCevent::~NPCevent()
//{
//	for (auto& pq : paquetes) {
//		delete pq;
//	}
//}

NPCevent::NPCevent():recompensas(),paquetes(),condiciones()
{
	numPaquetes = 0;
	numPaquetesToComplete = 0;
	completed = false;
	usingDifConditions = false;
	paquetesDone = 0;
}

NPCevent::~NPCevent()
{
	for (auto& pq : paquetes) {
		delete pq;
	}
}

void NPCevent::checkPaquete(Paquete* p)
{
	if (!completed)
	{
		bool valido = true;
		int i = 0;

		if (usingDifConditions) {
			while (valido && i < condiciones.size()) {
				int j = 0;
				while (valido && i < condiciones.size()) {
					valido = condiciones[i][j](p);
					j++;
				}
				i++;
			}
		}
		else {
			while (valido && i < condiciones[0].size()) {
				valido = condiciones[0][i](p);

				i++;
			}
		}

		if (valido)
			paqueteSuccesful();
	}
}

void NPCevent::paqueteSuccesful()
{
	paquetesDone++;
	completed = paquetesDone >= numPaquetesToComplete;
}
