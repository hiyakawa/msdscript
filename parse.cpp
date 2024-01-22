/**
 * \file parse.cpp
 * \brief Definitions of methods to parse input string
 * \author Laura Zhang
 */

#include "parse.h"
#include "expr.h"
#include <sstream>

PTR(Expr) parse(std::istream& in) {
    PTR(Expr) e = parse_expr(in);
    skip_whitespace(in);

    if (! in.eof()) {
        throw std::runtime_error("invalid input");
    }

    return e;
}

PTR(Expr) parse_str(const std::string& s) {
    std::stringstream ss(s);

    return parse(ss);
}

static PTR(Expr) parse_expr(std::istream& in) {
    PTR(Expr) e = parse_comparg(in);
    skip_whitespace(in);

    if (in.peek() == '=') {
        consume_str(in, "==");
        PTR(Expr) rhs = parse_expr(in);

        return NEW(EqExpr)(e, rhs);
    }

    return e;
}

static PTR(Expr) parse_comparg(std::istream& in) {
    PTR(Expr) e = parse_addend(in);
    skip_whitespace(in);

    if (in.peek() == '+') {
        consume(in, '+');
        PTR(Expr) rhs = parse_comparg(in);

        return NEW(AddExpr)(e, rhs);
    }

    return e;
}

static PTR(Expr) parse_addend(std::istream& in) {
    PTR(Expr) e = parse_multicand(in);
    skip_whitespace(in);

    if (in.peek() == '*') {
        consume(in, '*');
        PTR(Expr) rhs = parse_addend(in);

        return NEW(MultExpr)(e, rhs);
    }

    return e;
}

static PTR(Expr) parse_multicand(std::istream& in) {
    PTR(Expr) e = parse_inner(in);
    skip_whitespace(in);

    while (in.peek() == '(') {
        consume(in, '(');
        PTR(Expr) arg = parse_expr(in);
        skip_whitespace(in);

        if (in.get() != ')') {
            throw std::runtime_error("bad input");
        }
        e = NEW(CallExpr)(e, arg);
    }

    return e;
}

static PTR(Expr) parse_inner(std::istream& in) {
    skip_whitespace(in);
    int c = in.peek();

    if (c == '-' || isdigit(c)) {
        return parse_num(in);
    }
    if (c == '(') {
        consume(in, '(');
        PTR(Expr) e = parse_expr(in);
        skip_whitespace(in);
        c = in.get();

        if (c != ')') {
            throw std::runtime_error("bad input");
        }

        return e;
    }
    else if (isalpha(c)) {
        return parse_var(in);
    }
    else if (c == '_') {
        consume(in, '_');
        std::string keyword = parse_keyword(in);

        if (keyword == "let") {
            return parse_let(in);
        }
        else if (keyword == "true") {
            return NEW(BoolExpr)(true);
        }
        else if (keyword == "false") {
            return NEW(BoolExpr)(false);
        }
        else if (keyword == "if") {
            return parse_if(in);
        }
        else if (keyword == "fun") {
            return parse_fun(in);
        }
        else {
            throw std::runtime_error("bad input");
        }
    }
    else {
        consume(in, c);
        throw std::runtime_error("bad input");
    }
}

static PTR(NumExpr) parse_num(std::istream& in) {
    int n = 0;
    bool negative = false;

    if (in.peek() == '-') {
        negative = true;
        consume(in, '-');

        if (! isdigit(in.peek())) {
            throw std::runtime_error("invalid input");
        }
    }

    while (true) {
        int c = in.peek();
        if (isdigit(c)) {
            consume(in, c);

            if (n > (INT_MAX - (c - '0')) / 10) {
                throw std::runtime_error("int overflow");
            }

            n = 10 * n + (c - '0');
        }
        else {
            break;
        }
    }

    if (negative) {
        n = -n;
    }

    return NEW(NumExpr)(n);
}

static PTR(VarExpr) parse_var(std::istream& in) {
    std::string s = "";

    while (true) {
        int c = in.peek();

        if (isalpha(c)) {
            consume(in, c);
            s += c;
        }
        else {
            break;
        }
    }

    return NEW(VarExpr)(s);
}

static PTR(LetExpr) parse_let(std::istream& in) {
    skip_whitespace(in);
    std::string lhs = parse_var(in)->var_;
    skip_whitespace(in);

    if (in.peek() == '=') {
        consume(in, '=');
    }
    else {
        throw std::runtime_error("bad input");
    }

    PTR(Expr) rhs = parse_expr(in);
    skip_whitespace(in);

    if (parse_keyword(in) != "_in") {
        throw std::runtime_error("bad input");
    }

    PTR(Expr) body = parse_expr(in);

    return NEW(LetExpr)(lhs, rhs, body);
}

static PTR(IfExpr) parse_if(std::istream& in) {
    PTR(Expr) condition = parse_expr(in);
    skip_whitespace(in);

    if (parse_keyword(in) != "_then") {
        throw std::runtime_error("bad input");
    }

    PTR(Expr) then_expr = parse_expr(in);
    skip_whitespace(in);

    if (parse_keyword(in) != "_else") {
        throw std::runtime_error("bad input");
    }

    PTR(Expr) else_expr = parse_expr(in);

    return NEW(IfExpr)(condition, then_expr, else_expr);
}

static PTR(FunExpr) parse_fun(std::istream& in) {
    skip_whitespace(in);

    if (in.peek() == '(') {
        consume(in, '(');
    }
    else {
        throw std::runtime_error("bad input");
    }

    skip_whitespace(in);
    PTR(VarExpr) arg = parse_var(in);
    skip_whitespace(in);

    if (in.peek() == ')') {
        consume(in, ')');
    }
    else {
        throw std::runtime_error("bad input");
    }

    PTR(Expr) body = parse_expr(in);

    return NEW(FunExpr)(arg, body);
}

static std::string parse_keyword(std::istream& in) {
    std::string s = "";

    while (true) {
        int c = in.peek();

        if (isalpha(c) || c == '_') {
            consume(in, c);
            s += c;
        }
        else {
            break;
        }
    }

    return s;
}

static void skip_whitespace(std::istream& in) {
    while (true) {
        int c = in.peek();

        if (! isspace(c)) {
            break;
        }

        consume(in, c);
    }
}

static void consume(std::istream& in, int expect) {
    int c = in.get();
    
    if (c != expect) {
        throw std::runtime_error("consume error");
    }
}

static void consume_str(std::istream& in, std::string expect) {
    for (int i = 0; i < expect.length(); ++i) {
        if (in.peek() == expect[i]) {
            consume(in, expect[i]);
        }
        else {
            throw std::runtime_error("consume_str error");
        }
    }
}