#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <string>

namespace Oasis {
  using namespace eosio;
  using std::string;
  using std::vector;

  class [[eosio::contract]] Marketplace : public eosio::contract {	
      public:
        using contract::contract;
        Marketplace(name receiver, name code, datastream<const char*> ds) : contract(receiver, code, ds) {}
        
        struct [[eosio::table]] product {
          uint64_t product_id;
          string name;
          uint64_t power;
          uint64_t health;
          string ability;
          uint64_t level_up;
          uint64_t quantity;
          uint64_t price;

          uint64_t primary_key() const { return product_id; }
          EOSLIB_SERIALIZE(product, (product_id)(name)(power)(health)(ability)(level_up)(quantity)(price))
        };

        typedef eosio::multi_index<"products"_n, product> productIndex;

        [[eosio::action]]
        void add(name account, product newProduct);

        [[eosio::action]]
        void update(name account, uint64_t product_id, uint64_t quantity);

        [[eosio::action]]
        void remove(name account, uint64_t product_id);

        [[eosio::action]]
        void getbyid(uint64_t product_id);

        [[eosio::action]]
        void buy(name buyer, uint64_t product_id);

        
        
  };
  EOSIO_DISPATCH(Marketplace, (add)(update)(remove)(getbyid)(buy))
}