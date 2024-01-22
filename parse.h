/**
 * \file parse.h
 * \brief Declarations of methods to parse input string
 * \author Laura Zhang
 */

#pragma once

#include "pointer.h"

class Expr;
class LetExpr;
class NumExpr;
class VarExpr;
class IfExpr;
class FunExpr;

PTR(Expr)           parse(std::istream& in);
PTR(Expr)           parse_str(const std::string& s);
static PTR(Expr)    parse_expr(std::istream& in);
static PTR(Expr)    parse_comparg(std::istream& in);
static PTR(Expr)    parse_addend(std::istream& in);
static PTR(Expr)    parse_multicand(std::istream& in);
static PTR(Expr)    parse_inner(std::istream& in);
static PTR(NumExpr) parse_num(std::istream& in);
static PTR(VarExpr) parse_var(std::istream& in);
static PTR(LetExpr) parse_let(std::istream& in);
static PTR(IfExpr)  parse_if(std::istream& in);
static PTR(FunExpr) parse_fun(std::istream& in);
static std::string  parse_keyword(std::istream& in);
static void         skip_whitespace(std::istream& in);
static void         consume(std::istream& in, int expect);
static void         consume_str(std::istream& in, std::string expect);
