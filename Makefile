HEAD=ellul
VERS_MAJ=0
VERS_MIN=1

CFLAGS_LIB=-Ilib$(HEAD)/include
LDFLAGS_LIB=-lm -ldl

CFLAGS+=-Wall -Wextra -Wundef -Werror=uninitialized -Werror=implicit -D_XOPEN_SOURCE=700 -D_XOPEN_SOURCE_EXTENDED -Wno-address

# Debug code: un/set USE_ASAN to 'yes' to disable/enable AddressSanitizer code instrumentation
# (not compatible with Valgrind).
#
# In case of multiple DEADLYSIGNAL messages at runtime (on Linux), you may need to:
#   sudo sysctl vm.mmap_rnd_bits=28
#
USE_ASAN=yes

ifeq ($(USE_ASAN),yes)
ifeq ($(shell uname -s),Linux)
CFLAGS_ASAN=-fno-omit-frame-pointer -fno-common -fsanitize=undefined,leak,address -fsanitize-recover=all
LDFLAGS_ASAN=-lasan -lubsan
else # Assume everything else is Apple (no leak check on this target)
CFLAGS_ASAN=-fno-omit-frame-pointer -fno-common -fsanitize=undefined,address -fsanitize-recover=all
LDFLAGS_ASAN=-fsanitize=undefined,address
endif
endif

all: checks benchmarks report 

CFLAGS_DBG=$(CFLAGS) -ggdb3 -gdwarf-4 $(CFLAGS_ASAN) #-DMEM_TRACE
CFLAGS_REL=$(CFLAGS) -O3 -march=native -DNDEBUG


# Build release and debug versions of lib
LIB_SRC=$(wildcard lib$(HEAD)/src/*.c)
LIB_OBJ_DBG=$(LIB_SRC:.c=.o.dbg)
LIB_OBJ_REL=$(LIB_SRC:.c=.o.rel)
CFLAGS_LIB_DBG=$(CFLAGS_DBG) -fpic
CFLAGS_LIB_REL=$(CFLAGS_REL) -fpic


lib$(HEAD)-config : lib$(HEAD)/config.c # Handle flags externally
	$(CC) $(CFLAGS_LIB) -DLINK="$(LDFLAGS_LIB)" -DHEAD=$(HEAD) -DVERS="$(VERS_MAJ).$(VERS_MIN)" $< -o $@

%.o.dbg : %.c
	$(CC) `./lib$(HEAD)-config --flags --local` $(CFLAGS_LIB_DBG) $< -c -o $@

%.o.rel : %.c
	$(CC) `./lib$(HEAD)-config --flags --local` $(CFLAGS_LIB_REL) $< -c -o $@

lib$(HEAD)-debug.so : $(LIB_OBJ_DBG)
	$(CC) -shared -o $@.$(VERS_MAJ).$(VERS_MIN) $^ $(LDFLAGS_ASAN)
	ln -sf $@.$(VERS_MAJ).$(VERS_MIN) $@.$(VERS_MAJ)
	ln -sf $@.$(VERS_MAJ) $@

lib$(HEAD).so : $(LIB_OBJ_REL)
	$(CC) -shared -o $@.$(VERS_MAJ).$(VERS_MIN) $^
	ln -sf $@.$(VERS_MAJ).$(VERS_MIN) $@.$(VERS_MAJ)
	ln -sf $@.$(VERS_MAJ) $@

lib-debug : lib$(HEAD)-config lib$(HEAD)-debug.so

lib : lib$(HEAD)-config lib$(HEAD).so

lib-clean :
	find . -name "*.o.dbg" -delete
	find . -name "*.o.rel" -delete
	$(RM) lib$(HEAD)*so*
	$(RM) lib$(HEAD)-config


# Preprocessor tests
TESTS_PREPR_SRC=$(wildcard test/prepr/*.c)
TESTS_PREPR=$(TESTS_PREPR_SRC:.c=.prepr)

%.prepr : %.c # Check compilation, save preprocessor output beforehand
	$(CC) `./lib$(HEAD)-config --flags --local` $(CFLAGS) $< -E > $@ && $(CC) `./lib$(HEAD)-config --flags --local` $(CFLAGS) $< && $(RM) a.out

prepr-tests: $(TESTS_PREPR)

# Properties tests (and howto)
TESTS_PROPS_SRC=$(wildcard test/props/*.c)
TESTS_HOWTO_SRC=$(wildcard test/howto/*.c)
TESTS_PROPS=$(TESTS_PROPS_SRC:.c=.test)
TESTS_HOWTO=$(TESTS_HOWTO_SRC:.c=.test)

%.o.test : %.c # Compile test
	$(CC) `./lib$(HEAD)-config --flags --local` $(CFLAGS_DBG) $< -c -o $@

%.test : %.o.test lib-debug # Link test
	$(CC) `./lib$(HEAD)-config --link --local` $< -o $@ $(LDFLAGS_ASAN) `./lib$(HEAD)-config --link --debug`

%@debug : % # Run test in debugger
	gdb -x .env.gdb -ex r --args $< -g -v

%@leaks : % # Run test in terminal
	./$< -v

tests: $(TEST_HOWTO) $(TESTS_PROPS)

checks: $(TESTS_PROPS)
	make -s tests
	for test in $^ ; do \
		./$$test -v;\
	done

# Benchmarks
BENCH_SRC=$(wildcard bench/*.c)
BENCHMARKS=$(BENCH_SRC:.c=.csv)

%.o.bench : %.c
	$(CC) `./lib$(HEAD)-config --flags --local` $(CFLAGS_REL) $< -c -o $@

%.bench : %.o.bench # Compile benchmark
	$(CC) `./lib$(HEAD)-config --link --local` $< -o $@ `./lib$(HEAD)-config --link`

bench/%.csv : bench/%.bench bench/%.plt bench/benchmark.h
	./$< > $@
	gnuplot -c $(word 2,$^)

benchmarks: lib $(BENCHMARKS) bench/hashtable_benchmarks bench/common.plt bench/benchmark.h bench/array_benchmarks
	@echo "All plots generated"

benchmarks-clean:
	$(RM) bench/*.csv bench/*.png


#added lines for our own benchmarks
bench/bench_linear.bench: bench/bench_linear.o.bench
	$(CC) `./lib$(HEAD)-config --link --local` $< -o $@ `./lib$(HEAD)-config --link`

bench/bench_robinhood.bench: bench/bench_robinhood.o.bench
	$(CC) `./lib$(HEAD)-config --link --local` $< -o $@ `./lib$(HEAD)-config --link`

bench/bench_closed.bench: bench/bench_closed.o.bench
	$(CC) `./lib$(HEAD)-config --link --local` $< -o $@ `./lib$(HEAD)-config --link`

bench/bench_linear.csv: bench/bench_linear.bench bench/benchmark.h
	./$< > $@

bench/bench_robinhood.csv: bench/bench_robinhood.bench bench/benchmark.h
	./$< > $@

bench/bench_closed.csv: bench/bench_closed.bench bench/benchmark.h
	./$< > $@

# ----- Array benchmark -----

bench/bench_array.bench: bench/bench_array.o.bench
	$(CC) `./lib$(HEAD)-config --link --local` $< -o $@ `./lib$(HEAD)-config --link`

bench/bench_array.csv: bench/bench_array.bench bench/benchmark.h
	./$< > $@

bench/array_benchmarks: bench/bench_array.csv bench/bench_array.plt
	gnuplot bench/bench_array.plt
	@echo "Array benchmark plot generated"


bench/hashtable_benchmarks: bench/bench_linear.csv bench/bench_robinhood.csv bench/bench_closed.csv bench/bench_hashtable.plt
	gnuplot bench/bench_hashtable.plt
	@echo "Hashtable benchmark plots generated"


# All thinks TeX
TEX_OPTIONS=-shell-escape

# Report
REPORT=report/report.pdf
REPORT_SRC=$(wildcard report/*.tex)
REPORT_TEX=$(TEX_OPTIONS) -output-directory report/build -jobname report

$(REPORT) : $(REPORT_SRC) report/biblio.bib benchmarks
	mkdir -p report/build
	pdflatex $(REPORT_TEX) report/report.tex
	biber report/build/report
	pdflatex $(REPORT_TEX) report/report.tex
	pdflatex $(REPORT_TEX) report/report.tex
	mv report/build/report.pdf $@

report: $(REPORT)

report-clean:
	$(RM) -r report/build $(REPORT)

# Course
COURSE=course/course.pdf
COURSE_SRC=$(wildcard course/*.tex)
COURSE_TEX=$(TEX_OPTIONS) -output-directory course/build -jobname course

$(COURSE) : $(COURSE_SRC) report/biblio.bib report/preamble.tex benchmarks
	mkdir -p course/build
	pdflatex $(COURSE_TEX) course/course.tex
	biber course/build/course
	pdflatex $(COURSE_TEX) course/course.tex
	pdflatex $(COURSE_TEX) course/course.tex
	mv course/build/course.pdf $@

course: $(COURSE)
	make -C course/demo

course-clean:
	$(RM) -r course/build
	make -s -C course/demo clean

# Convenience
clean: lib-clean benchmarks-clean report-clean course-clean
	find . -name "*.prepr" -delete
	find . -name "*.test"  -delete
	find . -name "*.bench" -delete

deep-clean: clean
	find . -name "captured-*" -delete
	find . -name ".*~" -delete
	find . -name  "*~" -delete

# Required software
tex-install:
	echo "Installing TeX-related packages"
	sudo apt install texlive-science biber

c-install:
	echo "Installing C-related packages"
	sudo apt install git gdb clang gcc binutils libasan5 libubsan1 valgrind

gef-install: # https://github.com/hugsy/gef
	echo "Installing GEF (gdb extension)"
	bash -c "$(wget https://gef.blah.cat/sh -O -)"

vscode-install:
	echo "Installing VSCode"
	wget https://code.visualstudio.com/sha/download\?build=stable\&os=linux-deb-x64 -O code.deb
	sudo dpkg -i code.deb
	rm code.deb
	code --install-extension ms-vscode.cpptools
	code --install-extension ms-vscode.cpptools-extension-pack

first-install : tex-install c-install vscode-install gef-install
