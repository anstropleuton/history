use crate::{core::types::TypeName, data::primitive::PrimitiveType};

pub enum Assoc {
    Left,
    Right,
}

pub enum Fixture {
    Prefix,
    Suffix,
    Infix,
}

pub enum OpType {
    TypeName(TypeName),
    PromoGraph(Box<str>),
}

pub struct PrefixOp {
    pub symbol: Box<str>,
    pub right_type: OpType,
    pub result_type: OpType,
}

pub struct SuffixOp {
    pub symbol: Box<str>,
    pub left_type: OpType,
    pub result_type: OpType,
}

pub struct InfixOp {
    pub symbol: Box<str>,
    pub left_type: OpType,
    pub right_type: OpType,
    pub result_type: OpType,
}

pub enum PrecLevel {
    Prefix { ops: Vec<PrefixOp> },
    Suffix { ops: Vec<SuffixOp> },
    Infix { ops: Vec<InfixOp>, assoc: Assoc },
}

pub struct Operators {
    pub prec: Vec<PrecLevel>,
}

macro_rules! op_type {
    ($p:ident) => {
        OpType::TypeName(TypeName::PrimitiveType(PrimitiveType::$p))
    };

    ($s:literal) => {
        OpType::PromoGraph($s.into())
    };
}

macro_rules! prefix_ops {
    ($( $sym:expr => [ $( ($r:tt, $t:tt) ),* $(,)? ] ),* $(,)?) => {
        {
            PrecLevel::Prefix {
                ops: [
                    $(
                        $(
                            PrefixOp {
                                symbol: $sym.into(),
                                right_type: op_type!($r),
                                result_type: op_type!($t)
                            },
                        )*
                    )*
                ].into()
            }
        }
    };
}

macro_rules! suffix_ops {
    ($( $sym:expr => [ $( ($l:tt, $t:tt) ),* $(,)? ] ),* $(,)?) => {
        {
            PrecLevel::Suffix {
                ops: [
                    $(
                        $(
                            SuffixOp {
                                symbol: $sym.into(),
                                left_type: op_type!($l),
                                result_type: op_type!($t)
                            },
                        )*
                    )*
                ].into()
            }
        }
    };
}

macro_rules! infix_ops {
    ($assoc:ident; $( $sym:expr => [ $( ($l:tt, $r:tt, $t:tt) ),* $(,)? ] ),* $(,)?) => {
        {
            PrecLevel::Infix {
                assoc: Assoc::$assoc,
                ops: [
                    $(
                        $(
                            InfixOp {
                                symbol: $sym.into(),
                                left_type: op_type!($l),
                                right_type: op_type!($r),
                                result_type: op_type!($t)
                            },
                        )*
                    )*
                ].into()
            }
        }
    };
}

impl Operators {
    pub fn default() -> Operators {
        Operators {
            prec: vec![
                infix_ops!(Right;
                    "=" => [("promo_num", "promo_num", "promo_num"), (Char, Char, Char), (Bool, Bool, Bool), (String, String, String)],
                    "+=" => [("promo_num", "promo_num", "promo_num"), (String, String, String)],
                    "-=" => [("promo_num", "promo_num", "promo_num")],
                    "*=" => [("promo_num", "promo_num", "promo_num"), (String, "promo_uint", String)],
                    "/=" => [("promo_num", "promo_num", "promo_num")],
                    "%=" => [("promo_num", "promo_num", "promo_num")],
                ),
                infix_ops!(Left;
                    "&&" => [(Bool, Bool, Bool)],
                    "||" => [(Bool, Bool, Bool)],
                ),
                infix_ops!(Left;
                    "<" => [("promo_num", "promo_num", "promo_num")],
                    ">" => [("promo_num", "promo_num", "promo_num")],
                    "<=" => [("promo_num", "promo_num", "promo_num")],
                    ">=" => [("promo_num", "promo_num", "promo_num")],
                ),
                infix_ops!(Left;
                    "==" => [("promo_num", "promo_num", "promo_num"), (Char, Char, Char), (Bool, Bool, Bool), (String, String, String)],
                    "!=" => [("promo_num", "promo_num", "promo_num"), (Char, Char, Char), (Bool, Bool, Bool), (String, String, String)],
                ),
                infix_ops!(Left;
                    "+" => [("promo_num", "promo_num", "promo_num"), (String, String, String)],
                    "-" => [("promo_num", "promo_num", "promo_num")],
                ),
                infix_ops!(Left;
                    "*" => [("promo_num", "promo_num", "promo_num"), (String, "promo_uint", String)],
                    "/" => [("promo_num", "promo_num", "promo_num")],
                ),
                infix_ops!(Left;
                    "%" => [("promo_num", "promo_num", "promo_num")],
                ),
                infix_ops!(Right;
                    "**" => [("promo_num", "promo_num", "promo_num")],
                ),
                infix_ops!(Left;
                    "&" => [("promo_int", "promo_int", "promo_int")],
                    "|" => [("promo_int", "promo_int", "promo_int")],
                ),
                suffix_ops!(
                    "++" => [("promo_num", "promo_num")],
                    "--" => [("promo_num", "promo_num")],
                ),
                prefix_ops!(
                    "++" => [("promo_num", "promo_num")],
                    "--" => [("promo_num", "promo_num")],
                ),
                prefix_ops!(
                    "!" => [(Bool, Bool)],
                    "~" => [("promo_int", "promo_int")],
                ),
                prefix_ops!(
                    "+" => [("promo_num", "promo_num")],
                    "-" => [("promo_num", "promo_num")],
                ),
            ],
        }
    }
}
