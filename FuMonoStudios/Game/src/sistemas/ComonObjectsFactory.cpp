#include <utils/checkML.h>
#include "ComonObjectsFactory.h"
#include "../components/Transform.h"
#include "../components/Render.h"
#include "../components/Clickeable.h"
#include <components/HoverSensorComponent.h>
#include <components/RenderWithLight.h>

ComonObjectsFactory::ComonObjectsFactory(ecs::Scene* sc):scene_(sc), destLayer_(ecs::layer::DEFAULT),fontName_("arial"){}
ComonObjectsFactory::~ComonObjectsFactory()
{
	//futuro sistema de limpieza
	//primero hace falta que las COF sean un atributo de la clase escena
	for (auto t : createdTextures) {
		delete t;
	}
}

ecs::Entity* ComonObjectsFactory::createMultiTextureImage(const Vector2D& pos, const Vector2D& size, const std::vector<Texture*>& textures)
{
	auto entity = scene_->addEntity(destLayer_);
#ifdef _DEBUG
	std::cout << "Entidad multitextura creada en la capa " << (int)destLayer_ << std::endl;
#endif // _DEBUG
	entity->addComponent<Transform>(pos.getX(), pos.getY(), size.getX(), size.getY());
	entity->addComponent<RenderImage>(textures);
	return entity;
}

ecs::Entity* ComonObjectsFactory::createMultiTextureImage(const Vector2D& pos, const std::vector<Texture*>& textures)
{
	return createMultiTextureImage(pos,Vector2D(textures[0]->width(), textures[0]->height()),textures);
}

ecs::Entity* ComonObjectsFactory::createLabel(const Vector2D& pos , const std::string& text, int fontSize, SDL_Color textColor)
{
	return createImage(pos,createTextTexture(text,fontSize,textColor));
}
ecs::Entity* ComonObjectsFactory::createLabel(const Vector2D& pos,Uint32 width, const std::string& text, int fontSize, SDL_Color textColor)
{
	return  createImage(pos, createTextTexture(text, fontSize, textColor,width));
}
ecs::Entity* ComonObjectsFactory::createLabel(const Vector2D& pos, const Vector2D& size, const std::string& text, int fontSize, SDL_Color textColor)
{
	return createImage(pos, size, createTextTexture(text,fontSize,textColor,size.getX()));
}

ecs::Entity* ComonObjectsFactory::createImage(const Vector2D& pos, const Vector2D& size, Texture* texture)
{
#ifdef _DEBUG
	std::cout << "Entidad creada en la capa " << (int)destLayer_ << std::endl;
#endif // _DEBUG
	return createMultiTextureImage(pos, size, { texture });
}

ecs::Entity* ComonObjectsFactory::createImage(const Vector2D& pos, Texture* texture)
{
	return createImage(pos, Vector2D(texture->width(), texture->height()), texture);
}

ecs::Entity* ComonObjectsFactory::createImageButton(const Vector2D& pos, const Vector2D& size, Texture* texture, 
	CallbackClickeable call, std::string soundClick)
{
	auto entity = createImage(pos,size,texture);
	makeButton(entity, call, soundClick);
	return entity;
}

ecs::Entity* ComonObjectsFactory::createTextuButton(const Vector2D& pos, const std::string text, int fontSize, CallbackClickeable call, 
	std::string soundClick, SDL_Color textColor)
{
	auto entity = createLabel(pos, text, fontSize,textColor);
	makeButton(entity, call, soundClick);
	return entity;
}

void ComonObjectsFactory::addHoverColorMod(ecs::Entity* entity, SDL_Color c)
{
	HoverSensorComponent* hover = entity->getComponent<HoverSensorComponent>();
	if (hover== nullptr) {
		hover = entity->addComponent<HoverSensorComponent>();
	}
	//posiblemente meter en un metodo que agregue esta propiedad
	auto texture = entity->getComponent<RenderImage>()->getTexture();
	hover->addInCall([texture, c]() {texture->modColor(c.r, c.g, c.b); });
	hover->addOutCall([texture]() {texture->modColor(255, 255, 255); });
	hover->addDestoryCall([texture]() {texture->modColor(255, 255, 255); });
}

void ComonObjectsFactory::addHilghtOnHover(ecs::Entity* entity)
{
	HoverSensorComponent* hover = entity->getComponent<HoverSensorComponent>();
	if (hover == nullptr) {
		hover = entity->addComponent<HoverSensorComponent>();
	}
	//añade feeback al pasar el raton por encima
	auto light = entity->addComponent<RenderWithLight>();
	hover = entity->addComponent<HoverSensorComponent>();
	hover->addInCall([light]() {light->lightOn(); });
	hover->addOutCall([light]() {light->lightOff(); });

	Clickeable* click = entity->getComponent<Clickeable>();
	if(click != nullptr)
		click->addEvent([light]() {light->lightOff(); });
}

Texture* ComonObjectsFactory::createTextTexture(const std::string& text, int fontSize, SDL_Color c,int width)
{
	Texture* nText;
	if(width == 0)
		nText = new Texture(sdlutils().renderer(), text, sdlutils().fonts().at(fontName_ + std::to_string(fontSize)), c);
	else if (width > 0) {
		nText = new Texture(sdlutils().renderer(), text, sdlutils().fonts().at(fontName_ + std::to_string(fontSize)), c, width);
	}
	else {
		throw "Invalid Texture Width";
	}
	createdTextures.push_back(nText);
	return nText;
}

void ComonObjectsFactory::makeButton(ecs::Entity* entity, CallbackClickeable call, std::string clickSound)
{
	auto click = entity->addComponent<Clickeable>(clickSound);
	click->addEvent(call);
}

