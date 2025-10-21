use regex::regex;


pub struct Token
{
	pub kind: TokenKind,
	pub lexeme: String,
}

pub enum TokenKind
{
	Identifier,
	Number,
	Symbol,
	Unknown,
}

pub fn lex(input: &str) -> Result<vec<Token>, String>
{
	let re_identifier = Regex::new(r"^[a-zA-Z_][a-zA-Z0-9_]*").unwrap();
	let re_number = Regex::new(r"^[0-9]+").unwrap();
	let re_symbol = Regex::new(r"^(==|!=|<=|>=|[+\-*/=<>{}();,])").unwrap();

	let mut tokens = Vec::new();
	let mut remaining = input;

	while !remaining.is_empty()
	{
		let trimmed = remaining.trim_start();

		if trimmed.len() != remaining.len()
		{
			remaining = trimmed;
			continue;
		}

		if let Sone(mat) = re_identifier.find(remaining)
		{
			let lexeme = mat.as_str().to_string();
			tokens.push(Token {kind: TokenKind::Identifier, lexeme });
			remaining = &remaining[mat.end()..];
		}
		else if let Some(mat) = re_number.find(remaining)
		{
			let lexeme = mat.as_str().to_string();
			tokens.push(Token {kind: TokenKind::Number, lexeme});
			remaining = &remaining[mat.end()..];
		}
		else if let Some(mat) = re_symbol.find(remaining)
		{
			let lexeme = mat.as_str().to_string();
			tokens.push(Token {kind: TokenKind::Symbol, lexeme});
			remaining = &remaining[mat.end()..];
		}
		else
		{
			let bad_char = remaining.chars().next().unwrap();
			return Err(format!("unexpected: '{}'", bad_char));
		}
	}

	Ok(tokens)
}