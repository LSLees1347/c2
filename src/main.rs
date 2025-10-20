use std::env;
use std::process::Command;
use std::process::exit;
use std::fs;
use std::path::{Path, PathBuf};


fn main()
{
	let args: Vec<String> = env::args().collect();

	if args.len() != 2
	{
		eprintln!("invalid args");
		exit(1);
	}

	let source = &args[1];
	let output: PathBuf = Path::new("output").join("preproc.i");

	if !Path::new(source).exists()
	{
		eprintln!("invalid source");
		exit(1);
	}

	if let Some(p) = output.parent()
	{
		if let Err(e) = fs::create_dir_all(p)
		{
			eprintln!("mkdir fail - {}", e);
			exit(1);
		}
	}
	else
	{
		exit(1);
	}

	// run pre proc
	let status = Command::new("gcc")
		.arg("-E")
		.arg("-P")
		.arg(source)
		.arg("-o")
		.arg(&output)
		.status()
		.expect("gcc error");

	if let Some(code) = status.code()
	{
		exit(code);
	}
	else
	{
		eprintln!("process terminated");
		exit(1);
	}
}