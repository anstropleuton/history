use std::rc::Rc;

use crate::{
    diagnostic::Span,
    keyword::Keyword,
    operator::{Assoc, OpTable},
    parse_tree::PtNode,
    report::{Report, Severity},
    token::{Token, TokenKind, TokenValue},
};

macro_rules! gen_match_helpers {
    ($match_at_fn:ident, $match_with_at_fn:ident, $match_fn:ident, $match_with_fn:ident, $expect_at_fn:ident, $expect_with_at_fn:ident, $expect_fn:ident, $expect_with_fn:ident, $variant_type:ident, $value_type:ty) => {
        #[allow(dead_code)]
        fn $match_at_fn(&self, offset: usize) -> Option<$value_type> {
            if matches!(self.peek_at(offset), Some(token) if matches!(token.value, TokenValue::$variant_type { value: _ })) {
                return match &self.peek_at(offset).unwrap().value {
                    TokenValue::$variant_type { value } => Some(value.clone()),
                    _ => unreachable!(),
                };
            }

            return None;
        }

        #[allow(dead_code)]
        fn $match_with_at_fn(&self, value: $value_type, offset: usize) -> Option<$value_type> {
            if let Some(match_value) = self.$match_at_fn(offset) && match_value == value {
                return Some(match_value)
            }

            return None;
        }

        #[allow(dead_code)]
        fn $match_fn(&self) -> Option<$value_type> {
            self.$match_at_fn(0)
        }

        #[allow(dead_code)]
        fn $match_with_fn(&self, value: $value_type) -> Option<$value_type> {
            self.$match_with_at_fn(value, 0)
        }

        #[allow(dead_code)]
        fn $expect_at_fn(&mut self, offset: usize) -> Option<$value_type> {
            if let Some(value) = self.$match_at_fn(offset) {
                return Some(value);
            }

            self.reports.push(Report {
                message: format!("Expected {}, Found {}", TokenKind::$variant_type.as_str(), match self.peek_at(offset) {
                    Some(token) => token.value.value_as_str(),
                    None => "End of Stream".into()
                }).into(),
                severity: Severity::Error,
                span: if let Some(token) = self.peek_at(offset) {
                    token.span
                } else {
                    Span {
                        begin: 0,
                        length: 0,
                        pointer: 0
                    }
                },
            });

            return None;
        }

        #[allow(dead_code)]
        fn $expect_with_at_fn(&mut self, value: $value_type, offset: usize) -> Option<$value_type> {
            if let Some(value) = self.$match_with_at_fn(value.clone(), offset) {
                return Some(value);
            }

            self.reports.push(Report {
                message: format!("Expected {}, Found {}", TokenValue::$variant_type { value }.value_as_str(), match self.peek_at(offset) {
                    Some(token) => token.value.value_as_str(),
                    None => "End of Stream".into()
                }).into(),
                severity: Severity::Error,
                span: if let Some(token) = self.peek_at(offset) {
                    token.span
                } else {
                    Span {
                        begin: 0,
                        length: 0,
                        pointer: 0
                    }
                },
            });

            return None;
        }

        #[allow(dead_code)]
        fn $expect_fn(&mut self) -> Option<$value_type> {
            self.$expect_at_fn(0)
        }

        #[allow(dead_code)]
        fn $expect_with_fn(&mut self, value: $value_type) -> Option<$value_type> {
            self.$expect_with_at_fn(value, 0)
        }
    };
}

pub struct Parser {
    pub tree: Option<Box<PtNode>>,
    pub reports: Vec<Report>,

    tokens: Rc<[Token]>,
    op_table: OpTable,
    pos: usize,
}

impl Parser {
    pub fn new(tokens: Rc<[Token]>, op_table: OpTable) -> Self {
        Self {
            tree: None,
            reports: Vec::new(),
            tokens,
            op_table,
            pos: 0,
        }
    }

    gen_match_helpers! {match_id_at, match_id_with_at, match_id, match_id_with, expect_id_at, expect_id_with_at, expect_id, expect_id_with, Identifier, Rc<str>}
    gen_match_helpers! {match_kw_at, match_kw_with_at, match_kw, match_kw_with, expect_kw_at, expect_kw_with_at, expect_kw, expect_kw_with, Keyword, Keyword}
    gen_match_helpers! {match_int_at, match_int_with_at, match_int, match_int_with, expect_int_at, expect_int_with_at, expect_int, expect_int_with, IntLit, i32}
    gen_match_helpers! {match_float_at, match_float_with_at, match_float, match_float_with, expect_float_at, expect_float_with_at, expect_float, expect_float_with, FloatLit, f64}
    gen_match_helpers! {match_delim_at, match_delim_with_at, match_delim, match_delim_with, expect_delim_at, expect_delim_with_at, expect_delim, expect_delim_with, Delimiter, char}
    gen_match_helpers! {match_op_at, match_op_with_at, match_op, match_op_with, expect_op_at, expect_op_with_at, expect_op, expect_op_with, Operator, Rc<str>}

    fn len(&self) -> usize {
        self.tokens.len()
    }

    fn eof(&self) -> bool {
        self.pos >= self.len()
    }

    fn peek_at(&self, offset: usize) -> Option<&Token> {
        if self.pos + offset < self.len() {
            Some(&self.tokens[self.pos + offset])
        } else {
            None
        }
    }

    fn peek(&self) -> Option<&Token> {
        self.peek_at(0)
    }

    fn adv(&mut self) {
        if !self.eof() {
            self.pos += 1;
        }
    }

    pub fn parse(&mut self) {
        let mut comp: Vec<Box<PtNode>> = Vec::new();

        while !self.eof() {
            let node = if let Some(_) = self.match_kw_with(Keyword::Let) {
                self.parse_let()
            } else {
                let node = self.parse_expr(0);
                if let Some(_) = self.expect_delim_with(';') {
                    self.adv();
                }
                node
            };

            match node {
                Some(node) => comp.push(node),
                None => {
                    self.adv();
                }
            }
        }

        self.tree = Some(Box::new(PtNode::Comp {
            children: comp.into(),
        }));
    }

    fn parse_let(&mut self) -> Option<Box<PtNode>> {
        self.expect_kw_with(Keyword::Let)?;

        self.adv();

        let identifier = self.expect_id()?;

        self.adv();

        let value_type = if let Some(_) = self.match_delim_with(':') {
            self.adv();
            self.parse_id()
        } else {
            None
        };

        let value = if let Some(_) = self.match_op_with("=".into()) {
            self.adv();
            self.parse_expr(0)
        } else {
            None
        };

        if let Some(_) = self.expect_delim_with(';') {
            self.adv();
        }

        return Some(Box::new(PtNode::Let {
            identifier: identifier,
            value_type: value_type,
            value: value,
        }));
    }

    fn is_prefix_at(&self, offset: usize) -> Option<usize> {
        if let Some(token) = self.peek_at(offset) {
            match &token.value {
                TokenValue::Operator { value } => {
                    if let Some(operator) = self.op_table.find_prefix(value) {
                        return Some(operator);
                    }
                }
                _ => {}
            }
        }

        return None;
    }

    fn is_prefix(&self) -> Option<usize> {
        self.is_prefix_at(0)
    }

    fn is_suffix_at(&self, offset: usize) -> Option<usize> {
        if let Some(token) = self.peek_at(offset) {
            match &token.value {
                TokenValue::Operator { value } => {
                    if let Some(operator) = self.op_table.find_suffix(value) {
                        return Some(operator);
                    }
                }
                _ => {}
            }
        }

        return None;
    }

    fn is_suffix(&self) -> Option<usize> {
        self.is_suffix_at(0)
    }

    fn is_infix_at(&self, offset: usize) -> Option<(usize, Assoc)> {
        if let Some(token) = self.peek_at(offset) {
            match &token.value {
                TokenValue::Operator { value } => {
                    if let Some(operator) = self.op_table.find_infix(value) {
                        return Some(operator);
                    }
                }
                _ => {}
            }
        }

        return None;
    }

    fn is_infix(&self) -> Option<(usize, Assoc)> {
        self.is_infix_at(0)
    }

    fn can_start_expr_at(&self, mut offset: usize) -> bool {
        loop {
            if let Some(token) = self.peek_at(offset) {
                match &token.value {
                    TokenValue::IntLit { .. } | TokenValue::Identifier { .. } => return true,
                    _ => {}
                }

                if let Some(_) = self.is_prefix_at(offset) {
                    offset += 1;
                    continue;
                }
            }

            return false;
        }
    }

    fn parse_start_expr(&mut self) -> Option<Box<PtNode>> {
        if let Some(node) = self.parse_int() {
            return Some(node);
        }

        if let Some(node) = self.parse_id() {
            return Some(node);
        }

        if let Some(node) = self.parse_float() {
            return Some(node);
        }

        if let Some(operator) = self.match_op()
            && let Some(prec) = self.is_prefix()
            && self.can_start_expr_at(1)
        {
            let span = self.peek().unwrap().span;
            self.adv();
            let value = self.parse_expr(prec)?;
            return Some(Box::new(PtNode::PreOp {
                operator,
                value,
                span,
            }));
        }

        None
    }

    fn parse_expr(&mut self, min_prec: usize) -> Option<Box<PtNode>> {
        let mut left = self.parse_start_expr()?;

        loop {
            let operator = match self.match_op() {
                Some(operator) => operator,
                None => break,
            };

            let suffix = self.is_suffix().filter(|&prec| prec >= min_prec);
            let infix = self
                .is_infix()
                .filter(|&(prec, _)| prec >= min_prec && self.can_start_expr_at(1));

            if suffix.is_none() && infix.is_none() {
                break;
            }

            let op_prec = infix.map(|(prec, _)| prec).or(suffix).unwrap_or(0);
            let op_assoc = infix.map(|(_, assoc)| assoc).unwrap_or(Assoc::Left);

            let use_infix = if suffix.is_some()
                && infix.is_some()
                && let Some((next_prec, _)) = self.is_infix_at(1)
            {
                op_prec <= next_prec
            } else {
                infix.is_some()
            };

            let span = self.peek().unwrap().span;
            self.adv();

            if use_infix {
                let next_prec = if op_assoc == Assoc::Left {
                    op_prec + 1
                } else {
                    op_prec
                };

                let right = self.parse_expr(next_prec)?;

                left = Box::new(PtNode::InfOp {
                    operator,
                    left,
                    right,
                    span,
                })
            } else {
                left = Box::new(PtNode::SufOp {
                    operator,
                    value: left,
                    span,
                })
            }
        }

        Some(left)
    }

    fn parse_int(&mut self) -> Option<Box<PtNode>> {
        let value = self.match_int()?;
        let span = self.peek().unwrap().span;

        self.adv();

        return Some(Box::new(PtNode::Int { value, span }));
    }

    fn parse_float(&mut self) -> Option<Box<PtNode>> {
        let value = self.match_float()?;
        let span = self.peek().unwrap().span;

        self.adv();

        return Some(Box::new(PtNode::Float { value, span }));
    }

    fn parse_id(&mut self) -> Option<Box<PtNode>> {
        let value = self.match_id()?;
        let span = self.peek().unwrap().span;

        self.adv();

        return Some(Box::new(PtNode::Id { value: value, span }));
    }
}
