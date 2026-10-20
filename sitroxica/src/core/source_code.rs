pub struct SourceCode {
    pub string: Box<str>,
    pub chars: Box<[char]>,
    pub lines: Box<[Box<str>]>,
}
