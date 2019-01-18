#include "Marketplace.hpp"
#include <eosiolib/asset.hpp>

namespace Oasis {
  void Marketplace::buy(name buyer, const uint64_t product_id) {
    productIndex products(_code, _code.value);

    auto iterator = products.find(product_id);
    eosio_assert(iterator != products.end(), "The product was not found");

    auto product = products.get(product_id);
    eosio_assert(product.quantity > 0, "The product is out of stock");

    asset productPrice = asset(product.price, symbol("OAS", 4));

    action(permission_level{get_self(),"active"_n},  "anorak"_n, "transfer"_n, make_tuple(buyer, _self, productPrice, string(""))).send();
    action(permission_level{get_self(),"active"_n}, "market"_n, "additem"_n, make_tuple(buyer,
      product.product_id,
      product.name,
      product.power,
      product.health,
      product.ability,
      product.level_up
    )).send();

    update(buyer, product.product_id, -1);
  }

  void Marketplace::add(name account, const product newProduct) {
    require_auth(account);
    productIndex products(_code, _code.value);

    auto iterator = products.find(newProduct.product_id);
    eosio_assert(iterator == products.end(), "Product for this ID already exists!");

    products.emplace(account, [&](auto& product) {
      product.product_id = newProduct.product_id;
      product.name = newProduct.name;
      product.power = newProduct.power;
      product.health = newProduct.health;
      product.ability = newProduct.ability;
      product.level_up = newProduct.level_up;
      product.quantity = newProduct.quantity;
      product.price = newProduct.price;
    });
  }

  void Marketplace::update(name account, const uint64_t product_id, uint64_t quantity) {
    require_auth(account);
    productIndex products(_code, _code.value);
    auto iterator = products.find(product_id);
    eosio_assert(iterator != products.end(), "Product not found");

    products.modify(iterator, account,  [&](auto& product) {
      product.quantity += quantity;
    });
  }

  void Marketplace::remove(name account, const uint64_t product_id) {
    require_auth(account);

    productIndex products(_code, _code.value);
    auto iterator = products.find(product_id);
    eosio_assert(iterator != products.end(), "Product not found");

    products.erase(iterator);
  }

  void Marketplace::getbyid(const uint64_t product_id){
    productIndex products(_code, _code.value);
    auto iterator = products.find(product_id);
    eosio_assert(iterator != products.end(), "Product ID does not exist");
    auto product = products.get(product_id);
    print("Id: ", product.product_id);
    print(" | Name: ", product.name.c_str());
    print(" | Power: ", product.power);
    print(" | Health: ", product.health);
    print(" | Ability: ", product.ability.c_str());
    print(" | Level up: ", product.level_up);
    print(" | Quantity: ", product.quantity); 
    print(" | Price: ", product.price);
  }
}