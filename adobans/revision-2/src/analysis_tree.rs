use std::rc::Rc;

use crate::diagnostic::Span;

#[derive(Debug)]
pub enum AtNode {
    Int { value: i32, span: Span },
    Float { value: f64, span: Span },
    Binding { name: Rc<str>, span: Span },
}
