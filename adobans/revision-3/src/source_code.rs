use std::rc::Rc;

pub struct SourceCode {
    code: Rc<str>,
    lines: Rc<[Rc<str>]>,
}
