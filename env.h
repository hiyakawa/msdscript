/**
 * \file env.h
 * \brief Declarations of methods in Env
 * \author Laura Zhang
 */

#pragma once

#include "pointer.h"
#include "val.h"

CLASS(Env) {
public:
    static PTR(Env) empty;

    virtual PTR(Val) lookup(std::string) = 0;
    virtual bool     equals(PTR(Env) rhs) = 0;
};

class EmptyEnv : public Env {
public:
    PTR(Val) lookup(std::string) override;
    bool     equals(PTR(Env) rhs) override;
};

class ExtendedEnv : public Env {
public:
    std::string name_;
    PTR(Val)    val_;
    PTR(Env)    env_;

    ExtendedEnv(std::string, PTR(Val), PTR(Env));
    PTR(Val) lookup(std::string) override;
    bool equals(PTR(Env) rhs) override;
};