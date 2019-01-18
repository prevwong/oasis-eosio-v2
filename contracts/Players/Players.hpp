#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <string>

namespace Oasis {
  using namespace eosio;
  using std::string;
  using std::vector;
  
  class [[eosio::contract]] Players : public eosio::contract {	
    public:
      using contract::contract;
      Players(name receiver, name code, datastream<const char*> ds) : contract(receiver, code, ds) {}

      struct [[eosio::table]] item {
        uint64_t item_id;
        string name;
        uint64_t power;	
        uint64_t health;
        string ability;
        uint64_t level_up;

        uint64_t primary_key() const { return item_id; }
        EOSLIB_SERIALIZE(item, (item_id)(name)(power)(health)(ability)(level_up))
      };

      
      [[eosio::action]]
      void add(const name account, string& username);

      [[eosio::action]]
      void update(const name account, uint64_t level, int64_t healthPoints, int64_t energyPoints);

      [[eosio::action]]
      void getplayer(const name account);

      [[eosio::action]]
      void addability(const name account, string& ability);

      [[eosio::action]]
      void additem(const name account, item purchased_item);

      struct [[eosio::table]] player {
        name name;
        string username;
        uint64_t level;
        uint64_t health_points = 1000;
        uint64_t energy_points = 1000;
        vector<string> abilities; 
        vector<item> inventory; 
        uint64_t primary_key() const { return name.value; }
        EOSLIB_SERIALIZE(player, (name)(username)(level)(health_points)(energy_points)(abilities))

      };
      typedef eosio::multi_index<"players"_n, player> playerIndex;
    };
  EOSIO_DISPATCH(Players, (add)(update)(getplayer)(additem)(addability))
}
