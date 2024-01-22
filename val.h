/**
 * \file val.h
 * \brief Declarations of methods in Val base class
 * \author Laura Zhang
 */

#pragma once

#include "pointer.h"
#include "parse.h"
#include <string>

class Expr;
class VarExpr;
class Env;

class Val {
public:
    virtual PTR(Expr)   to_expr() = 0;
    virtual bool        equals(PTR(Val) rhs) = 0;
    virtual PTR(Val)    add_to(PTR(Val) rhs) = 0;
    virtual PTR(Val)    mult_with(PTR(Val) rhs) = 0;
    virtual std::string to_string() = 0;
    virtual bool        is_true() = 0;
    virtual PTR(Val)    call(PTR(Val) arg) = 0;
};

class NumVal : public Val {
public:
    int val_;

    NumVal(int val);

    PTR(Expr)   to_expr() override;
    bool        equals(PTR(Val) rhs) override;
    PTR(Val)    add_to(PTR(Val) rhs) override;
    PTR(Val)    mult_with(PTR(Val) rhs) override;
    std::string to_string() override;
    bool        is_true() override;
    PTR(Val)    call(PTR(Val) arg) override;
};

class BoolVal : public Val {
public:
    bool bool_val_;

    BoolVal(bool bool_val);

    PTR(Expr)   to_expr() override;
    bool        equals(PTR(Val) rhs) override;
    PTR(Val)    add_to(PTR(Val) rhs) override;
    PTR(Val)    mult_with(PTR(Val) rhs) override;
    std::string to_string() override;
    bool        is_true() override;
    PTR(Val)    call(PTR(Val) arg) override;
};

class FunVal : public Val {
public:
    PTR(VarExpr) arg_;
    PTR(Expr)    body_;
    PTR(Env)     env_;

    FunVal(PTR(VarExpr) arg, PTR(Expr) body);

    PTR(Expr)   to_expr() override;
    bool        equals(PTR(Val) rhs) override;
    PTR(Val)    add_to(PTR(Val) rhs) override;
    PTR(Val)    mult_with(PTR(Val) rhs) override;
    std::string to_string() override;
    bool        is_true() override;
    PTR(Val)    call(PTR(Val) arg) override;
};
