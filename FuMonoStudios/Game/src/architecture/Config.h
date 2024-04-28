#pragma once
#include <utils/checkML.h>
#include "../utils/Singleton.h"
#include "../utils/Vector2D.h"

#include <SDL.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <exception>


class Config : public Singleton<Config> {

	friend Singleton<Config>; // needed to give access to private constructors

public:
	/// <summary>
	/// Esctructura que guarda la informacion de las flechas de navegacion de la escena de exploracion
	/// </summary>
	struct Arrows {

		Vector2D pos;

		std::string destination_;

		double scale_;

		bool directionRight_;

		int flip_;

		Arrows(double x, double y, std::string destination, double scale, bool directionRight, int flip) : pos(x, y), destination_(destination),
			scale_(scale), directionRight_(directionRight), flip_(flip){}


	};
	/// <summary>
	/// Esctructura que guarda la informacion de los personajes en una escena
	/// </summary>
	struct Characters {

		Vector2D pos;

		std::string name_;

		double scale_;

		bool directionRight_;

		Characters(double x, double y, std::string name, double scale, bool directionRight) : pos(x, y), name_(name),
			scale_(scale), directionRight_(directionRight) {}


	};
	/// <summary>
	/// Esctructura que guarda la informacion de los objetos interactuables en una escena
	/// </summary>
	struct InteractableObjs {

		Vector2D pos;

		std::string name_;

		double scaleX_;
		double scaleY_;

		bool directionRight_;

		InteractableObjs(double x, double y, std::string name, double scaleX, double scaleY, bool directionRight) : pos(x, y), name_(name),
			scaleX_(scaleX), scaleY_(scaleY), directionRight_(directionRight) {}


	};
	/// <summary>
	/// Estructura que guarda la informacion de un lugar navegable en la escena de exploracion
	/// </summary>
	struct Places {

		std::vector<Arrows> myArrows;

		std::vector<Characters> myCharacters;

		std::vector<InteractableObjs> myInteractableObjs;

		Places(std::vector<Arrows> arrows, std::vector<Characters> characters, std::vector<InteractableObjs> interactableObjs) :
			myArrows(arrows), myCharacters(characters), myInteractableObjs(interactableObjs){

		}

	};


	template<typename T>
	using sdl_resource_table = std::unordered_map<std::string, T>;

	template<typename T>
	class map_access_wrapper {
		sdl_resource_table<T>& map_;
		std::string desc_;
	public:
		map_access_wrapper(sdl_resource_table<T>& map, std::string desc) :
			map_(map), desc_(desc) {
		}

		inline T& at(const std::string& key) {
	
			try {
				return map_.at(key);
			}
			catch (std::out_of_range& e) {
				throw "Key '" + key + "' does not exists in '" + desc_ + "'";
			}
			catch (std::exception& e) {
				throw "Error when accessing key '" + key + "' of '" + //
					desc_ + "'. Original error was: " + e.what();
			}
			catch (...) {
				throw "Unknown error when accessing key '" + key + "' of '" + //
					desc_ + "'";
			}
		
		}

		inline T& operator[](const std::string& key) {
			return at(key);
		}
	};

	/// <summary>
	/// accesor a los lugares de navegacion
	/// </summary>
	/// <returns></returns>
	inline auto& places() {
		return placesAccessWrapper_;
	}

private:

	Config();
	Config(std::string filename);

	/// <summary>
	/// limpia los datos de la clase
	/// </summary>
	void closeInfo();
	/// <summary>
	/// carga los datos del fichero indicado
	/// </summary>
	/// <param name="filename">ruta del fichero json para cargar</param>
	void loadReasources(std::string filename);


	sdl_resource_table<Places> places_;

	map_access_wrapper<Places> placesAccessWrapper_;

};

inline Config& config() {
	return *Config::instance();
}

