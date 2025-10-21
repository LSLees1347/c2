mod preproc;
mod lexer;

use std::env;
use std::process::exit;


fn main()
{
	let args: Vec<String> = env::args().collect();

	if args.len() != 2
	{
		eprintln!("invalid args");
		exit(1);
	}

	let source = &args[1];
	let code = preproc::preproc_run(source);
	if code != 0
	{
		exit(code);
	}

	let input = std::fs::read_to_string("output/preproc")
		.expect("failed to read preproc");

	match lexer::lex(&input)
	{
		Ok(tokens) =>
		{
			for token in tokens
			{
				println!("{:?}", token);
			}
		}
		Err(e) =>
		{
			eprintln!("lexer erros: {}", e);
			exit(1);
		}
	}
}