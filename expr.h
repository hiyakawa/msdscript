/**
 * \file expr.h
 * \brief Declarations of methods in Expr and its child classes
 * \author Laura Zhang
 */

#pragma once

#include "pointer.h"
#include "env.h"
#include <string>
#include <ostream>

typedef enum {
    prec_none,
    prec_add,
    prec_mult,
} precedence_t;

class Val;

class Expr {
public:
    virtual bool      equals(PTR(Expr) e) = 0;
    virtual PTR(Val)  interp(PTR(Env)) = 0;
//    virtual bool      has_variable() = 0;
//    virtual PTR(Expr) subst(std::string parameter, PTR(Expr) e) = 0;
    virtual void      print(std::ostream& ot) = 0;
    void              pretty_print(std::ostream& ot);
    virtual void      pretty_print_at(std::ostream& , precedence_t, bool, bool, std::streampos&) = 0;
    std::string       to_string();
    std::string       to_pretty_string();
};

class NumExpr : public Expr {
public:
    int val_;

    NumExpr(int val);
    bool      equals(PTR(Expr) rhs) override;
    PTR(Val)  interp(PTR(Env)) override;
//    bool      has_variable();
//    PTR(Expr) subst(std::string parameter, PTR(Expr) e);
    void      print(std::ostream& ot) override;

private:
    void      pretty_print_at(std::ostream& , precedence_t, bool, bool, std::streampos&) override;
};

class AddExpr : public Expr {
public:
    PTR(Expr) lhs_;
    PTR(Expr) rhs_;

    AddExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    bool      equals(PTR(Expr) rhs) override;
    PTR(Val)  interp(PTR(Env)) override;
//    bool      has_variable();
//    PTR(Expr) subst(std::string parameter, PTR(Expr) e);
    void      print(std::ostream& ot) override;

private:
    void      pretty_print_at(std::ostream& , precedence_t, bool, bool, std::streampos&) override;
};

class MultExpr : public Expr {
public:
    PTR(Expr) lhs_;
    PTR(Expr) rhs_;

    MultExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    bool      equals(PTR(Expr) rhs) override;
    PTR(Val)  interp(PTR(Env)) override;
//    bool      has_variable();
//    PTR(Expr) subst(std::string parameter, PTR(Expr) e);
    void      print(std::ostream& ot) override;

private:
    void      pretty_print_at(std::ostream& , precedence_t, bool, bool, std::streampos&) override;
};

class VarExpr : public Expr {
public:
    std::string var_;

    VarExpr(std::string var);
    bool      equals(PTR(Expr) rhs) override;
    PTR(Val)  interp(PTR(Env)) override;
//    bool      has_variable();
//    PTR(Expr) subst(std::string parameter, PTR(Expr) e);
    void      print(std::ostream& ot) override;

private:
    void      pretty_print_at(std::ostream& , precedence_t, bool, bool, std::streampos&) override;
};

class LetExpr : public Expr {
public:
    std::string var_;
    PTR(Expr) rhs_;
    PTR(Expr) body_;

    LetExpr(std::string var, PTR(Expr) rhs, PTR(Expr) body);
    bool      equals(PTR(Expr) rhs) override;
    PTR(Val)  interp(PTR(Env)) override;
//    bool      has_variable();
//    PTR(Expr) subst(std::string parameter, PTR(Expr) e);
    void      print(std::ostream& ot) override;

private:
    void      pretty_print_at(std::ostream& , precedence_t, bool, bool, std::streampos&) override;
};

class BoolExpr : public Expr {
public:
    bool var_;

    BoolExpr(bool var);
    bool      equals(PTR(Expr) rhs) override;
    PTR(Val)  interp(PTR(Env)) override;
//    bool      has_variable();
//    PTR(Expr) subst(std::string parameter, PTR(Expr) e);
    void      print(std::ostream& ot) override;

private:
    void      pretty_print_at(std::ostream& , precedence_t, bool, bool, std::streampos&) override;
};

class IfExpr : public Expr {
public:
    PTR(Expr) condition_;
    PTR(Expr) then_;
    PTR(Expr) else_;

    IfExpr(PTR(Expr) condition, PTR(Expr) then_expr, PTR(Expr) else_expr);
    bool      equals(PTR(Expr) rhs) override;
    PTR(Val)  interp(PTR(Env)) override;
//    bool      has_variable();
//    PTR(Expr) subst(std::string parameter, PTR(Expr) e);
    void      print(std::ostream& ot) override;

private:
    void      pretty_print_at(std::ostream& , precedence_t, bool, bool, std::streampos&) override;
};

class EqExpr : public Expr {
public:
    PTR(Expr) lhs_;
    PTR(Expr) rhs_;

    EqExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    bool      equals(PTR(Expr) rhs) override;
    PTR(Val)  interp(PTR(Env)) override;
//    bool      has_variable();
//    PTR(Expr) subst(std::string parameter, PTR(Expr) e);
    void      print(std::ostream& ot) override;

private:
    void      pretty_print_at(std::ostream& , precedence_t, bool, bool, std::streampos&) override;
};

class FunExpr : public Expr {
public:
    PTR(VarExpr) arg_;
    PTR(Expr) body_;

    FunExpr(PTR(VarExpr) arg, PTR(Expr) body);
    bool      equals(PTR(Expr) rhs) override;
    PTR(Val)  interp(PTR(Env)) override;
//    PTR(Expr) subst(std::string parameter, PTR(Expr) e);
    void      print(std::ostream& ot) override;

private:
    void      pretty_print_at(std::ostream& , precedence_t, bool, bool, std::streampos&) override;
};

class CallExpr : public Expr {
public:
    PTR(Expr) callee_;
    PTR(Expr) arg_;

    CallExpr(PTR(Expr) callee, PTR(Expr) arg);
    bool      equals(PTR(Expr) rhs) override;
    PTR(Val)  interp(PTR(Env)) override;
//    PTR(Expr) subst(std::string parameter, PTR(Expr) e);
    void      print(std::ostream& ot) override;

private:
    void      pretty_print_at(std::ostream& , precedence_t, bool, bool, std::streampos&) override;
};
