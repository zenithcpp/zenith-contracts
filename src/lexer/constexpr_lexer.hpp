#pragma once
#include "token.hpp"
#include "string_literal.hpp"
#include <array>
namespace detail {
  constexpr bool is_space(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }
  constexpr bool is_alpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
  constexpr bool is_digit(char c) { return c >= '0' && c <= '9'; }
  constexpr bool is_ident_start(char c) { return is_alpha(c) || c == '_'; }
  constexpr bool is_ident_continue(char c) { return is_alpha(c) || is_digit(c) || c == '_'; }
  constexpr bool str_eq(const char* a, const char* b, std::size_t n) {
    for (std::size_t i = 0; i < n; ++i) if (a[i] != b[i]) return false;
    return true;
  }
}
template<StringLiteral Src>
struct ConstexprLexer {
  static constexpr auto lex() {
    std::array<Token, 512> tokens{};
    std::size_t n = 0;
    const char* p = Src.data;
    const char* end = p + Src.size();
    SourceLocation loc{"<constexpr>", 1, 1};
    while (p < end && n < tokens.size() - 1) {
      while (p < end && detail::is_space(*p)) {
        if (*p == '\n') { ++loc.line; loc.column = 1; } else ++loc.column;
        ++p;
      }
      if (p >= end) break;
      const char* start = p;
      SourceLocation start_loc = loc;
      if (detail::is_ident_start(*p)) {
        while (p < end && detail::is_ident_continue(*p)) ++p, ++loc.column;
        std::size_t len = p - start;
        TokenKind k = TokenKind::Identifier;
        if (len == 3 && detail::str_eq(start, "int", 3)) k = TokenKind::Int;
        if (len == 9 && detail::str_eq(start, "constexpr", 9)) k = TokenKind::Constexpr;
        if (len == 4 && detail::str_eq(start, "main", 4)) k = TokenKind::Identifier;
        if (len == 4 && detail::str_eq(start, "bool", 4)) k = TokenKind::Bool;
        if (len == 4 && detail::str_eq(start, "true", 4)) k = TokenKind::True;
        if (len == 5 && detail::str_eq(start, "false", 5)) k = TokenKind::False;
        tokens[n++] = Token(k, start_loc, start, len);
      }
      else if (detail::is_digit(*p)) {
        while (p < end && detail::is_digit(*p)) ++p, ++loc.column;
        tokens[n++] = Token(TokenKind::IntegerLiteral, start_loc, start, p - start);
      }
      else {
        char c = *p++;
        ++loc.column;
        TokenKind k = TokenKind::Error;
        if (c == '(') k = TokenKind::LParen;
        else if (c == ')') k = TokenKind::RParen;
        else if (c == '{') k = TokenKind::LBrace;
        else if (c == '}') k = TokenKind::RBrace;
        else if (c == ';') k = TokenKind::Semicolon;
        else if (c == '=') k = TokenKind::Equal;
        else if (c == '+') k = TokenKind::Plus;
        else if (c == '-') k = TokenKind::Minus;
        else if (c == '*') k = TokenKind::Star;
        tokens[n++] = Token(k, start_loc, start, 1);
      }
    }
    tokens[n++] = Token(TokenKind::EndOfFile, loc, end, 0);
    return std::pair{tokens, n};
  }
  static constexpr auto result = lex();
  static constexpr auto tokens = result.first;
  static constexpr std::size_t count = result.second;
};
