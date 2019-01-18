#include "Players.hpp"

namespace Oasis {
		void Players::add(const name account, string& username) {
			require_auth(account);
			playerIndex players(_code, _code.value);	
			auto iterator = players.find(account.value);
			eosio_assert(iterator == players.end(), "Address for account already exists");
			players.emplace(account, [&](auto& player) {
				player.name = account;
				player.username = username;
				player.level = 1;
				player.health_points = 1000;
				player.energy_points = 1000;
			});
		}
		void Players::update(const name account, uint64_t level, int64_t healthPoints, int64_t energyPoints) {
			require_auth(account);
			playerIndex players(_code, _code.value);
			auto iterator = players.find(account.value);
			eosio_assert(iterator != players.end(), "Address for account not found");

			players.modify(iterator, account, [&](auto& player){
				player.level = level;
				if ( (player.health_points - healthPoints) < 0 ) {
					player.health_points = 0;
				} else {
					player.health_points -= healthPoints;
				}
				if ( (player.energy_points - energyPoints) < 0 ) {
					player.energy_points = 0;
				} else {
					player.energy_points -= energyPoints;
				}
			});
		}

		void Players::getplayer(const name account) {
			playerIndex players(_code, _code.value);
			auto iterator = players.find(account.value);
			eosio_assert(iterator != players.end(), "Address for account not found");
			auto currentPlayer = players.get(account.value);
			print("Username: ", currentPlayer.username.c_str());
			print(" Level: ", currentPlayer.level);
			print(" Health: ", currentPlayer.health_points);
			print(" Energy: ", currentPlayer.energy_points);
							
			if (currentPlayer.abilities.size() > 0) {
					print(" Abilities: ");

					for (uint32_t i = 0; i < currentPlayer.abilities.size(); i++) {
							print(currentPlayer.abilities.at(i).c_str(), " ");
					}
			} else {
					print(" No Abilities");
			}
		}

		void Players::addability(const name account, string& ability) {
			require_auth(account);

			playerIndex players(_code, _code.value);

			auto iterator = players.find(account.value);
			eosio_assert(iterator != players.end(), "Address for account not found");

			players.modify(iterator, account, [&](auto& player) {
				player.abilities.push_back(ability);
			});
		}	
	
	void Players::additem(const name account, item purchased_item) {
		playerIndex players(_code, _code.value);
		auto iterator = players.find(account.value);
		eosio_assert(iterator != players.end(), "Address for account not found");
		players.modify(iterator, account, [&](auto& player) {
			player.energy_points += purchased_item.power;
			player.health_points += purchased_item.health;
			player.level += purchased_item.level_up;
			player.abilities.push_back(purchased_item.ability);
			player.inventory.push_back(item{
				purchased_item.item_id,
				purchased_item.name,
				purchased_item.power,
				purchased_item.health,
				purchased_item.ability,
				purchased_item.level_up	
			});
		});
		print("Item Id: ", purchased_item.item_id);
    print(" | Name: ", purchased_item.name.c_str());
    print(" | Power: ", purchased_item.power); 
    print(" | Health: ", purchased_item.health);
    print(" | Ability: ", purchased_item.ability.c_str());
    print(" | Level up: ", purchased_item.level_up);
	}
}
