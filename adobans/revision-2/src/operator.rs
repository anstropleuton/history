use std::rc::Rc;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Assoc {
    Left,
    Right,
}

#[derive(Debug, Clone, Copy)]
pub enum OpKind {
    Prefix,
    Suffix,
    Infix,
}

#[derive(Debug)]
pub enum PrecLevel {
    Prefix { symbols: Vec<Rc<str>> },
    Suffix { symbols: Vec<Rc<str>> },
    Infix { symbols: Vec<Rc<str>>, assoc: Assoc },
}

#[derive(Debug)]
pub struct OpTable {
    pub precs: Vec<PrecLevel>,
}

impl OpTable {
    pub fn default() -> Self {
        Self {
            precs: vec![
                PrecLevel::Infix {
                    symbols: vec!["=".into()],
                    assoc: Assoc::Right,
                },
                PrecLevel::Infix {
                    symbols: vec!["+".into(), "-".into()],
                    assoc: Assoc::Left,
                },
                PrecLevel::Infix {
                    symbols: vec!["*".into(), "/".into()],
                    assoc: Assoc::Left,
                },
                PrecLevel::Infix {
                    symbols: vec!["**".into()],
                    assoc: Assoc::Right,
                },
                PrecLevel::Suffix {
                    symbols: vec!["!".into()],
                },
                PrecLevel::Prefix {
                    symbols: vec!["-".into()],
                },
                PrecLevel::Prefix {
                    symbols: vec!["!".into(), "~".into()],
                },
            ],
        }
    }

    pub fn find_prefix(&self, symbol: &str) -> Option<usize> {
        for (index, level) in self.precs.iter().enumerate() {
            let PrecLevel::Prefix { symbols } = level else {
                continue;
            };

            for s in symbols {
                if **s == *symbol {
                    return Some(index);
                }
            }
        }

        return None;
    }

    pub fn find_suffix(&self, symbol: &str) -> Option<usize> {
        for (index, level) in self.precs.iter().enumerate() {
            let PrecLevel::Suffix { symbols } = level else {
                continue;
            };

            for s in symbols {
                if **s == *symbol {
                    return Some(index);
                }
            }
        }

        return None;
    }

    pub fn find_infix(&self, symbol: &str) -> Option<(usize, Assoc)> {
        for (index, level) in self.precs.iter().enumerate() {
            let PrecLevel::Infix { symbols, assoc } = level else {
                continue;
            };

            for s in symbols {
                if **s == *symbol {
                    return Some((index, *assoc));
                }
            }
        }

        return None;
    }
}
