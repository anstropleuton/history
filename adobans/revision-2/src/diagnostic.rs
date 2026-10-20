use std::rc::Rc;

#[derive(Debug, Clone, Copy)]
pub struct Span {
    pub begin: usize,
    pub length: usize,
    pub pointer: usize,
}

pub fn preview_span(text: &str, lines: &Rc<[(usize, usize)]>, span: Span, indent: usize) -> String {
    let end = span.begin + span.length;

    let ln_width = lines.len().to_string().len();
    let indent_spaces = " ".repeat(indent);

    let mut out = String::new();

    for (row, &(line_begin, line_len)) in lines.iter().enumerate() {
        let line_end = line_begin + line_len;

        if line_end <= span.begin || line_begin >= end {
            continue;
        }

        let line = &text[line_begin..line_end];

        let mark_begin = span.begin.saturating_sub(line_begin);
        let mark_end = (end - line_begin).min(line_len);

        out.push_str(&indent_spaces);
        out.push_str(&format!("{:>width$} | ", row + 1, width = ln_width));
        out.push_str(line);
        out.push('\n');

        out.push_str(&indent_spaces);
        out.push_str(&" ".repeat(ln_width));
        out.push_str(" | ");
        out.push_str(&" ".repeat(mark_begin));

        for j in mark_begin..mark_end {
            let ch = if span.pointer >= line_begin
                && span.pointer < line_end
                && j == span.pointer - line_begin
            {
                '^'
            } else if j == mark_begin {
                '<'
            } else if j == mark_end - 1 {
                '>'
            } else {
                '~'
            };

            out.push(ch);
        }

        out.push('\n');
    }

    out
}
