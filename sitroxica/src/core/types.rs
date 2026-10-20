use crate::data::primitive::PrimitiveType;

#[derive(Clone)]
pub struct TypeId(pub u64);

#[derive(Clone)]
pub struct ClassId(pub u64);

#[derive(Clone)]
pub struct EnumId(pub u64);

#[derive(Clone)]
pub enum Modifier {
    Constant,
    Reference,
}

#[derive(Clone)]
pub enum TypeName {
    PrimitiveType(PrimitiveType),
    FunctionType {
        params: Box<[TypeId]>,
        returns: Box<[TypeId]>,
    },
    ClassType(ClassId),
    EnumType(EnumId),
    TupleType {
        elements: Box<[TypeId]>,
    },
    ArrayType {
        base: TypeId,
        size: Option<usize>,
    },
    SetType {
        base: TypeId,
        size: Option<usize>,
    },
    MapType {
        from: TypeId,
        into: TypeId,
    },
    ModifiedType {
        base: TypeId,
        modifier: Modifier,
    },
}
