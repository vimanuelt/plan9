         I   H      ������������B�>P��ՒP����<K�            u#!/bin/rc

for(i in `{games/jukefs -l | sort -u}) test -e $i || echo $i
