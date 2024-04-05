#include "EndGameScene.h"
#include "../sistemas/ComonObjectsFactory.h"
#include <vector>
void EndGameScene::init()
{
	std::vector<Texture*> texturesArray = {
		&sdlutils().images().at("placeHolder")
	};
	endImage = factory_->createMultiTextureImage(Vector2D(0, 0),texturesArray);
}
