#ifndef MINI_LANG_DIAG_H
#define MINI_LANG_DIAG_H
#include "common.h"
#include <stdexcept>
#include <sstream>
#include <vector>
#include <iostream>

struct Diagnostic {
    enum class Severity { Error, Warning, Info };
    Severity severity;
    Loc loc;
    std::string message;
    std::string code; // optional error code

    std::string to_string() const {
        std::ostringstream os;
        os << (severity==Severity::Error ? "error: " : (severity==Severity::Warning ? "warning: " : "info: "));
        os << loc.to_string() << ": " << message;
        if (!code.empty()) os << " [" << code << "]";
        return os.str();
    }
};

// Exception wrapper for diagnostics (fatal mode)
struct DiagnosticError : public std::runtime_error {
    Diagnostic diag;
    explicit DiagnosticError(const Diagnostic &d)
        : std::runtime_error(d.to_string()), diag(d) {}
};

// Diagnostics collector: accumulates diagnostics for reporting later.
struct Diagnostics {
    std::vector<Diagnostic> items;

    void report(const Diagnostic &d) {
        items.push_back(d);
    }
    bool has_errors() const {
        for (auto &d : items) if (d.severity == Diagnostic::Severity::Error) return true;
        return false;
    }
    void print_all(std::ostream &os = std::cerr) const {
        for (auto &d : items) os << d.to_string() << "\n";
    }
};

// Helper: when diag pointer is provided, report into it; otherwise throw.
[[noreturn]] inline void emit_fatal(const Diagnostic &d, Diagnostics *diag /*nullable*/) {
    if (diag) {
        diag->report(d);
        // after collecting a fatal error, we still throw to stop the immediate stack,
        // but callers can catch DiagnosticError and continue if desired.
        throw DiagnosticError(d);
    } else {
        throw DiagnosticError(d);
    }
}

// Helper: report or throw for non-fatal (we treat reported errors as non-fatal for continuation).
inline void emit_report(const Diagnostic &d, Diagnostics *diag /*nullable*/) {
    if (diag) diag->report(d);
    else throw DiagnosticError(d);
}

#endif // MINI_LANG_DIAG_H
