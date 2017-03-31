#pragma once

namespace Cards
{
	struct Card_EX1_350 : MinionCardBase<Card_EX1_350> {
		static bool HandleEvent(state::CardRef self, state::Events::EventTypes::CalculateHealDamageAmount::Context&& context) {
			state::PlayerIdentifier owner = context.manipulate_.Board().GetCard(self).GetPlayerIdentifier();
			if (context.source_card_.GetPlayerIdentifier() != owner) return true; // for friendly only
			if (context.source_card_.GetCardType() == state::kCardTypeSpell ||
				context.source_card_.GetCardType() == state::kCardTypeHeroPower)
			{
				*context.amount_ *= 2; // both healing and damaging. Positive for damaging; nagitive for healing
			}
			return true;
		};

		Card_EX1_350() {
			RegisterEvent<MinionInPlayZone, NonCategorized_SelfInLambdaCapture, state::Events::EventTypes::CalculateHealDamageAmount>();
		}
	};
	
	/*struct Card_EX1_350 : MinionCardBase<Card_EX1_350> {
		static void PlayerFlagAuraApply(state::board::Player & player) {
			player.IncreaseProphetVelenEffects();
		}
		static void PlayerFlagAuraRemove(state::board::Player & player) {
			player.DecreaseProphetVelenEffects();
		}
		Card_EX1_350() {
			PlayerFlagAura<AliveWhenInPlay>();
		}
	};*/

	/* Example to register multiple events
	RegisterEvents<
	RegisteredEventType<InPlayZone, NonCategorized_SelfInLambdaCapture, state::Events::EventTypes::PrepareDamage>,
	RegisteredEventType<InPlayZone, NonCategorized_SelfInLambdaCapture, state::Events::EventTypes::PrepareDamage>
	>();
	*/
}

REGISTER_CARD(EX1_350)