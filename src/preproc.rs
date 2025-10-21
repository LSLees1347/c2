use std::fs;
use std::process::Command;
use std::path::{Path, PathBuf};

pub fn preproc_run(source: &str) -> i32
{
	let output: PathBuf = Path::new("output").join("preproc.i");

	if !Path::new(source).exists()
	{
		eprintln!("invalid args");
		return 1;
	}

	if let Some(p) = output.parent()
	{
		if let Err(e) = fs::create_dir_all(p)
		{
			eprintln!("mkdir fail - {}", e);
			return 1;
		}
	}
	else
	{
		eprintln!("no parent dir");
		return 1;
	}

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
		return code;
	}
	else
	{
		eprintln!("process terminated");
		return 1;
	}
}