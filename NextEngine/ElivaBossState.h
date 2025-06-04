#pragma once
#include "ElivaBoss.h"

namespace StateTransition {
	bool blinkToSerumInject(ElivaBoss* boss);
	bool blinkToFury(ElivaBoss* boss);
	bool blinkToRifleShot(ElivaBoss* boss);
	bool blinkToBayonetSlash(ElivaBoss* boss);
	bool blinkToPoisonCloud(ElivaBoss* boss);

	bool rifleShotToBayonetSlash(ElivaBoss* boss);
	bool rifleShotToCooldown(ElivaBoss* boss);

	bool bayonetSlashToCooldown(ElivaBoss* boss);
	bool cooldownToBlink(ElivaBoss* boss);

	bool serumInjectToRifleShot(ElivaBoss* boss);
	bool serumInjectToBayonetSlash(ElivaBoss* boss);
	bool serumInjectToPoisonCloud(ElivaBoss* boss);
	bool serumInjectToFury(ElivaBoss* boss);
		
	bool poisonCloudToBayonetSlash(ElivaBoss* boss);

	bool furyToFuryBlink(ElivaBoss* boss);
	bool furyBlinkToRapidBurst(ElivaBoss* boss);
	bool rapidBurstToCloseBlink(ElivaBoss* boss);
	bool closeBlinkToBayonetSlash(ElivaBoss* boss);

	bool stunnedToCooldown(ElivaBoss* boss);
	bool stunnedToFuryCooldown(ElivaBoss* boss);

	bool introToBlink(ElivaBoss* boss);
}