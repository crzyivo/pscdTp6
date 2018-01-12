#!/bin/bash
dir=$1
puerto=$2
if [ "$#" -ne 2 ]; then
    echo "Uso $0 [direccion] [puerto]"
else
echo -e "Lanzando cliente manual a $dir : $puerto\n"
exec xterm  -hold -e ./bin/Cliente -p $puerto -d $dir &
read -n 1 -s
fi
exit 0 
