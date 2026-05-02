#pragma once
#include "string_literal.hpp"

struct Escrow {
  enum class State { Pending, Funded, Released, Disputed };

  State state = State::Pending;

  void deposit_buyer() {
    if (state != State::Pending) return;
    state = State::Funded;
  }

  void deposit_seller() {
    if (state != State::Pending) return;
    state = State::Funded;
  }

  void release() {
    if (state != State::Funded) return;
    state = State::Released;
  }

  void dispute() {
    if (state != State::Funded) return;
    state = State::Disputed;
  }
};

constexpr auto escrow_contract = StringLiteral(R"(
struct Escrow {
  enum class State { Pending, Funded, Released, Disputed };

  State state = State::Pending;

  void deposit_buyer() {
    if (state != State::Pending) return;
    state = State::Funded;
  }

  void deposit_seller() {
    if (state != State::Pending) return;
    state = State::Funded;
  }

  void release() {
    if (state != State::Funded) return;
    state = State::Released;
  }

  void dispute() {
    if (state != State::Funded) return;
    state = State::Disputed;
  }
};
)");
