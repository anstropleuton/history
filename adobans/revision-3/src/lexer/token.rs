use std::rc::Rc;

pub enum Keyword {
    Let,
    Fn,
    Return,
    If,
    Else,
    While,
    For,
    In,
    Class,
    Ns,
    Import,
    Export,
    True,
    False,
}

impl Keyword {
    pub fn from_ident(s: &str) -> Option<Self> {
        match s {
            "let" => Some(Self::Let),
            "fn" => Some(Self::Fn),
            "return" => Some(Self::Return),
            "if" => Some(Self::If),
            "else" => Some(Self::Else),
            "while" => Some(Self::While),
            "for" => Some(Self::For),
            "in" => Some(Self::In),
            "class" => Some(Self::Class),
            "ns" => Some(Self::Ns),
            "import" => Some(Self::Import),
            "export" => Some(Self::Export),
            "true" => Some(Self::True),
            "false" => Some(Self::False),
            _ => None,
        }
    }

    pub fn to_ident(&self) -> &str {
        match self {
            Self::Let => "let",
            Self::Fn => "fn",
            Self::Return => "return",
            Self::If => "if",
            Self::Else => "else",
            Self::While => "while",
            Self::For => "for",
            Self::In => "in",
            Self::Class => "class",
            Self::Ns => "ns",
            Self::Import => "import",
            Self::Export => "export",
            Self::True => "true",
            Self::False => "false",
        }
    }
}

// Arranged in order of highest to lowest precedence
#[repr(u8)]
pub enum Operator {
    Factorial,
    Increment,
    Decrement,
    BitNot,
    LogicNot,
    BitLeft,
    BitRight,
    BitXor,
    BitAnd,
    BitOr,
    Mod,
    Multiply,
    Divide,
    Add,
    Subtract,
    Equal,
    Inequal,
    LessThan,
    MoreThan,
    LessThanEqual,
    MoreThanEqual,
    LogicAnd,
    LogicOr,
    Assign,
}

pub enum Fixture {
    Prefix,
    Infix,
    Suffix,
}

impl Operator {
    const MAX: u8 = Self::Assign as u8;

    pub fn from_sym(s: &str) -> Option<Self> {
        match s {
            "!!" => Some(Self::Factorial),
            "++" => Some(Self::Increment),
            "--" => Some(Self::Decrement),
            "~" => Some(Self::BitNot),
            "!" => Some(Self::LogicNot),
            "<<" => Some(Self::BitLeft),
            ">>" => Some(Self::BitRight),
            "^" => Some(Self::BitXor),
            "&" => Some(Self::BitAnd),
            "|" => Some(Self::BitOr),
            "%" => Some(Self::Mod),
            "*" => Some(Self::Multiply),
            "/" => Some(Self::Divide),
            "+" => Some(Self::Add),
            "-" => Some(Self::Subtract),
            "==" => Some(Self::Equal),
            "!=" => Some(Self::Inequal),
            "<" => Some(Self::LessThan),
            ">" => Some(Self::MoreThan),
            "<=" => Some(Self::LessThanEqual),
            ">=" => Some(Self::MoreThanEqual),
            "&&" => Some(Self::LogicAnd),
            "||" => Some(Self::LogicOr),
            "=" => Some(Self::Assign),
            _ => None,
        }
    }

    pub fn to_sym(&self) -> &str {
        match self {
            Self::Factorial => "!!",
            Self::Increment => "++",
            Self::Decrement => "--",
            Self::BitNot => "~",
            Self::LogicNot => "!",
            Self::BitLeft => "<<",
            Self::BitRight => ">>",
            Self::BitXor => "^",
            Self::BitAnd => "&",
            Self::BitOr => "|",
            Self::Mod => "%",
            Self::Multiply => "*",
            Self::Divide => "/",
            Self::Add => "+",
            Self::Subtract => "-",
            Self::Equal => "==",
            Self::Inequal => "!=",
            Self::LessThan => "<",
            Self::MoreThan => ">",
            Self::LessThanEqual => "<=",
            Self::MoreThanEqual => ">=",
            Self::LogicAnd => "&&",
            Self::LogicOr => "||",
            Self::Assign => "=",
        }
    }

    pub fn fixture(self) -> Fixture {
        match self {
            Self::Factorial => Fixture::Suffix,
            Self::Increment => Fixture::Suffix,
            Self::Decrement => Fixture::Suffix,
            Self::BitNot => Fixture::Prefix,
            Self::LogicNot => Fixture::Prefix,
            Self::BitLeft => Fixture::Infix,
            Self::BitRight => Fixture::Infix,
            Self::BitXor => Fixture::Infix,
            Self::BitAnd => Fixture::Infix,
            Self::BitOr => Fixture::Infix,
            Self::Mod => Fixture::Infix,
            Self::Multiply => Fixture::Infix,
            Self::Divide => Fixture::Infix,
            Self::Add => Fixture::Infix,
            Self::Subtract => Fixture::Infix,
            Self::Equal => Fixture::Infix,
            Self::Inequal => Fixture::Infix,
            Self::LessThan => Fixture::Infix,
            Self::MoreThan => Fixture::Infix,
            Self::LessThanEqual => Fixture::Infix,
            Self::MoreThanEqual => Fixture::Infix,
            Self::LogicAnd => Fixture::Infix,
            Self::LogicOr => Fixture::Infix,
            Self::Assign => Fixture::Infix,
        }
    }

    pub fn precedence(self) -> u8 {
        return Self::MAX - self as u8;
    }
}

pub enum Punctuation {
    LBrace,
    RBrace,
    LParen,
    RParen,
    LBracket,
    RBracket,
    Comma,
    Dot,
    Colon,
    Semicolon,
    Arrow,
}

impl Punctuation {
    pub fn from_sym(s: &str) -> Option<Self> {
        match s {
            "{" => Some(Self::LBrace),
            "}" => Some(Self::RBrace),
            "(" => Some(Self::LParen),
            ")" => Some(Self::RParen),
            "[" => Some(Self::LBracket),
            "]" => Some(Self::RBracket),
            "," => Some(Self::Comma),
            "." => Some(Self::Dot),
            ":" => Some(Self::Colon),
            ";" => Some(Self::Semicolon),
            "->" => Some(Self::Arrow),
            _ => None,
        }
    }
}

pub enum TokenKind {
    IntLiteral(i32),
    FloatLiteral(f64),
    CharLiteral(char),
    BoolLiteral(bool),
    StringLiteral(Rc<str>),
    Keyword(Keyword),
}
