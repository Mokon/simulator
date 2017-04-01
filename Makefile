# Copyright (C) 2017 David 'Mokon' Bond, All Rights Reserved
CXXFLAGS += -Wall -Wextra -std=c++1y -pedantic -Werror -Weffc++
CXXFLAGS += -Wpedantic -pedantic-errors -Wundef
CXXFLAGS += -Wshadow -Wstrict-aliasing -Woverloaded-virtual
CXXFLAGS += -Wconversion -Wsign-compare -Wsign-conversion
CXXFLAGS += -Wdeprecated -Wint-to-void-pointer-cast
CXXFLAGS += -Wold-style-cast -Wcast-qual -Wcast-align
CXXFLAGS += -Wunused-parameter -Wunused
CXXFLAGS += -Wchar-subscripts -Wcomment -Winit-self
CXXFLAGS += -Wmissing-declarations
CXXFLAGS += -Wmissing-field-initializers -Wparentheses -Wformat=2

all: debug
debug: CXXFLAGS += -DDEBUG -g -O0 -fstack-protector-all -pg
debug: executable
release: CXXFLAGS += -g0 -O3 
release: executable
executable:
	clang++ ${CXXFLAGS} ${LDFLAGS} src/*.cpp -lboost_program_options -o sim 
install:
	echo "install not supported"
clean:
	/bin/rm -f gmon.out sim *.ERR *.bak
format:
	perltidy -b *.pm *.pl
	/bin/rm -f *.pm.bak *.pl.bak
test: debug
	./test.pl
test-release: release
	./test.pl
