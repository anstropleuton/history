use std::rc::Rc;

use crate::{diagnostic::Span, keyword::Keyword};

#[derive(Debug)]
pub enum TokenValue {
    Identifier { value: Rc<str> },
    Keyword { value: Keyword },
    IntLit { value: i32 },
    FloatLit { value: f64 },
    Delimiter { value: char },
    Operator { value: Rc<str> },
}

impl TokenValue {
    pub fn value_as_str(&self) -> Rc<str> {
        match &self {
            TokenValue::Identifier { value } => format!("Identifier '{}'", value).into(),
            TokenValue::Keyword { value } => format!("Keyword '{}'", value.as_str()).into(),
            TokenValue::IntLit { value } => format!("Integer Literal '{}'", value).into(),
            TokenValue::FloatLit { value } => format!("Floating-point Literal '{}'", value).into(),
            TokenValue::Delimiter { value } => format!("Delimiter '{}'", value).into(),
            TokenValue::Operator { value } => format!("Operator '{}'", value).into(),
        }
    }

    pub fn kind_as_str(&self) -> Rc<str> {
        match &self {
            TokenValue::Identifier { .. } => format!("Identifier").into(),
            TokenValue::Keyword { .. } => format!("Keyword").into(),
            TokenValue::IntLit { .. } => format!("Integer Literal").into(),
            TokenValue::FloatLit { .. } => format!("Floating-point Literal").into(),
            TokenValue::Delimiter { .. } => format!("Delimiter").into(),
            TokenValue::Operator { .. } => format!("Operator").into(),
        }
    }
}

#[derive(Debug)]
pub enum TokenKind {
    Identifier,
    Keyword,
    IntLit,
    FloatLit,
    Delimiter,
    Operator,
}

impl TokenKind {
    pub fn from_value(value: TokenValue) -> Self {
        match value {
            TokenValue::Identifier { .. } => TokenKind::Identifier,
            TokenValue::Keyword { .. } => TokenKind::Keyword,
            TokenValue::IntLit { .. } => TokenKind::IntLit,
            TokenValue::FloatLit { .. } => TokenKind::FloatLit,
            TokenValue::Delimiter { .. } => TokenKind::Delimiter,
            TokenValue::Operator { .. } => TokenKind::Operator,
        }
    }

    pub fn as_str(&self) -> Rc<str> {
        match &self {
            TokenKind::Identifier { .. } => format!("Identifier").into(),
            TokenKind::Keyword { .. } => format!("Keyword").into(),
            TokenKind::IntLit { .. } => format!("Integer Literal").into(),
            TokenKind::FloatLit { .. } => format!("Floating-point Literal").into(),
            TokenKind::Delimiter { .. } => format!("Delimiter").into(),
            TokenKind::Operator { .. } => format!("Operator").into(),
        }
    }
}

#[derive(Debug)]
pub struct Token {
    pub value: TokenValue,
    pub span: Span,
}
