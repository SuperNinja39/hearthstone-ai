#pragma once

#include <assert.h>
#include "FlowControl/Manipulate.h"
#include "FlowControl/Manipulators/Helpers/EnchantmentHelper.h"
#include "Cards/Database.h"

namespace FlowControl
{
	namespace Manipulators
	{
		namespace Helpers
		{
			inline void EnchantmentHelper::Update()
			{
				state::Cards::EnchantableStates const& current_states = card_.GetRawData().enchantable_states;
				state::Cards::EnchantmentAuxData & data = card_.GetMutableEnchantmentAuxDataGetter().Get();

				if (!data.need_update) return;

				state::Cards::EnchantableStates new_states = data.origin_states;
				data.enchantments.ApplyAll(new_states);

				auto card_manipulator = Manipulate(state_, flow_context_).Card(card_ref_);

				static_assert(state::Cards::EnchantableStates::kFieldChangeId == 3, "enchantable fields changed");

				// update states field by field
				if (new_states.player != current_states.player) {
					switch (card_.GetCardType()) {
					case state::kCardTypeMinion:
						ChangeMinionPlayer(new_states.player);
						break;
					default:
						throw std::exception("unsupported enchantment type");
					}
				}

				if (new_states.cost != current_states.cost) {
					card_manipulator.Cost(new_states.cost);
					assert(card_.GetCost() == new_states.cost);
				}
				if (new_states.attack != current_states.attack) {
					card_manipulator.Attack(new_states.attack);
					assert(card_.GetAttack() == new_states.attack);
				}
				if (new_states.max_hp != current_states.max_hp) {
					card_manipulator.MaxHP(new_states.max_hp);
					assert(card_.GetMaxHP() == new_states.max_hp);
				}

				if (new_states.taunt != current_states.taunt) {
					card_manipulator.Taunt(new_states.taunt);
					assert(card_.GetRawData().enchantable_states.taunt == new_states.taunt);
				}

				if (new_states.shielded != current_states.shielded) {
					card_manipulator.Shield(new_states.shielded);
					assert(card_.GetRawData().enchantable_states.shielded == new_states.shielded);
				}

				data.need_update = false;
			}

			inline void EnchantmentHelper::ChangeMinionPlayer(state::PlayerIdentifier player)
			{
				assert(card_.GetZone() == state::kCardZonePlay);

				if (state_.board.Get(player).minions_.Full()) {
					Manipulate(state_, flow_context_).Minion(card_ref_).Zone().ChangeTo<state::kCardZoneGraveyard>(player);
					assert(card_.GetZone() == state::kCardZoneGraveyard);
					assert(card_.GetPlayerIdentifier() == player);
				}
				else {
					int location = (int)state_.board.Get(player).minions_.Size();

					Manipulate(state_, flow_context_).Minion(card_ref_).Zone().ChangeTo<state::kCardZonePlay>(player, location);
					assert(card_.GetZone() == state::kCardZonePlay);
					assert(card_.GetPlayerIdentifier() == player);
				}
			}
		}
	}
}