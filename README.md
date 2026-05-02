# Zenith Contracts

**Compile-Time Verified Smart Contracts — No Blockchain Required**

---

## What This Is

Zenith Contracts is a system for executing financial agreements as deterministic, compile-time verified state machines. Contracts are compiled into native binaries whose behavior is fully specified, auditable, and reproducible — before a single line executes.

The core insight: a well-drafted financial contract is already a finite-state machine. This project makes that structure explicit, enforces it mechanically, and compiles it into a native binary.

---

## The Problem

Financial automation is fragile. Legal agreements get translated into ad hoc software with:

- Ambiguous execution paths
- Undocumented runtime assumptions
- Failures never contemplated in the original legal text

Blockchain-based smart contracts attempt to solve this but introduce new problems: virtual machines, gas semantics, reentrancy vulnerabilities, and weak alignment with traditional legal enforcement.

---

## The Approach

Financial contracts are modeled as **deterministic finite automata (DFA)** — a foundational model from computation theory.

A DFA-based contract defines:
- A finite set of **states** (e.g. `Pending`, `Performing`, `Default`, `Terminated`)
- A finite set of **events** with legal significance (e.g. `payment_received`, `deadline_missed`)
- Explicit **transition rules** — given a state and an event, exactly one next state is produced

The compiler verifies at build time that:
- The state space is finite and explicit
- All transitions are defined and reachable
- No re-entrant or recursive paths exist
- Execution is fully deterministic

**If any condition fails, the contract does not compile.**

---

## Proof-of-Concept Output

```
$ ./build/contracts
Escrow contract reflected — states (4): Pending Funded Released Disputed
Detected transitions:
  deposit_buyer: Pending -> Funded
  deposit_seller: Pending -> Funded
  release: Funded -> Released
  dispute: Funded -> Disputed
Reentrancy detected: NO
Transitions verified: YES
Executing reflected escrow contract...
State machine running — deterministic path.
Return: 0 (success)
Native binary emitted — execution complete.
```

---

## Key Properties

| Property | Description |
|---|---|
| Determinism | Same state + same event = same result, always |
| Completeness | Every state/event combination has a defined outcome |
| State sufficiency | All relevant history encoded in current state — no hidden variables |
| Native execution | No VM, no gas, no consensus layer |
| Auditability | Execution traces are deterministic and reproducible by any third party |

---

## Why Not Blockchain

Legal contracts already rely on **legal finality**, not computational consensus. Consensus mechanisms add cost and complexity without reducing legal risk. Native execution provides higher performance, lower cost, and stronger alignment with existing legal frameworks.

---

## Use Cases

- Escrow and conditional settlement
- Loan lifecycle management (payment, default, acceleration)
- Covenant monitoring
- Delivery-versus-payment agreements
- Supply chain penalty and incentive enforcement

---

## Build

```bash
cmake -S . -B build
cmake --build build
./build/contracts
```

Requires C++17 or later.

---

## Research Foundation

This project is grounded in formal computer science and financial contract theory.

The theoretical basis for representing financial contracts as deterministic finite automata is established in:

> Flood, M. D. & Goodenough, O. R. (2017). *Contract as Automaton: The Computational Representation of Financial Agreements.* Office of Financial Research Working Paper 15-04.

Full documentation and white papers are included in the [`zenith_contracts/`](./zenith_contracts/) directory.

---

## Status

Active proof-of-concept. Escrow contract lifecycle implemented and verified. Loan contract (multi-payment, default, acceleration) in progress.

---

## License

APACHE

BSL


## Provenance

This project has been cryptographically timestamped via [OpenTimestamps](https://opentimestamps.org) and anchored to the Bitcoin blockchain. The timestamp proves this work existed in its current form prior to any future claims. 
The `.ots` proof file is included in this repository.
