use std::rc::Rc;

#[derive(Debug)]
pub struct TypeName {
    pub name: Rc<str>,
    pub internal: bool,
}

#[derive(Debug)]
pub struct TypeTable {
    pub type_names: Vec<TypeName>,
}

impl TypeTable {
    pub fn default() -> Self {
        Self {
            type_names: vec![
                TypeName {
                    name: "int".into(),
                    internal: true,
                },
                TypeName {
                    name: "float".into(),
                    internal: true,
                },
            ],
        }
    }
}
