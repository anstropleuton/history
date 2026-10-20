use std::rc::Rc;

use crate::{
    diagnostic::Span,
    keyword::Keyword,
    report::{Report, Severity},
    source_code::SourceCode,
    token::{Token, TokenValue},
};

pub struct Lexer {
    pub tokens: Vec<Token>,
    pub reports: Vec<Report>,

    source_code: Rc<SourceCode>,
    pos: usize,
}

impl Lexer {
    pub fn new(source_code: Rc<SourceCode>) -> Self {
        Self {
            tokens: Vec::new(),
            reports: Vec::new(),
            source_code,
            pos: 0,
        }
    }

    fn is_id_start(c: char) -> bool {
        c.is_alphabetic() || "_$".contains(c)
    }

    fn is_id_cont(c: char) -> bool {
        c.is_alphanumeric() || "_$".contains(c)
    }

    fn is_num_start(c: char) -> bool {
        c.is_numeric()
    }

    fn is_num_cont(c: char) -> bool {
        c.is_numeric() || "._".contains(c)
    }

    fn is_delim(c: char) -> bool {
        "@(){}:;,.".contains(c)
    }

    fn is_op(c: char) -> bool {
        "~!%^&*-+=[]\\|<>/?".contains(c)
    }

    fn code(&self) -> &[char] {
        &self.source_code.code
    }

    fn len(&self) -> usize {
        self.code().len()
    }

    fn eof(&self) -> bool {
        self.pos >= self.len()
    }

    fn peek(&self) -> Option<char> {
        if self.eof() {
            None
        } else {
            Some(self.code()[self.pos])
        }
    }

    fn adv(&mut self) {
        if !self.eof() {
            self.pos += 1;
        }
    }

    pub fn lex(&mut self) {
        while !self.eof() {
            self.skip_ws();
            self.lex_id_or_kw();
            self.lex_num();
            self.lex_delim();
            self.lex_op();
        }
    }

    fn skip_ws(&mut self) {
        while let Some(c) = self.peek()
            && c.is_whitespace()
        {
            self.adv();
        }
    }

    fn lex_id_or_kw(&mut self) {
        if let Some(c) = self.peek()
            && Self::is_id_start(c)
        {
            let begin = self.pos;

            while let Some(c) = self.peek()
                && Self::is_id_cont(c)
            {
                self.adv();
            }

            let end = self.pos;
            let length = end - begin;

            let word = &self.code()[begin..end];
            let word = word.iter().collect::<String>();

            self.tokens.push(Token {
                value: if let Some(keyword) = Keyword::from_str(&word) {
                    TokenValue::Keyword { value: keyword }
                } else {
                    TokenValue::Identifier { value: word.into() }
                },
                span: Span {
                    begin,
                    length,
                    pointer: begin,
                },
            });
        }
    }

    fn lex_num(&mut self) {
        if let Some(c) = self.peek()
            && Self::is_num_start(c)
        {
            let begin = self.pos;

            while let Some(c) = self.peek()
                && Self::is_num_cont(c)
            {
                self.adv();
            }

            let end = self.pos;
            let length = end - begin;

            let number = &self.code()[begin..end];
            let number = number.iter().collect::<String>();

            if number.matches('.').count() > 1 {
                self.reports.push(Report {
                    message: format!("Numeric literal may only contain one decimal point").into(),
                    span: Span {
                        begin,
                        length,
                        pointer: begin,
                    },
                    severity: Severity::Error,
                });
                self.tokens.push(Token {
                    value: TokenValue::FloatLit { value: 0.0 },
                    span: Span {
                        begin,
                        length,
                        pointer: begin,
                    },
                });
                return;
            }

            let number = number.replace("\'", "");

            self.tokens.push(Token {
                value: if number.contains('.') {
                    TokenValue::FloatLit {
                        value: number.parse().unwrap(),
                    }
                } else {
                    TokenValue::IntLit {
                        value: number.parse().unwrap(),
                    }
                },
                span: Span {
                    begin,
                    length,
                    pointer: begin,
                },
            });
        }
    }

    fn lex_delim(&mut self) {
        while let Some(c) = self.peek()
            && Self::is_delim(c)
        {
            self.tokens.push(Token {
                value: TokenValue::Delimiter { value: c },
                span: Span {
                    begin: self.pos,
                    length: 1,
                    pointer: self.pos,
                },
            });
            self.adv();
        }
    }

    fn lex_op(&mut self) {
        if let Some(c) = self.peek()
            && Self::is_op(c)
        {
            let begin = self.pos;

            while let Some(c) = self.peek()
                && Self::is_op(c)
            {
                self.adv();
            }

            let end = self.pos;
            let length = end - begin;
            let operator = self.code()[begin..end].iter().collect::<String>();

            self.tokens.push(Token {
                value: TokenValue::Operator {
                    value: operator.into(),
                },
                span: Span {
                    begin,
                    length,
                    pointer: begin,
                },
            });
        }
    }
}
