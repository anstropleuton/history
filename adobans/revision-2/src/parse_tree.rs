use std::rc::Rc;

use crate::diagnostic::Span;

#[derive(Debug)]
pub enum PtNode {
    Int {
        value: i32,
        span: Span,
    },
    Float {
        value: f64,
        span: Span,
    },
    Id {
        value: Rc<str>,
        span: Span,
    },
    InfOp {
        operator: Rc<str>,
        left: Box<PtNode>,
        right: Box<PtNode>,
        span: Span,
    },
    PreOp {
        operator: Rc<str>,
        value: Box<PtNode>,
        span: Span,
    },
    SufOp {
        operator: Rc<str>,
        value: Box<PtNode>,
        span: Span,
    },
    Let {
        identifier: Rc<str>,
        value_type: Option<Box<PtNode>>,
        value: Option<Box<PtNode>>,
    },
    Print {
        value_type: Option<Box<PtNode>>,
    },
    Comp {
        children: Rc<[Box<PtNode>]>,
    },
}
