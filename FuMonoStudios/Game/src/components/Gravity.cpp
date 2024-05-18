#include "Gravity.h"
#include <components/Transform.h>
#include <architecture/Entity.h>
#include <architecture/Time.h>
#include <sistemas/SoundEmiter.h>

const float MIN_SPEED_SOUND = 5.0f;
const std::string DEFAULT_SOUND = "choque";

Gravity::Gravity() : tr_(nullptr), gravityForce_(DEFAUTL_FORCE), velocity_(0), active_(true), 
    thumpSound_(DEFAULT_SOUND){}

Gravity::Gravity(std::string sound) : tr_(nullptr), gravityForce_(DEFAUTL_FORCE), velocity_(0), 
    active_(true), thumpSound_(sound) { }

Gravity::Gravity(float force, std::string sound) : tr_(nullptr), gravityForce_(force), 
    velocity_(0), active_(true), thumpSound_(sound) {}

Gravity::~Gravity() {}

void Gravity::initComponent() {

    tr_ = ent_->getComponent<Transform>();

    assert(tr_ != nullptr);

    latestcontactGround_ = (tr_->getCenter().getY() >= GRAVITY_LIMIT);
}

void Gravity::update() {

    if (active_) {
        bool contactGround = (tr_->getCenter().getY() >= GRAVITY_LIMIT);

        if (latestcontactGround_ != contactGround) {
            //sdlutils().soundEffects().at("choque0").play();
            if (velocity_ > MIN_SPEED_SOUND) {
                float mod = velocity_ / MAX_VELOCITY;
                std::cout << std::to_string(mod);
                SoundEmiter::instance()->playSound(thumpSound_, mod);
            }
            latestcontactGround_ = contactGround;
        }

        if (!contactGround) {
            velocity_ += gravityForce_ * Time::getDeltaTime();
            if (velocity_ > MAX_VELOCITY) {
                velocity_ = MAX_VELOCITY; // Limitamos la velocidad
            }
            float newY = tr_->getPos().getY() + velocity_;
            if (newY > GRAVITY_LIMIT) {
                newY = GRAVITY_LIMIT;
                velocity_ = 0.0f; // Reseteamos velocidad al tocar el suelo
            }
            tr_->setPos(tr_->getPos().getX(), newY);
        }
    }
}

void Gravity::setActive(bool value) {
    active_ = value;
    velocity_ = 0.0f;
    if(!value)
        latestcontactGround_ = false;
}
