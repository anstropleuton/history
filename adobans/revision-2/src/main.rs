use std::rc::Rc;

use adobans::{lexer::Lexer, operator::OpTable, parser::Parser, source_code::SourceCode};

fn main() {
    let source_code =
        Rc::new(SourceCode::from_filename("program_1.adb").expect("Failed to open file"));

    let mut lexer = Lexer::new(source_code.clone());

    lexer.lex();

    for token in &lexer.tokens {
        dbg!(token);
    }

    for report in &lexer.reports {
        report.pretty_print(&source_code);
    }

    let mut parser = Parser::new(lexer.tokens.into(), OpTable::default().into());

    parser.parse();

    dbg!(&parser.tree);

    for report in &parser.reports {
        report.pretty_print(&source_code);
    }
}
