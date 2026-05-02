#pragma once
#include "contract.hpp"
#include "lexer/constexpr_lexer.hpp"
#include "lexer/token_kind.hpp"
#include <iostream>
#include <string_view>
#include <array>

template<StringLiteral Src>
struct ContractReflection {
  using Lexer = ConstexprLexer<Src>;
  static constexpr auto tokens = Lexer::tokens;
  static constexpr std::size_t token_count = Lexer::count;

  static constexpr bool is_identifier(const auto& token, const char* name) {
    return token.kind == TokenKind::Identifier &&
           std::string_view(token.start, token.length) == name;
  }

  static constexpr bool is_punct(const auto& token, const char* p) {
    return std::string_view(token.start, token.length) == p;
  }

  // Fixed state order enforcement
  static constexpr std::array<std::string_view, 4> state_names = []() {
    std::array<std::string_view, 4> names{};
    std::size_t pos = 0;
    while (pos < token_count) {
      if (is_identifier(tokens[pos], "enum")) {
        std::size_t temp = pos + 1;
        if (temp < token_count && is_identifier(tokens[temp], "class")) ++temp;
        if (temp >= token_count || !is_identifier(tokens[temp], "State")) { ++pos; continue; }
        ++temp;
        if (temp >= token_count || tokens[temp].kind != TokenKind::LBrace) { ++pos; continue; }
        pos = temp + 1;
        std::size_t idx = 0;
        while (pos < token_count) {
          if (tokens[pos].kind == TokenKind::RBrace) return names;
          if (tokens[pos].kind == TokenKind::Identifier) {
            names[idx++] = std::string_view(tokens[pos].start, tokens[pos].length);
          }
          ++pos;
        }
        return names;
      }
      ++pos;
    }
    return names;
  }();

  static_assert(state_names[0] == "Pending", "State 0 must be Pending");
  static_assert(state_names[1] == "Funded", "State 1 must be Funded");
  static_assert(state_names[2] == "Released", "State 2 must be Released");
  static_assert(state_names[3] == "Disputed", "State 3 must be Disputed");

  constexpr static int Pending = 0;
  constexpr static int Funded = 1;
  constexpr static int Released = 2;
  constexpr static int Disputed = 3;

  struct DetectedTransition {
    std::string_view func_name{};
    int from_state = -1;
    int to_state = -1;
    bool has_call = false;
  };

  struct ReflectionData {
    std::array<DetectedTransition, 8> transitions{};
    std::size_t count = 0;
    std::array<std::array<bool, 4>, 4> matrix{};
    bool reentrancy_detected = false;
  };

  static constexpr auto reflection_data = []() -> ReflectionData {
    ReflectionData data{};
    std::size_t pos = 0;
    int brace_level = 0;
    DetectedTransition current{};

    auto get_state_index = [](std::string_view sv) constexpr -> int {
      for (int i = 0; i < 4; ++i) {
        if (state_names[i] == sv) return i;
      }
      return -1;
    };

    while (pos < token_count) {
      if (tokens[pos].kind == TokenKind::LBrace) {
        ++brace_level;
        ++pos;
        continue;
      }
      if (tokens[pos].kind == TokenKind::RBrace) {
        --brace_level;
        if (brace_level == 1 && current.from_state != -1 && current.to_state != -1) {
          data.transitions[data.count] = current;
          data.matrix[current.from_state][current.to_state] = true;
          if (current.has_call) data.reentrancy_detected = true;
          ++data.count;
          current = {};
        }
        ++pos;
        continue;
      }

      // Function entry
      if (brace_level == 1 && is_identifier(tokens[pos], "void")) {
        current.has_call = false;
        ++pos;
        if (pos < token_count && tokens[pos].kind == TokenKind::Identifier) {
          current.func_name = std::string_view(tokens[pos].start, tokens[pos].length);
          ++pos;
          while (pos < token_count && tokens[pos].kind != TokenKind::LBrace) ++pos;
          continue;
        }
      }

      // Guard detection (top-level)
      if (brace_level == 2 && is_identifier(tokens[pos], "if")) {
        std::size_t check = pos + 1;
        bool matched = true;
        int potential_from = -1;
        if (check >= token_count || !is_punct(tokens[check], "(")) matched = false; else ++check;
        if (matched && (check >= token_count || !is_identifier(tokens[check], "state"))) matched = false; else ++check;
        if (matched && (check >= token_count || !is_punct(tokens[check], "!="))) matched = false; else ++check;
        if (matched && (check >= token_count || !is_identifier(tokens[check], "State"))) matched = false; else ++check;
        if (matched && (check >= token_count || !is_punct(tokens[check], "::"))) matched = false; else ++check;
        if (matched && (check >= token_count || tokens[check].kind != TokenKind::Identifier)) matched = false;
        if (matched) potential_from = get_state_index(std::string_view(tokens[check].start, tokens[check].length));
        if (matched) ++check;
        if (matched && (check >= token_count || !is_punct(tokens[check], ")"))) matched = false; else ++check;
        if (matched && (check >= token_count || !is_identifier(tokens[check], "return"))) matched = false; else ++check;
        if (matched && (check >= token_count || !is_punct(tokens[check], ";"))) matched = false; else ++check;
        if (matched && potential_from != -1) {
          current.from_state = potential_from;
          pos = check;
          continue;
        }
      }

      // State assignment (any depth)
      if (brace_level >= 2 && is_identifier(tokens[pos], "state")) {
        std::size_t check = pos + 1;
        bool matched = true;
        int potential_to = -1;
        if (check >= token_count || !is_punct(tokens[check], "=")) matched = false; else ++check;
        if (matched && (check >= token_count || !is_identifier(tokens[check], "State"))) matched = false; else ++check;
        if (matched && (check >= token_count || !is_punct(tokens[check], "::"))) matched = false; else ++check;
        if (matched && (check >= token_count || tokens[check].kind != TokenKind::Identifier)) matched = false;
        if (matched) potential_to = get_state_index(std::string_view(tokens[check].start, tokens[check].length));
        if (matched) ++check;
        if (matched && (check >= token_count || !is_punct(tokens[check], ";"))) matched = false; else ++check;
        if (matched && potential_to != -1) {
          current.to_state = potential_to;
          pos = check;
          continue;
        }
      }

      // Potential external/member call detection (skip control flow keywords)
      if (brace_level >= 2 && tokens[pos].kind == TokenKind::Identifier &&
          pos + 1 < token_count && tokens[pos + 1].kind == TokenKind::LParen) {
        if (is_identifier(tokens[pos], "if") || is_identifier(tokens[pos], "while") ||
            is_identifier(tokens[pos], "for") || is_identifier(tokens[pos], "switch")) {
          ++pos;
          continue;
        }
        current.has_call = true;
        std::size_t paren_level = 1;
        pos += 2;
        while (pos < token_count && paren_level > 0) {
          if (tokens[pos].kind == TokenKind::LParen) ++paren_level;
          else if (tokens[pos].kind == TokenKind::RParen) --paren_level;
          ++pos;
        }
        continue;
      }
      ++pos;
    }
    return data;
  }();

  static constexpr std::size_t state_count = 4;

  static void verify() {
    static_assert(state_count == 4, "Escrow must have exactly 4 states");
    static_assert(reflection_data.count == 4, "Must detect exactly 4 guarded transition functions");
    static_assert(reflection_data.matrix[Pending][Funded], "Missing Pending → Funded transition");
    static_assert(reflection_data.matrix[Funded][Released], "Missing Funded → Released transition");
    static_assert(reflection_data.matrix[Funded][Disputed], "Missing Funded → Disputed transition");
    static_assert(!reflection_data.matrix[Pending][Pending], "No self-loop on Pending");
    static_assert(!reflection_data.matrix[Funded][Funded], "No self-loop on Funded");
    static_assert(!reflection_data.matrix[Released][Released], "No self-loop on Released");
    static_assert(!reflection_data.matrix[Disputed][Disputed], "No self-loop on Disputed");
    static_assert(!reflection_data.matrix[Pending][Released], "Invalid Pending → Released");
    static_assert(!reflection_data.matrix[Pending][Disputed], "Invalid Pending → Disputed");
    static_assert(!reflection_data.matrix[Funded][Pending], "Invalid Funded → Pending");
    static_assert(!reflection_data.matrix[Released][Pending] && !reflection_data.matrix[Released][Funded] &&
                  !reflection_data.matrix[Released][Released] && !reflection_data.matrix[Released][Disputed],
                  "No outgoing transitions from Released");
    static_assert(!reflection_data.matrix[Disputed][Pending] && !reflection_data.matrix[Disputed][Funded] &&
                  !reflection_data.matrix[Disputed][Released] && !reflection_data.matrix[Disputed][Disputed],
                  "No outgoing transitions from Disputed");
    static_assert(!reflection_data.reentrancy_detected, "Reentrancy risk detected: state changed after potential call");
    std::cout << "Escrow contract reflected — states (" << state_count << "):";
    for (const auto& name : state_names) std::cout << " " << name;
    std::cout << "\n";
    std::cout << "Detected transitions:\n";
    for (std::size_t i = 0; i < reflection_data.count; ++i) {
      const auto& t = reflection_data.transitions[i];
      std::cout << "  " << t.func_name << ": " << state_names[t.from_state] << " -> " << state_names[t.to_state] << "\n";
    }
    std::cout << "Reentrancy detected: " << (reflection_data.reentrancy_detected ? "YES (FAIL)" : "NO") << "\n";
    std::cout << "Transitions verified: YES\n";
  }
};
