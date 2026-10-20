use crate::{core::types::TypeName, data::primitive::PrimitiveType};

pub enum ConversionType {
    Implicit,
    Explicit,
}

pub struct Conversion {
    pub from: TypeName,
    pub to: TypeName,
    pub conversion_type: ConversionType,
}

pub struct PromoGraph {
    pub name: Box<str>,
    pub graph: Box<[(TypeName, TypeName)]>,
}

macro_rules! promo_graph {
    ($( $from:ident => $to:ident ),* $(,)?) => {
        [
           $(
               (
                   TypeName::PrimitiveType(PrimitiveType::$from),
                   TypeName::PrimitiveType(PrimitiveType::$to)
               ),
           )*
        ] as [(TypeName, TypeName); _]
    };
}

pub struct Promotions {
    pub promos: Box<[PromoGraph]>,
}

impl Promotions {
    pub fn default() -> Promotions {
        let promo_sint = promo_graph!(
            Int8  => Int16,
            Int16 => Int32,
            Int32 => Int64,
            Int64 => Int128,
        );

        let promo_uint = promo_graph!(
            UInt8  => UInt16,
            UInt16 => UInt32,
            UInt32 => UInt64,
            UInt64 => UInt128,
        );

        let promo_sint_uint = promo_graph!(
            UInt8  => Int16,
            UInt16 => Int32,
            UInt32 => Int64,
            UInt64 => Int128,
        );

        let promo_float = promo_graph!(
            // Float16 => Float32,
            Float32 => Float64,
            // Float64 => Float128,
            // Float128 => Float256,
        );

        let promo_sint_float = promo_graph!(
            // Int8  => Float16,
            Int16 => Float32,
            Int32 => Float64,
            // Int64  => Float128,
            // Int128 => Float256,
        );

        let promo_uint_float = promo_graph!(
            // UInt8  => Float16,
            UInt16 => Float32,
            UInt32 => Float64,
            // UInt64  => Float128,
            // UInt128 => Float256,
        );

        let promo_decimal = promo_graph!(
            // Decimal32 => Decimal64,
            // Decimal64 => Decimal128,
        );

        let promo_float_decimal = promo_graph!(
            // Float16  => Decimal32,
            // Float32  => Decimal32,
            // Float64  => Decimal64,
            // Float128 => Decimal128,
        );

        let promo_sint_decimal = promo_graph!(
            // Int8  => Decimal32,
            // Int16 => Decimal32,
            // Int32 => Decimal64,
            // Int64 => Decimal128,
        );

        let promo_uint_decimal = promo_graph!(
            // UInt8  => Decimal32,
            // UInt16 => Decimal32,
            // UInt32 => Decimal64,
            // UInt64 => Decimal128,
        );

        let mut promo_int = Vec::new();
        promo_int.extend_from_slice(&promo_sint);
        promo_int.extend_from_slice(&promo_uint);
        promo_int.extend_from_slice(&promo_sint_uint);

        let mut promo_real = Vec::new();
        promo_real.extend_from_slice(&promo_float);
        promo_real.extend_from_slice(&promo_decimal);
        promo_real.extend_from_slice(&promo_float_decimal);

        let mut promo_int_float = Vec::new();
        promo_int_float.extend_from_slice(&promo_sint_float);
        promo_int_float.extend_from_slice(&promo_uint_float);

        let mut promo_int_decimal = Vec::new();
        promo_int_decimal.extend_from_slice(&promo_sint_decimal);
        promo_int_decimal.extend_from_slice(&promo_uint_decimal);

        let mut promo_int_real = Vec::new();
        promo_int_real.extend_from_slice(&promo_int_float);
        promo_int_real.extend_from_slice(&promo_int_decimal);

        let mut promo_num = Vec::new();
        promo_num.extend_from_slice(&promo_int);
        promo_num.extend_from_slice(&promo_real);
        promo_num.extend_from_slice(&promo_int_real);

        Promotions {
            promos: vec![
                PromoGraph {
                    name: "promo_sint".into(),
                    graph: promo_sint.into(),
                },
                PromoGraph {
                    name: "promo_uint".into(),
                    graph: promo_uint.into(),
                },
                PromoGraph {
                    name: "promo_sint_uint".into(),
                    graph: promo_sint_uint.into(),
                },
                PromoGraph {
                    name: "promo_float".into(),
                    graph: promo_float.into(),
                },
                PromoGraph {
                    name: "promo_sint_float".into(),
                    graph: promo_sint_float.into(),
                },
                PromoGraph {
                    name: "promo_uint_float".into(),
                    graph: promo_uint_float.into(),
                },
                PromoGraph {
                    name: "promo_decimal".into(),
                    graph: promo_decimal.into(),
                },
                PromoGraph {
                    name: "promo_float_decimal".into(),
                    graph: promo_float_decimal.into(),
                },
                PromoGraph {
                    name: "promo_sint_decimal".into(),
                    graph: promo_sint_decimal.into(),
                },
                PromoGraph {
                    name: "promo_uint_decimal".into(),
                    graph: promo_uint_decimal.into(),
                },
                PromoGraph {
                    name: "promo_int".into(),
                    graph: promo_int.into(),
                },
                PromoGraph {
                    name: "promo_real".into(),
                    graph: promo_real.into(),
                },
                PromoGraph {
                    name: "promo_int_float".into(),
                    graph: promo_int_float.into(),
                },
                PromoGraph {
                    name: "promo_int_decimal".into(),
                    graph: promo_int_decimal.into(),
                },
                PromoGraph {
                    name: "promo_int_real".into(),
                    graph: promo_int_real.into(),
                },
                PromoGraph {
                    name: "promo_num".into(),
                    graph: promo_num.into(),
                },
            ]
            .into(),
        }
    }
}
