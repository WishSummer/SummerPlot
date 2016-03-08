#pragma once

#include <vector>
#include <stack>

#include "Token.h"
#include "Tokenizer.h"

using std::vector;
using std::stack;

class Parser
{
protected:

    static int op_preced(const Token & token)
    {
        switch (token.op)
        {
        case Operators::Divide:
        case Operators::Multiply:
            return 3;
        case Operators::Add:
        case Operators::Minus:
            return 2;
        }
        return 0;
    }

    static unsigned int op_arg_count(const Token & token)
    {
        switch (token.op)
        {
        case Operators::Add:
        case Operators::Divide:
        case Operators::Minus:
        case Operators::Multiply:
            return 2;
        }
        return 0;
    }

    static bool op_left_assoc(const Token & token)
    {
        switch (token.op)
        {
        case Operators::Add:
        case Operators::Divide:
        case Operators::Minus:
        case Operators::Multiply:
            return true;
        }
        return false;
    }

public:

    static vector<Token> parse(vector<Token> tokens)
    {
        stack<Token> sk;
        vector<Token> output;

        for (Token & token : tokens)
        {
            if (token.is(Tokens::Operand))
            {
                output.push_back(token);
                continue;
            }
            if (token.is(Tokens::Function))
            {
                sk.push(token);
                continue;
            }
            if (token.is(Tokens::FunctionComma))
            {
                bool pe = false;
                while (!sk.empty())
                {
                    Token sc = sk.top();
                    if (sc.is(Tokens::LeftBracket))
                    {
                        pe = true;
                        break;
                    }
                    else
                    {
                        output.push_back(sc);
                        sk.pop();
                    }
                }
                if (!pe)
                {
                    throw "Missing bracket";
                }
                continue;
            }
            if (token.is(Tokens::Operator))
            {
                while (!sk.empty())
                {
                    Token sc = sk.top();
                    if (sc.is(Tokens::Operator) &&
                        (op_left_assoc(sc) && op_preced(token) < op_preced(sc) ||
                            !op_left_assoc(sc) && op_preced(token) < op_preced(sc)
                            ))
                    {
                        output.push_back(sc);
                        sk.pop();
                    }
                    else
                    {
                        break;
                    }
                }
                sk.push(token);
                continue;
            }
            if (token.is(Tokens::LeftBracket))
            {
                sk.push(token);
                continue;
            }
            if (token.is(Tokens::RightBracket))
            {
                bool pe = false;
                while (!sk.empty())
                {
                    Token sc = sk.top();
                    if (sc.is(Tokens::LeftBracket))
                    {
                        pe = true;
                        break;
                    }
                    else
                    {
                        output.push_back(sc);
                        sk.pop();
                    }
                }
                if (!pe)
                {
                    throw "Missing bracket";
                }
                sk.pop();
                if (!sk.empty())
                {
                    Token sc = sk.top();
                    if (sc.is(Tokens::Function))
                    {
                        output.push_back(sc);
                        sk.pop();
                    }
                }
            }
        }
        while (!sk.empty())
        {
            Token sc = sk.top();
            if (sc.is(Tokens::LeftBracket) || sc.is(Tokens::RightBracket))
            {
                throw "Missing bracket";
            }
            output.push_back(sc);
            sk.pop();
        }

        return output;
    }
};
