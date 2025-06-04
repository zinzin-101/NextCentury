#pragma once
#include "ColliderObject.h"
#include "GameEngine.h"
#include "LivingEntity.h"
#include "ParticleProperties.h"
#include "PlayerObject.h"
#include "BlightFlame.h"
#include "Wailer.h"
#include "Zealot.h"
#include "ElivaBoss.h"
#include "FlameDamage.h"
#include "ProjectileObject.h"
#include "SquareBorderMesh.h"

#include "random.h"

template <class TargetEntityType>
class DamageCollider : public ColliderObject {
	private:
		float lifespan;
		int damage;
		bool canDamage;
		LivingEntity* owner;

		float timeRemaining;
		bool canDecreaseTimeRemaining;

		bool followOwner;
		glm::vec3 followOffset;

		std::string damageTag;

		ParticleSystem* emitter;


	public:
		DamageCollider(LivingEntity* owner, int damage, float lifespan);
		~DamageCollider();
		virtual void update(std::list<DrawableObject*>& objectsList);
		virtual void onCollisionEnter(Collider* collider);
		virtual void onCollisionStay(Collider* collider);
		virtual void onTriggerEnter(Collider* collider);
		virtual void onTriggerStay(Collider* collider);

		void trigger(glm::vec3 pos);
		void trigger(glm::vec3 pos, glm::vec3 offset);

		void setLifeSpan(float lifespan);
		void setDamage(int damage);
		void setOwner(LivingEntity* owner);
		void setFollowOwner(bool value);
		void setFollowOffset(glm::vec3 offset);
		void setCanDecreaseTime(bool value);
		void setDamageTag(std::string tag);
		void setCanDamage(bool value);

		void addEmitter(std::list<DrawableObject*>& objectsList);

		LivingEntity* getOwner() const;
		std::string getDamageTag() const;
		ParticleSystem* getEmitter() const;

		virtual void render(glm::mat4 globalModelTransform);
		virtual void drawCollider();
};

template <class TargetEntityType>
DamageCollider<TargetEntityType>::DamageCollider(LivingEntity* owner, int damage, float lifespan) : owner(owner), damage(damage), lifespan(lifespan) {
	this->getColliderComponent()->setTrigger(true);
	this->setName(owner->getName() + "DamageCollider");
	this->timeRemaining = 0.0f;
	this->setActive(false);
	this->followOwner = false;
	//this->setDrawCollider(true); // for debug
	this->canDecreaseTimeRemaining = true;
	damageTag = "";
	emitter = nullptr;
	canDamage = true;
}

template <class TargetEntityType>
DamageCollider<TargetEntityType>::~DamageCollider() {
	if (emitter != nullptr) {
		//destroyObject(emitter); // Destroyed by the main loop, no need to call it here.
	}
}

template <class TargetEntityType>
void DamageCollider<TargetEntityType>::update(std::list<DrawableObject*>& objectsList) {
	processCollider();

	if (emitter != nullptr) {
		emitter->update(objectsList);
	}

	if (followOwner) {
		glm::vec3 pos = followOffset;
		pos.x *= (owner->getIsFacingRight() ? 1.0f : -1.0f);
		pos += owner->getTransform().getPosition();
		this->transform.setPosition(pos);
	}

	if (lifespan <= 0.0f) {
		return;
	}

	if (timeRemaining > 0.0f && canDecreaseTimeRemaining) {
		//std::cout << "hitbox active" << std::endl;
		timeRemaining -= GameEngine::getInstance()->getTime()->getDeltaTime();
		if (timeRemaining <= 0.0f) {
			//std::cout << "hitbox deactivated" << std::endl;
			this->setActive(false);
			timeRemaining = 0.0f;
		}
	}
}

template <class TargetEntityType>
void DamageCollider<TargetEntityType>::onCollisionEnter(Collider* collider) {
	if (!canDamage) {
		return;
	}

	DrawableObject* obj = collider->getObject();
	//EnemyObject* entity = dynamic_cast<EnemyObject*>(obj);
	TargetEntityType* entity = dynamic_cast<TargetEntityType*>(obj);

	if (entity != NULL) {
		EnemyObject* enemy = dynamic_cast<EnemyObject*>(obj);
		if (enemy != NULL) {
			PlayerObject* playerAsOwner = dynamic_cast<PlayerObject*>(this->owner);

			if (playerAsOwner != NULL && playerAsOwner->getIsParrying()) {
				return;
			}

			if (damageTag == "HeavyAttack2") {
				GameEngine::getInstance()->pauseTimeForSeconds(0.2f); // hitstop
				float playerX = playerAsOwner->getTransform().getPosition().x;
				float enemyX = enemy->getTransform().getPosition().x;
				(playerX <= enemyX) ? enemy->knockback(glm::vec2(2.0f, 2.0f), 0.5f) : enemy->knockback(glm::vec2(-2.0f, 2.0f), 0.5f);
			}

			Zealot* zealot = dynamic_cast<Zealot*>(obj);
			if (zealot != NULL && (damageTag == "HeavyAttack1" || damageTag == "HeavyAttack2" || damageTag == "FinalNormalAttack")) {
				zealot->setCurrentState(EnemyObject::FLINCH);
				entity->takeDamage(damage);
				return;
			}

			BlightFlame* bf = dynamic_cast<BlightFlame*>(obj);
			if (bf != NULL) {
				bf->takeDamage(damage);

				if (damageTag == "FinalNormalAttack") {
					bf->setCurrentState(EnemyObject::FLINCH);
					return;
				}

				if (damageTag == "HeavyAttack2") {
					float playerX = playerAsOwner->getTransform().getPosition().x;
					float bfX = bf->getTransform().getPosition().x;
					(playerX <= bfX) ? bf->knockback(glm::vec2(5.0f, 5.0f), 0.5f) : bf->knockback(glm::vec2(-5.0f, 5.0f), 0.5f);
					bf->setCurrentState(EnemyObject::FLINCH);
					return;
				}

				return;
			}

			Wailer* wailer = dynamic_cast<Wailer*>(obj);
			if (wailer != NULL) {
				wailer->takeDamage(damage);

				if (damageTag == "HeavyAttack1" || damageTag == "HeavyAttack2" || damageTag == "FinalNormalAttack") {
					Animation::State animState = wailer->getAnimationComponent()->getCurrentAnimationState();
					
					if (animState.name == "WindUp") {
						float playerX = playerAsOwner->getTransform().getPosition().x;
						float wailerX = wailer->getTransform().getPosition().x;
						(playerX <= wailerX) ? wailer->knockback(glm::vec2(5.0f, 5.0f), 0.5f) : wailer->knockback(glm::vec2(-5.0f, 5.0f), 0.5f);
						wailer->setCurrentState(Wailer::FLINCH);
						return;
					}

					if (animState.name == "Summoning" && animState.isPlaying) {
						wailer->setCurrentState(Wailer::FLINCH);
					}

					return;
				}

				return;
			}

			ElivaBoss* boss = dynamic_cast<ElivaBoss*>(obj);
			if (boss != NULL) {
				boss->takeDamage(damage);

				if (damageTag == "HeavyAttack2") {
					boss->signalStagger();
				}

				return;
			}
		}
		
		entity->takeDamage(damage);
		//std::cout << entity->getName() << " took " << damage << " damage" << std::endl;
		PlayerObject* player = dynamic_cast<PlayerObject*>(obj);
		if (player != NULL && !player->getIsParrying() && player->getCanTakeDamage()) {
			player->flinch(PlayerStat::FLINCH_TIME);
			return;
		}
	}
}

template <class TargetEntityType>
void DamageCollider<TargetEntityType>::onCollisionStay(Collider* collider) {
	PlayerObject* playerAsOwner = dynamic_cast<PlayerObject*>(this->owner);

	if (playerAsOwner != NULL) {
		return;
	}

	if (canDamage) {
		DrawableObject* obj = collider->getObject();
		TargetEntityType* entity = dynamic_cast<TargetEntityType*>(obj);
		
		if (entity == NULL) {
			return;
		}
		
		entity->takeDamage(damage);

		PlayerObject* player = dynamic_cast<PlayerObject*>(obj);
		if (player != NULL && !player->getIsParrying() && player->getCanTakeDamage()) {
			canDamage = false;
			player->flinch(PlayerStat::FLINCH_TIME);
			return;
		}
	}
}

template <class TargetEntityType>
void DamageCollider<TargetEntityType>::onTriggerEnter(Collider* collider) { // for handling parrying from player
	if (!canDamage) {
		return;
	}
	DrawableObject* obj = collider->getObject();
	DamageCollider<EnemyObject>* playerDamageCollider = dynamic_cast<DamageCollider<EnemyObject>*>(obj);

	if (playerDamageCollider != NULL) {
		PlayerObject* player = dynamic_cast<PlayerObject*>(playerDamageCollider->getOwner());
		if (player != NULL) {
			if (player->getIsParrying() && !this->canDamage) {
				//std::cout << owner->getName() << " got parried" << std::endl;
				GameEngine::getInstance()->getRenderer()->getCamera()->startShake(0.3f);
				EnemyObject* enemyObj = dynamic_cast<EnemyObject*>(this->getOwner());

				if (enemyObj != NULL) {
					ElivaBoss* boss = dynamic_cast<ElivaBoss*>(enemyObj);
					if (boss != NULL) {
						boss->signalStun();
					}
					else {
						enemyObj->setCurrentState(EnemyObject::STUNNED);
						this->canDamage = false;
					}

					float parryDirection = (player->getTransform().getPosition().x < enemyObj->getTransform().getPosition().x) ? 1.0f : -1.0f;
					for (int i = 0; i < 5; i++) {
						ParticleProperties particleProps = ParticleProperties(
							enemyObj->getTransform().getPosition(),
							glm::vec2(parryDirection * (30.0f * Random::Float() + 20.f), 10.0f * Random::Float() + 20.0f),
							glm::vec2(-0.1f, 0.1f),
							glm::vec3(0.976f, 0.914f, 0.035f),
							0.15f, 0.1f, 0.05f
						);
						enemyObj->getEmitter()->emit(particleProps);
					}

					GameEngine::getInstance()->pauseTimeForSeconds(0.1f); // hitstop

					player->signalSuccessfulParry();

					return;
				}
			}

			Zealot* zealot = dynamic_cast<Zealot*>(this->getOwner());
			if (zealot != NULL && playerDamageCollider->getDamageTag() == "HeavyAttack2") {
				GameEngine::getInstance()->getRenderer()->getCamera()->startShake(0.3f);
				zealot->setCurrentState(EnemyObject::STUNNED);
				float parryDirection = (player->getTransform().getPosition().x < zealot->getTransform().getPosition().x) ? 1.0f : -1.0f;
				for (int i = 0; i < 5; i++) {
					ParticleProperties particleProps = ParticleProperties(
						zealot->getTransform().getPosition(),
						20.0f * glm::vec2(parryDirection * Random::Float(), Random::Float()),
						glm::vec2(-0.1f, 0.1f),
						glm::vec3(0.8f, 0, 0),
						0.2f, 0.1f, 0.05f, 1.0f
					);
					zealot->getEmitter()->emit(particleProps);
				}

				return;
			}

			BlightFlame* bf = dynamic_cast<BlightFlame*>(this->getOwner());
			if (bf != NULL && (playerDamageCollider->getDamageTag() == "HeavyAttack1" || playerDamageCollider->getDamageTag() == "HeavyAttack2")) {
				bf->setCurrentState(EnemyObject::FLINCH);
				return;
			}
		}
	}

	PlayerObject* player = dynamic_cast<PlayerObject*>(this->getOwner());
	if (player != NULL) {
		ProjectileObject<PlayerObject>* projectile = dynamic_cast<ProjectileObject<PlayerObject>*>(collider->getObject());
		if (projectile != NULL) {
			GameEngine::getInstance()->pauseTimeForSeconds(0.1f); // hitstop
			GameEngine::getInstance()->getRenderer()->getCamera()->startShake(0.2f);

			float parryDirection = (player->getTransform().getPosition().x < projectile->getTransform().getPosition().x) ? -1.0f : 1.0f;
			for (int i = 0; i < 5; i++) {
				ParticleProperties particleProps = ParticleProperties(
					projectile->getTransform().getPosition(),
					20.0f * glm::vec2(parryDirection * Random::Float(), Random::Float()),
					glm::vec2(-0.1f, 0.1f),
					glm::vec3(0.0f, 0.9f, 0.0f),
					0.2f, 0.1f, 0.05f, 1.0f
				);
				if (this->emitter != nullptr) {
					this->getEmitter()->emit(particleProps);
				}
			}

			player->signalSuccessfulParry();
			projectile->disable();
			return;
		}
	}
}

template <class TargetEntityType>
void DamageCollider<TargetEntityType>::onTriggerStay(Collider* collider) {
	PlayerObject* playerAsOwner = dynamic_cast<PlayerObject*>(this->owner);

	if (playerAsOwner != NULL) {
		return;
	}

	DrawableObject* obj = collider->getObject();
	DamageCollider<EnemyObject>* playerDamageCollider = dynamic_cast<DamageCollider<EnemyObject>*>(obj);

	if (playerDamageCollider != NULL) {
		PlayerObject* player = dynamic_cast<PlayerObject*>(playerDamageCollider->getOwner());
		if (player != NULL) {
			if (player->getIsParrying() && !this->canDamage) {
				//std::cout << owner->getName() << " got parried" << std::endl;
				GameEngine::getInstance()->getRenderer()->getCamera()->startShake(0.3f);
				EnemyObject* enemyObj = dynamic_cast<EnemyObject*>(this->getOwner());

				if (enemyObj != NULL) {
					ElivaBoss* boss = dynamic_cast<ElivaBoss*>(enemyObj);
					if (boss != NULL) {
						boss->signalStun();
					}
					else {
						enemyObj->setCurrentState(EnemyObject::STUNNED);
						this->canDamage = false;
						this->setActive(false);
					}

					float parryDirection = (player->getTransform().getPosition().x < enemyObj->getTransform().getPosition().x) ? 1.0f : -1.0f;
					for (int i = 0; i < 5; i++) {
						ParticleProperties particleProps = ParticleProperties(
							enemyObj->getTransform().getPosition(),
							glm::vec2(parryDirection * (30.0f * Random::Float() + 20.f), 10.0f * Random::Float() + 20.0f),
							glm::vec2(-0.1f, 0.1f),
							glm::vec3(0.976f, 0.914f, 0.035f),
							0.15f, 0.1f, 0.05f
						);
						enemyObj->getEmitter()->emit(particleProps);
					}

					GameEngine::getInstance()->pauseTimeForSeconds(0.1f); // hitstop

					player->signalSuccessfulParry();

					return;
				}
			}
		}
	}
}

template <class TargetEntityType>
void DamageCollider<TargetEntityType>::trigger(glm::vec3 pos) {
	timeRemaining = lifespan;
	this->transform.setPosition(pos + followOffset);
	this->setActive(true);

	// debug //
	//std::cout << "damage: " << damage << std::endl;
}

template <class TargetEntityType>
void DamageCollider<TargetEntityType>::trigger(glm::vec3 pos, glm::vec3 offset) {
	timeRemaining = lifespan;
	this->transform.setPosition(pos + offset);
	this->setActive(true);
}

template <class TargetEntityType>
void DamageCollider<TargetEntityType>::setLifeSpan(float lifespan) {
	this->lifespan = lifespan;
}

template <class TargetEntityType>
void DamageCollider<TargetEntityType>::setDamage(int damage) {
	this->damage = damage;
}

template <class TargetEntityType>
void DamageCollider<TargetEntityType>::setOwner(LivingEntity* owner) {
	this->owner = owner;
}

template <class TargetEntityType>
void DamageCollider<TargetEntityType>::setFollowOwner(bool value) {
	followOwner = value;
}

template <class TargetEntityType>
void DamageCollider<TargetEntityType>::setFollowOffset(glm::vec3 offset) {
	followOffset = offset;
}

template <class TargetEntityType>
void DamageCollider<TargetEntityType>::setCanDecreaseTime(bool value) {
	canDecreaseTimeRemaining = value;
}

template <class TargetEntityType>
void DamageCollider<TargetEntityType>::setDamageTag(std::string tag) {
	this->damageTag = tag;
}

template <class TargetEntityType>
void DamageCollider<TargetEntityType>::setCanDamage(bool value) {
	this->canDamage = value;
}

template <class TargetEntityType>
void DamageCollider<TargetEntityType>::addEmitter(std::list<DrawableObject*>& objectsList) {
	if (emitter == nullptr) {
		emitter = new ParticleSystem();
		emitter->setName(owner->getName() + "DamageColliderEmitter");
		objectsList.emplace_back(emitter);
	}
}

template <class TargetEntityType>
LivingEntity* DamageCollider<TargetEntityType>::getOwner() const {
	return owner;
}

template <class TargetEntityType>
std::string DamageCollider<TargetEntityType>::getDamageTag() const {
	return damageTag;
}

template <class TargetEntityType>
ParticleSystem* DamageCollider<TargetEntityType>::getEmitter() const {
	return emitter;
}

template <class TargetEntityType>
void DamageCollider<TargetEntityType>::render(glm::mat4 globalModelTransform) {
	if (collider == nullptr) {
		return;
	}

	if (emitter != nullptr) {
		emitter->render(globalModelTransform);
	}

	if (!canDrawCollider || !this->getIsActive()) {
		return;
	}

	SquareBorderMesh* squareBorderMesh = dynamic_cast<SquareBorderMesh*> (GameEngine::getInstance()->getRenderer()->getMesh(SquareBorderMesh::MESH_NAME));

	GLuint modelMatixId = GameEngine::getInstance()->getRenderer()->getModelMatrixAttrId();
	GLuint renderModeId = GameEngine::getInstance()->getRenderer()->getModeUniformId();
	GLuint colorId = GameEngine::getInstance()->getRenderer()->getColorUniformId();
	GLuint brightnessId = GameEngine::getInstance()->getRenderer()->getBrightnessUniformId();

	if (modelMatixId == -1) {
		cout << "Error: Can't perform transformation " << endl;
		return;
	}
	if (renderModeId == -1) {
		cout << "Error: Can't set renderMode in DamageCollider " << endl;
		return;
	}
	if (colorId == -1) {
		cout << "Error: Can't set color " << endl;
		return;
	}

	vector <glm::mat4> matrixStack;

	//glm::mat4 currentMatrix = this->getTransformMat4();
	Transform colTransform = collider->getGlobalTransform(this->transform);
	colTransform.scales(collider->getWidth(), collider->getHeight());
	glm::mat4 currentMatrix = colTransform.getTransformMat4();
	//glm::mat4 currentMatrix = collider->getGlobalTransform(this->transform).getTransformMat4();

	if (squareBorderMesh != nullptr) {

		//currentMatrix = globalModelTransform * currentMatrix;
		glUniformMatrix4fv(modelMatixId, 1, GL_FALSE, glm::value_ptr(currentMatrix));
		glUniform3f(colorId, 1, 0, 0);
		glUniform1i(renderModeId, 0);
		glUniform1f(brightnessId, 1.0f);
		squareBorderMesh->render();
	}
}

template <class TargetEntityType>
void DamageCollider<TargetEntityType>::drawCollider() {
	this->render(glm::mat4());
}