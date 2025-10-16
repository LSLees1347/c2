use std::env;
use std::process::Command;
use std::process::exit;


fn main()
{
    let args: Vec<String> = env::args().collect();
    
    if args.len() != 3
    {
        eprintln!("INVALID: <source.c>");
        exit(1);
    }

    let source_file = &args[1];
    let output_file = "PREPROCESSED_FILE";

    // run gcc pre proc
    let status = Command::new("gcc")
        .args(&["-E", "-P", source_file, "-o", output_file])
        .status()
        .expect("Failed to run gcc");

    if let Some(code) = status.code()
    {
        exit(code);
    }
    else
    {
        eprintln!("Process terminated by signal");
        exit(1);
    }
}