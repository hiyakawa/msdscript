# MSDScript User Instructions Handbook

## Description

MSDScript is a C++ program that is able to parse and interpret functions and loops,  calculate expressions such as: assigning variables, arithmetic operations add or multiply, and logical operations boolean, if, then and else. MSDScript can be used in two modes: as a MSDScript executable or as a library to be used on other applications. 

> MSDscript is not a very rich language at the moment—it doesn't even have subtraction—it's not far from the the kinds of languages that are embedded in applications. For example, someone implementing a calendar program might want to have a language for advanced users to caculate dates for repeated meetings (say, more sophisticated than “every Tuesday”), and MSDscript could just about work for that.

## Build and Installation

MSDScript can be used in two modes: as a MSDScript executable or as a library to be used on other applications.

To use MSDScript as a part of another application, please see the library files which is provided in the zip folder called MSDScript which inside the **Achive_MSDScript_and_Documentation** folder. 

The example below will show how to utilize the library archive files and using the `which_day.cpp` as an example of embeded application. You can use the which day to test it or use your own main.cpp or embeded application.

- ##### Add the following #includes to your main executable app file: 

```
#include "../parse.h"
#include "../expr.h"
#include "../step.h"
#include "../pointer.h"//moving #includes to the correct directory by adding corrent amounts of dots to find the correct directory. So you may need the ../ or you may not need it is all depending on the location of the which_day.cpp and the files in the zip folder. 
```

- Add the following following command line to the main application:

  ```
      //Add the following command-line argument to the which_day.cpp: 
      
      PTR(Expr) y = parse_str(expr);
      PTR(Val) v;
      v =   Step::interp_by_steps(y);
      std::cout<< v->to_string() << std::endl;
  ```

- **Save** zip archive files folder called **MSDScript** which inside the **Achive_MSDScript_and_Documentation** folder in your computer directory

- In the terminal go to the directory you saved all the archive files and enter the following command below 

- ##### Create an archive file by running the following command in the terminal:

  - ###### `$ make libraryImage ` 

    - ##### the **$ make libraryImage** will create the objects files and will create the libraryImage.a file as well. 

- ##### Link the archive  libraryImage.a to your executable:

  - `$ c++ -o which_day which_day.cpp ../libraryImage.a`

  - `$ which_day.cpp 13`

    > - ##### The goal is for `./which_day 13` to evaluate the function call and print `4` (which means ”Thursday”).

- **To create the `msdscript`executable run the following:**

  - **Save** zip archive files folder called **MSDScript** which inside the **Achive_MSDScript_and_Documentation** folder in your computer directory
  - In the terminal go to the directory you saved all the archive files and enter the following command below **make** will create the **executable** and once the executable is created the **run** the ./msdscript: 
  - `$ make`
  - `$ ./msdscript`

  ### Running the MSDScript executable: 

  - ##### ***To see all the MSDScript flags available type:  `./msdscript --help`***

    ```
    You can use the following commands within this MSDScript program:
    
    $ ./msdscript --help 
    the help command will print all the commands available  
    
    $ ./msdscript --test 
    the test command will run all the available tests and display if all of them pass or fails
    
    $ ./msdscript --print 
    the print command will print parenthesis for a given expression in order of operation
    
    $ ./msdscript--interp 
    the interp command will return the values of a provided expression 
    
    $ ./msdscript --step 
    the step command will return the values of a provided expression. The steps uses the heaps instead of the stack so you do not have to worry about stack overflow.   
    ```

  - ##### To evaluate the expression 

  - ##### Run: `/.msdscript --interp`  

  - ##### Press enter and input your expression. 

  - Press ==**control d**== to print out the results.

    **Expression Rules:** 

    ```
    1. Negative: An expression cannot be negative
    2. Division or fractions: not allowed
    
    3. MSDScript is not sensitive to whitespace.
    4. MSDScript Expression available: Arithmetic operations such as ADD or MULTIPLY, Boolean, If Statement, Functions, Let Binding including loops.
    
    5. $ ./msdscript --step 
    the step command will return the values of a provided expression. The steps uses the heaps instead of the stack so you do not have to worry about stack overflow. 
    ```

    ##### List of keywords: 

    - ```
      _Let, _in: to create a let expression to bind a variable to a value.           The expression is in the format:
      _let <var> = <expr1> _in  <expr2>
      ```

    - ```
      _true, _false: it represent a boolean value true or false
      ```

    - ```
      _if, _then, _else: it evalue if an expression is true and if it is true it will evaluate _then or if it is false it will evaluate _else. Note: if the value does not evalue to a boolean it will throw a runtime_error.
      
      _if _true <expr1> _then <expr2> _else <expr3>
      ```

    - ```
      _fun: it asks for a variable which will be passed to its expression and you can bind the variable using the _let expression.
      
      _fun (<var>) <expr>
      ```

    ##### **Note:** for more details on the keywords implementation please see the API below.

    **Example when running it from the terminal:** 

  - ##### Add Expression: 

    ```
    $ `/.msdscript --interp`
    3+2 
    5
    ```

  - ##### Multiply Expression: 

    ```
    $ `/.msdscript --interp`
    3*2 
    6
    ```

  - ##### Let Expression: 

    ```
    $ `/.msdscript --interp`
    _let x = 1 _in x + 2                         
    3
    ```

  - ##### Boolean Epression: 

    ```
    $ `/.msdscript --interp`
    _if _true _then 1 _else 2
    1
    ```

  - ##### Function Expression: 

    ```
    The function below will evaluate a factoral 10!, which is 3628800:
    
    _let factrl = _fun (factrl)
                    _fun (x)
                      _if x == 1
                      _then 1
                      _else x * factrl(factrl)(x + -1)
    _in  factrl(factrl)(10)
    ```

  - ##### Loops: 

    - To evalue big operations:   `/.msdscript --step`

    ```
    _let countdown = _fun(countdown)
                       _fun(n)
                         _if n == 0
                         _then 0
                         _else countdown(countdown)(n + -1)
    _in countdown(countdown)(1000000)
    ```

  - To add the proper parenthesis for the expressions:  `/.msdscript --print`

    ```
    7*3+6
    - press control d
    ((7*3)+6
    
    ```

  - To run all MSDScript test: `/.msdscript --test`

    ```
    All tests passed (155 assertions in 17 test cases)
    ```

  ## MSDScript Language Specifications: 

  - #### pointer class:

    ##### This pointer class uses Macros to help refactor **old** pointers to **new** pointers: To use share pointers and reduce memory leaks:

    > Macro Refactoring Summary
    >
    > **old**                                          **new**
    >
    > T *.                                         PTR(T)
    >
    > newT(arg, ...)                        NEW(T)(arg, ...)        
    >
    > dynamic_cast<T>(arg)        CAST(T)(arg)
    >
    > classT { .... };                         CLASS(T) { .... };
    >
    > this                                        THIS

  - Example: 

    ```
     PTR(Expr) num1 =  NEW (NumExpr)(5); // Making a pointer to the Expr clas
       PTR(Expr) num2 = NEW (NumExpr)(2);
     CHECK(num1->to_string() == "5");
       CHECK(num2->to_string() == "2");
      
        //(5+2)*(5+2)
        //    Expr* add15 =  new Add(num1, num2);
        PTR(Expr) mult =  NEW (MultExpr)(add10, add10); //(5+2) *( 5+2)
        CHECK(mult->to_string() == "((5+2)*(5+2))");
    ```

  - #### parse class:

    ##### Parse: turns text into Expr objects

    **PTR(Expr) parse_str(std::string s);**

    The parse_str will parse a string and will return expression. If user input is not in a form of expression the parse_str will **throw** runtime error.

    Parse test example found in test.cpp: 

    ```
    // testing a number:
    CHECK( parse_str("10")->equals(NEW (NumExpr) (10))); 
    
    // testing variable:
    CHECK( parse_str("feb")->equals(NEW (VarExpr)("feb"))); 
    
    // testing adding expression:
     PTR(Expr) ten_plus_one = NEW (AddExpr)(NEW (NumExpr)(10), NEW (NumExpr)(1));
     CHECK( parse_str("10+1")->equals(ten_plus_one)); 
    
    // testing multiplying expression:
    PTR(Expr) five_mult_two = NEW (MultExpr)(NEW (NumExpr)(5), NEW (NumExpr)(2));
    CHECK( parse_str("5*2")->equals(NEW (MultExpr)(NEW (NumExpr)(5), NEW (NumExpr)(2)))); 
    
    //parse a big number so it become negative and trigger the integer over flow check with the message: "Integer Overflow...! "
    CHECK_THROWS_WITH(parse_str("100000000000000"), "Integer Overflow...! ");
    
    // testing invalid input: 
    CHECK_THROWS_WITH(parse_str(" $ "), "invalid input");
    CHECK_THROWS_WITH(parse_str(" _leete "), "invalid keyword is not a _if or _let or _true or _false or _fun" ); // "it should have an _let"
    
    // "it should have an _let"
    CHECK_THROWS_WITH(parse_str("_let x = 1 _innnN x + 2 "), "it should have an _in");//"it should have an _in"
    
    // testing a function: _let f = _fun (x) x*x_in  f(2):
    CHECK( parse_str("(_fun (x) x + 1)(10)")
              ->interp(NEW(ExtendedEnv)("x", NEW(NumVal)(10),
                NEW(EmptyEnv)()))->equals(NEW (NumVal)(11)));
                
    
    // testing mising the conditional _then: 
    // if not _then throw an error: "it should have a _then"
        CHECK_THROWS_WITH( parse_str("_if _true _car _else 2 ")->equals( (NEW (IfExpr)(NEW (BoolExpr)(true),                                                  NEW (NumExpr)(1), (NEW (NumExpr)((2)))))), "it should have a _then");
        
    CHECK_THROWS_WITH( parse_str("_if _false _then 2 _basket 1 ")->equals( (NEW (IfExpr)(NEW (BoolExpr)(false),                                                NEW (NumExpr)(2), NEW (NumExpr)((1))))), "it should have a _else");
    
    
    ```

    - Below is the parse grammar which can help understand the flow of the parse class:

      #### Parsing with Functions and Calls:

      > 〈expr〉           =〈comparg〉

    >​			              |〈comparg〉==〈expr〉
    >
    >〈comparg〉  =〈addend〉
    >
    >​						 |〈addend〉+〈comparg〉
    >
    >〈addend〉   =〈multicand〉
    >
    >​						|〈multicand〉*〈addend〉
    >
    >〈multicand  =〈inner〉
    >
    >​						|〈multicand〉(〈expr〉)
    >
    >〈inner〉      =〈number〉|(〈expr〉)|〈variable〉
    >
    >​				       |   _let〈variable〉=〈expr〉_in〈expr〉
    >
    >​				       |  _true|_false
    >
    >​				       | _if〈expr〉_then〈expr〉__else〈expr〉_
    >
    >​                       | _fun(〈variable〉)〈expr

  - #### catch class:

    The catch.h class extends to the test.cpp class and help with the test generation for the MSDScript

  - #### cmdline class:

    **The cmdline contain the arguments for the MSDScript such as:** 

    **--help:** 
    the help command will print all the commands available  

    **--test:** 
    the test command will run all the available tests and display if all of them pass or fails

    **--print:** 
    the print command will print parenthesis for a given expression in order of operation

    **--interp:** 
    the interp command will return the values of a provided expression 

    **--step:** 
    the step command will return the values of a provided expression. The steps uses the heaps instead of the stack so you do not have to worry about stack overflow.  

  - #### cont class: 

    Is the parent class for the continuation subclasses and methods

    ##### Functions and properties: 

    - virtual **void** step_continue()

      > step_continue` method sets the mode to either `interp_mode` or `continue_mode

    - static PTR(Cont) done;

      used to show there is no more steps 

    - #### subclasses and definitions below: 

      - ##### Done: 

        Used to suggest there are no more steps in evaluating an Expression extends from parent class Cont. 

        Also, it throws the following runtime error: **throw** std::runtime_error("can't continue done");

      - ##### IfBranchCont:

        It indicate the  final step in evaluating a CallExpr and extends from parent class Cont

        ##### Functions and properties: 

        PTR(Expr) then_part: Expression from IfExpr  

        PTR(Expr) else_part: Expression from IfExpr  

        PTR(Env) env: Environment remembered from previous step 
        PTR(Cont) rest: Continuation from previous step

        IfBranchCont(PTR(Expr) then_part, PTR(Expr) else_part, PTR(Env) env, PTR(Cont) rest);

        Also, step_continue() calls Val::is_true() to determine true or false

      - ##### LetBodyCont:

        It used to evaluate a LetExpr and it extends from the parent class Cont.

        ##### Functions and properties: 

         std::string var; - used for the left hand side definition of var

          PTR(Expr) body; - to utilize the declared var

          PTR(Env) env; - the environment stored from the prior step

          PTR(Cont) rest; - continuation from the prior step

        LetBodyCont(std::string var, PTR(Expr) body, PTR(Env) env, PTR(Cont) rest);

        Also, step_continue() need to create a NEW(ExtendedEnv) to keep track of var and Step::val and it extends from all the virtual methods the parent class Cont.

      - ##### ArgThenCallCont:

        It indicates a step in the evaluation of a CallExpr and it extends from the parent class Cont.

        ##### Functions and properties: 

         PTR(Expr) actual_arg; - the expression from CallExpr

          PTR(Env) env; - the environment stored from the prior step

          PTR(Cont) rest; - continuation from the prior step

        Also, ArgThenCallCont(PTR(Expr) actual_arg, PTR(Env) env, PTR(Cont) rest); it extends from all the virtual methods the parent class Cont.

      - ##### CallCont

        It indicates the final step in the evaluation of a CallExpr and it extends from the parent class Cont.

        ##### Functions and properties:

        PTR(Val) to_be_called_val; - the value set in Val::call_step(PTR(Val) actual_arg_val, PTR(Cont) rest);

        PTR(Cont) rest; - PTR(Cont) rest: is a continuation from the ArgThenCallCont::step_continue()

        Also, CallCont(PTR(Val) to_be_called_val, PTR(Cont) rest); it extends from all the virtual methods the parent class Cont.

      - ##### RightThenAddCont

        It indicates a step in the evaluation of an AddExpr and it extends from the parent class Cont.

        ##### Functions and properties:

        PTR(Expr) rhs; - the value set in AddExpr::step_interp()

         PTR(Env) env; - the environment stored from the prior step

         PTR(Cont) rest; -  continuation from the prior step

        Also, RightThenAddCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest); and it extends from all the virtual methods the parent class Cont.

      - ##### AddCont:

        It indicates the final step in the evaluation of a AddExpr it extends from the parent class Cont.

        ##### Functions and properties:

          PTR(Val) lhs_val; -  the value set in AddCont::step_continue()

          PTR(Cont) rest; -  continuation from the prior step

          Also,  AddCont(PTR(Val) lhs_val, PTR(Cont) rest); it extends from all the virtual methods the parent class Cont.

      - ##### RightThenMultCont

        It indicates a step in the evaluation of an MultExpr and it extends from the parent class Cont.

        ##### Functions and properties:

        PTR(Expr) rhs; - the value set in AddExpr::step_interp()

         PTR(Env) env; - the environment stored from the prior step

         PTR(Cont) rest; -  continuation from the prior step

          Also,  RightThenMultCont(PTR(Val) lhs_val, PTR(Cont) rest); it extends from all the virtual methods the parent class Cont.

      - ##### MultCont

        It indicates the final step in the evaluation of a MultExpr it extends from the parent class Cont.

        ##### Functions and properties:

          PTR(Val) lhs_val; -  the value set in AddCont::step_continue()

          PTR(Cont) rest; -  continuation from the prior step

          Also,  MultCont(PTR(Val) lhs_val, PTR(Cont) rest); it extends from all the virtual methods the parent class Cont.

      - ##### RightThenEqCont

        It indicates a step in the evaluation of an EqExpr and it extends from the parent class Cont.

        ##### Functions and properties:

        PTR(Expr) rhs; - the value set in AddExpr::step_interp()

         PTR(Env) env; - the environment stored from the prior step

         PTR(Cont) rest; -  continuation from the prior step

          Also,  RightThenEqCont(PTR(Val) lhs_val, PTR(Cont) rest); it extends from all the virtual methods the parent class Cont.

      - ##### EqCont

        It indicates the final step in the evaluation of a EqExpr it extends from the parent class Cont.

        ##### Functions and properties:

          PTR(Val) lhs_val; -  the value set in AddCont::step_continue()

          PTR(Cont) rest; -  continuation from the prior step

          Also,  EqCont(PTR(Val) lhs_val, PTR(Cont) rest); it extends from all the virtual methods the parent class Cont.

    

  - #### env class:

    Is the parent class for the environment subclasses and methods

  - ##### Subclasses:

    - EmptyEnv
    - ExtendedEnv

  - ##### Functions and properties:

    ***virutal* PTR(Val) lookup(std::string find_name);** - will search in the present environment to look the variable that match - find_name that are called in VarExpr::**interp**(PTR(Env) env). It It will return the value assigned to find_name. It will throw a runtime error if search for Env::EmptyEnv.  

    **static PTR(Env) empty**

  - ##### EmptyEnv:

    It is an empty environment and it extends from the parent class Env.

    ##### Functions and properties:

    It extends from all the virtual methods the parent class Env.

    lookup(std::string find_name) will throw the following error:  **throw** std::runtime_error("free variable: " + find_name).

  - **ExtendedEnv:**

    Is an environment which has a new variable and will take all the variable that has been defined previously.

    ##### Functions and properties:

    string name: new variable name  

    PTR(Val) val: the value assigned for thenew variable  

    PTR(Env) rest: it is the previous Environment

    Also,  ExtendedEnv(std::string name, PTR(Val) val, PTR(Env) rest); it extends from all the virtual methods the parent class Env.

    

  - #### Expr class: 

    It is the parent class for all the expression subclasses and methods 

    

    - Below is the expression grammar: 

      ##### Backus - Naur Form (BNF) Grammar:

      ```
      Expressions grammar:
      
      〈expr〉=〈number〉
             |〈boolean〉
             |〈expr〉==〈expr〉
             |〈expr〉+〈expr〉
             |〈expr〉*〈expr〉
             |〈expr〉(〈expr〉)
             |〈variable〉
             |_let〈variable〉=〈expr〉_in〈expr〉
             |_if〈expr〉_then〈expr〉_else〈expr〉
             |_fun(〈variable〉)〈expr〉
      
      ```

  - ##### Subclasses:

  - NumExpr

  - VarExpr

  - AddExpr

  - MultExpr

  - LetExpr

  - IfExpr

  - BoolExpr

  - EqExpr

  - FunExpr

  - CallExpr

    ##### Funtions: 

    - virtual **bool** equals(PTR(Expr) other); -- compare two expression and see if the are identical.

    - virtual **PTR(Val)** interp(PTR(Env) env); -- it evaluate an expression and return the value. 

    - **void** print(std::ostream &stream); -- it will print the expression to the stream.

    - virtual **void** pretty_print_at(std::ostream &stream, print_mode_t prettyValue);  it will print the expression to the stream but add the extract space around the add or multiply signs and It takes print group.

    - virtual **void** step_interp(); evaluete by using it own continuations instead of using the C++ stack.

    - **void** pretty_print(std::ostream &stream); -- it is a driver method for the pretty_print_at.

    - std::string to_string(); --  it converts an expression to a string

    - std::string to_string_pretty(); -- it provides better indentation that to_string() method.

      - Please see sample below found in test.cpp of true and false using the equals method: 

        virtual **bool** equals(PTR(Expr) other); -- compare two expression and see if the are identical:

    ```
      // true
        PTR(Expr) num44 =  NEW (NumExpr)(23); // creating new Num object to assign           
        memory for that object Making a pointer to the Expr class
        PTR(NumExpr) num45 = NEW (NumExpr)(23);
        CHECK(num45->equals(num44 ) == true);
        
      //false
        PTR(Expr) variable40 =  NEW (VarExpr)("this is variable"); // creating new    
        Variable  object to assign memory for that object Making a pointer to the  
        Expr class
        PTR(VarExpr) variable41 = NEW (VarExpr)("variable2");
        CHECK(variable41->equals(variable40 ) == false);
    ```

    - Interp example: 

      virtual **PTR(Val)** interp(PTR(Env) env); -- it evaluate an expression and return the value. 

      ```
        // 1 == 1
        // x == y
        PTR(EqExpr) equalexpr = NEW (EqExpr)(NEW (NumExpr) (1), NEW (NumExpr) (1) );
          CHECK(equalexpr->interp(Env::empty)->equals(NEW (BoolVal)(true)) == true);
          
          PTR(EqExpr) equalexpr2 = NEW (EqExpr)(NEW (NumExpr) (1), NEW (NumExpr) (3) );
          CHECK(equalexpr2->interp(Env::empty)->equals(NEW (BoolVal)(false)) == true);
      ```

      std::string **to_string_pretty**(); -- it provides better indentation that to_string() method and will return the string representation of the present expression. See example below: 

      ```
       // pretty_print:
          PTR(EqExpr) e1 = NEW (EqExpr)(NEW (VarExpr)("a"), NEW (NumExpr)(1));
          PTR(AddExpr) a1 = NEW (AddExpr)(e1, NEW (NumExpr)(2));
          PTR(MultExpr) m1 = NEW (MultExpr)(NEW (NumExpr)(1), a1);
          CHECK( m1->to_string_pretty() == "1 * ((a == 1) + 2)" );
      ```

      std::string **to_string()**; -- it provides better indentation that to_string() method and will return the string representation of the present expression. See example below: 

      ```
       PTR(IfExpr) e1 = NEW (IfExpr)(NEW (BoolExpr)(true), NEW (NumExpr)(1), NEW (NumExpr)(4));
          CHECK( e1->to_string() == "(_if _true _then 1 _else 4)");
      ```

      

  - #### step class:

    The step it is used for the continuation mode. Also the step class uses the heap instead of the stack which prevents stack overflow.

    ##### Functions and properties:

      **static** mode_t mode;  /* choosesmode */

      **static** PTR(Expr) expr; /* forinterp_mode */

      **static** PTR(Env) env; /* forinterp_mode */

      **static** PTR(Val) val; /* forcontinue_mode */

      **static** PTR(Cont) cont; /* allmodes */

      **static** PTR(Val) interp_by_steps(PTR(Expr) e); -- it initialize the global variables and it   	calcualte the provided expression to interp as the 'e' expression and will return the value of e. 

    

  - #### test class:

    The test.cpp class contain all the test for the entire MSDScript program and uses the Catch.h class

    

  - #### val class: 

    The Val class is the parent class for all the subclasses and methods that extends from this class

  - ##### Subclasses:

    - **NumVal:**

      It is a number value that extends from the parent class Val

      ##### Functions and properties: 

       int val - val mean number value. 

      NumVal(**int** val) and it extends for all the  vitual methods in Val

      NumVal::is_true() -- will **throw** std::runtime_error("NumVal is not a Bool value");

      NumVal::add_to(PTR(Val) other )

      NumVal::mult_to(PTR(Val) other)

      NumVal::call(PTR(Val) actual_arg) - will **throw** std::runtime_error("unable to call a number");

      NumVal::call_step(PTR(Val) actual_arg_val, PTR(Cont) rest) - will **throw** std::runtime_error("unable to call a number");

       

    - **BoolVal**

      It is a boolean value that extends from the parent class Val

      ##### Functions and properties: 

      BoolVal(**int** val) and it extends for all the  vitual methods in Val

      BoolVal::is_true() -- will **throw** std::runtime_error("NumVal is not a Bool value");

      BoolVal::add_to(PTR(Val) other )

      BoolVal::mult_to(PTR(Val) other)

      BoolVal::call(PTR(Val) actual_arg) - will **throw** std::runtime_error("unable to call a number");

      BoolVal::call_step(PTR(Val) actual_arg_val, PTR(Cont) rest) - will **throw** std::runtime_error("unable to call a number");

    - **FunVal**

      It is a value representation of a FunExpr that extends from the parent class Val

      ##### Functions and properties: 

      **this**->formal_arg = formal_arg; -  in the function substitute the formal_arg

      **this**->body = body; - it is computed in the function 

      **this**->env = env; - it is the environment of values stored previously.

      FunVal::is_true() -- will **throw** std::runtime_error("NumVal is not a Bool value");

      FunVal::add_to(PTR(Val) other )

      FunVal::mult_to(PTR(Val) other)

      FunVal::call(PTR(Val) actual_arg) - will **throw** std::runtime_error("unable to call a number");

  - ##### Functions and properties: 

    **bool** equals(PTR(Val) other);

    PTR(Val) add_to(PTR(Val) other);

    PTR(Val) mult_to(PTR(Val) other);

    PTR(Val) call(PTR(Val) actual_arg);

    std::string to_string(); // add the to_string

    **bool** is_true(); // IS_TRUE

    **void** call_step(PTR(Val) actual_arg_val, PTR(Cont) rest);

  - **bool** equals(PTR(Val) other); - check if two values are equals and return a boolean value

    - Equal example below: 

      ```
      CHECK((NEW(NumVal)(1))->equals(NULL) == false);
      CHECK((NEW(NumVal)(2))->equals(NEW(NumVal)(2)) == true);
       //"NumVal is not a Bool value"
      CHECK_THROWS_WITH((NEW(NumVal)(1))->is_true(), "NumVal is not a Bool value");
          
      ```

    - PTR(Val) add_to(PTR(Val) other); -- addition of two values and returns result of the addition. 

      ```
       //check is adding two numbers: 
       CHECK_THROWS_WITH( (NEW(MultExpr)(NEW(NumExpr)(1),NEW(NumExpr)(4)))-> interp(Env::empty)->add_to(NULL),"add of non-number");
       
      CHECK_THROWS_WITH((NEW (FunVal)("x", NEW (NumExpr)(1), NEW(EmptyEnv)()))->add_to(NEW (FunVal)("x", NEW (NumExpr)(1), NEW(EmptyEnv)())), "unable to add two functions...");
      ```

      - PTR(Val) mult_to(PTR(Val) other); --

        ```
        //"mult of non-number"
            CHECK_THROWS_WITH( (NEW(AddExpr)(NEW(NumExpr)(2),NEW(NumExpr)(5)))->interp(Env::empty)->mult_to(NULL),"mult of non-number");
        ```

      - **bool** is_true(); // IS_TRUE

        check if the value is a boolean 

        see example: 

        ```
         //"NumVal is not a Bool value"
        CHECK_THROWS_WITH((NEW(NumVal)(1))->is_true(), "NumVal is not a Bool value");
        ```

    - **void** call_step(PTR(Val) actual_arg_val, PTR(Cont) rest);

      Will do the portion call of the FunExp and will return the value of the call function expression.

      see example: 

      ```
      //"NumVal unable to call a number"
      CHECK_THROWS_WITH((NEW(NumVal)(1))->call(NEW(NumVal)(2)), "unable to call a number");
      ```

      

## API Documentation:

#### LIST OF ALL THE FUNCTIONS:

### CLASS (Expr): 

#### Description: 

##### Expr:  it is an arithmetic expression that can be evaluated. 

#### Method Summary:

##### **Abstract Methods**:

**bool** equals(PTR(Expr) other); -- compare two expression and see if the are identical.

**PTR(Val)** interp(PTR(Env) env); -- it evaluate an expression and return the value. 

**bool** has_variable(); -- check if an expression has a variable. 

**void** print(std::ostream &stream); -- it will print the expression to the stream.

**void** pretty_print_at(std::ostream &stream, print_mode_t prettyValue);  it will print the expression to the stream but add the extract space around the add or multiply signs and It takes print group.

**void** step_interp(); evaluete by using it own continuations instead of using the C++ stack.

#### Implemented Method: 

##### **void** pretty_print(std::ostream &stream); -- it is a driver method for the pretty_print_at.

##### std::string to_string(); --  it converts an expression to a string

##### std::string to_string_pretty(); -- it provides better indentation that to_string() method.



## **class** NumExpr : **public** Expr; 

#### Description: 

##### NumExpr:  It represent a number. 

### Constructor Summary:

  NumExpr(**int** val); 

#### Method Summary:

##### **Abstract Methods**:

**bool** equals(PTR(Expr) other); -- compare two expression and see if the are identical.

**PTR(Val)** interp(PTR(Env) env); -- it evaluate an expression and return the value. 

**bool** has_variable(); -- check if an expression has a variable. 

**void** print(std::ostream &stream); -- it will print the expression to the stream.

**void** pretty_print_at(std::ostream &stream, print_mode_t prettyValue);  it will print the expression to the stream but add the extract space around the add or multiply signs and It takes print group.

**void** step_interp(); evaluete by using it own continuations instead of using the C++ stack.

#### Implemented Method: 

##### **void** pretty_print(std::ostream &stream); -- it is a driver method for the pretty_print_at.

##### std::string to_string(); --  it converts an expression to a string

##### std::string to_string_pretty(); -- it provides better indentation that to_string() method.



## **class** VarExpr : **public** Expr;

#### Description: 

##### VarExpr:  It represent a string. 

### Constructor Summary:

**VarExpr**(std::string val);

### Method Summary:

**bool** equals(PTR(Expr) other); -- compare two expression and see if the are identical.

**PTR(Val)** interp(PTR(Env) env); -- it evaluate an expression and return the value. 

**bool** has_variable(); -- check if an expression has a variable. 

**void** print(std::ostream &stream); -- it will print the expression to the stream.

**void** pretty_print_at(std::ostream &stream, print_mode_t prettyValue);  it will print the expression to the stream but add the extract space around the add or multiply signs and It takes print group.

**void** step_interp(); evaluete by using it own continuations instead of using the C++ stack.

##### **void** pretty_print(std::ostream &stream); -- it is a driver method for the pretty_print_at.

##### std::string to_string(); --  it converts an expression to a string

##### std::string to_string_pretty(); -- it provides better indentation that to_string() method.



## **class** AddExpr : **public** Expr;

#### Description: 

##### AddExpr:  It represent two expression added together lhs + rhs. 

### Constructor Summary:

**AddExpr**(PTR(Expr) lhs, PTR(Expr) rhs);

### Method Summary:

**bool** equals(PTR(Expr) other); -- compare two expression and see if the are identical.

**PTR(Val)** interp(PTR(Env) env); -- it evaluate an expression and return the value. 

**bool** has_variable(); -- check if an expression has a variable. 

**void** print(std::ostream &stream); -- it will print the expression to the stream.

**void** pretty_print_at(std::ostream &stream, print_mode_t prettyValue);  it will print the expression to the stream but add the extract space around the add or multiply signs and It takes print group.

**void** step_interp(); evaluete by using it own continuations instead of using the C++ stack.

##### **void** pretty_print(std::ostream &stream); -- it is a driver method for the pretty_print_at.

##### std::string to_string(); --  it converts an expression to a string

##### std::string to_string_pretty(); -- it provides better indentation that to_string() method.



## **class** MultExpr : **public** Expr;

#### Description: 

##### MultExpr:  It represent two expression multiply together lhs * rhs. 

### Constructor Summary:

**MultExpr**(PTR(Expr) lhs, PTR(Expr) rhs);

### Method Summary:

**bool** equals(PTR(Expr) other); -- compare two expression and see if the are identical.

**PTR(Val)** interp(PTR(Env) env); -- it evaluate an expression and return the value. 

**bool** has_variable(); -- check if an expression has a variable. 

**void** print(std::ostream &stream); -- it will print the expression to the stream.

**void** pretty_print_at(std::ostream &stream, print_mode_t prettyValue);  it will print the expression to the stream but add the extract space around the add or multiply signs and It takes print group.

**void** step_interp(); evaluete by using it own continuations instead of using the C++ stack.

##### **void** pretty_print(std::ostream &stream); -- it is a driver method for the pretty_print_at.

##### std::string to_string(); --  it converts an expression to a string

##### std::string to_string_pretty(); -- it provides better indentation that to_string() method.



## **class** LetExpr : **public** Expr;

#### Description: 

##### LetExpr:  It allows a variable to have a value assign to that variable;  

### Constructor Summary:

  **LetExpr**(PTR(VarExpr) lhs, PTR(Expr) rhs, PTR(Expr) body);

### Method Summary:

**bool** equals(PTR(Expr) other); -- compare two expression and see if the are identical.

**PTR(Val)** interp(PTR(Env) env); -- it evaluate an expression and return the value. 

**bool** has_variable(); -- check if an expression has a variable. 

**void** print(std::ostream &stream); -- it will print the expression to the stream.

**void** pretty_print_at(std::ostream &stream, print_mode_t prettyValue);  it will print the expression to the stream but add the extract space around the add or multiply signs and It takes print group.

**void** step_interp(); evaluete by using it own continuations instead of using the C++ stack.

##### **void** pretty_print(std::ostream &stream); -- it is a driver method for the pretty_print_at.

##### std::string to_string(); --  it converts an expression to a string

##### std::string to_string_pretty(); -- it provides better indentation that to_string() method.



## **class** IfExpr : **public** Expr;

#### Description: 

##### IflExpr:  it evaluate Bool Expression and determine weather it is true or false;  

### Constructor Summary:

  **IfExpr**(PTR(Expr) _if, PTR(Expr) _then, PTR(Expr) _else);

### Method Summary:

**bool** equals(PTR(Expr) other); -- compare two expression and see if the are identical.

**PTR(Val)** interp(PTR(Env) env); -- it evaluate an expression and return the value. 

**bool** has_variable(); -- check if an expression has a variable. 

**void** print(std::ostream &stream); -- it will print the expression to the stream.

**void** pretty_print_at(std::ostream &stream, print_mode_t prettyValue);  it will print the expression to the stream but add the extract space around the add or multiply signs and It takes print group.

**void** step_interp(); evaluete by using it own continuations instead of using the C++ stack.

##### **void** pretty_print(std::ostream &stream); -- it is a driver method for the pretty_print_at.

##### std::string to_string(); --  it converts an expression to a string

##### std::string to_string_pretty(); -- it provides better indentation that to_string() method.



## **class** BoolExpr : **public** Expr;

#### Description: 

##### BoolExpr:  it evaluate to express to either true or false;  

### Constructor Summary:

 **BoolExpr**(**bool** rep);

### Method Summary:

**bool** equals(PTR(Expr) other); -- compare two expression and see if the are identical.

**PTR(Val)** interp(PTR(Env) env); -- it evaluate an expression and return the value. 

**bool** has_variable(); -- check if an expression has a variable. 

**void** print(std::ostream &stream); -- it will print the expression to the stream.

**void** pretty_print_at(std::ostream &stream, print_mode_t prettyValue);  it will print the expression to the stream but add the extract space around the add or multiply signs and It takes print group.

**void** step_interp(); evaluete by using it own continuations instead of using the C++ stack.

##### **void** pretty_print(std::ostream &stream); -- it is a driver method for the pretty_print_at.

##### std::string to_string(); --  it converts an expression to a string

##### std::string to_string_pretty(); -- it provides better indentation that to_string() method.





## **class** EqExpr : **public** Expr;

#### Description: 

##### EqExpr:  it evaluate if the two expressions are equals to each other.  

### Constructor Summary:

**EqExpr** (PTR(Expr) lhs, PTR(Expr) rhs);

### Method Summary:

**bool** equals(PTR(Expr) other); -- compare two expression and see if the are identical.

**PTR(Val)** interp(PTR(Env) env); -- it evaluate an expression and return the value. 

**bool** has_variable(); -- check if an expression has a variable. 

**void** print(std::ostream &stream); -- it will print the expression to the stream.

**void** pretty_print_at(std::ostream &stream, print_mode_t prettyValue);  it will print the expression to the stream but add the extract space around the add or multiply signs and It takes print group.

**void** step_interp(); evaluete by using it own continuations instead of using the C++ stack.

##### **void** pretty_print(std::ostream &stream); -- it is a driver method for the pretty_print_at.

##### std::string to_string(); --  it converts an expression to a string

##### std::string to_string_pretty(); -- it provides better indentation that to_string() method.



## **class** FunExpr : **public** Expr;

#### Description: 

##### FunExpr: has variable that will be passed to the expression then it is called by the Call Expression;  

### Constructor Summary:

 **FunExpr**(std::string formal_arg, PTR(Expr) body);

### Method Summary:

**bool** equals(PTR(Expr) other); -- compare two expression and see if the are identical.

**PTR(Val)** interp(PTR(Env) env); -- it evaluate an expression and return the value. 

**bool** has_variable(); -- check if an expression has a variable. 

**void** print(std::ostream &stream); -- it will print the expression to the stream.

**void** pretty_print_at(std::ostream &stream, print_mode_t prettyValue);  it will print the expression to the stream but add the extract space around the add or multiply signs and It takes print group.

**void** step_interp(); evaluete by using it own continuations instead of using the C++ stack.

##### **void** pretty_print(std::ostream &stream); -- it is a driver method for the pretty_print_at.

##### std::string to_string(); --  it converts an expression to a string

##### std::string to_string_pretty(); -- it provides better indentation that to_string() method.



## **class** CallExpr : **public** Expr;

#### Description: 

##### CallExpr: will execute the expression initialized by the FunExpr. 

### Constructor Summary:

**CallExpr**(PTR(Expr) to_be_called, PTR(Expr) actual_arg);

### Method Summary:

**bool** equals(PTR(Expr) other); -- compare two expression and see if the are identical.

**PTR(Val)** interp(PTR(Env) env); -- it evaluate an expression and return the value. 

**bool** has_variable(); -- check if an expression has a variable. 

**void** print(std::ostream &stream); -- it will print the expression to the stream.

**void** pretty_print_at(std::ostream &stream, print_mode_t prettyValue);  it will print the expression to the stream but add the extract space around the add or multiply signs and It takes print group.

**void** step_interp(); evaluete by using it own continuations instead of using the C++ stack.

##### **void** pretty_print(std::ostream &stream); -- it is a driver method for the pretty_print_at.

##### std::string to_string(); --  it converts an expression to a string

##### std::string to_string_pretty(); -- it provides better indentation that to_string() method.


# Class Parse;

#### Description: 

##### Parse: turns text into Expr objects

### Method Summary:

**PTR(Expr) parse_str(std::string s);**

The parse_str will parse a string and will return expression. If user input is not in a form of expression the parse_str will **throw** runtime error.

#### Makefile commands:

- ##### To build a **libraryImage**:

  - `$make libraryImage `
  - `$ c++ -o which_day which_day.cpp ../libraryImage.a`

- ##### To build a msdscript executable:

  - **Save all the files found on the MSDScript` zip file into a directory on your computer.**

  - `$ make`

  - `$./msdscript`

#### License:

- Please use the MIT license:

```
The MIT License (X11 License)[6] is a permissive free software license originating at the Massachusetts Institute of Technology (MIT)[7] in the late 1980s.[8] As a permissive license, it puts only very limited restriction on reuse and has, therefore, high license compatibility. https://en.wikipedia.org/wiki/MIT_License
```
