#pragma once
#include "../components/Paquete.h"
#include <vector>

namespace tb {
	struct SwappedPipe {
		//Comprueba si la tuber�a est� cambiada
		bool swapActive;
		//Comprueba si la tuber�a tambi�n lleva a su destino original
		bool originalDis;
		//Distrito al que cambia (si est� activo)
		pq::Distrito changedDis;
	};

	struct WeightRestriction {
		//Checks if the weight is restricted
		bool weightRestricted;
		//Checks if only one type is restricted
		bool singleType;
		//0 if weight has to be higher than x, 1 if it needs to be x, 2 if it needs to be lower than x, 
		//3 if it can be any weight except x
		int minOrMax;
		//Peso comprobado
		pq::NivelPeso x;
		//Si solo comprueban pesos de un tipo, el tipo
		pq::TipoPaquete typeToWeight;
	};


	enum restrictionId {

		BLOCK_PIPE,
		SWAP_PIPE,
		BAN_TYPE_IN_PIPE,
		WEIGHT_RESTRICT_PIPE
	};

	struct WorkEvent {
		std::string dialogue;
		restrictionId id;
		// if we use union we save memory, but then use only primitive
		// types, otherwise you will need to define constructors almost
		// every where.
		union {
			struct
			{
				Distrito targetPipe;
			} block_pipe_data;

			struct
			{
				Distrito targetPipe;
				SwappedPipe dest;
			} swap_pipe_data;

			struct
			{
				Distrito targetPipe;
				TipoPaquete ban;
			} ban_type_pipe_data;

			struct
			{
				Distrito targetPipe;
				WeightRestriction restrictions;
			} weight_res_pipe_data;
		};
	};

}

using namespace tb;

class PipeManager
{
public:
	PipeManager();
	~PipeManager();

	void init();
	void setReturnPipe(pq::Distrito);

	bool checkPackage(Paquete*, pq::Distrito);

	/// <summary>
	/// Blocks a pipe
	/// </summary>
	/// <param name="target">Target pipe</param>
	void blockPipe(pq::Distrito target);
	/// <summary>
	/// Updates the swapped condition of a pipe
	/// </summary>
	/// <param name="target">Target pipe</param>
	/// <param name="toNew">New swapped state</param>
	void swapPipe(pq::Distrito target, SwappedPipe toNew);
	/// <summary>
	/// Bans a certain type of package in a pipe
	/// </summary>
	/// <param name="target">Target pipe</param>
	/// <param name="ban">Type to ban</param>
	void banTypeInPipe(pq::Distrito target, pq::TipoPaquete ban);
	/// <summary>
	/// Changes weight restrictions to be applied to a pipe
	/// </summary>
	/// <param name="target">Target pipe</param>
	/// <param name="restrictions">New weight restrictions to be applied</param>
	void weightRestrictPipe(pq::Distrito target, WeightRestriction restrictions);

	void activateEvent(WorkEvent evento);
private:
	//Comprueba si la tuber�a est� bloqueada o cambiada
	bool checkPipeConditions(Paquete*, pq::Distrito);
	//Comprueba restricciones de peso y tipo
	bool checkPipeRestrictions(Paquete*, pq::Distrito);
	//Comprueba las restriciones de peso
	bool checkWeightRestrictions(Paquete*, pq::Distrito);
	//Comprueba si es imposible mandar un paquete correcto (por ejemplo, si su destino est� bloqueado)
	bool checkReturningConditions(Paquete*);
	//Comprueba si alguna tuberia de las cambiadas es la del distrito del paquete
	bool checkSwappedPipes(Paquete*);

	pq::Distrito returnPipe_;
	std::vector<bool> blockedPipes_;
	std::vector<SwappedPipe> swappedPipes_;
	std::vector<std::pair<bool, pq::TipoPaquete>> bannedTypePipes_;
	std::vector <WeightRestriction> weightRestrictionTypes_;
};

