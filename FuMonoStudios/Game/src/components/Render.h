#pragma once
#include "../architecture/Component.h"
#include "../sdlutils/Texture.h"
#include <assert.h>

class Transform;
class Texture;

class RenderImage : public ecs::Component
{
public:
	__CMP_DECL__(ecs::cmp::IMAGE)
	RenderImage();
	RenderImage(Texture*);
	/// <summary>
	/// Constructora para guardar multiples texturas
	/// </summary>
	/// <param name="textures"></param>
	RenderImage(const std::vector<Texture*>& textures);
	RenderImage(const std::vector<Texture*>& textures, int startIndx);
	~RenderImage();
	void initComponent() override;
	void render() const;
	/// <summary>
	/// cambia a la textura indicada por el indice
	/// </summary>
	/// <param name="indx"></param>
	inline void setTextureIndx(int indx) { 
		assert(texturesVector_.size() > indx);
		currentTextureIndx_ = indx; 
	};
	/// <summary>
	/// cambia la textura de la posicion inicada del array de texturas
	/// </summary>
	/// <param name="texture"></param>
	/// <param name="indx"></param>
	inline void setTexture(Texture* texture, int indx = 0) {
		assert(texturesVector_.size() > indx);
		texturesVector_[indx] = texture;
	}
	/// <summary>
	/// aniade una textura al final de array de texturas
	/// </summary>
	/// <param name="texture"></param>
	inline void addTexture(Texture* texture) { texturesVector_.push_back(texture); }
	inline void setVector(const std::vector<Texture*>& textures) { texturesVector_ = textures; }
	Texture* getCurrentTexture();
	std::vector<Texture*>* getVector();
	void nextTexture();
	void previousTexture();
	void setNumberTexture(int i);
private:
	/// <summary>
	/// indice que indica en que textura del array se encuentra
	/// </summary>
	int currentTextureIndx_;
	/// <summary>
	/// Transform sobre el que se va a renderizar la imagen
	/// </summary>
	Transform* mTr_;
	/// <summary>
	/// Vector de texturas multiples
	/// </summary>
	std::vector<Texture*> texturesVector_;

};

