pub enum Keyword {
    As,
    Break,
    Class,
    Cont,
    Else,
    Export,
    Fn,
    For,
    If,
    Import,
    Is,
    Let,
    Null,
    Ns,
    Return,
    While,
}

impl Keyword {
    pub fn from_str(s: &str) -> Option<Keyword> {
        match s {
            "as" => Some(Keyword::As),
            "break" => Some(Keyword::Break),
            "class" => Some(Keyword::Class),
            "cont" => Some(Keyword::Cont),
            "else" => Some(Keyword::Else),
            "export" => Some(Keyword::Export),
            "fn" => Some(Keyword::Fn),
            "for" => Some(Keyword::For),
            "if" => Some(Keyword::If),
            "import" => Some(Keyword::Import),
            "is" => Some(Keyword::Is),
            "let" => Some(Keyword::Let),
            "null" => Some(Keyword::Null),
            "ns" => Some(Keyword::Ns),
            "return" => Some(Keyword::Return),
            "while" => Some(Keyword::While),
            _ => None,
        }
    }

    pub fn to_str(&self) -> &str {
        match self {
            Keyword::As => "as",
            Keyword::Break => "break",
            Keyword::Class => "class",
            Keyword::Cont => "cont",
            Keyword::Else => "else",
            Keyword::Export => "export",
            Keyword::Fn => "fn",
            Keyword::For => "for",
            Keyword::If => "if",
            Keyword::Import => "import",
            Keyword::Is => "is",
            Keyword::Let => "let",
            Keyword::Null => "null",
            Keyword::Ns => "ns",
            Keyword::Return => "return",
            Keyword::While => "while",
        }
    }
}
