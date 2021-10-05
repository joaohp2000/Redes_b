#!/bin/bash
if [ $1 = "cliente" ]
   then
    echo "Compilando Cliente:"
    gcc cliente.c fragmentos.c ip.c transporte.c -o cliente
elif [ $1 = "servidor" ]
    then
    echo "Compilando Servidor: "
    gcc serv.c fragmentos.c ip.c transporte.c -o servidor
else
    echo "Erro: digite: ./compilar servidor ou ./compilar cliente "
fi
