use std::{
    fs::File,
    io::{BufReader, Read},
    rc::Rc,
};

use crate::util::get_lines;

pub struct SourceCode {
    pub name: Rc<str>,
    pub code: Rc<[char]>,
    pub code_str: Rc<str>,
    pub lines: Rc<[(usize, usize)]>,
}

impl SourceCode {
    pub fn from_filename(filename: &str) -> Result<Self, anyhow::Error> {
        let file = File::open(filename)?;
        let mut reader = BufReader::new(file);
        let mut code = String::new();
        reader.read_to_string(&mut code)?;

        let lines = get_lines(&code);

        Ok(Self {
            name: filename.into(),
            code: code.chars().collect::<Vec<char>>().into(),
            code_str: code.into(),
            lines: lines,
        })
    }
}
