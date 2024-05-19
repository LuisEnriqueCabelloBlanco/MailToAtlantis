#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include <architecture/Scene.h>
#include <sdlutils/VirtualTimer.h>

class EndWorkScene :public ecs::Scene {
public:
	EndWorkScene();
	virtual ~EndWorkScene();

	void init() override;
	void close() override;

	void update() override;
private:
	void createTextAnim(std::string msg); // Nos crea un texto preparandolo para animarlo
	void animTextos(ecs::Entity* texto);	  // Efecto que le damos a fallos/aciertos
	void animNumeros(ecs::Entity* number); // Efecto incremento de numeros
	void createButtons(); // Lo llamamos al final para crear botones despu�s de las anims

	bool animFinish; // Para indicar si debemos crear los botones u otras cosas
	int totalAnims_ = 0; // Cuantas anims tendremos empezando desde el 0,
						 // iremos contandolas a medida que se crean con createTextAnim()
	int currentAnim_ = 0; // Lleva la cuenta de por que anim vamos, empieza desde el 0
	int offset_ = 80; // Distancia entre cada texto
	double animCooldown_ = 0.8; // Tiempo en segundos entre cada anim
	VirtualTimer timer_;
	Vector2D pos_; // Donde vamos a ir colocando el texto

	std::vector<ecs::Entity*> anims_;
};