use error::RhErr;
use parser::TokenNode;

mod code_gen;
mod error;
#[allow(unused_assignments)]
mod lexer;
mod linker;
mod parser;

pub fn parse_c_buffer(contents: String, debug: bool) -> Result<parser::TokenNode, RhErr> {
    let (tokens, line_tracker) = lexer::string_to_tokens(&contents).unwrap();
    Ok(parser::program(tokens, line_tracker, debug)?)
}

pub fn parse_c_file(
    file_name: std::path::PathBuf,
    debug: bool,
) -> Result<parser::TokenNode, RhErr> {
    let buff = std::fs::read_to_string(file_name).expect("Source file must exist");
    Ok(parse_c_buffer(buff, debug)?)
}

pub fn generate_asm(node: TokenNode, debug: bool) -> String {
    code_gen::main(&node, debug)
}

pub fn compile_c_buffer(contents: String, debug: bool) -> Result<String, RhErr> {
    let node = parse_c_buffer(contents, debug)?;
    if debug {
        node.print(&mut 0);
    }
    let code = generate_asm(node, debug);
    if debug {
        println!("{}", code);
    }
    Ok(code)
}

pub fn compile_c_file(file_name: std::path::PathBuf, exe_name: String, debug: bool) {
    let node = parse_c_file(file_name, debug).expect("Parsing Failed");
    if debug {
        node.print(&mut 0);
    }
    let code = code_gen::main(&node, debug);
    if debug {
        println!("{}", code);
    }
    let _ = std::fs::write(exe_name, code);
}
