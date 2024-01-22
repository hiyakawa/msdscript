/**
 * \mainpage MSDScript
 * \author Laura Zhang
 * \date 02-07-2023
 */

#define CATCH_CONFIG_RUNNER

#include "catch.h"
#include "expr.h"
#include "parse.h"
#include "cmdline.h"
#include "val.h"
#include "env.h"
#include <iostream>

bool run_tests() {
     const char *argv[] = {"arith"};
     return (Catch::Session().run(1, argv) == 0);
}

run_mode_t use_arguments(int argc, const char * argv[]) {
    if (argc > 1) {
        bool tested = false;

        // iterate through all the arguments
        for (int i = 1; i < argc; i++) {
            std::string cur_cmd = argv[i];

            if (cur_cmd == "--help") {
                std::cout << "Options include:" << std::endl;
                std::cout << "    --help <get helper text that describes what arguments are allowed>" << std::endl;
                std::cout << "    --test <run catch2 test on current execution file>" << std::endl;
                std::cout << "    --interp <accept a single expression and print the result>" << std::endl;
                std::cout << "    --print <accept a single expression and print it to standard output>" << std::endl;
                std::cout << "    --pretty-print <accept a single expression and print it to standard output using the pretty_print method>" << std::endl;

                exit(0);
            }
            else if (cur_cmd == "--test") {
                // if "--test" is seen before
                if (tested) {
                    std::cerr << "Error: Duplicated tests." << std::endl;
                    exit(1);
                }

                // run catch2 tests, exit with 1 if there are any failures
                if (! run_tests()) {
                    exit(1);
                }

                tested = true;
            }
            else if (cur_cmd == "--interp") {
                std::cout << "Type your expression here: ";

                std::string line;
                while (std::getline(std::cin, line)) {
                    std::cout << "--------------------" << std::endl;
                    std::cout << "interp value: " << parse_str(line)->interp(Env::empty) << std::endl;
                    std::cout << "--------------------" << std::endl << std::endl;
                }

                return do_interp;
            }
            else if (cur_cmd == "--print") {
                std::cout << "Type your expression here: ";

                std::string line;
                while (std::getline(std::cin, line)) {
                    std::cout << "--------------------" << std::endl;
                    std::cout << "print value: ";
                    parse_str(line)->print(std::cout);
                    std::cout << std::endl;
                    std::cout << "--------------------" << std::endl << std::endl;
                }

                return do_print;
            }
            else if (cur_cmd == "--pretty-print") {
                std::cout << "Type your expression here: ";

                std::string line;
                while (std::getline(std::cin, line)) {
                    std::cout << "--------------------" << std::endl;
                    std::cout << "pretty print value: " << std::endl;
                    std::cout << parse_str(line)->to_pretty_string() << std::endl;
                    std::cout << "--------------------" << std::endl << std::endl;
                }

                return do_pretty_print;
            }
            else {
                std::cerr << "Error: Invalid command." << std::endl;
                exit(1);
            }
        }
    }
    else {
        std::cerr << "Error: At least two arguments are required. "
                  << "Please use --help for the options." << std::endl;
        exit(1);
    }

    exit(0);
}

int main(int argc, const char * argv[]) {
    try {
        use_arguments(argc, argv);

        exit(0);
    }
    catch (std::runtime_error exn) {
        std::cerr << exn.what() << std::endl;
        return 1;
    }
}

TEST_CASE("equals") {
    // testing NumExpr::equals()
    CHECK((NEW(NumExpr)(1))->equals(NEW(NumExpr)(1)));
    CHECK((NEW(NumExpr)(1))->equals(NEW(NumExpr)(0)) == false);
    CHECK((NEW(NumExpr)(1))->equals(NEW(NumExpr)(-1)) == false);
    CHECK((NEW(NumExpr)(5))->equals(nullptr) == false);

    // testing AddExpr::equals()
    // adding NumExpr
    CHECK((NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)))->equals(NEW(AddExpr)(NEW(NumExpr)(1),NEW(NumExpr)(2))));
    CHECK((NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)))->equals(NEW(AddExpr)(NEW(NumExpr)(3),NEW(NumExpr)(2))) == false);
    CHECK((NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)))->equals(nullptr) == false);
    // adding VarExpr
    CHECK((NEW(AddExpr)(NEW(VarExpr)("abc"), NEW(VarExpr)("")))->equals(NEW(AddExpr)(NEW(VarExpr)("abc"), NEW(VarExpr)(""))));
    CHECK((NEW(AddExpr)(NEW(VarExpr)("abc"), NEW(VarExpr)("def")))->equals(NEW(AddExpr)(NEW(VarExpr)("abc"), NEW(VarExpr)(""))) == false);

    // testing MultExpr::equals()
    // multiplying NumExpr
    CHECK((NEW(MultExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)))->equals(NEW(MultExpr)(NEW(NumExpr)(1),NEW(NumExpr)(2))));
    CHECK((NEW(MultExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)))->equals(NEW(MultExpr)(NEW(NumExpr)(2),NEW(NumExpr)(1))) == false);
    CHECK((NEW(MultExpr)(NEW(NumExpr)(-2), NEW(NumExpr)(2)))->equals(NEW(AddExpr)(NEW(NumExpr)(1),NEW(NumExpr)(2))) == false);
    // multiplying VarExpr
    CHECK((NEW(MultExpr)(NEW(VarExpr)("abc"), NEW(VarExpr)("")))->equals(NEW(MultExpr)(NEW(VarExpr)("abc"), NEW(VarExpr)(""))));
    CHECK((NEW(MultExpr)(NEW(VarExpr)("abc"), NEW(VarExpr)("def")))->equals(NEW(MultExpr)(NEW(VarExpr)("abc"), NEW(VarExpr)("abc"))) == false);

    // testing VarExpr::equals()
    CHECK((NEW(VarExpr)("abc"))->equals(NEW(VarExpr)("abc")));
    CHECK((NEW(VarExpr)(""))->equals(NEW(VarExpr)("")));
    CHECK((NEW(VarExpr)("abc"))->equals(NEW(VarExpr)("")) == false);
    CHECK((NEW(VarExpr)("x"))->equals(NEW(VarExpr)("y")) == false);
    CHECK((NEW(VarExpr)("x"))->equals(NEW(NumExpr)(1)) == false);
    CHECK((NEW(VarExpr)("abc"))->equals(nullptr) == false);

    // testing LetExpr::equals()
    CHECK((NEW(LetExpr)("", (NEW(NumExpr)(-1)), (NEW(AddExpr)(NEW(VarExpr)(""), NEW(NumExpr)(2)))))->equals(nullptr) == false);
    CHECK((NEW(LetExpr)("x", (NEW(NumExpr)(0)), (NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)))))
    ->equals((NEW(LetExpr)("x", (NEW(NumExpr)(0)), (NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)))))));
}

TEST_CASE("interp") {
    // testing NumExpr::interp(Env::empty)
    CHECK((NEW(NumExpr)(0))->interp(Env::empty)->equals(NEW(NumVal)(0)));
    CHECK((NEW(NumExpr)(2))->interp(Env::empty)->equals(NEW(NumVal)(2)));
    CHECK((NEW(NumExpr)(-1))->interp(Env::empty)->equals(NEW(NumVal)(-1)));
    
    // testing AddExpr::interp(Env::empty)
    CHECK((NEW(AddExpr)(NEW(NumExpr)(-100), NEW(NumExpr)(100)))->interp(Env::empty)->equals(NEW(NumVal)(0)));
    CHECK((NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(10), NEW(NumExpr)(15)),
                       NEW(AddExpr)(NEW(NumExpr)(20), NEW(NumExpr)(20))))->interp(Env::empty)->equals(NEW(NumVal)(65)));
    
    // testing MultExpr::interp(Env::empty)
    CHECK((NEW(MultExpr)(NEW(NumExpr)(3), NEW(NumExpr)(2)))->interp(Env::empty)->equals(NEW(NumVal)(6)));
    CHECK((NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(-1), NEW(NumExpr)(2)),
                        NEW(MultExpr)(NEW(NumExpr)(3), NEW(NumExpr)(4))))->interp(Env::empty)->equals(NEW(NumVal)(-24)));
    
    // testing VarExpr::interp(Env::empty)
    CHECK_THROWS_WITH((NEW(VarExpr)("x"))->interp(Env::empty), "free variable: x");
    CHECK_THROWS_WITH((NEW(VarExpr)(""))->interp(Env::empty), "free variable: ");

    // testing LetExpr::interp(Env::empty)
    CHECK((NEW(AddExpr)(
            NEW(MultExpr)(
                    NEW(NumExpr)(5),
                    (NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x")))),
            NEW(NumExpr)(1)))->interp(Env::empty)->equals(NEW(NumVal)(26)));

}

//TEST_CASE("has_variable"){
//    // testing NumExpr::has_variable()
//    CHECK((NEW(NumExpr)(0))->has_variable() == false);
//    CHECK((NEW(NumExpr)(-5))->has_variable() == false);
//
//    // testing AddExpr::has_variable()
//    CHECK((NEW(AddExpr)(NEW(NumExpr)(3), NEW(NumExpr)(1)))->has_variable() == false);
//    CHECK((NEW(AddExpr)(NEW(VarExpr)("VarExpr"), NEW(NumExpr)(2)))->has_variable());
//    CHECK((NEW(AddExpr)(NEW(VarExpr)("VarExpr_1"), NEW(VarExpr)("VarExpr_2")))->has_variable());
//
//    // testing MultExpr::has_variable()
//    CHECK((NEW(MultExpr)(NEW(NumExpr)(3), NEW(NumExpr)(1)))->has_variable() == false);
//    CHECK((NEW(MultExpr)(NEW(VarExpr)("VarExpr"), NEW(NumExpr)(5)))->has_variable());
//    CHECK((NEW(MultExpr)(NEW(VarExpr)("VarExpr_1"), NEW(VarExpr)("VarExpr_2")))->has_variable());
//
//    // testing VarExpr::has_variable()
//    CHECK((NEW(VarExpr)("x"))->has_variable());
//    CHECK((NEW(VarExpr)(""))->has_variable());
//
//    // testing LetExpr::has_variable()
//    CHECK((NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))))->has_variable());
//    CHECK((NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2))))->has_variable() == false);
//}

//TEST_CASE("subst") {
//    // testing NumExpr::subst()
//    CHECK((NEW(NumExpr)(0))->subst("VarExpr", NEW(NumExpr)(0)));
//    CHECK((NEW(NumExpr)(-10))->subst("VarExpr", NEW(NumExpr)(-10)));
//    CHECK((NEW(NumExpr)(5))->subst("VarExpr", NEW(NumExpr)(5)));
//
//    // testing AddExpr::subst()
//    CHECK((NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(7)))->subst("x", NEW(NumExpr)(9)));
//    CHECK((NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7)))->subst("x", NEW(VarExpr)("y"))
//          ->equals(NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7))));
//
//    // testing MultExpr::subst()
//    CHECK((NEW(MultExpr)(NEW(NumExpr)(3), NEW(NumExpr)(4)))->subst("x", NEW(NumExpr)(12)));
//
//    // testing VarExpr::subst()
//    CHECK((NEW(VarExpr)("VarExpr_1"))->subst("VarExpr_1", NEW(VarExpr)("VarExpr_1")));
//    CHECK((NEW(VarExpr)("VarExpr_2"))->subst("VarExpr_1", NEW(VarExpr)("VarExpr_1")));
//    CHECK((NEW(VarExpr)("x"))->subst("x", NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7)))
//          ->equals(NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7))));
//
//    // testing LetExpr::subst()
//    CHECK((NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(NumExpr)(2)))->subst("y", NEW(VarExpr)("x"))->equals(NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(NumExpr)(2))));
//    CHECK((NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(NumExpr)(2)))->subst("a", NEW(VarExpr)("x"))->equals(NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(NumExpr)(2))));
//}

TEST_CASE("print") {
    // testing to_string()
    CHECK((NEW(MultExpr)((NEW(MultExpr)(
                            (NEW(NumExpr)(10)),
                            (NEW(NumExpr)(10)))),
                        (NEW(MultExpr)(
                            (NEW(NumExpr)(10)),
                            (NEW(NumExpr)(10))))))->to_string() == "((10*10)*(10*10))");

    // testing NumExpr::print()
    std::stringstream ot1("");
    (NEW(NumExpr)(0))->print(ot1);
    CHECK(ot1.str() == "0");

    // testing VarExpr::print()
    std::stringstream ot2("");
    (NEW(VarExpr)("variable"))->print(ot2);
    CHECK(ot2.str() == "variable");

    // testing AddExpr::print()
    std::stringstream ot3("");
    (NEW(AddExpr)((NEW(NumExpr)(-1)), (NEW(VarExpr)("rhs"))))->print(ot3);
    CHECK(ot3.str() == "(-1+rhs)");

    // testing MultExpr::print()
    std::stringstream ot4("");
    (NEW(MultExpr)((NEW(VarExpr)("lhs")), (NEW(NumExpr)(0))))->print(ot4);
    CHECK(ot4.str() == "(lhs*0)");

    // testing LetExpr::print()
    std::stringstream ot5("");
    (NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))), NEW(VarExpr)("x"))))->print(ot5);
    CHECK(ot5.str() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
}

TEST_CASE("pretty_print") {
    // testing NumExpr::pretty_print()
    CHECK((NEW(NumExpr)(0))->to_pretty_string() == "0");

    // testing VarExpr::pretty_print()
    CHECK((NEW(VarExpr)("variable"))->to_pretty_string() == "variable");

    // testing AddExpr::pretty_print()
    CHECK((NEW(AddExpr)((NEW(NumExpr)(-1)), (NEW(VarExpr)("rhs"))))->to_pretty_string() == "-1 + rhs");

    // testing MultExpr::pretty_print()
    CHECK((NEW(MultExpr)((NEW(VarExpr)("lhs")), (NEW(NumExpr)(0))))->to_pretty_string() == "lhs * 0");

    // going wild
    CHECK((NEW(AddExpr)(NEW(NumExpr)(1), NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3))))->to_pretty_string() == "1 + 2 * 3");

    CHECK((NEW(MultExpr)(NEW(NumExpr)(1), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3))))->to_pretty_string() == "1 * (2 + 3)");

    CHECK((NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)), NEW(NumExpr)(4)))->to_pretty_string() == "(2 * 3) * 4");

    CHECK((NEW(MultExpr)(NEW(NumExpr)(2), NEW(MultExpr)(NEW(NumExpr)(3), NEW(NumExpr)(4))))->to_pretty_string() == "2 * 3 * 4");

    CHECK((NEW(AddExpr)(
            NEW(MultExpr)(
                    NEW(NumExpr)(9),
                    NEW(AddExpr)(
                            NEW(NumExpr)(4),
                            NEW(NumExpr)(3))),
            NEW(AddExpr)(
                    NEW(MultExpr)(
                            NEW(NumExpr)(2),
                            NEW(NumExpr)(4)),
                    NEW(NumExpr)(1))))->to_pretty_string() == "9 * (4 + 3) + 2 * 4 + 1");

    CHECK((NEW(MultExpr)(
            NEW(MultExpr)(
                    NEW(NumExpr)(10),
                    NEW(MultExpr)(
                            NEW(MultExpr)(
                                    NEW(NumExpr)(10),
                                    NEW(NumExpr)(10)),
                            NEW(NumExpr)(10))),
            NEW(MultExpr)(
                    NEW(NumExpr)(10),
                    NEW(NumExpr)(10))))->to_pretty_string() == "(10 * (10 * 10) * 10) * 10 * 10");

    CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5),
                       NEW(AddExpr)(NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))),
                                   NEW(VarExpr)("x"))))->to_pretty_string() == "_let x = 5\n"
                                   "_in (_let y = 3\n"
                                   "     _in y + 2) + x");

    CHECK((NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(2),
                                     NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))),
                        NEW(NumExpr)(3)))
                  ->to_pretty_string() == "(2 * _let x = 5\n"
                                  "     _in x + 1) * 3");
}

TEST_CASE("Pretty Print examples_Kevin") {
    std::stringstream out("");
    (NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))->pretty_print(out);
    CHECK(out.str() == "_let x = 5\n"
                       "_in x + 1");
    out.str(std::string());
    (NEW(AddExpr)(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x")), NEW(NumExpr)(1)))->pretty_print(out);
    CHECK(out.str() == "(_let x = 5\n"
                       " _in x) + 1");
    out.str(std::string());
    (NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))))->pretty_print(out);
    CHECK(out.str() == "5 * _let x = 5\n"
                       "    _in x + 1");
    out.str(std::string());
    (NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x"))), NEW(NumExpr)(1)))->pretty_print(out);
    CHECK(out.str() == "5 * (_let x = 5\n"
                       "     _in x) + 1");
    out.str(std::string());
    (NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))), NEW(VarExpr)("x"))))
            ->pretty_print(out);
    CHECK(out.str() == "_let x = 5\n"
                       "_in (_let y = 3\n"
                       "     _in y + 2) + x");
    out.str(std::string());
    (NEW(LetExpr)("x", NEW(LetExpr)("y", NEW(NumExpr)(6), NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))->pretty_print(out);
    CHECK(out.str() == "_let x = _let y = 6\n"
                       "         _in y * 2\n"
                       "_in x + 1");

    PTR(LetExpr) tripleNestedLetExpr = NEW(LetExpr)("x", NEW(NumExpr)(1),
                                   NEW(LetExpr)("y", NEW(NumExpr)(1),
                                           NEW(MultExpr)(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y")), NEW(VarExpr)("z"))));
    PTR(LetExpr) tripleNestedLetExpr2 = NEW(LetExpr)("x", NEW(NumExpr)(1),
                                    NEW(LetExpr)("y", NEW(NumExpr)(1),
                                            NEW(LetExpr)("z", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)),
                                                    NEW(AddExpr)(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y")), NEW(VarExpr)("z")))));
    PTR(LetExpr) tripleNestedLetExpr3 = NEW(LetExpr)("x", NEW(NumExpr)(1),
                                    NEW(LetExpr)("y", NEW(NumExpr)(1),
                                            NEW(LetExpr)("z", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)),
                                                    NEW(MultExpr)(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y")), NEW(VarExpr)("z")))));


    CHECK(tripleNestedLetExpr->to_pretty_string() ==
          "_let x = 1\n"
          "_in _let y = 1\n"
          "    _in (x + y) * z"
   );
    CHECK(tripleNestedLetExpr2->to_pretty_string() ==
          "_let x = 1\n"
          "_in _let y = 1\n"
          "    _in _let z = x + 1\n"
          "        _in (x + y) + z"
   );
    CHECK(tripleNestedLetExpr3->to_pretty_string() ==
          "_let x = 1\n"
          "_in _let y = 1\n"
          "    _in _let z = x + 1\n"
          "        _in (x + y) * z"
   );
    PTR(LetExpr) tripleNestedLetExpr4 =NEW(LetExpr)("x", NEW(NumExpr)(5),
                                   NEW(LetExpr)("y", NEW(NumExpr)(3),
                                           NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))));
    PTR(LetExpr) tripleNestedLetExpr5 =NEW(LetExpr)("x", NEW(NumExpr)(5),
                                   NEW(AddExpr)(NEW(LetExpr)("y", NEW(NumExpr)(3),
                                                   NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))), NEW(VarExpr)("x")));

    SECTION("assignment_examples") {
        CHECK((NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(2), NEW(LetExpr)("x", NEW(NumExpr)(5),
                                                                     NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))),
                            NEW(NumExpr)(3)))->to_pretty_string()
               == "(2 * _let x = 5\n"
                  "     _in x + 1) * 3");
    }
    SECTION("new_edge") {
        CHECK((NEW(MultExpr)(NEW(NumExpr)(2), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(
                1)))))->to_pretty_string()
               == "2 * _let x = 5\n"
                  "    _in x + 1");
        CHECK((NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5),
                                                                    NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)))),
                           NEW(NumExpr)(1)))->to_pretty_string()
               == "5 * (_let x = 5\n"
                  "     _in x * 2) + 1");
        CHECK((NEW(MultExpr)((NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5),
                                                                                  NEW(MultExpr)(NEW(VarExpr)("x"),
                                                                                               NEW(NumExpr)(2)))),
                                         NEW(NumExpr)(1))), NEW(NumExpr)(7)))->to_pretty_string()
               == "(5 * (_let x = 5\n"
                  "      _in x * 2) + 1) * 7");
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(10), NEW(MultExpr)(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(MultExpr)(
                                                                      NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)), NEW(NumExpr)(10))),
                                                              NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)))))
                      ->to_pretty_string() == "_let x = 10\n"
                                                      "_in (x * (10 * 10) * 10) * 10 * 10");
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(MultExpr)(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(MultExpr)(
                                                                     NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)), NEW(VarExpr)("x"))),
                                                             NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)))))
                      ->to_pretty_string() == "_let x = 1\n"
                                                      "_in (x * (10 * 10) * x) * 10 * 10");
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(MultExpr)(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(MultExpr)(
                                                                     NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)), NEW(VarExpr)("x"))),
                                                             NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(10)))))
                      ->to_pretty_string() == "_let x = 1\n"
                                                      "_in (x * (10 * 10) * x) * y * 10");
    }
}

TEST_CASE("LetExpr_equals_mine") {
    SECTION("Values_same") {
        REQUIRE((NEW(LetExpr)("x", NEW(NumExpr)(4), NEW(AddExpr)(NEW(NumExpr)(2), NEW(VarExpr)("x"))))->equals(NEW(LetExpr)("x", NEW(NumExpr)(4), NEW(AddExpr)(NEW(NumExpr)(2), NEW(VarExpr)("x")))));
    }
    SECTION("Values_same_different_rhs") {
        REQUIRE(!(NEW(LetExpr)("x", NEW(NumExpr)(4), NEW(AddExpr)(NEW(NumExpr)(2), NEW(VarExpr)("x"))))->equals(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(NumExpr)(2), NEW(VarExpr)("x")))));
    }
    SECTION("Values_same_different_lhs") {
        REQUIRE(!(NEW(LetExpr)("x", NEW(NumExpr)(4), NEW(AddExpr)(NEW(NumExpr)(2), NEW(VarExpr)("x"))))->equals(NEW(LetExpr)("y", NEW(NumExpr)(4), NEW(AddExpr)(NEW(NumExpr)(2), NEW(VarExpr)("x")))));
    }
    SECTION("Values_same_different_body") {
        REQUIRE(!(NEW(LetExpr)("x", NEW(NumExpr)(4), NEW(AddExpr)(NEW(NumExpr)(2), NEW(VarExpr)("x"))))->equals(NEW(LetExpr)("x", NEW(NumExpr)(4), NEW(MultExpr)(NEW(NumExpr)(3), NEW(VarExpr)("y")))));
    }
    SECTION("different_types") {
        REQUIRE(!(NEW(LetExpr)("x", NEW(NumExpr)(4), NEW(AddExpr)(NEW(NumExpr)(2), NEW(VarExpr)("x"))))->equals( NEW(MultExpr)(NEW(NumExpr)(3), NEW(VarExpr)("y"))));
    }

    CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))), NEW(VarExpr)("x"))))->to_string()
           == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
    CHECK((NEW(LetExpr)("x", NEW(NumExpr)(1),
                    NEW(LetExpr)("y", NEW(NumExpr)(1),
                            NEW(LetExpr)("z", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)),
                                    NEW(MultExpr)(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y")), NEW(VarExpr)("z"))))))->to_string()
           == "(_let x=1 _in (_let y=1 _in (_let z=(x+1) _in ((x+y)*z))))");

    SECTION("hw_examples") {
        CHECK((NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x"))), NEW(NumExpr)(1)))->interp(Env::empty)->equals(NEW(NumVal)(26)));
        CHECK((NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))))->interp(Env::empty)->equals(NEW(NumVal)(30)));
    }
    SECTION("from_pretty_print_edge") {
        CHECK((NEW(MultExpr)((NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)))), NEW(NumExpr)(1))), NEW(NumExpr)(7)))->interp(Env::empty)->equals(NEW(NumVal)(357))); // 51 * 7
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(10), NEW(MultExpr)( NEW(MultExpr)(NEW(VarExpr)("x"), NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)), NEW(NumExpr)(10))), NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)))))
                       ->interp(Env::empty)->equals(NEW(NumVal)(1000000)));
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(MultExpr)( NEW(MultExpr)(NEW(VarExpr)("x"), NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)), NEW(VarExpr)("x"))), NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)))))
                       ->interp(Env::empty)->equals(NEW(NumVal)(10000)));
        CHECK_THROWS_WITH(((NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(MultExpr)(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)), NEW(VarExpr)("x"))), NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(10)))))->interp(Env::empty)->equals(NEW(NumVal)(10000))), "free variable: y");
    }
    SECTION("bypass_middle_let") {
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(2), NEW(LetExpr)("z", NEW(NumExpr)(4), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(10)))))
                       ->interp(Env::empty)->equals(NEW(NumVal)(12)));
    }
}

TEST_CASE("parse") {
    CHECK_THROWS_WITH(parse_str(""), "bad input");
    CHECK_THROWS_WITH(parse_str("()"), "bad input");

    CHECK(parse_str("(1)")->equals(NEW(NumExpr)(1)));
    CHECK(parse_str("(((1)))")->equals(NEW(NumExpr)(1)));

    CHECK_THROWS_WITH(parse_str("(1"), "bad input");

    CHECK(parse_str("1")->equals(NEW(NumExpr)(1)));
    CHECK(parse_str("10")->equals(NEW(NumExpr)(10)));
    CHECK(parse_str("-3")->equals(NEW(NumExpr)(-3)));
    CHECK(parse_str("  \n 5  ")->equals(NEW(NumExpr)(5)));
    CHECK_THROWS_WITH(parse_str("-"), "invalid input");
    CHECK_THROWS_WITH(parse_str(" -   5  "), "invalid input");
    CHECK_THROWS_WITH(parse_str("0 + "), "bad input");
    CHECK_THROWS_WITH(parse_str("0        ++"), "bad input");

    CHECK(parse_str("x")->equals(NEW(VarExpr)("x")));
    CHECK(parse_str("xyz")->equals(NEW(VarExpr)("xyz")));
    CHECK(parse_str("xYz")->equals(NEW(VarExpr)("xYz")));
    CHECK_THROWS_WITH(parse_str("*t"), "bad input");
    CHECK_THROWS_WITH(parse_str("x Y"), "invalid input");
    CHECK_THROWS_WITH(parse_str("x_z"), "invalid input");

    CHECK(parse_str("x + y")->equals(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y"))));
    CHECK(parse_str("x * y")->equals(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y"))));
    CHECK(parse_str("z * x + y")
                   ->equals(NEW(AddExpr)(NEW(MultExpr)(NEW(VarExpr)("z"), NEW(VarExpr)("x")),
                                    NEW(VarExpr)("y"))));
    CHECK(parse_str("z * (x + y)")
                   ->equals(NEW(MultExpr)(NEW(VarExpr)("z"),
                                     NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y")))));

    CHECK_THROWS_WITH(parse_str("_leet x = 5 _in 1"), "bad input");
    CHECK_THROWS_WITH(parse_str("_let x 5 _in 1"), "bad input");
    CHECK_THROWS_WITH(parse_str("_let x = 5 _on 1"), "bad input");

    CHECK(parse_str("  _let  x  =  5  _in  x  +  1")->equals(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))));
    CHECK(parse_str("_let x=5 _in (_let y = 3 _in y+2)+x")
                   ->equals(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))), NEW(VarExpr)("x")))));
}

TEST_CASE("val") {
    CHECK((NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)))
                       ->interp(Env::empty)->equals(NEW(NumVal)(3)));
    CHECK(parse_str("1 + 2")->interp(Env::empty)->equals(NEW(NumVal)(3)));
    CHECK(parse_str("1 + 2")->interp(Env::empty)->to_string() == "3");

    CHECK((NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)))
                      ->interp(Env::empty)->equals(NEW(BoolVal)(false)));
    CHECK(parse_str("1 == 2")->interp(Env::empty)->equals(NEW(BoolVal)(false)));
    CHECK(parse_str("1 == 2")->interp(Env::empty)->to_string() == "_false");
    
    CHECK((NEW(LetExpr)("x", NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)),
                       NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x"))))
                       ->interp(Env::empty)->equals(NEW(NumVal)(25)));

    CHECK((NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(1), NEW(NumExpr)(2)))
                      ->interp(Env::empty)->equals(NEW(NumVal)(1)));

    CHECK((NEW(NumVal)(-1))->to_expr()->equals(NEW(NumExpr)(-1)));
    CHECK((NEW(NumVal)(0))->to_expr()->equals(NEW(NumExpr)(0)));
    CHECK((NEW(NumVal)(1))->to_expr()->equals(NEW(NumExpr)(0)) == false);

    CHECK_THROWS_WITH((NEW(NumVal)(5))->add_to(nullptr), "invalid type for NumVal::add_to()");
    CHECK_THROWS_WITH((NEW(NumVal)(0))->mult_with(nullptr), "invalid type for NumVal::mult_with()");

    CHECK((NEW(BoolVal)(true))->to_expr()->equals(NEW(BoolExpr)(true)));
    CHECK((NEW(BoolVal)(false))->equals(nullptr) == false);

    CHECK_THROWS_WITH((NEW(BoolVal)(true))->add_to(NEW(BoolVal)(true)), "invalid type for BoolVal::add_to()");
    CHECK_THROWS_WITH((NEW(BoolVal)(false))->mult_with(NEW(BoolVal)(true)), "invalid type for BoolVal::mult_with()");
}

TEST_CASE("BoolExpr&IfExpr&EqExpr") {
    CHECK((NEW(BoolExpr)(true))->equals(nullptr) == false);
//    CHECK((NEW(BoolExpr)(true))->has_variable() == false);
    CHECK((NEW(BoolExpr)(true))->to_string() == "_true");
    CHECK((NEW(BoolExpr)(false))->to_string() == "_false");
    CHECK((NEW(BoolExpr)(true))->to_pretty_string() == "_true");
    CHECK((NEW(BoolExpr)(false))->to_pretty_string() == "_false");

    CHECK((NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(1), NEW(NumExpr)(0)))->equals(nullptr) == false);
//    CHECK((NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(1), NEW(NumExpr)(0)))->has_variable() == false);
//    CHECK((NEW(IfExpr)(NEW(BoolExpr)(true), NEW(VarExpr)("x"), NEW(NumExpr)(0)))->has_variable() == true);

//    CHECK((NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(BoolExpr)(true), NEW(BoolExpr)(false)))
//            ->subst("x", NEW(BoolExpr)(false))->equals(NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(BoolExpr)(true), NEW(BoolExpr)(false))));
//    CHECK((NEW(IfExpr)(NEW(EqExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)), NEW(BoolExpr)(true), NEW(BoolExpr)(false)))
//            ->subst("x", NEW(NumExpr)(2))->equals(NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(2), NEW(NumExpr)(2)), NEW(BoolExpr)(true), NEW(BoolExpr)(false))));
//    CHECK((NEW(IfExpr)(NEW(VarExpr)("test"), NEW(BoolExpr)(true), NEW(BoolExpr)(false)))
//            ->subst("test", NEW(EqExpr)(NEW(BoolExpr)(true), NEW(BoolExpr)(false)))
//            ->equals(NEW(IfExpr)(NEW(EqExpr)(NEW(BoolExpr)(true), NEW(BoolExpr)(false)), NEW(BoolExpr)(true), NEW(BoolExpr)(false))));
    
    CHECK((NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(1), NEW(NumExpr)(0)))->to_string() == "(_if _true _then 1 _else 0)");
    CHECK((NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(BoolExpr)(true), NEW(BoolExpr)(false)))
                  ->to_string() == "(_if (1==2) _then _true _else _false)");
    CHECK((NEW(IfExpr)(NEW(BoolExpr)(false), NEW(AddExpr)(NEW(NumExpr)(0), NEW(NumExpr)(0)), NEW(EqExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(1))))
                  ->to_string() == "(_if _false _then (0+0) _else (y==1))");
    CHECK((NEW(IfExpr)(NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1)), NEW(BoolExpr)(true), NEW(BoolExpr)(false)), NEW(NumExpr)(10), NEW(NumExpr)(20)))
                  ->to_string() == "(_if (_if (1==1) _then _true _else _false) _then 10 _else 20)");

    CHECK((NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(BoolExpr)(true), NEW(BoolExpr)(false)))
                  ->to_pretty_string() == "_if 1 == 2\n"
                                          "_then _true\n"
                                          "_else _false");
    CHECK((NEW(IfExpr)(NEW(BoolExpr)(false), NEW(AddExpr)(NEW(NumExpr)(0), NEW(NumExpr)(0)), NEW(EqExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(1))))
                  ->to_pretty_string() == "_if _false\n"
                                          "_then 0 + 0\n"
                                          "_else y == 1");
    CHECK((NEW(IfExpr)(NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1)), NEW(BoolExpr)(true), NEW(BoolExpr)(false)), NEW(NumExpr)(10), NEW(NumExpr)(20)))
                  ->to_pretty_string() == "_if _if 1 == 1\n"
                                          "    _then _true\n"
                                          "    _else _false\n"
                                          "_then 10\n"
                                          "_else 20");
    CHECK((NEW(AddExpr)(NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(1), NEW(NumExpr)(0)), NEW(NumExpr)(5)))
                  ->to_pretty_string() == "(_if _true\n"
                                          " _then 1\n"
                                          " _else 0) + 5");

    CHECK((NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1)))->equals(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1))) == true);
    CHECK((NEW(EqExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))->equals(NEW(EqExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))) == true);
    CHECK((NEW(EqExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))->equals(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1))) == false);
    CHECK((NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1)))->equals(NEW(BoolExpr)(true)) == false);

    CHECK((NEW(EqExpr)(NEW(NumExpr)(0), NEW(NumExpr)(0)))->interp(Env::empty)->equals(NEW(BoolVal)(true)));
    CHECK((NEW(EqExpr)(NEW(NumExpr)(-99), NEW(NumExpr)(0)))->interp(Env::empty)->equals(NEW(BoolVal)(false)));
    CHECK((NEW(EqExpr)(NEW(BoolExpr)(true), NEW(BoolExpr)(true)))->interp(Env::empty)->equals(NEW(BoolVal)(true)));
    CHECK((NEW(EqExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(1)))->interp(Env::empty)->equals(NEW(BoolVal)(false)));
    CHECK_THROWS_WITH((NEW(EqExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)))->interp(Env::empty)->equals(NEW(BoolVal)(false)), "free variable: x");

//    CHECK((NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1)))->has_variable() == false);
//    CHECK((NEW(EqExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(1)))->has_variable() == true);
//    CHECK((NEW(EqExpr)(NEW(NumExpr)(1), NEW(VarExpr)("test")))->has_variable() == true);
//    CHECK((NEW(EqExpr)(NEW(NumExpr)(1), NEW(AddExpr)(NEW(NumExpr)(1), NEW(VarExpr)("x"))))->has_variable() == true);

//    CHECK((NEW(EqExpr)(NEW(VarExpr)("y"), NEW(VarExpr)("y")))->subst("y", NEW(NumExpr)(20))
//            ->equals(NEW(EqExpr)(NEW(NumExpr)(20), NEW(NumExpr)(20))));
//    CHECK((NEW(EqExpr)(NEW(NumExpr)(0), NEW(NumExpr)(0)))->subst("x", NEW(NumExpr)(-1))
//            ->equals(NEW(EqExpr)(NEW(NumExpr)(0), NEW(NumExpr)(0))));
//    CHECK((NEW(EqExpr)(NEW(NumExpr)(1), NEW(VarExpr)("x")))->subst("t", NEW(NumExpr)(0))
//            ->equals(NEW(EqExpr)(NEW(NumExpr)(1), NEW(VarExpr)("t"))));
//    CHECK((NEW(EqExpr)(NEW(VarExpr)("x"),
//                      NEW(AddExpr)(NEW(NumExpr)(3),
//                                       NEW(VarExpr)("y"))))->subst("hey", NEW(NumExpr)(0))
//->equals(NEW(EqExpr)(NEW(VarExpr)("hey"),
//                       NEW(AddExpr)(NEW(NumExpr)(3),
//                                        NEW(VarExpr)("hey")))));

    CHECK((NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1)))->to_string() == "(1==1)");
    CHECK((NEW(EqExpr)(NEW(VarExpr)("zero"), NEW(NumExpr)(0)))->to_string() == "(zero==0)");
    CHECK((NEW(EqExpr)(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1)), NEW(VarExpr)("y")))->to_string() == "((1+1)==y)");
    CHECK((NEW(EqExpr)(NEW(BoolExpr)(false), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))->to_string() == "(_false==(x+1))");

    CHECK((NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1)))->to_pretty_string() == "1 == 1");
    CHECK((NEW(EqExpr)(NEW(VarExpr)("zero"), NEW(NumExpr)(0)))->to_pretty_string() == "zero == 0");
    CHECK((NEW(EqExpr)(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1)), NEW(VarExpr)("y")))
                  ->to_pretty_string() == "1 + 1 == y");
    CHECK((NEW(AddExpr)(NEW(BoolExpr)(false), NEW(EqExpr)(NEW(NumExpr)(0), NEW(BoolExpr)(false))))
                  ->to_pretty_string() == "_false + (0 == _false)");
    CHECK((NEW(EqExpr)(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(NumExpr)(3)))
                  ->to_pretty_string() == "(1 == 2) == 3");
    CHECK((NEW(AddExpr)(NEW(MultExpr)(NEW(EqExpr)(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)), NEW(EqExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(9))), NEW(VarExpr)("x")), NEW(NumExpr)(1)))
                  ->to_pretty_string() == "(x + 1 == _true == 9) * x + 1");
}

TEST_CASE("from quiz") {
    CHECK((parse_str("_if 1 == 2 _then 5 _else 6")) ->
            equals(NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(NumExpr)(5), NEW(NumExpr)(6))));
    
    CHECK((((parse_str("_if 1 == 2 _then 5 _else 6"))->interp(Env::empty))->to_string()) == "6");
    CHECK((((parse_str("1 == 2"))->interp(Env::empty))->to_string()) == "_false");
    CHECK((((parse_str("(1 + 2) == (3 + 0)"))->interp(Env::empty))->to_string()) == "_true");
    CHECK((((parse_str("1 + 2 == 3 + 0"))->interp(Env::empty))->to_string()) == "_true");
    CHECK_THROWS_WITH(((((parse_str("(1 == 2) + 3 "))->interp(Env::empty))->to_string()) == "_true"), "invalid type for BoolVal::add_to()");
    CHECK((((parse_str("1==2+3"))->interp(Env::empty))->to_string()) == "_false");
    CHECK((((parse_str("_if _false\n"
                          "_then 5\n"
                          "_else 6"))->interp(Env::empty))->to_string()) == "6");
    CHECK((((parse_str("_if _false\n"
                          "_then _false\n"
                          "_else _true"))->interp(Env::empty))->to_string()) == "_true");
    CHECK((((parse_str("_if _false\n"
                          "_then 5\n"
                          "_else _false"))->interp(Env::empty))->to_string()) == "_false");
    CHECK_THROWS_WITH(((((parse_str("_true + _false"))->interp(Env::empty))->to_string()) == "_false"), "invalid type for BoolVal::add_to()");
    CHECK_THROWS_WITH(((((parse_str("_true + 1"))->interp(Env::empty))->to_string()) == "_false"), "invalid type for BoolVal::add_to()");
    CHECK((((parse_str("_true == _true"))->interp(Env::empty))->to_string()) == "_true");
    CHECK((((parse_str("1 == _true"))->interp(Env::empty))->to_string()) == "_false");
    CHECK_THROWS_WITH(((((parse_str("_if 1 + 2\n"
                                       "_then _false\n"
                                       "_else _true"))->interp(Env::empty))->to_string()) == "_false"), "invalid type for NumVal::is_true()");
    CHECK((((parse_str("_if _true\n"
                          "_then 5\n"
                          "_else _true + 1"))->interp(Env::empty))->to_string()) == "5");
    CHECK_THROWS_WITH(((((parse_str("_if _false\n"
                                       "_then 5\n"
                                       "_else _true + 1"))->interp(Env::empty))->to_string()) == "_false"), "invalid type for BoolVal::add_to()");
    CHECK_THROWS_WITH(((((parse_str("_let x = _true + 1\n"
                                       "_in  _if _true\n"
                                       "     _then 5\n"
                                       "     _else x"))->interp(Env::empty))->to_string()) == "5"), "invalid type for BoolVal::add_to()");
    CHECK((((parse_str("(_if _true\n"
                          " _then 5\n"
                          " _else _true) + 1"))->interp(Env::empty))->to_string()) == "6");
    CHECK((((parse_str("_if (_if 1 == 2\n"
                          "     _then _false\n"
                          "     _else _true)\n"
                          "_then 5\n"
                          "_else 6"))->interp(Env::empty))->to_string()) == "5");
    CHECK((((parse_str("_if (_if 1 == 2\n"
                          "     _then _true\n"
                          "      _else _false)\n"
                          "_then 5\n"
                          "_else 6"))->interp(Env::empty))->to_string()) == "6");
}

TEST_CASE("FunExpr equals() tests") {
    CHECK((NEW(FunExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
                  ->equals(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))) == true);
    CHECK((NEW(FunExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
                  ->equals(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))) == false);
}

TEST_CASE("FunExpr interp(Env::empty) tests") {
    CHECK((NEW(FunExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))->interp(Env::empty)->equals(NEW(FunVal)(NEW(VarExpr)("x"), NEW(NumExpr)(1))));

    CHECK((NEW(FunExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))->interp(Env::empty)->equals(NEW(FunVal)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))));

    CHECK((NEW(FunExpr)(NEW(VarExpr)("y"), NEW(MultExpr)(NEW(VarExpr)("y"), NEW(VarExpr)("y"))))->interp(Env::empty)->equals(NEW(FunVal)(NEW(VarExpr)("y"), NEW(MultExpr)(NEW(VarExpr)("y"), NEW(VarExpr)("y")))));
}

TEST_CASE("FunExpr print() tests") {
    CHECK((NEW(FunExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))->to_string() == "(_fun (x) 1)");
    CHECK((NEW(FunExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
                  ->to_string() == "(_fun (x) (x+1))");
    CHECK((NEW(FunExpr)(NEW(VarExpr)("x"), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y"))))
                  ->to_string() == "(_fun (x) (x*y))");
    CHECK((NEW(FunExpr)(NEW(VarExpr)("x"), NEW(FunExpr)(NEW(VarExpr)("y"), NEW(AddExpr)(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x")), NEW(MultExpr)(NEW(VarExpr)("y"), NEW(VarExpr)("y"))))))
                  ->to_string() == "(_fun (x) (_fun (y) ((x*x)+(y*y))))");
}

TEST_CASE("FunExpr pretty_print() tests") {
    CHECK((NEW(FunExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))->to_pretty_string() ==
        "_fun (x)\n1");
    CHECK((NEW(FunExpr)(NEW(VarExpr)("x"), NEW(FunExpr)(NEW(VarExpr)("y"), NEW(AddExpr)(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x")), NEW(MultExpr)(NEW(VarExpr)("y"), NEW(VarExpr)("y"))))))->to_pretty_string() == "_fun (x)\n"
                                          "_fun (y)\n"
                                          "x * x + y * y");
}

TEST_CASE("FunExpr") {
    SECTION("Equals") {
        CHECK((NEW(FunExpr)(NEW(VarExpr)("z"), NEW(VarExpr)("x")))->equals(NEW(FunExpr)(NEW(VarExpr)("z"), NEW(VarExpr)("x"))));
        CHECK((NEW(FunExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(3)))
                       ->equals(NEW(FunExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(3))));
        CHECK(! (NEW(FunExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(4)))
                ->equals(NEW(FunExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(4))));
        CHECK(! (NEW(FunExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(31)))
                ->equals(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(41))));
        CHECK(! (NEW(FunExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(31)))
                ->equals(nullptr));
    }

    SECTION("interp"){
        CHECK((NEW(FunExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(41)))->interp(Env::empty)->equals(NEW(FunVal)(NEW(VarExpr)("x"), NEW(NumExpr)(41))));
        CHECK((NEW(FunExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(NumExpr)(41), NEW(VarExpr)("x"))))->interp(Env::empty)->equals(NEW(FunVal)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(NumExpr)(41), NEW(VarExpr)("x")))));
    }

//    SECTION("subst") {
//        CHECK((NEW(FunExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(4))))->subst("x", NEW(NumExpr)(5))->equals(
//                NEW(FunExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(4)))
//        ));
//
//        CHECK((NEW(FunExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(4))))->subst("z", NEW(NumExpr)(5))->equals(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(4)))));
//    }

    CHECK((NEW(FunExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(15)))->to_string() == "(_fun (x) 15)");
}

TEST_CASE("parse fun") {
    CHECK_THROWS_WITH(parse_str("(  _fun (x) x  ) (0"), "bad input");
    CHECK_THROWS_WITH(parse_str("_fun y 1"), "bad input");
    CHECK_THROWS_WITH(parse_str("_fun (y 1"), "bad input");
    CHECK(parse_str("_fun (x) x+1")
                  ->equals(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))));
    CHECK(parse_str("(_fun (x) (_fun (y) ((x*x)+(y*y))))")
                  ->equals(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(FunExpr)(NEW(VarExpr)("y"), NEW(AddExpr)(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x")), NEW(MultExpr)(NEW(VarExpr)("y"), NEW(VarExpr)("y")))))));
}

TEST_CASE("env") {
    CHECK((NEW(EmptyEnv)())->equals(NEW(EmptyEnv)()) == true);
    CHECK((NEW(EmptyEnv)())->equals(NEW(ExtendedEnv)("x", NEW(NumVal)(1), NEW(EmptyEnv)())) == false);

    CHECK_THROWS_WITH((NEW(EmptyEnv)())->lookup("x"), "free variable: x");

    CHECK((NEW(ExtendedEnv)("x", NEW(NumVal)(1), Env::empty))->equals(NEW(ExtendedEnv)("x", NEW(NumVal)(1), Env::empty)) == true);
    CHECK((NEW(ExtendedEnv)("x", NEW(NumVal)(1), NEW(ExtendedEnv)("x", NEW(NumVal)(99), Env::empty)))->equals(NEW(ExtendedEnv)("x", NEW(NumVal)(1), Env::empty)) == false);

    CHECK_THROWS_WITH((NEW(ExtendedEnv)("x", NEW(NumVal)(1), NEW(ExtendedEnv)("y", NEW(NumVal)(99), Env::empty)))->lookup("b"), "free variable: b");
    CHECK((NEW(ExtendedEnv)("x", NEW(NumVal)(1), NEW(ExtendedEnv)("y", NEW(NumVal)(99), Env::empty)))->lookup("y")->equals(NEW(NumVal)(99)));
}
