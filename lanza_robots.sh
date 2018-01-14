#!/bin/bash
dir=$1
puerto=$2
if [ "$#" -ne 2 ]; then
    echo "Uso $0 [direccion] [puerto]"
else
echo -e "Lanzando 3 clientes automaticos a $dir : $puerto\n"
exec xterm  -hold -e ./bin/Cliente -p $puerto -d $dir -a &
exec xterm  -hold -e ./bin/Cliente -p $puerto -d $dir -a &
exec xterm  -hold -e ./bin/Cliente -p $puerto -d $dir -a &
exec xterm  -hold -e ./bin/Cliente -p $puerto -d $dir -a &
exec xterm  -hold -e ./bin/Cliente -p $puerto -d $dir -a &
exec xterm  -hold -e ./bin/Cliente -p $puerto -d $dir -a &
exec xterm  -hold -e ./bin/Cliente -p $puerto -d $dir -a &
exec xterm  -hold -e ./bin/Cliente -p $puerto -d $dir -a &
exec xterm  -hold -e ./bin/Cliente -p $puerto -d $dir -a &
exec xterm  -hold -e ./bin/Cliente -p $puerto -d $dir -a &
fi
exit 0
