all: aha

CC			= gcc -std=c99
OPTS 		= -Og -Wall -Werror -Wno-error=unused-variable -Wno-error=unused-function
DEBUG		= -g
CFLAGS  = $(OPTS) $(DEBUG) 

aha:	fp_program.o swfp_lib.o lex.yy.o 
	${CC} ${CFLAGS} -o aha fp_program.o swfp_lib.o lex.yy.o -lfl -lm

swfp_lib.o:	swfp_lib.c fp.h
	${CC} ${CFLAGS} -c swfp_lib.c	

fp_program.o:	fp_program.c fp_parse.h
	${CC} ${CFLAGS} -c fp_program.c	

lex.yy.o:	lex.yy.c fp_parse.h
	gcc -c lex.yy.c

lex.yy.c:	fp.l
	flex fp.l

clean:
	rm -rf aha lex.yy.c lex.yy.o fp_program.o swfp_lib.o 

reallyclean:
	rm -rf aha lex.yy.c lex.yy.o fp_program.o swfp_lib.o 
