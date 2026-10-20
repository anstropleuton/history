#[derive(Debug, PartialEq, Eq, Clone)]
pub enum Keyword {
    Let,
    Print,
}

impl Keyword {
    pub fn as_str(&self) -> &str {
        match self {
            Self::Let => "let",
            Self::Print => "print",
        }
    }

    pub fn from_str(string: &str) -> Option<Self> {
        match string {
            "let" => Some(Self::Let),
            "print" => Some(Self::Print),
            _ => None,
        }
    }
}
