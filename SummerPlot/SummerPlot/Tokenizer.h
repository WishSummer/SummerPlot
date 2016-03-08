#pragma once

#include <vector>
#include <sstream>

#include "Token.h"

using std::wstring;
using std::wistringstream;
using std::vector;

class Tokenizer
{
public:
    static vector<Token> tokenize(wstring & input)
    {
        vector<Token> tokens;
        wistringstream in(input);
        wchar_t token;

        while (in.get(token))
        {
            if (token == L' ' || token == L'\t' || token == L'\r' || token == L'\n') continue;
            if (token == L'.' || iswdigit(token))
            {
                in.putback(token);

                double v;
                in >> v;
                Token newToken(v);
                tokens.push_back(newToken);
                continue;
            }
            if (iswalpha(token))
            {
                wstring str;
                while (iswalpha(token))
                {
                    str.append(1, token);
                    if (!in.get(token)) break;
                }
                if (token != 0) in.putback(token);
                Token newToken(Tokens::Symbol, str);
                tokens.push_back(newToken);
                continue;
            }
            if (token == L'(')
            {
                Token newToken(Tokens::LeftBracket);
                tokens.push_back(newToken);
                continue;
            }
            if (token == L')')
            {
                Token newToken(Tokens::RightBracket);
                tokens.push_back(newToken);
                continue;
            }
            if (token == L'+')
            {
                Token newToken(Operators::Add);
                tokens.push_back(newToken);
                continue;
            }
            if (token == L'-')
            {
                Token newToken(Operators::Minus);
                tokens.push_back(newToken);
                continue;
            }
            if (token == L'*')
            {
                Token newToken(Operators::Multiply);
                tokens.push_back(newToken);
                continue;
            }
            if (token == L'/')
            {
                Token newToken(Operators::Divide);
                tokens.push_back(newToken);
                continue;
            }
            if (token == L'^')
            {
                Token newToken(Operators::Power);
                tokens.push_back(newToken);
                continue;
            }
            if (token == L',')
            {
                Token newToken(Tokens::FunctionComma);
                tokens.push_back(newToken);
                continue;
            }
        }

        for (vector<Token>::iterator it = tokens.begin(); it != tokens.end(); it++)
        {
            if (it->is(Tokens::Symbol))
            {
                if (std::next(it) != tokens.end() && std::next(it)->is(Tokens::LeftBracket))
                {
                    it->type = Tokens::Function;
                }
            }
        }

        for (vector<Token>::iterator it = tokens.begin(); it != tokens.end(); ++it)
        {
            if (it->is(Tokens::Symbol) && (it->str == L"x" || it->str == L"X"))
            {
                it->type = Tokens::Operand;
                it->unknown = true;
                it->str = L"x";
            }
        }

        return tokens;
    }
};
