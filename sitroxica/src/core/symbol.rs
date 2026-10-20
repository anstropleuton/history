pub type SymbolId = u64;

pub enum Symbol {
    Variable,
    Function(FunctionType),
    Class(ClassType),
    Enum(EnumType),
}

pub struct SymTable {}
