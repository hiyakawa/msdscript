/**
 * \file env.cpp
 * \brief Definitions of methods in Env
 * \author Laura Zhang
 */

#include "env.h"

PTR(Env) Env::empty = NEW(EmptyEnv)();

bool EmptyEnv::equals(std::shared_ptr<Env> rhs) {
    PTR(EmptyEnv) ee = CAST(EmptyEnv)(rhs);
    if (ee == nullptr) {
        return false;
    }
    else {
        return true;
    }
}

PTR(Val) EmptyEnv::lookup(std::string input) {
    throw std::runtime_error("free variable: " + input);
}

ExtendedEnv::ExtendedEnv(std::string name, PTR(Val) val, PTR(Env) rest) {
    name_ = name;
    val_ = val;
    env_ = rest;
}

bool ExtendedEnv::equals(std::shared_ptr<Env> rhs) {
    PTR(ExtendedEnv) extendedEnv = CAST(ExtendedEnv)(rhs);

    if (extendedEnv == nullptr) {
        return false;
    }

    return (name_ == extendedEnv->name_ &&
            val_->equals(extendedEnv->val_) &&
            env_->equals(extendedEnv->env_));
}

PTR(Val) ExtendedEnv::lookup(std::string input) {
    if (input == name_) {
        return val_;
    }

    return env_->lookup(input);
}
