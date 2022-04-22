#include <cstdio>
#include <string>
#include <sstream>
#include <cctype>
#include <unistd.h>
#include <vector>

std::string test = R"(
  x : int := 1;
  y : @int;
  z : float; // comment
  // another comment
)";


//
// @Parser
//

// All our token definitions
enum Token_Def {
  tok_eof = -1,
  tok_identifier = 1,
  tok_ignore = 2,
  tok_type = 3,
  tok_type_ptr = 4,
  tok_defin = 5,
  tok_assignment = 6,
  tok_number = 7,
  tok_string = 8,
  tok_expr_end = 9,
  tok_comment = 10
};

// Save token number and string
struct Token {
  int tok_def;
  std::string tok_str;
};

// @? might not use this
#define STATE_PROC 1

char get_char(std::stringstream *ifs) {
  char c;
  ifs->get(c);
  if(ifs->eof()) return EOF;
  return c;
}

void eat_space(std::stringstream *ifs, int *lastChar) {
  while (isspace(*lastChar))
    *lastChar = get_char(ifs);
}

void eat_comment(std::stringstream *ifs, int *lastChar) {
  while ((*lastChar != '\r') && (*lastChar != '\n'))
    *lastChar = get_char(ifs);
}

int get_identifier(std::stringstream *ifs, Token *token, int *lastChar) {
  std::string token_str;

  if (isalpha(*lastChar)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
    token_str = *lastChar;
    while (isalnum((*lastChar = get_char(ifs))))
      token_str += *lastChar;

    token->tok_def = tok_identifier;
    token->tok_str = token_str;
    return tok_identifier;
  }
  return 0;
}

int get_type(std::stringstream *ifs, Token *token, int *lastChar) {
  int result;

  token->tok_def = tok_type;
  if(*lastChar == '@') {
    token->tok_def = tok_type_ptr;
    *lastChar = get_char(ifs);
  }

  Token tmp;
  //@Todo: check result;
  result = get_identifier(ifs, &tmp, lastChar);
  token->tok_str = tmp.tok_str;

    return token->tok_def;
 }

int get_number(std::stringstream *ifs, Token *token, int *lastChar) {
  std::string token_str;

  if (isdigit(*lastChar)) { // digit: [0-9]*
    token_str = *lastChar;
    while (isdigit((*lastChar = get_char(ifs))))
      token_str += *lastChar;

    token->tok_def = tok_number;
    token->tok_str = token_str;
    return tok_number;
  }
  return 0;
}

int get_tok(std::stringstream *ifs, Token *token, int *lastChar) {
  int result;

  eat_space(ifs, lastChar);

  // Check for : or :=
  if(*lastChar == ':') {
    *lastChar = get_char(ifs);
    if(*lastChar == '=') {
      token->tok_def = tok_assignment;
      token->tok_str = ":=";
      *lastChar = get_char(ifs);
      return tok_assignment;
    } else {
      eat_space(ifs, lastChar);
      return get_type(ifs, token, lastChar);
    }
  }

  if(*lastChar == '/') {
    *lastChar = get_char(ifs);
    if(*lastChar == '/') {
      eat_comment(ifs, lastChar);
      return tok_ignore;
    } else {
      //@Todo: division /
    }
  }

    if((result = get_identifier(ifs, token, lastChar))) return 0;

    if((result = get_number(ifs, token, lastChar))) return 0;

    switch(*lastChar) {
    case ':':
    case '/':
      return tok_ignore;
      break;
    case ';':
      token->tok_def = tok_expr_end;
      token->tok_str = *lastChar;
      *lastChar = get_char(ifs);
      return *lastChar;
      break;
    case EOF:
      return tok_eof;
      break;
    default:
      return *lastChar;
    }
}


int main(int argc, char *argv[]) {
  int lastChar = ' ', tok_ident = 0;
  std::stringstream ifs(test);
  Token *token;
  std::vector<Token *> token_seq;

  token = new(Token);
  while(tok_ident != EOF) {
    tok_ident = get_tok(&ifs, token, &lastChar);
    if(!((tok_ident == tok_ignore) || (tok_ident == tok_eof))) {
      token_seq.push_back(token);
      token = new(Token);
    }
  }

  for (Token *t : token_seq) {
    printf("%s %d\n", t->tok_str.c_str(), t->tok_def);
    delete(t);
  }

  return 0;
}
