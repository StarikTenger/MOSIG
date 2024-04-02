/* Tutorial written by Matthieu Moy */
/* modified by Olivier Muller, 2012 */
/* translated in English by Vania Marangozova 2015 */

#include <stdio.h>

void f(void);
void compute_and_print_fact(int n);
int fact(int n);

/*
  This is a step-by-step tutorial for the gdb debugger.  A
  debugger is a precious tool when trying to understand what is
  wrong with a program.  The few minutes you will spend now to
  learn how to use gdb will be repaid in your future development
  process.

  Start with the compiler. To be able to use gdb, you need to use
  the -g option:

  $ gcc -g gdb-tutorial_EN.c -o gdb-tutorial

  You can launch the executable in gdb like this:

  $ gdb ./gdb-tutorial

  Let us start with something simple: launch a program.  To do this,
  we type "run" or simply "r" in the gdb line command.
*/

int main(void) {
  unsigned i;
  int j;
  int *ptr = NULL; /* NULL is an invalid address. We do not have
                                                  the right to
                                                  dereference. */
  printf("Hello\n");
  printf("I will execute an invalid memory access\n");
  /*
     The following line will provoke a "Segmentation fault".  The
     debugger will stop at this line.
  */
  *ptr = 0;
  /*
    The bug was easy to find.
    You can correct it by commenting the offending line.
    Obviously, you need to recompile.

    Then, you can
    - either quit gdb (quit), recompile, relaunch
    gdb and continue the totorial from here
    - or continue our session by jumping over the error.  To
    move around the
    program without recompiling, use the "jump" command.  Here,
    a "jump gdb-tutorial_EN.c:43", will allow you to restart
    just after this line.

    Now we will do a step-by-step execution. You need to give
    gdb a place to stop, called a breakpoint.

    In the gdb command line, write "break gdb-tutorial_EN.c:60"
    (or "b 60") to make gdb stop at the line 61. Relaunch the
    program with "run".
  */
  printf("We continue ...\n"); /* mettre le point d'arret ici */
                               /*
                                 Now, you should have your program stopped at the previous
                                 line.  Type "next" (or "n"), multiple times, to continue
                                 with the execution.  After typing "n" once, typing Enter has
                                 the same effect as typing "n".
                               */
  printf("We continue still ...\n");
  printf("We continue a little bit more ...\n");
  printf("More ...\n");
  printf("Last time ...\n");
  printf("Ta daaa !\n");
  /*
    There are two commands to execute a program step-by-step:
    - next, to execute the next instruction
    - step, which enters the function, if the next instruction
    is a function.

    Try first "step" and then "next" on the following lines.
  */
  f();
  f();
  f();
  /*
    Ok, executing step-by-step is fine but is annoying when the
    code is big.  For example, as the following loop takes a
    long time, we can jump over it.

    To do this, define a breakpoint on the indicated line and,
    instead of "n" try "continue" (or "c").in gdb
  */
  j = 0;
  for (i = 0; i <= 1000; ++i) {
    j = j + i;
  }
  printf("End of loop\n"); /* define a breakpoint HERE */
  /*
    In gdb, you can see the value of a variable with the "print"
  (or "p") command.  You can permanently print the variable if
  you use "display".

  For example, try "display i", and then execute the following
  loop step-by-step.  Use "p j" to consult the value of j.
  */

  for (i = 1; i <= 1000; i = i * 2) {
    j = j + i;
  }
  /*
    You can also print the value of more complicated C expressions.
    Try for example

    (gdb) p i+j
    (gdb) p &i
    (gdb) p &j
    (gdb) p say_hello("Monsieur")

    (in the last case, gdb will call the say_hello function
    defined later)

    We can also have some fun with pointers. Try

    (gdb) p &i
    $2 = (unsigned int *) 0x12345678

    The 0x12345678 value, the address of i, will certainly be
    different for you.

    Now print the expression *(unsigned int *)0x12345678 (by
    replacing 0x12345678 pwth the value that you have obtained).
    You should obtain 1024, which is the value of i.
  */
  printf("i=%i\n", i);
  /*
    Things get more complicated when we deal with functions.
    Step into the follwoing function ("s" command), see you down
    there!
  */
  compute_and_print_fact(10);

  /*
    A last point : sometimes there is an infinite loop and we
    would like to locate it.

    Very simple: launch the program ("c" for "continue"), then
    type Control-c to stop its execution and get back to main in
    gdb.

    Use this technique to understand which loop is infinite and
    which is just too long.

    The "jump" command can be used to jump over a long loop.
    What you should have in mind is that the code you jumped
    over will not be executed by gdb so the execution may
    continue with some wrong values.

    Some compilers are intelligent, detect infinite loops and
    suppress the code that follows!  If you try to jump over,
    gdb will indicate that you try to jump out of the program!
  */
  j = 1;
  while (j > 0) {
    j++;
  }

  i = 10;
  while (i >= 0) {
    i--;
  }

  i = 0;
  j = 0;
  while (i == j) {
    i++;
    j++;
  }

  /*
    That's all, you have finished and you know almost everything
    you need to know in order to use gdb.  To get more details,
    you have the "help" command or the gdb manual:
    http://sourceware.org/gdb/download/onlinedocs/
  */
}

void f(void) {
  printf("Call of f()\n");
  printf("End of f()\n");
}

void compute_and_print_fact(int n) {
  /*
    Hello again,

    Here n should be equal to 10. Check with "p n".
    Step into the fact function with "step".
  */
  int res = fact(n);
  printf("fact(%i) = %i\n", n, res);
}

int fact(int n) {
  printf("Computation of  fact(%i)\n", n);
  /*
    If this is the first time that you go through here, n should
    be equal to 10.
    Check with "p n". Continue the execution step-by-step with
    "step" (and not "next", otherwise you will miss the recrsive
    calls).  Follow for example 5 calls and then read the next
    comment
  */

  /*
    Ok, let's say you ar at the 5th recursive call. So, "n"
    should be equal to 5. Check with "p n".

    In the stack, there are the other calls, with the other
    values of n, going from 10 to 5. Type "where" to see the
    state of the stack (or "backtrace").

    To navigate in the stack, use the "up" et "down" commands.
    You can execute, for example, twice "up" and then "p n". You
    should see the value of "n" during the 3d call, i.e 7.

    Execute twice "down", to come back.
    Execute "finish" to terminate the call of the current function and
    go back to "main".
  */
  if (n <= 1) {
    return 1;
  } else {
    return n * fact(n - 1);
  }
}

int say_hello(char *name) {
  printf("Hello, %s\n", name);
  return 42;
}
