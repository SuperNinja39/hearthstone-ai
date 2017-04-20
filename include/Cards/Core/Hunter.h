#pragma once

// http://www.hearthpwn.com/cards?filter-premium=1&filter-set=2&filter-class=8&sort=-cost&display=1
// Last-finished card: Hunter's Mark

namespace Cards
{
	struct Card_DS1_185 : public SpellCardBase<Card_DS1_185> {
		Card_DS1_185() {
			onplay_handler.SetSpecifyTargetCallback([](Contexts::SpecifiedTargetGetter const& context) {
				*context.allow_no_target_ = false;
				*context.targets_ = TargetsGenerator(context.player_).SpellTargetable().GetInfo();
				return true;
			});
			onplay_handler.SetOnPlayCallback([](FlowControl::onplay::context::OnPlay const& context) {
				int spell_damage = context.manipulate_.Board().GetSpellDamage(context.player_);
				state::CardRef target = context.GetTarget();
				if (!target.IsValid()) return;
				context.manipulate_.OnBoardCharacter(target).Damage(context.card_ref_, 2 + spell_damage);
			});
		}
	};

	struct Card_CS2_084e : public Enchantment<Card_CS2_084e, SetMaxHP<1>> {};
	struct Card_CS2_084 : public SpellCardBase<Card_CS2_084> {
		Card_CS2_084() {
			onplay_handler.SetSpecifyTargetCallback([](Contexts::SpecifiedTargetGetter const& context) {
				*context.allow_no_target_ = false;
				*context.targets_ = TargetsGenerator(context.player_).Minion().SpellTargetable().GetInfo();
				return true;
			});
			onplay_handler.SetOnPlayCallback([](FlowControl::onplay::context::OnPlay const& context) {
				state::CardRef target = context.GetTarget();
				if (!target.IsValid()) return;
				context.manipulate_.OnBoardMinion(target).Enchant().Add<Card_CS2_084e>();
			});
		}
	};

	struct Card_DS1_184 : public SpellCardBase<Card_DS1_184> {
		Card_DS1_184() {
			onplay_handler.SetOnPlayCallback([](FlowControl::onplay::context::OnPlay const& context) {
				auto & deck = context.manipulate_.Board().Player(context.player_).deck_;
				std::vector<Cards::CardId> cards;
				for (int i = 0; i < 3; ++i) {
					if (deck.Empty()) break;
					cards.push_back((Cards::CardId)deck.GetLast());
					deck.RemoveLast();
				}
				if (cards.empty()) return;
				size_t choice = context.manipulate_.GetChooseOneUserAction(cards);
				context.manipulate_.Hero(context.player_).AddHandCard(cards[choice]);
			});
		}
	};

	struct Card_DS1_175o : public Enchantment<Card_DS1_175o, Attack<1>> {};
	struct Card_DS1_175 : public MinionCardBase<Card_DS1_175> {
		static auto GetAuraTargets(FlowControl::aura::contexts::AuraGetTargets context) {
			state::PlayerIdentifier player = context.manipulate_.GetCard(context.card_ref_).GetPlayerIdentifier();
			TargetsGenerator(player)
				.Ally(context).Minion().Beasts() // friendly beasts
				.Exclude(context.card_ref_) // only apply on other
				.GetInfo().Fill(context.manipulate_, context.new_targets);
		}
		Card_DS1_175() {
			Aura<Card_DS1_175o, EmitWhenAlive, FlowControl::aura::kUpdateWhenMinionChanges>();
		}
	};

	struct Card_NEW1_032 : public MinionCardBase<Card_NEW1_032, Taunt> {};

	struct Card_NEW1_033o : public Enchantment<Card_NEW1_033o, Attack<1>> {};
	struct Card_NEW1_033 : public MinionCardBase<Card_NEW1_033> {
		static auto GetAuraTargets(FlowControl::aura::contexts::AuraGetTargets context) {
			state::PlayerIdentifier player = context.manipulate_.GetCard(context.card_ref_).GetPlayerIdentifier();
			TargetsGenerator(player)
				.Ally(context).Minion() // friendly minions
				.Exclude(context.card_ref_) // only apply on other
				.GetInfo().Fill(context.manipulate_, context.new_targets);
		}
		Card_NEW1_033() {
			Aura<Card_NEW1_033o, EmitWhenAlive, FlowControl::aura::kUpdateWhenMinionChanges>();
		}
	};

	struct Card_NEW1_034 : public MinionCardBase<Card_NEW1_034, Charge> {};

	struct Card_NEW1_031 : public SpellCardBase<Card_NEW1_031> {
		Card_NEW1_031() {
			onplay_handler.SetSpecifyTargetCallback([](Contexts::SpecifiedTargetGetter const& context) {
				if (context.manipulate_.Board().Player(context.player_).minions_.Full()) {
					return false;
				}
				return true;
			});
			onplay_handler.SetOnPlayCallback([](FlowControl::onplay::context::OnPlay const& context) {
				int pos = (int)context.manipulate_.Board().Player(context.player_).minions_.Size();

				std::array<Cards::CardId, 3> cards{
					Cards::ID_NEW1_032,
					Cards::ID_NEW1_033,
					Cards::ID_NEW1_034
				};
				int rand = context.manipulate_.GetRandom().Get(3);
				Cards::CardId card_id = cards[rand];

				context.manipulate_.Board().SummonMinionById(card_id, context.player_, pos);
			});
		}
	};
}

REGISTER_CARD(NEW1_031)
REGISTER_CARD(NEW1_032)
REGISTER_CARD(NEW1_033)
REGISTER_CARD(NEW1_034)
REGISTER_CARD(DS1_175)
REGISTER_CARD(DS1_184)
REGISTER_CARD(CS2_084)
REGISTER_CARD(DS1_185)