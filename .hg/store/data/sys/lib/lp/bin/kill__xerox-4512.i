         ]   \        ��������C[�߾"�N��ڵ��g�i��            u#!/bin/rc

if (~ $#* 0) exits 'no argument'
/$cputype/bin/aux/lpdsend -dPORT1 -k $1 $OUTDEV
