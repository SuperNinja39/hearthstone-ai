#pragma once

#include <unordered_map>
#include "State/CardRef.h"
#include "Enchantments/TieredEnchantments.h"

namespace Entity
{
	class AuraAuxData
	{
	public:
		AuraAuxData() : last_update_hint_id(0) {}

		std::unordered_map<State::CardRef, TieredEnchantments::ContainerType::Identifier> applied_enchantments;
		int last_update_hint_id;
	};
}