         S   R        ���������K�������OG,'�g�O��            u#!/bin/rc

netdir=`{echo $3 | sed 's;/[0-9]+$;!*!0;'}
exec exportfs -a -A $netdir
