#pragma once
#include <cstdint>

enum class TokenKind : uint16_t {
  // Punctuators
  LParen, RParen, LBrace, RBrace, LBracket, RBracket,
  Semicolon, Colon, Comma, Dot, Arrow, DotStar, ArrowStar,
  Plus, Minus, Star, Slash, Percent, Caret, Amp, Pipe, Tilde,
  PlusPlus, MinusMinus, EqualEqual, BangEqual, Less, Greater,
  LessEqual, GreaterEqual, Spaceship, AmpAmp, PipePipe, Equal,
  PlusEqual, MinusEqual, StarEqual, SlashEqual, PercentEqual,
  CaretEqual, AmpEqual, PipeEqual, LessLess, GreaterGreater,
  LessLessEqual, GreaterGreaterEqual, Question, DoubleColon,
  Hash, HashHash,
  Bang,                     // <-- ADDED HERE
  Exclaim = Bang,           // alias for compatibility

  // Keywords (C++23)
  Auto, Bool, Char, Char8, Char16, Char32, WChar, Class, Const,
  Constexpr, Constinit, Co_await, Co_return, Co_yield, Concept,
  Delete, Double, Enum, Explicit, Export, Extern, Float, Friend,
  Inline, Int, Long, Mutable, Namespace, New, Noexcept, Nullptr,
  Operator, Private, Protected, Public, Register, Requires,
  Short, Signed, Sizeof, Static, Static_assert, Struct, Template,
  This, Thread_local, Throw, Typedef, Typeid, Typename, Union,
  Unsigned, Using, Virtual, Void, Volatile, Alignas, Alignof,
  Asm, Break, Case, Catch, Continue, Default, Do, Else, False,
  For, Goto, If, Import, Module, Return, Switch, True, Try,
  While,
  Char8_t, Consteval,

  // Literals
  Identifier,
  IntegerLiteral,
  FloatLiteral,
  StringLiteral,
  CharLiteral,
  UserDefinedLiteral,

  // Special
  EndOfFile,
  Error
};
