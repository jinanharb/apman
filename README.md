# Slightly Involved C Programming: Course handout and code+report skeleton

Always stay in the directory of this `README.md` file!

Required software may be installed with `make first-install`.

Bottom line: 
- Always work with the debugger until your tests pass (or equivalently, debug from VSCode):  `make test/props/foo.test@debug`;
- Then make final checks right in the terminal for memory leaks and pre-/post-conditions:
  `make test/props/foo.test@leaks`;

# Using the `Makefile`

Use `make` to generate tests, benchmarks and your report in `report/report.pdf`. 

Use `make course` to produce the course handout in `course/course.pdf`. Demos are compiled and executed.

Use `make test/props/foo.test` to only compile the test in `test/props/foo.c`. 

Use `make test/props/foo.test@debug` same as above + launch in `gdb`.

Use `make tests` to only compile all tests. 

Use `make checks` to compile *and execute* all tests. 

Use `make report` to compile your report.

See the `Makefile` for more details and other useful targets. 

You must read *both* documents to understand what to do and how to start. 


# VSCode integration

Launch a test in a debugger *after* bringing the focus to its window. 

Choose `>>LE DÉBOGUEUR QUI VA BIEN<<` from the list. 

Internally, VSCode integration will use the same configs than in the terminal:
- `.env.gdb` for debugging;
- `Makefile` for compiling. 


# Caveat

## While running a test inside `gdb` (from terminal or VSCode)

- `AddressSanitizer` cannot detect memory leaks (shared use of `ptrace()` by `LSAN`);
- `test_abort()`'s (pre/post-conditions) cannot be tested (shared `SIGABRT` by `unitest`);
- Test will abort for manual debugging on first glitch detected by `AddressSanitizer`.

## While running a test outside `gdb` (from the terminal)

- Memory leaks are eventually reported by `LSAN` (results may differ with `valgrind`);
- `test_abort()`'s are executed;
- Tests may continue even if `Addresssanitizer` screams about unhappiness.



-- Happy hacking!
