#include "EndGameScene.h"
#include "../sistemas/ComonObjectsFactory.h"
#include <vector>
#include <sistemas/Felicidad.h>
void EndGameScene::init()
{
	std::vector<Texture*> texturesArray = {
		&sdlutils().images().at("placeHolder")
	};
	factory_->setLayer(ecs::layer::BACKGROUND);
	endImage = factory_->createMultiTextureImage(Vector2D(0, 0), texturesArray);
}
