         d   c        ���������f�b�{�	zn���'�            u/*
 * pANS stdio -- rewind
 */
#include "iolib.h"
void rewind(FILE *f){
	fseek(f, 0L, SEEK_SET);
}
