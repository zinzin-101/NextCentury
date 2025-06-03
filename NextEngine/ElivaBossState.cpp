#include "ElivaBossState.h"

namespace StateTransition {
	bool blinkToSerumInject(ElivaBoss* boss) {
		const Animation::State& currentAnim = boss->getAnimationComponent()->getCurrentAnimationStateRef();

		if (currentAnim.isPlaying) {
			return false;
		}

		if (boss->getHealth() <= ElivaStat::HEALTH_TO_BEGIN_SERUM_INJECT && !boss->isShieldActivated()) {
			return !boss->hasSerumBeenInjected();
		}

		return false;
	}

	bool blinkToFury(ElivaBoss* boss) {
		if (boss->hasFuryBeenActivated()) {
			return false;
		}

		if (boss->getHealth() > ElivaStat::HEALTH_TO_BEGIN_FURY) {
			return false;
		}

		const Animation::State& currentAnim = boss->getAnimationComponent()->getCurrentAnimationStateRef();

		if (currentAnim.isPlaying) {
			return false;
		}

		return true;
	}

	bool blinkToRifleShot(ElivaBoss* boss) {
		const Animation::State& currentAnim = boss->getAnimationComponent()->getCurrentAnimationStateRef();

		if (currentAnim.isPlaying) {
			return false;
		}

		float distance = boss->getDistanceFromPlayer();
		if (distance <= ElivaStat::BAYONET_SLASH_RANGE) {
			return false;
		}

		return true;
	}

	bool blinkToBayonetSlash(ElivaBoss* boss) {
		const Animation::State& currentAnim = boss->getAnimationComponent()->getCurrentAnimationStateRef();

		if (currentAnim.isPlaying) {
			return false;
		}

		float distance = boss->getDistanceFromPlayer();
		if (distance > ElivaStat::BAYONET_SLASH_RANGE) {
			return false;
		}

		return true;
	}

	bool blinkToPoisonCloud(ElivaBoss* boss) {
		if (boss->getCurrentPhase() == ElivaBoss::Phase::First) {
			return false;
		}

		const Animation::State& currentAnim = boss->getAnimationComponent()->getCurrentAnimationStateRef();

		if (currentAnim.isPlaying) {
			return false;
		}

		float distance = boss->getDistanceFromPlayer();
		return boss->getCanUsePoisonCloud() && distance <= ElivaStat::BAYONET_SLASH_RANGE;
	}

	bool rifleShotToBayonetSlash(ElivaBoss* boss) {
		const Animation::State& currentAnim = boss->getAnimationComponent()->getCurrentAnimationStateRef();

		if (currentAnim.isPlaying) {
			return false;
		}

		float distance = boss->getDistanceFromPlayer();

		if (distance <= ElivaStat::BAYONET_SLASH_RANGE) {
			return true;
		}

		return false;
	}

	bool rifleShotToCooldown(ElivaBoss* boss) {
		const Animation::State& currentAnim = boss->getAnimationComponent()->getCurrentAnimationStateRef();

		if (currentAnim.isPlaying) {
			return false;
		}

		return true;
	}

	bool bayonetSlashToCooldown(ElivaBoss* boss) {
		const Animation::State& currentAnim = boss->getAnimationComponent()->getCurrentAnimationStateRef();

		if (currentAnim.isPlaying) {
			return false;
		}

		return true;
	}
	
	bool cooldownToBlink(ElivaBoss* boss) {
		if (boss->getCoolDownTimer() > 0.0f) {
			return false;
		}

		return true;
	}

	bool serumInjectToRifleShot(ElivaBoss* boss) {
		const Animation::State& currentAnim = boss->getAnimationComponent()->getCurrentAnimationStateRef();

		if (currentAnim.isPlaying) {
			return false;
		}

		return true;
	}

	bool serumInjectToBayonetSlash(ElivaBoss* boss) {
		const Animation::State& currentAnim = boss->getAnimationComponent()->getCurrentAnimationStateRef();

		if (currentAnim.isPlaying) {
			return false;
		}

		return true;
	}

	bool serumInjectToPoisonCloud(ElivaBoss* boss) {
		const Animation::State& currentAnim = boss->getAnimationComponent()->getCurrentAnimationStateRef();

		if (currentAnim.isPlaying) {
			return false;
		}

		return boss->getCanUsePoisonCloud();
	}

	bool serumInjectToFury(ElivaBoss* boss) {
		const Animation::State& currentAnim = boss->getAnimationComponent()->getCurrentAnimationStateRef();

		if (currentAnim.isPlaying) {
			return false;
		}

		if (boss->getHealth() <= ElivaStat::HEALTH_TO_BEGIN_FURY && !boss->hasFuryBeenActivated()) {
			return true;
		}

		return false;
	}

	bool poisonCloudToBayonetSlash(ElivaBoss* boss) {
		const Animation::State& currentAnim = boss->getAnimationComponent()->getCurrentAnimationStateRef();

		if (currentAnim.isPlaying) {
			return false;
		}

		float distance = boss->getDistanceFromPlayer();

		if (distance <= ElivaStat::BAYONET_SLASH_RANGE) {
			return true;
		}

		return false;
	}

	bool furyToFuryBlink(ElivaBoss* boss) {
		const Animation::State& currentAnim = boss->getAnimationComponent()->getCurrentAnimationStateRef();

		if (currentAnim.isPlaying) {
			return false;
		}

		return true;
	}

	bool furyBlinkToRapidBurst(ElivaBoss* boss) {
		const Animation::State& currentAnim = boss->getAnimationComponent()->getCurrentAnimationStateRef();

		if (currentAnim.isPlaying) {
			return false;
		}

		return true;
	}

	bool rapidBurstToCloseBlink(ElivaBoss* boss) {
		const Animation::State& currentAnim = boss->getAnimationComponent()->getCurrentAnimationStateRef();

		if (currentAnim.isPlaying) {
			return false;
		}

		return true;
	}

	bool closeBlinkToBayonetSlash(ElivaBoss* boss) {
		const Animation::State& currentAnim = boss->getAnimationComponent()->getCurrentAnimationStateRef();

		if (currentAnim.isPlaying) {
			return false;
		}

		return true;
	}

	bool stunnedToCooldown(ElivaBoss* boss) {
		float stunnedTimer = boss->getStunnedTimer();

		if (stunnedTimer > 0.0f) {
			return false;
		}

		return boss->getCurrentPhase() != ElivaBoss::Phase::Third;
	}

	bool stunnedToFuryCooldown(ElivaBoss* boss) {
		float stunnedTimer = boss->getStunnedTimer();

		if (stunnedTimer > 0.0f) {
			return false;
		}

		return boss->getCurrentPhase() == ElivaBoss::Phase::Third;
	}
}