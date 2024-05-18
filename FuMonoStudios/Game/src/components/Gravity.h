#pragma once
#include <architecture/Component.h>
#include <architecture/GameConstants.h>
#include <string>

class Transform;

// Aplica gravedad al Transform. �Importante!
// si el entity tiene draganddrop, aplicar el componente
// gravity antes de aplicar el de draganddrop
// Para evitar fallos, a�adir este componente justo despu�s del Transform
class Gravity : public ecs::Component
{
public:
	__CMP_DECL__(ecs::cmp::GRAVITY)
	Gravity();
	Gravity(std::string);
	Gravity(float gravityForce, std::string sound);
	~Gravity();

	void initComponent() override;
	void update() override;
	/// <summary>
	/// Activa o desactiva la gravedad
	/// </summary>
	/// <param name="value"></param>
	void setActive(bool value);
private:

	Transform* tr_;
	float gravityForce_;
	float velocity_;
	bool active_;
	bool latestcontactGround_;

	std::string thumpSound_;
};

