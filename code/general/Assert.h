#ifndef HANDMADE_ASSERT
#define HANDMADE_ASSERT

#if HANDMADE_SLOW
// TODO(casey): Complete assertion macro - don't worry everyone!
#define Assert(Expression) if(!(Expression)) {*(int *)0 = 0;}
#else
#define Assert(Expression)
#endif

#define StaticAssert(Expression) typedef char __StaticAssert__[(Expression)?1:-1]

#endif