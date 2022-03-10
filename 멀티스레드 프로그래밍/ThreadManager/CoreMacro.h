#pragma once
/*---------
:	Crash
----------*/
//#define을 여러줄을 하려면 \을 적어줘야한다.
//고의로 Crash를 내려면 컴파일러가 잡아주기 때문에
//고의로 Crash를 내기위해
//crash를 nullptr로 만든후 nullptr이 아니라고 가정시킨다.
#define CRASH(cause)				\
{									 \
	uint32 *crash = nullptr;		 \
	__analysis_assume(crash!=nullptr); \
	*crash = 0xDEADBEEF	;			\
}		

#define ASSERT_CRASH(expr)	\
{							\
	if (!(expr))			\
	{						\
		CRASH("ASSERT_CRASH");\
		__analysis_assume(expr);\
	}							\
}
