use std::{cell::RefCell, rc::Rc, sync::Weak};

use crate::typing::TypeName;

#[derive(Debug)]
pub struct Binding {
    pub name: Rc<str>,
    pub typename: TypeName,
}

#[derive(Debug)]
pub struct Scope {
    pub bindings: Vec<Rc<Binding>>,
    pub type_names: Vec<Rc<TypeName>>,
    pub parent: Weak<RefCell<Scope>>,
    pub children: Vec<Rc<RefCell<Scope>>>,
}

impl Scope {
    pub fn new() -> Self {
        Self {
            bindings: [].into(),
            type_names: [].into(),
            parent: Weak::new().into(),
            children: vec![].into(),
        }
    }

    pub fn find_symbol(&self, name: &str) -> Option<Rc<Binding>> {
        if let Some(sym) = self.bindings.iter().rfind(|sym| sym.name == name.into()) {
            return Some(sym.clone());
        }

        if let Some(parent) = self.parent.upgrade() {
            return parent.borrow().find_symbol(name);
        }

        return None;
    }

    pub fn add_symbol(&mut self, binding: Binding) -> Rc<Binding> {
        let binding = Rc::new(binding);
        self.bindings.push(binding.clone());

        return binding;
    }
}
