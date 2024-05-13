#pragma once
#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include <string>
#include <architecture/Scene.h>
#include <architecture/Entity.h>
#include <components/Clickeable.h>

/*
Clase a la que llamar para crear objetos que se crean frecuentemente
Tales como:
	-Imagenes
	-Textos
	-Botones
Los metodos devuelven un puntero a la entidad por si se le quiere aplicar mas operaciones en tras crearlo

Existen otras operaciones como los setters de capa y fuente para determinar que se va a usar (como si fuera openGL)
Ademas de operaciones para aniadir funcionalidad a las entidades como detecciones de hover o iluminacion en el hover
*/
class ComonObjectsFactory
{
public:
	ComonObjectsFactory(ecs::Scene* sc);
	virtual ~ComonObjectsFactory();
	/// <summary>
	/// establece la capa donde se crearan los objetos usando esta factory
	/// </summary>
	/// <param name="lay"></param>
	void setLayer(ecs::layer::layerId lay) { destLayer_ = lay; }
	/// <summary>
	/// establece que fuente se usara al crear textos con esta factory
	/// </summary>
	/// <param name="name"></param>
	void setFont(const std::string& name) { fontName_ = name; }
	/// <summary>
	/// Crea un objeto que contiene varias im�genes si no se le a�ade tama�o 
	/// se asume el tama�o de la primera textura pasada
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="size"></param>
	/// <param name="textures"></param>
	/// <returns></returns>
	ecs::Entity* createMultiTextureImage(const Vector2D& pos, const Vector2D& size, const std::vector<Texture*>& textures);
	/// <summary>
	/// Crea un objeto que contiene varias im�genes si no se le a�ade tama�o 
	/// se asume el tama�o de la primera textura pasada
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="textures"></param>
	/// <returns></returns>
	ecs::Entity* createMultiTextureImage(const Vector2D& pos, const std::vector<Texture*>& textures);
	/// <summary>
	///  crea un objeto que contiene texto
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="text"></param>
	/// <param name="fontSize"></param>
	/// <param name="textColor"></param>
	/// <returns></returns>
	ecs::Entity* createLabel(const Vector2D& pos,const std::string& text, int fontSize, SDL_Color textColor = build_sdlcolor(0x000000ff));
	/// <summary>
	/// crea un objeto que contiene texto con warp text
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="width">ancho maximo que va a ocupar el texto</param>
	/// <param name="text"></param>
	/// <param name="fontSize"></param>
	/// <param name="textColor"></param>
	/// <returns></returns>
	ecs::Entity* createLabel(const Vector2D& pos, Uint32 width, const std::string& text, int fontSize, SDL_Color textColor = build_sdlcolor(0x000000ff));
	/// <summary>
	/// crea un objeto que contiene texto y ajusta su ancho 
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="size">espacio que ocupa al etiqueta (el acho ajusta el warp del texto)</param>
	/// <param name="text"></param>
	/// <param name="fontSize"></param>
	/// <param name="textColor"></param>
	/// <returns></returns>
	ecs::Entity* createLabel(const Vector2D& pos, const Vector2D& size, const std::string& text, int fontSize, SDL_Color textColor= build_sdlcolor(0x000000ff));
	/// <summary>
	/// Crea un objeto que es una imagen
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="size">vector que representa el ancho(x) y el alto(y) de la imagen </param>
	/// <param name="texture"></param>
	/// <returns></returns>
	ecs::Entity* createImage(const Vector2D& pos, const Vector2D& size, Texture* texture);
	ecs::Entity* createImage(const Vector2D& pos, Texture* texture);
	/// <summary>
	/// crea un boton asociado a una imagen con un callback ya vinculado
	/// </summary>
	/// <param name="pos"></param>
	/// <param name=""></param>
	/// <param name="texture"></param>
	/// <param name="call"></param>
	/// <returns></returns>
	ecs::Entity* createImageButton(const Vector2D& pos, const Vector2D& size, Texture* texture, CallbackClickeable call, const std::string& soundClick);
	/// <summary>
	/// crea un boton a partir de un texto pasado y con un callback ya vinculado
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="text"></param>
	/// <param name="fontSize"></param>
	/// <param name="call"></param>
	/// <param name="textColor"></param>
	/// <returns></returns>
	ecs::Entity* createTextuButton(const Vector2D& pos, const std::string& text,
		int fontSize, CallbackClickeable call, const std::string& soundClick = "click", SDL_Color textColor = build_sdlcolor(0x000000ff));


	/// <summary>
	/// hace que un objeto se pueda oscurecer/modificar su color al pasar el rat�n por ecima de el
	/// </summary>
	/// <param name="entity"></param>
	/// <param name="c"></param>
	void addHoverColorMod(ecs::Entity* entity, SDL_Color c = build_sdlcolor(0x000000ff));
	/// <summary>
	/// a�ade el comportamiento de resaltar cuando el rat�n pasa por encima
	/// </summary>
	/// <param name="entity"></param>
	void addHilghtOnHover(ecs::Entity* entity);

	/// <summary>
	/// Crea una textura de texto de la que se encargar� la 
	/// factory de destruir
	/// </summary>
	/// <param name="text"></param>
	/// <param name="fontSize"></param>
	/// <param name="c"></param>
	/// <returns></returns>
	Texture* createTextTexture(const std::string& text, int fontSize, SDL_Color c = build_sdlcolor(0x000000ff), int width = 0);

	ecs::layer::layerId getLayer() { return destLayer_; }
private:
	/// <summary>
	/// a�ade a la entidad pasada los componentes para ser un boton que da feedback al pasar el cursor por encima
	/// </summary>
	/// <param name=""></param>
	/// <param name="call"></param>
	void makeButton(ecs::Entity*, CallbackClickeable call, std::string clickSound);


	/// <summary>
	/// Scene to create the objects
	/// </summary>
	ecs::Scene* scene_;
	/// <summary>
	/// capa donde se van a crear los objetos
	/// </summary>
	ecs::layer::layerId destLayer_;
	/// <summary>
	/// nombre de la fuente que se usara al crear texturas de texto
	/// </summary>
	std::string fontName_;
	/// <summary>
	/// Vector de texturas creadas que se elimnaran al destruir la instancia de la clase
	/// </summary>
	std::vector<Texture*> createdTextures;
};

