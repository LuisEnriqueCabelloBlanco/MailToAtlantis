#pragma once
#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include "../architecture/Entity.h"
#include "../architecture/GeneralData.h"
#include "../components/Wrap.h"
#include "../components/MoverTransform.h"
#include <components/Paquete.h>
#include <json/JSON.h>




class PaqueteBuilder
{
	friend NPCeventSystem;
public:
	//Método al que se llama que devuelve un Paquete generado aleatoriamente 
	ecs::Entity* buildPackage(int level, ecs::Scene*);
	//Método al que se llama que devuelve una Carta generada aleatoriamente 
	void cartaRND(ecs::Entity* packageBase);

	ecs::Entity* customPackage(pq::Distrito, pq::Calle, const std::string& remitente, pq::TipoPaquete, bool correcto = true, 
		pq::NivelPeso nivPeso = pq::Ninguno, int peso = 0,
		bool fragil = false, bool carta = false);


	static void initdata();

	PaqueteBuilder(ecs::Scene*);

	~PaqueteBuilder();

private:
	typedef struct {
		int streetErrorChance;
		int stampErrorChance;
		int weithChance;
		int weightErrorChance;
		int notFragileChance;
	}DifficultySettings;


	pq::Distrito distritoRND();	//Método que elige un distrito aleatorio de los que hay
	pq::TipoPaquete tipoRND();		//Método que elige un tipo de paquete aleatorio entre los que hay
	pq::Calle calleRND(int probError);	//Método que elige una calle aleatoria de las posibilidades. El valor probError es, sobre 100, la probabilidad de que sea una calle incorrecta
	bool boolRND(int probFalse);		//Método que genera un bool con valor aleatorio entre true y false. El valor probFalse es, sobre 100, la probabilidad de que sea false
	pq::NivelPeso pesoRND(int probPeso, int probError, int& peso);	//Método que elige si un paquete tiene peso, y si es erróneo, devolviendo un peso para el paquete con la variable "peso"
	std::string remitenteRND();			//Método que elige un nombre random de Remitente

	ecs::Entity* buildBasePackage(ecs::Scene* mScene, bool esCarta);
	void stdRandPackage(ecs::Entity*, int);
	void paqueteNPC(ecs::Entity*);
	/// <summary>
	/// determina si se debe crear un paquete de nPC
	/// </summary>
	/// <returns></returns>
	bool shouldBuildNPCPackage();

	/// <summary>
	/// Funcion auxiliar para cargar en el mapa las direcciones
	/// </summary>
	/// <param name="filename">direccion del fichero json</param>
	/// <param name="dist">valor enum del distritio al que pertenece</param>
	/// <param name="distString">valor string del distrito al que pertenece</param>
	static void getStreetsFromJSON(JSONObject& root, Distrito dist);
	/// <summary>
	/// Carga los nombres y apellidos que pueden salir en los paquetes
	/// </summary>
	static void getNamesFromJSON();

	/// <summary>
	/// Metodo para leer todos los patrones de empaquetado del JSON
	/// </summary>
	static void getRoutesFromJSON();

	/// <summary>
	/// Selecciona una ruta de las que estan en la lista de rutas
	/// </summary>
	void selectRandomRoute();

	DifficultySettings getLevelSetings(int lvl);

	// Se llama a este después de crear el paquete
	void addVisualElements(ecs::Entity* paq);
	// coge el paquete recien creado y le añade el componente visual de dirección
	void createVisualDirections(ecs::Entity* paq, Paquete* paqComp);
	// coge el paquete recien creado y le añade el componente visual de sello
	void crearSello(ecs::Entity* paq, const std::string& texKey, int x, int y, int width, int height);

	// esto hay que cambiarlo de sitio, al scene o algo
	Font* directionsFont;

	std::vector<Texture*> createdTextures;

	ecs::Scene* mScene_;

	//Ruta con los distintos puntos por los que debe pasar el ratón

	/// <summary>
	/// Puntero a la ruta con los distintos puntos por los que debe pasar el ratón
	/// </summary>
	std::list<int> route;

	/// <summary>
	/// Almacena las rutas de la cinta de envalar para los paquetes
	/// </summary>
	static std::vector<std::list<int>> allRoutes;

	//Indice de la ruta en el array de allRoutes
	int selectedRouteIndex;

	/*Se hacen estáticos estos atributos ya que comparten el mismo valor entre todas
	las instancias. Se podria hacer tal vez en una clase auxiliar que se encargue solo de esto? tal vez pero por ahora es un hotfix
	Firmado Luis*/
	/// <summary>
	/// mapa que relaciona cada distrito con su calle
	/// usado para la generacion del string de la direccion
	/// </summary>
	static std::unordered_map<Distrito, std::vector<std::string>> distritoCalle_;
	/// <summary>
	/// vectores con nombres y apellidos para generar nombres de remitentes
	/// </summary>	
	static std::vector<std::string> names;
	static std::vector<std::string> surnames;
};
