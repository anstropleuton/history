use std::rc::Rc;

use crate::{
    diagnostic::{Span, preview_span},
    source_code::SourceCode,
    util::get_line_row_col,
};

#[derive(Debug, Clone, Copy)]
pub enum Severity {
    Note,
    Warning,
    Error,
    Help,
}

#[derive(Debug)]
pub struct Report {
    pub message: Rc<str>,
    pub span: Span,
    pub severity: Severity,
}

impl Report {
    pub fn pretty_print(&self, source: &Rc<SourceCode>) {
        let name = &source.name;

        let (row, column) = get_line_row_col(&source.lines, self.span.begin);

        let severity = match self.severity {
            Severity::Note => "note",
            Severity::Warning => "warning",
            Severity::Error => "error",
            Severity::Help => "help",
        };

        let message = &self.message;

        println!("{name}:{row}:{column} {severity}: {message}");

        println!(
            "{}",
            preview_span(&source.code_str, &source.lines, self.span, 0)
        );
    }
}
