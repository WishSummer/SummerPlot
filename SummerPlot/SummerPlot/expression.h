#pragma once

#include <vector>
#include <string>
#include <cmath>

#include <windows.h>

#include "Parser.h"
#include "Tokenizer.h"

#include "Token.h"

class Expression
{
private:
    std::vector<Token> tokens;
    double opndStack[100];
    int sp = 0;
public:
    HPEN hPen = NULL;
    DWORD color;
    bool ok;

    Expression(std::wstring exp, DWORD _color)
    {
        try
        {
            tokens = Tokenizer::tokenize(exp);
            tokens = Parser::parse(tokens);
            execute(0);
            ok = true;
        }
        catch (...)
        {
            ok = false;
        }
        setColor(_color);
    }

    ~Expression()
    {
        if (hPen)
        {
            DeleteObject(hPen);
            hPen = NULL;
        }
    }

    void setColor(DWORD _color)
    {
        color = _color;

        if (hPen)
        {
            DeleteObject(hPen);
            hPen = NULL;
        }

        hPen = CreatePen(PS_SOLID, 2, color);
    }

    double execute(double x)
    {
        sp = 0;
        for (Token & token : tokens)
        {
            if (token.is(Tokens::Operand))
            {
                if (token.unknown)
                {
                    opndStack[sp++] = x;
                    continue;
                }
                opndStack[sp++] = token.opnd;
            }
            else if (token.is(Tokens::Operator))
            {
                switch (token.op)
                {
                case Operators::Add:
                    opndStack[sp - 2] = opndStack[sp - 2] + opndStack[sp - 1];
                    sp--;
                    break;
                case Operators::Minus:
                    opndStack[sp - 2] = opndStack[sp - 2] - opndStack[sp - 1];
                    sp--;
                    break;
                case Operators::Multiply:
                    opndStack[sp - 2] = opndStack[sp - 2] * opndStack[sp - 1];
                    sp--;
                    break;
                case Operators::Divide:
                    opndStack[sp - 2] = opndStack[sp - 2] / opndStack[sp - 1];
                    sp--;
                    break;
                case Operators::Negative:
                    opndStack[sp - 1] = -opndStack[sp - 1];
                    break;
                case Operators::Power:
                    opndStack[sp - 2] = std::pow(opndStack[sp - 2], opndStack[sp - 1]);
                    sp--;
                    break;
                }
            }
            else if (token.is(Tokens::Function))
            {
                if (token.str == L"sin")
                {
                    opndStack[sp - 1] = std::sin(opndStack[sp - 1]);
                }
                else if (token.str == L"cos")
                {
                    opndStack[sp - 1] = std::cos(opndStack[sp - 1]);
                }
                else if (token.str == L"sqrt")
                {
                    opndStack[sp - 1] = std::sqrt(opndStack[sp - 1]);
                }
                else if (token.str == L"tan")
                {
                    opndStack[sp - 1] = std::tan(opndStack[sp - 1]);
                }
                else if (token.str == L"cot")
                {
                    opndStack[sp - 1] = 1.0 / std::tan(opndStack[sp - 1]);
                }
                else if (token.str == L"log2")
                {
                    opndStack[sp - 1] = std::log2(opndStack[sp - 1]);
                }
                else if (token.str == L"ln")
                {
                    opndStack[sp - 1] = std::log(opndStack[sp - 1]);
                }
                else if (token.str == L"lg")
                {
                    opndStack[sp - 1] = std::log10(opndStack[sp - 1]);
                }
            }
        }

        if (sp > 1) throw "Unmatch bracket";

        return opndStack[0];
    }
};