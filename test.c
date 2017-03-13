#include <stdio.h>
#include "cpplib.h"

#define CASE(x) case x:
#define ANY(...) CPP_FOREACH(CASE,__VA_ARGS__)

#define CLOSEBRACE(_,_2) }
#define CLOSE(n) CPP_REPEAT(n,CLOSEBRACE)

#define LIST_FOO_(X,XLAST) \
	X(Bar) \
	X(Baz) \
	X(Quux) \
	X(Qaz) \
	XLAST(Lower)
#define LIST_FOO(X) LIST_FOO_(X,X)

typedef enum { LIST_FOO_(CPP_ENUMERATE,CPP_ID) } foo;

foo foo_of_string(const char *s)
{
#define COMPARE_Lower(x) x
#define FOO_CMP(name) CPP_IF(CPP_EQUAL(name,Lower))(if(strcmp(s,"lower")==0)return Lower;, if(strcmp(s,#name)==0)return name;)
	LIST_FOO(FOO_CMP)
#undef FOO_CMP
#undef COMPARE_Lower
	return (foo)-1;
}

const char *string_of_foo(foo x)
{
#define COMPARE_Quux(x) x
#define COMPARE_Qaz(x) x
#define FOO_CMP(name) CPP_IF(CPP_OR(CPP_EQUAL(name,Quux))(CPP_EQUAL(name,Qaz))) \
	( \
		if(x==name)return "Q";, \
		if(x==name)return #name; \
	)
	LIST_FOO(FOO_CMP)
#undef FOO_CMP
#undef COMPARE_Qaz
#undef COMPARE_Quux
}

int main(int argc, const char **argv)
{
	if (argc != 2) return 1;
	else {
		foo x = foo_of_string(argv[1]);
		switch (x)
		{
			case (foo)-1: puts("invalid"); break;
			ANY(Bar,Baz) puts("starts with b"); break;
			default: puts(string_of_foo(x));

CLOSE(3)