/**
 * \file val.cpp
 * \brief Definitions of methods in Val base class
 * \author Laura Zhang
 */

#include "expr.h"
#include "val.h"

/*
 * NumVal
 */

NumVal::NumVal(int val) {
    val_ = val;
}

PTR(Expr) NumVal::to_expr() {
    return NEW(NumExpr)(val_);
}

bool NumVal::equals(PTR(Val) rhs) {
    PTR(NumVal) num_rhs = CAST(NumVal)(rhs);

    if (num_rhs == nullptr) {
        return false;
    }

    return val_ == num_rhs->val_;
}

PTR(Val) NumVal::add_to(PTR(Val) rhs) {
    PTR(NumVal) num_rhs = CAST(NumVal)(rhs);

    if (num_rhs == nullptr) {
        throw std::runtime_error("invalid type for NumVal::add_to()");
    }

    return NEW(NumVal)(val_ + num_rhs->val_);
}

PTR(Val) NumVal::mult_with(PTR(Val) rhs) {
    PTR(NumVal) num_rhs = CAST(NumVal)(rhs);

    if (num_rhs == nullptr) {
        throw std::runtime_error("invalid type for NumVal::mult_with()");
    }

    return NEW(NumVal)(val_ * num_rhs->val_);
}

std::string NumVal::to_string() {
    return std::to_string(val_);
}

bool NumVal::is_true() {
    throw std::runtime_error("invalid type for NumVal::is_true()");
}

PTR(Val) NumVal::call(PTR(Val) arg) {
    throw std::runtime_error("invalid type for NumVal::call()");
}

/*
 * BoolVal
 */

BoolVal::BoolVal(bool bool_val) {
    bool_val_ = bool_val;
}

PTR(Expr) BoolVal::to_expr() {
    return NEW(BoolExpr)(bool_val_);
}

bool BoolVal::equals(PTR(Val) rhs) {
    PTR(BoolVal) bool_rhs = CAST(BoolVal)(rhs);

    if (bool_rhs == nullptr) {
        return false;
    }

    return bool_val_ == bool_rhs->bool_val_;
}

PTR(Val) BoolVal::add_to(PTR(Val) rhs) {
    throw std::runtime_error("invalid type for BoolVal::add_to()");
}

PTR(Val) BoolVal::mult_with(PTR(Val) rhs) {
    throw std::runtime_error("invalid type for BoolVal::mult_with()");
}

std::string BoolVal::to_string() {
    if (bool_val_) {
        return "_true";
    }

    return "_false";
}

bool BoolVal::is_true() {
    return bool_val_;
}

PTR(Val) BoolVal::call(PTR(Val) arg) {
    throw std::runtime_error("invalid type for BoolVal::call()");
}

/*
 * FunVal
 */

FunVal::FunVal(PTR(VarExpr) arg, PTR(Expr) body) {
    arg_ = arg;
    body_ = body;
}

PTR(Expr) FunVal::to_expr() {
    return NEW(FunExpr)(arg_, body_);
}

bool FunVal::equals(PTR(Val) rhs) {
    PTR(FunVal) fun_rhs = CAST(FunVal)(rhs);

    if (fun_rhs == nullptr) {
        return false;
    }

    return arg_->equals(fun_rhs->arg_) &&
           body_->equals(fun_rhs->body_);
}

PTR(Val) FunVal::add_to(PTR(Val) rhs) {
    throw std::runtime_error("invalid type for FunVal::add_to()");
}

PTR(Val) FunVal::mult_with(PTR(Val) rhs) {
    throw std::runtime_error("invalid type for FunVal::mult_with()");
}

std::string FunVal::to_string() {
    return "[function]";
}

bool FunVal::is_true() {
    throw std::runtime_error("invalid type for FunVal::is_true()");
}

PTR(Val) FunVal::call(PTR(Val) arg) {
    return body_->interp(NEW(ExtendedEnv)(arg_->var_, arg, env_));
}
