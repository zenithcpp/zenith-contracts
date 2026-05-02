#pragma once

struct LetterOfCredit {
  enum class State {
    Applied,      // Applicant requests LC
    Issued,       // Issuing Bank issues
    Advised,      // Advising Bank notifies Beneficiary
    Presented,    // Beneficiary presents documents
    Compliant,    // Documents checked OK
    Accepted,     // Issuing Bank accepts
    Paid,         // Payment to Beneficiary
    Rejected,     // Documents non-compliant
    Expired,      // LC expiry reached
    Disputed      // Dispute raised
  };

  State state = State::Applied;

  // Simulated data (in real use: amounts, dates, parties, etc.)
  int applicant_funds = 0;
  int beneficiary_entitled = 0;
  bool documents_compliant = false;
  bool expired = false;

  // Applicant requests LC (initial)
  void apply(int amount) {
    if (state != State::Applied) return;
    applicant_funds = amount;
    state = State::Issued;
  }

  // Issuing Bank issues LC
  void issue() {
    if (state != State::Issued) return;
    state = State::Advised;
  }

  // Advising Bank advises Beneficiary
  void advise() {
    if (state != State::Advised) return;
    // Ready for presentation
    state = State::Presented;  // transition on shipment assumed
  }

  // Beneficiary presents documents
  void present_documents(bool compliant) {
    if (state != State::Presented) return;
    documents_compliant = compliant;
    if (compliant) {
      state = State::Compliant;
    } else {
      state = State::Rejected;
    }
  }

  // Issuing Bank checks and accepts
  void accept() {
    if (state != State::Compliant) return;
    beneficiary_entitled = applicant_funds;
    state = State::Accepted;
  }

  // Payment released
  void pay() {
    if (state != State::Accepted) return;
    state = State::Paid;
  }

  // Reject non-compliant documents
  void reject() {
    if (state != State::Rejected) return;
    state = State::Rejected;  // terminal
  }

  // Expiry handling
  void check_expiry(bool is_expired) {
    if (state == State::Issued || state == State::Advised || state == State::Presented) {
      if (is_expired) {
        expired = true;
        state = State::Expired;
      }
    }
  }

  // Dispute (from Compliant, Accepted, Paid, Rejected)
  void dispute() {
    if (state != State::Compliant && state != State::Accepted &&
        state != State::Paid && state != State::Rejected) return;
    state = State::Disputed;
  }
};
