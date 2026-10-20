//! Diagnostics with source spans, severity and additional information trait.

use std::rc::Rc;

pub enum Severity {
    Help,
    Note,
    Hint,
    Warning,
    Error, // Cancels compilation
}

// Byte-based range; guaranteed to align with UTF-8 code point boundaries.
pub struct Span {
    pub begin: u32,
    pub length: u32,
}

pub trait Info {
    fn print_info(&self);
}

pub struct Diagnostic {
    pub span: Span,
    pub message: Rc<str>,
    pub severity: Severity,
    pub info: Option<Rc<dyn Info>>,
}

pub struct PrintConfig {
    pub include_source_code: bool,
    pub colorized: bool, // Active only in interactive terminal; uses ANSI escape codes
    pub syntax_highlight: bool, // Active only if lexer/parser succeeds and colorized output is true
    pub underline_squiggle: bool,
    pub line_number: bool,
}

impl Default for PrintConfig {
    fn default() -> Self {
        Self {
            include_source_code: true,
            colorized: true,
            syntax_highlight: true,
            underline_squiggle: true,
            line_number: true,
        }
    }
}

pub fn print_diagnostic(diagnostic: Diagnostic, print_config: PrintConfig) {}
