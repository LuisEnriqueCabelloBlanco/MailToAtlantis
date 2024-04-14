#include "ComonObjectsFactory.h"
#include "../components/Transform.h"
#include "../components/Render.h"
#include "../components/Clickeable.h"



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
	Texture* labelText = new Texture(sdlutils().renderer(), text, sdlutils().fonts().at(fontName_ + std::to_string(fontSize)), textColor);
	createdTextures.push_back(labelText);
	return createImage(pos,labelText);
}
ecs::Entity* ComonObjectsFactory::createLabel(const Vector2D& pos,Uint32 width, const std::string& text, int fontSize, SDL_Color textColor)
{
	Texture* labelText = new Texture(sdlutils().renderer(), text, sdlutils().fonts().at(fontName_ + std::to_string(fontSize)), textColor, width);
	createdTextures.push_back(labelText);
	return createImage(pos, labelText);
}
ecs::Entity* ComonObjectsFactory::createLabel(const Vector2D& pos, const Vector2D& size, const std::string& text, int fontSize, SDL_Color textColor)
{
	Texture* labelText = new Texture(sdlutils().renderer(), text, sdlutils().fonts().at(fontName_ + std::to_string(fontSize)), textColor, size.getX());
	createdTextures.push_back(labelText);
	
	return createImage(pos, size, labelText);
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

ecs::Entity* ComonObjectsFactory::createImageButton(const Vector2D& pos, const Vector2D& size, Texture* texture, CallbackClickeable call)
{
	auto entity = createImage(pos,size,texture);
	auto click = entity->addComponent<Clickeable>();
	click->addEvent(call);
	return entity;
}

ecs::Entity* ComonObjectsFactory::createTextuButton(const Vector2D& pos, const std::string text, int fontSize, CallbackClickeable call, SDL_Color textColor)
{
	auto entity = createLabel(pos, text, fontSize,textColor);
	auto click = entity->addComponent<Clickeable>();
	click->addEvent(call);
	return entity;
}

