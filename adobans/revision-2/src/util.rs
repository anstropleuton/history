use std::rc::Rc;

pub fn get_lines(s: &str) -> Rc<[(usize, usize)]> {
    let mut lines = Vec::new();
    let mut pos = 0;

    while pos <= s.len() {
        let next = s[pos..].find('\n').map(|i| pos + i).unwrap_or(s.len());

        lines.push((pos, next - pos));
        pos = next + 1; // skip '\n'
    }

    lines.into()
}

pub fn get_line_row_col(lines: &[(usize, usize)], pos: usize) -> (usize, usize) {
    for (row, &(begin, len)) in lines.iter().enumerate() {
        let end = begin + len;
        if pos >= begin && pos < end {
            return (row + 1, pos - begin + 1);
        }
    }

    panic!("position {pos} out of range");
}
