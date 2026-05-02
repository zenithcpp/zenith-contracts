#pragma once
#include <cstdint>

struct SourceLocation {
  const char* file = nullptr;
  uint32_t line   = 0;
  uint32_t column = 0;

  constexpr SourceLocation() = default;
  constexpr SourceLocation(const char* f, uint32_t l, uint32_t c)
      : file(f), line(l), column(c) {}
};
