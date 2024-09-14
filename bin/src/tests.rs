use std::{fs, path::Path, process::Command};
//use test_each_file::test_each_path;
use test_each::path;

// test_each_path!{ for ["rh", "out"] in "./tests/core" => test };
//
// fn test([input, output]: [&Path; 2]) {
//     let name = input
//         .file_name()
//         .unwrap()
//         .to_str()
//         .unwrap()
//         .split(".")
//         .collect::<Vec<&str>>()[0];
//     let expected_output = fs::read_to_string(output).unwrap().into_bytes();
//
//     Command::new("bash")
//         .arg("../test_rh.sh")
//         .arg(format!("{}", name))
//         .spawn()
//         .expect("Assembling failed");
//
//     let generated_output = Command::new(format!("./gen/core/{}", name))
//         .output()
//         .unwrap()
//         .stdout;
//     assert_eq!(generated_output, expected_output);
// }
fn test([input, output]: [&Path; 2]) {
    // Make assertions on the path of `input` and `output` here.
}
