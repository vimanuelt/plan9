         Z   Y        ��������6=*��5}�/y-�F�R��            u#include "lib9.h"

ulong
getcallerpc(void *p)
{
	ulong *lp;

	lp = p;

	return lp[-1];
}
