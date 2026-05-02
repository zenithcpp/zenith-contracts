#pragma once

template <std::size_t N>
struct StringLiteral {
  char data[N]{};

  constexpr StringLiteral(const char (&str)[N]) {
    for (std::size_t i = 0; i < N; ++i) data[i] = str[i];
  }

  constexpr const char* c_str() const { return data; }
  constexpr std::size_t size() const { return N - 1; }
};
