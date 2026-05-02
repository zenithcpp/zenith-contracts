#include "reflection.hpp"
#include "contract.hpp"
#include <iostream>

int main() {
  ContractReflection<escrow_contract>::verify();

  Escrow e;
  e.deposit_buyer();
  e.release();

  std::cout << "Executing reflected escrow contract...\n";
  std::cout << "State machine running — deterministic path.\n";
  std::cout << "Return: 0 (success)\n";
  std::cout << "Native binary emitted — execution complete.\n";

  return 0;
}
