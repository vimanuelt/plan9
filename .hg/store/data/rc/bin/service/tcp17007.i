         W   V        ���������������ӜM���P)�e[O            u#!/bin/rc
netdir=`{echo $3 | sed 's;/[0-9]+$;!*!0;'}
exec /bin/exportfs -a -A $netdir
