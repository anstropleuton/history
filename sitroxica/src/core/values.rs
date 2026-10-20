use crate::{
    core::{symbol::SymbolId, types::TypeId},
    data::primitive::PrimitiveValue,
};

pub enum ValueRepr {
    PrimitiveValue(PrimitiveValue),
    FunctionValue(FunctionValue),
    ClassValue(ClassValue),
    EnumValue(EnumValue),
    TupleValue(TupleValue),
    ArrayValue(ArrayValue),
    SetValue(SetValue),
    MapValue(MapValue),
    ModifiedValue(ModifiedValue),
}

pub struct FunctionValue {
    pub type_name_id: TypeId,
    pub symbol_id: SymbolId,
}

pub struct ClassEntryValue {
    pub type_name_id: TypeId,
    pub value: Box<ValueRepr>,
}

pub struct ClassValue {
    pub type_name_id: TypeId,
    pub entries: Box<[ClassEntryValue]>,
}

pub struct EnumValue {
    pub type_name_id: TypeId,
    pub value: u64,
}

pub struct TupleValue {
    pub type_name_id: TypeId,
    pub members: Box<[ValueRepr]>,
}

pub struct ArrayValue {
    pub type_name_id: TypeId,
    pub values: Box<[ValueRepr]>,
}

pub struct SetValue {
    pub type_name_id: TypeId,
    pub values: Box<[ValueRepr]>,
}

pub struct MapValue {
    pub type_name_id: TypeId,
    pub values: Box<[(ValueRepr, ValueRepr)]>,
}

pub struct ModifiedValue {
    pub type_name_id: TypeId,
    pub value: Box<ValueRepr>,
}
