         ]   \        ˙˙˙˙˙˙˙˙.`Q'Vy`1¸äk_o            u#include "lib9.h"

int
canlock(Lock *l)
{
	extern int tas(void*);

	return !tas(&l->val);
}
