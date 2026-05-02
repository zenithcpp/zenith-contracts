#pragma once
#include "token_kind.hpp"
#include "source_location.hpp"

struct Token {
  TokenKind kind = TokenKind::EndOfFile;
  SourceLocation loc{};
  const char* start = nullptr;
  uint32_t length = 0;

  constexpr Token() = default;
  constexpr Token(TokenKind k, SourceLocation l, const char* s, uint32_t len)
      : kind(k), loc(l), start(s), length(len) {}
};
