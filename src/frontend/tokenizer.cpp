#include <frontend.h>

struct tokenizer {
	source_file File;
	source_buffer *Buffer;
	const char *Position;
};

tokenizer *CreateTokenizer(source_file file)
{
	auto t = boa::make<tokenizer>();
	t->File = file;
	t->Buffer = OpenSourceBuffer(file);
}

void FreeTokenizer(tokenizer *t)
{
	CloseSourceBuffer(t->Buffer);
}

static ident g_TokenTypeIdentifier[T_Count];

void InitializeTokenizer()
{
	g_TokenTypeIdentifier[T_Null] = InternIdent("(null)");
	g_TokenTypeIdentifier[T_Error] = InternIdent("(error)");
	g_TokenTypeIdentifier[T_End] = InternIdent("(end)");
	g_TokenTypeIdentifier[T_PreNewline] = InternIdent("(newline)");

	g_TokenTypeIdentifier[T_Plus] = InternIdent("+");
	g_TokenTypeIdentifier[T_Minus] = InternIdent("-");
	g_TokenTypeIdentifier[T_Star] = InternIdent("*");
	g_TokenTypeIdentifier[T_Slash] = InternIdent("/");
	g_TokenTypeIdentifier[T_PlusEq] = InternIdent("+=");
	g_TokenTypeIdentifier[T_MinusEq] = InternIdent("-=");
	g_TokenTypeIdentifier[T_StarEq] = InternIdent("*=");
	g_TokenTypeIdentifier[T_SlashEq] = InternIdent("/=");
	g_TokenTypeIdentifier[T_DoublePlus] = InternIdent("++");
	g_TokenTypeIdentifier[T_DoubleMinus] = InternIdent("--");
	g_TokenTypeIdentifier[T_Arrow] = InternIdent("->");
	g_TokenTypeIdentifier[T_PreHash] = InternIdent("#");
	g_TokenTypeIdentifier[T_PreDoubleHash] = InternIdent("##");
	g_TokenTypeIdentifier[T_ParenOpen] = InternIdent("(");
	g_TokenTypeIdentifier[T_ParenClose] = InternIdent(")");
	g_TokenTypeIdentifier[T_BlockOpen] = InternIdent("{");
	g_TokenTypeIdentifier[T_BlockClose] = InternIdent("}");
	g_TokenTypeIdentifier[T_BracketOpen] = InternIdent("[");
	g_TokenTypeIdentifier[T_BracketClose] = InternIdent("]");
}

#define IDENT_BEGIN_CHAR \
	     'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': \
	case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': \
	case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': \
	case 'y': case 'z': case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': \
	case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': \
	case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': \
	case 'W': case 'X': case 'Y': case 'Z': case '_'

#define DIGIT_CHAR \
	     '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': \
	case '8': case '9'

void Tokenize(tokenizer *t, boa::buf<token> &tokens)
{
	const char *pos = t->Position;
	char c;
	token *token;
	while (token = tokens.try_push()) {
		c = *pos++;

	again:
		while (c == ' ' || c == '\t') {
			c = *pos++;
		}

		token_type type;

		char la = *pos;
		switch (c) {
		case IDENT_BEGIN_CHAR:

			break;

		case DIGIT_CHAR:

			break;

		case '"':
			break;

		case '\'':
			break;

		case '\r':
			pos++;
			if (la == '\n') {
				type = T_PreNewline;
				goto ident_from_token;
			} else {
				c = la;
				goto again;
			}

		case '\n':
			type = T_PreNewline;
			goto ident_from_token;

		case ';': type = T_Semicolon; goto ident_from_token;
		case ':': type = T_Colon; goto ident_from_token;
		case '?': type = T_Question; goto ident_from_token;
		case '(': type = T_ParenOpen; goto ident_from_token;
		case ')': type = T_ParenClose; goto ident_from_token;
		case '[': type = T_BracketOpen; goto ident_from_token;
		case ']': type = T_BracketClose; goto ident_from_token;
		case '{': type = T_BlockOpen; goto ident_from_token;
		case '}': type = T_BlockClose; goto ident_from_token;

		case '\0':
			pos--;
			type = T_End;
			goto ident_from_token;

		case '+':
			if (la == '+') { pos++; type = T_DoublePlus; }
			else if (la == '=') { pos++; type = T_PlusEq; }
			else { type = T_Plus; }
			goto ident_from_token;

		case '-':
			if (la == '-') { pos++; type = T_DoubleMinus; }
			else if (la == '=') { pos++; type = T_MinusEq; }
			else if (la == '>') { pos++; type = T_Arrow; }
			else { type = T_Minus; }
			goto ident_from_token;

		case '*':
			if (la == '=') { pos++; type = T_StarEq; }
			else { type = T_Star; }
			goto ident_from_token;

		case '/':
			if (la == '/') {
				do {
					c = *++pos;
				} while (!(c == '\n' || c == '\0'));
				if (c != '\0') pos++;
				goto again;
			} else if (la == '*') {
				do {
					c = *++pos;
				} while (!((c == '*' && *pos == '/') || c == '\0'));
				if (c != '\0') pos++;
				goto again;
			} else if (la == '=') { pos++; type = T_SlashEq; }
			else { type = T_Slash; }
			goto ident_from_token;

		case '=':
			if (la == '=') { pos++; type = T_EqualEq; }
			else { type = T_Equal; }
			goto ident_from_token;

		case '!':
			if (la == '=') { pos++; type = T_NotEq; }
			else { type = T_Not; }
			goto ident_from_token;

		ident_from_token:
			token->Ident = g_TokenTypeIdentifier[type];
			break;

		} 


	}

	t->Position = pos;
}
