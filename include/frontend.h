#pragma once

#include "base.h"

struct source_file {
	uint32_t Index;
};

struct source_buffer {
	source_file File;
	char *Data;
	uint32_t Length;
};

source_file CreateFileSource(const char *filename);
source_file CreateDataSource(const char *name, const char *data, uint32_t length);

source_buffer *OpenSourceBuffer(source_file file);
void CloseSourceBuffer(source_buffer *buffer);

struct source_loc {
	source_file SourceFile;
	uint32_t ByteOffset;
	uint32_t ByteLength;
};

enum token_type {

	T_Null = 0,
	T_Error,
	T_End,
	T_PreNewline,

	/*  #   */ T_PreHash,
	/*  ##  */ T_PreDoubleHash,

	/*  +   */ T_Add,
	/*  +=  */ T_AddAssign,
	/*  ++  */ T_Increment,

	/*  -   */ T_Sub,
	/*  -=  */ T_SubAssign,
	/*  --  */ T_Decrement,
	/*  ->  */ T_Arrow,

	/*  *   */ T_Mul,
	/*  *=  */ T_MulAssign,

	/*  /   */ T_Div,
	/*  /=  */ T_DivAssign,

	/*  %   */ T_Mod,
	/*  %=  */ T_ModAssign,

	/*  &   */ T_BitAnd,
	/*  &=  */ T_BitAndAssign,
	/*  &&  */ T_LogicAnd,

	/*  |   */ T_BitOr,
	/*  |=  */ T_BitOrAssign,
	/*  ||  */ T_LogicOr,

	/*  ^   */ T_BitXor,
	/*  ^=  */ T_BitXorAssign,

	/*  =   */ T_Assign,
	/*  ==  */ T_Equal,

	/*  !   */ T_Not,
	/*  !=  */ T_NotEqual,

	/*  <   */ T_Less,
	/*  <=  */ T_LessEq,
	/*  <<  */ T_LeftShift,
	/*  <<= */ T_LeftShiftAssign,

	/*  >   */ T_Greater,
	/*  >=  */ T_GreaterEq,
	/*  >>  */ T_RightShift,
	/*  >>= */ T_RightShiftAssign,

	/*  :   */ T_Colon,
	/*  ::  */ T_DoubleColon,

	/*  \   */ T_PreBackslash,
	/*  .   */ T_Dot,
	/*  ,   */ T_Comma,
	/*  (   */ T_ParenOpen,
	/*  )   */ T_ParenClose,
	/*  {   */ T_BlockOpen,
	/*  }   */ T_BlockClose,
	/*  [   */ T_BracketOpen,
	/*  ]   */ T_BracketClose,
	/*  ;   */ T_Semicolon,
	/*  ?   */ T_Question,

	T_Count,
};

struct token {
	token_type Type;
	ident Ident;
	source_loc Loc;
};

struct tokenizer;

tokenizer *CreateTokenizer(source_file file);
void FreeTokenizer(tokenizer *t);

void Tokenize(tokenizer *t, boa::buf<token> &tokens);
