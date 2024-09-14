#[cfg(test)]
mod tests;
use clap::{ArgAction, Parser};
use rhododendron::compile_c_file;

#[derive(Parser)]
#[command(author, version, about, long_about = None)]
struct Args {
    path: std::path::PathBuf,
    name: String,
    #[clap(long, short, action=ArgAction::SetFalse)]
    debug: bool,
}

pub fn main() {
    let args = Args::parse();
    let file_name: std::path::PathBuf = args.path;
    let exe_name: String = args.name;
    compile_c_file(file_name, exe_name, args.debug);
}
