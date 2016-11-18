#pragma once

#include <memory>
#include <utility>
#include "Entity/Card.h"
#include "Entity/EnchantmentAuxData.h"

namespace Manipulators
{
	namespace Helpers
	{
		class EnchantmentHelper
		{
		public:
			EnchantmentHelper(Entity::Card &card) :
				data_(card.GetMutableEnchantmentAuxDataGetter().Get())
			{
			}

			template <typename T>
			decltype(auto) Add(T&& enchantment)
			{
				data_.need_update = true;
				return data_.enchantments.PushBack(std::forward<T>(enchantment));
			}

			template <typename EnchantmentType, typename T>
			decltype(auto) Remove(T&& id)
			{
				data_.need_update = true;
				return data_.enchantments.Remove<EnchantmentType>(std::forward<T>(id));
			}

		private:
			Entity::EnchantmentAuxData & data_;
		};
	}
}