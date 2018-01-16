#!/bin/bash
puerto=$1
if [ "$#" -ne 1 ]; then
    echo "Uso $0 [puerto]"
else
echo -e "Lanzando el administrador\n"
exec xterm  -hold -e  ./bin/administrador -p $puerto &
echo -e "Lanzando el gestor de Vallas\n"
echo -e "Lanzando el subastador en el puerto $puerto\n"
fi
exit 0 
