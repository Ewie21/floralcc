mod code_gen;
mod error;
#[allow(unused_assignments)]
mod lexer;
mod linker;
mod parser;

pub fn compile_c_file(file_name: std::path::PathBuf, exe_name: String, debug: bool) {
    let buff = std::fs::read_to_string(file_name).expect("Source file must exist");
    let (tokens, line_tracker) = lexer::string_to_tokens(&buff).unwrap();
    let node = parser::program(tokens, line_tracker, debug).unwrap();
    node.print(&mut 0);
    let code = code_gen::main(&node, debug);
    println!("{}", code);
    let _ = std::fs::write(exe_name, code);
}
