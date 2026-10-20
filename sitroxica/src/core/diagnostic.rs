pub struct Span {
    pub index: usize,
    pub length: usize,
}

pub enum Severity {
    Warning,
    Error,
    Note,
    Help,
    Fix,
    Suggest,
}

pub struct Report {
    pub severity: Severity,
    pub span: Span,
    pub message: Box<str>,
}
