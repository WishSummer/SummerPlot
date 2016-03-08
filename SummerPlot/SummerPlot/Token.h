#pragma once

#include <string>

using std::wstring;

enum class Tokens
{
    LeftBracket,
    RightBracket,
    Comma,
    Operator,
    Operand,
    Symbol,
    Function,
    FunctionComma,
};

enum class Operators
{
    Add,
    Multiply,
    Divide,
    Minus,
    Negative,
    Power,
};

class Token
{
public:
    double opnd;
    Operators op;
    wstring str;
    Tokens type;
    bool unknown = false;

    Token(Tokens _type)
    {
        type = _type;
    }

    Token(double v)
    {
        type = Tokens::Operand;
        opnd = v;
    }

    Token(Operators _op)
    {
        type = Tokens::Operator;
        op = _op;
    }

    Token(Tokens _type, wstring _str)
    {
        type = _type;
        str = _str;
    }

    inline bool is(Tokens _type) const
    {
        return (type == _type);
    }

    inline bool isBinary() const
    {
        return (this->op != Operators::Power);
    }
};
