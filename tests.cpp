/**
 * \file tests.cpp
 * \brief Generate random tests
 * \author Laura Zhang
 */

#include "exec.h"
#include <iostream>

std::string random_var();
std::string random_expr_string();
std::string random_nested_expr();

std::string random_var() {
    int rand_num = rand() % 10;

    if (rand_num < 6) {
        // 60% chance of generating "x"
        return "x";
    }

    // generate a random string
    std::string word = "";

    if (rand_num % 2 == 0) {
        // add an uppercase letter to the string
        word += (char) ((rand() % 26) + 65);
    }
    else {
        word += (char) ((rand() % 26) + 97);
    }

    rand_num = rand() % 100;

    while (rand_num > 33) {
        if (rand_num % 2 == 0) {
            // 33% chance of adding an uppercase letter
            word += (char) ((rand() % 26) + 65);
        }
        else {
            word += (char) ((rand() % 26) + 97);
        }

        rand_num = rand() % 100;
    }

    // 34% chance of jumping out of the loop
    return word;
}

std::string random_expr_string() {
    int rand_num = rand() % 10;

    if (rand_num < 3) {
        // 30% chance of generating a NumExpr
        return std::to_string(rand());
    }
    if (rand_num < 6) {
        // 30% chance of generating a VarExpr
        return random_var();
    }
    if (rand_num == 6) {
        // 10% chance of generating a nested Expr
        return "(" + random_expr_string() + ")";
    }
    if (rand_num == 7) {
        // 10% chance of generating an AddExpr
        return random_expr_string() + "+" + random_expr_string();
    }
    if (rand_num == 8) {
        // 10% chance of generating a MultExpr
        return random_expr_string() + "*" + random_expr_string();
    }
    // 10% chance of generating a LetExpr
    return "_let " + random_var() + " = " + random_expr_string() + " _in " + random_expr_string();
}

std::string random_nested_expr() {
    int rand_num = rand() % 10;

    if (rand_num < 4) {
        // 40% chance of generating a NumExpr
        return std::to_string(rand());
    }
    if (rand_num < 6) {
        // 20% chance of generating a nested Expr
        return "(" + random_nested_expr() + ")";
    }
    if (rand_num < 8) {
        // 20% chance of generating an AddExpr
        return random_nested_expr() + "+" + random_nested_expr();
    }
    // 20% chance of generating a MultExpr
    return random_nested_expr() + "*" + random_nested_expr();
}

int tests_main(int argc, char* argv[]) {
    srand(time(nullptr));

    if (argc < 2 || argc > 3) {
        std::cerr << "Error: Two or three arguments are required." << std::endl;
        exit(1);
    }

    const char* const interp1_argv[] = {argv[1], "--interp"};
    const char* const print1_argv[] = {argv[1], "--print"};
    const char* const pretty_print1_argv[] = {argv[1], "--pretty-print"};

    if (argc == 2) {
        for (int i = 0; i < 100; i++) {
            std::string in = random_nested_expr();
            std::cout << "Trying " << in << "\n";

            // throw error message if any of the tests did not exit 0
            ExecResult interp_result = exec_program(2, interp1_argv, in);
            if (interp_result.exit_code != 0) {
                throw std::runtime_error("Error: interp() did not exit 0");
            }

            ExecResult print_result = exec_program(2, print1_argv, in);
            if (interp_result.exit_code != 0) {
                throw std::runtime_error("Error: print() did not exit 0");
            }

            ExecResult pretty_print_result = exec_program(2, pretty_print1_argv, in);
            if (interp_result.exit_code != 0) {
                throw std::runtime_error("Error: pretty_print() did not exit 0");
            }

            // throw error message if any of the tests has different result
            ExecResult interp_print_result = exec_program(2, interp1_argv, print_result.out);
            if (interp_print_result.out != interp_result.out) {
                throw std::runtime_error("Error: different result for interp() and print()");
            }

            ExecResult interp_pretty_print_result = exec_program(2, interp1_argv, pretty_print_result.out);
            if (interp_pretty_print_result.out != interp_result.out) {
                throw std::runtime_error("Error: different result for interp() and pretty_print()");
            }

            ExecResult pretty_print_pretty_print_result = exec_program(2, pretty_print1_argv, pretty_print_result.out);
            if (pretty_print_pretty_print_result.out != pretty_print_result.out) {
                throw std::runtime_error("Error: different result for pretty_print() and pretty_print()");
            }
        }
    }
    else if (argc == 3) {
        const char* const interp2_argv[] = {argv[2], "--interp"};
        const char* const print2_argv[] = {argv[2], "--print"};
        const char* const pretty_print2_argv[] = {argv[2], "--pretty-print"};

        for (int i = 0; i < 100; ++i) {
            std::string in = random_expr_string();
            std::cout << "Trying " << in << "\n";

            // throw error message if any of the tests did not return the same result
            ExecResult interp1_result = exec_program(2, interp1_argv, in);
            ExecResult interp2_result = exec_program(2, interp2_argv, in);
            if (interp1_result.out != interp2_result.out) {
                throw std::runtime_error("interp results did not match");
            }

            ExecResult print1_result = exec_program(2, print1_argv, in);
            ExecResult print2_result = exec_program(2, print2_argv, in);
            if (print1_result.out != print2_result.out) {
                throw std::runtime_error("print results did not match");
            }

            ExecResult pretty_print1_result = exec_program(2, pretty_print1_argv, in);
            ExecResult pretty_print2_result = exec_program(2, pretty_print2_argv, in);
            if (pretty_print1_result.out != pretty_print2_result.out) {
                throw std::runtime_error("pretty_print results did not match");
            }
        }
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
