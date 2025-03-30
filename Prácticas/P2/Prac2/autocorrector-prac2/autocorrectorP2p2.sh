#!/bin/bash

MAXTIME=10     # Tiempo máximo de ejecución (en segundos) de una prueba
PRINCIPAL=./prac2	# Nombre del ejecutable de la práctica
ENT=pruebas    # Directorio con los ficheros de pruebas de entrada
FICH=files    # Directorio con los ficheros originales
FUENTE=../prac2.cc  # Nombre del fichero de código fuente
SAL=salida-correcta-esperada     # Directorio con los ficheros de salidas correctas
OBT=salida-obtenida       # Directorio con los ficheros obtenidos tras la ejecución de la práctica
VAL="valgrind -q"  # Si valgrind no está instalado, quitar "valgrind -q" de esta linea (quedaría VAL=""). Ojo: La práctica debe probarse siempre con valgrind antes de la entrega.
npruebasa=$(ls -1 pruebas/*.txt|wc -l)
mata=./mata
comparefiles=./comparefiles
cmpBinPatients=./cmpBinPatients
cmpBinAnalysis=./cmpBinAnalysis

mkdir -p $OBT

# -------------- generar y compilar los ficheros auxiliares mata.c y comparefiles.cc -----
function genMata() {

if ! test -x mata ; then  # si ya existe no se genera
echo "#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

const int TAMMAXFICH=300000;
int f;int segundos,st;
int segejec=0;
char *nfsal=NULL;
void SeAcabo(int i){
  fprintf(stderr,\"ERROR, tiempo de ejecución agotado... (%d segundos)\\n\",segundos);
  fflush(stderr);
  kill(f,SIGKILL);
  exit(-1);
}
void SeAcaboFich(int i){
  fprintf(stderr,\"ERROR, fichero de salida muy grande (cuelgue?)... \\n\");
  fflush(stderr);
  kill(f,SIGKILL);
  exit(-1);
}
int FicheroGrande() {
  struct stat s;  int ret=0;
  if (nfsal != NULL) {
    stat(nfsal,&s);
    if (s.st_size > TAMMAXFICH) ret = 1;
  }
  return ret;
}
void Control(int i){
  segejec++;
  if (segejec >= segundos) SeAcabo(i);
  else if (FicheroGrande()) SeAcaboFich(i);
  else alarm(1);
}
void Salir(int i){exit(1);}
int main(int argc,char *argv[]){
  if (argc < 4) exit(1);
  segundos = atoi(argv[1]);
  nfsal = argv[2];
  signal(SIGALRM,Control);
  signal(SIGCHLD,Salir);
  alarm(1);
  if ((f = fork()) == 0) execvp(argv[3],&argv[3]);
  else wait(&st);
}
" > mata.c
gcc -o mata mata.c
fi
}



echo "*********************************************************"
echo "Autocorrector P2p2"


# Comprobar si está valgrind instalado
hayValgrind=$(which valgrind)
if test "$hayValgrind" == "" ; then
  echo "AVISO: El autocorrector se ejecutará sin valgrind, por lo que es posible que"
  echo "el resultado de la corrección no sea fiable. Para comprobar realmente que la"
  echo "práctica es correcta, debes probar el autocorrector en un ordenador Linux"
  echo "con valgrind instalado antes de la entrega."
  echo
  read -p "Pulsa [Enter] para continuar"
  VAL=""
fi

echo " Generando ficheros auxiliares... "
genMata

rm -f mata.c
rm -f patients.bin
rm -f statistics.txt
rm -f wrong_patients.txt

# función que prepara los argumentos y el fichero binario, si es necesario
function prepArgsBin ()
{
  args=""
  VAL="valgrind -q" 
  
  if test "$1" == "05_exportAnalysis" ; then
    VAL="" # desactivar valgrind para esta prueba
  elif test "$1" == "07_deletePatient" ; then
    VAL="" # desactivar valgrind para esta prueba
  elif test "$1" == "09_savePatients" ; then
    VAL="" # desactivar valgrind para esta prueba
  elif test "$1" == "10_arguments_error" ; then
    args="-n pruebas/analysis.txt -s"
  elif test "$1" == "11_arguments_error" ; then
    args="-s "
   elif test "$1" == "12_arguments_error" ; then
    args="-f "
  elif test "$1" == "13_arguments_error" ; then
    args="-s "
  elif test "$1" == "14_arguments_error" ; then
    args="-s -f"
  elif test "$1" == "15_arguments_error" ; then
    args="-s -f fileNotExist.txt -s"
  elif test "$1" == "16_arguments_ok" ; then
    args="-f fileNotExist.txt -s"
   elif test "$1" == "17_arguments_ok" ; then
    args="-f files/analytics.txt"
  elif test "$1" == "18_arguments_ok" ; then
    args=" -f files/analytics.txt -s "
  elif test "$1" == "19_arguments_ok" ; then
    args="-s -f files/analytics.txt"
  elif test "$1" == "20_arguments_ok" ; then
    args="-f fileNotExist.txt"
  fi
}


let npruebas=$npruebasa
let subp=3
pok=0;
pokaux=0;
# Compilacion
echo
echo "*********************************************************"
echo " Compilando..."
echo "*********************************************************"
rm -rf $PRINCIPAL
tempfile=`mktemp /tmp/prog2iiXXXXX`
g++ -Wall -std=c++11 -g  $FUENTE -o $PRINCIPAL 2> $tempfile
if [ $? -ne 0 ]; then
        cat $tempfile
  echo "LA PRACTICA NO COMPILA"
else
  # aviso warnings
  nlineasWarnings=$(wc --lines $tempfile | cut -f 1 -d ' ')
  if [ $nlineasWarnings -ne 0 ]; then
     cat $tempfile
     echo
     echo "LA PRACTICA COMPILA, PERO TIENE WARNINGS, DEBES QUITARLOS"
     read -p "Pulsa [Enter] para continuar"
     echo
  fi



  # Ejecucion y comprobacion de la salida
  echo
  echo "*********************************************************"
  echo " Ejecutando y comprobando salida a la vez..."
  echo "*********************************************************"
  echo ""
  echo "------------------------------------------------------------------------"
  echo "¡AVISO! : TODAS LAS PRUEBAS CARGAN INICIALMENTE EL FICHERO patients.bin "
  echo "EXCEPTO EN AQUELLAS QUE EXPLÍCITAMENTE SE INDIQUE OTRA COSA "
  echo "------------------------------------------------------------------------"
  echo

	# Ejecucion del programa
  rm -rf patients.bin
  cp $FICH/analysis.bin analysis.bin
  rm -rf statistics.txt
  
  for i in `ls $ENT`
  do
    bn=$(basename $i .txt)
    if test $bn = $i ; then 
      continue  # si no es un .txt no es un fichero de entrada
    fi

    echo "Prueba $bn"
    prepArgsBin $bn

    if test "$args" != "" ; then
      echo " (argumentos:$args)"
    else
      echo 
    fi

    # Ejecucion del programa
    # rm -f patients.bin

    # Si existe el fichero de entrada .bin lo copiamos con el nombre teachers.bin
 #   if test -f $ENT/${bn}.bin ; then
 #     cp $ENT/${bn}.bin teachers.bin
 #   fi
    if test "$bn" == "01_addPatients"; then
        echo "Esta primera prueba no carga el fichero patients.bin"
    fi
    
    if test "$bn" == "08_statistics"; then
        cp $FICH/patients2.bin patients.bin  # para la prueba: 08_statistics 
        echo "Esta prueba carga de inicio el fichero $FICH/patients2.bin"
    fi
    if test "$bn" == "09_savePatients"; then
        echo "Esta prueba carga de inicio el fichero $FICH/patients2.bin"
    fi
    
    $mata $MAXTIME $OBT/${bn}.salida-obtenida $VAL $PRINCIPAL $args < $ENT/$i > $OBT/${bn}.salida-obtenida 2> $tempfile
    if test "$bn" != "07_deletePatient"; then 
       if test "$bn" != "09_savePatients"; then
            cp $FICH/patients.bin patients.bin  # Despues de ejecutar el primer test actualiza al patients.bin original todas las pruebas excepto en la 7 y en la 9)
    	 fi
    fi 
   if test -s $tempfile; then
      echo "ERROR DE EJECUCION..."
      cat $tempfile
      rm -rf $OBT/${bn}.salida-obtenida $tempfile
      
    else
     
      $comparefiles $OBT/${bn}.salida-obtenida $SAL/${bn}.salida-esperada
      if [ $? -ne 0 ]; then
        diff -EwB $OBT/${bn}.salida-obtenida $SAL/${bn}.salida-esperada 2>&1
      else   
        if test "$bn" == "05_exportAnalysis"; then
          mv analysis.bin $OBT/05_analysis.bin
          $VAL $cmpBinAnalysis $OBT/05_analysis.bin $SAL/05_analysis.bin 2>&1
          if [ $? -eq 0 ]; then
            echo "OK"
            let pok++
          fi
          cp $FICH/analysis.bin analysis.bin # Actualizamos analysis.bin, modificado previamente en esta opción
        elif test "$bn" == "06_importAnalysis"; then
          mv wrong_patients.txt $OBT/06_wrong_patients.txt
          $comparefiles $OBT/06_wrong_patients.txt $SAL/06_wrong_patients.txt 2>&1
          if [ $? -ne 0 ]; then
		        diff -EwB $OBT/06_wrong_patients.txt $SAL/06_wrong_patients.txt 2>&1
	        else
 	          echo "OK"
	          let pok++
	        fi 
	        cp $FICH/patients.bin patients.bin  # para la siguiente prueba: 07_deletePatients
	        # rm -rf wrong_patients.txt
           
        elif test "$bn" == "07_deletePatient"; then
          # Movemos patients.bin y analysis.bin obtenidos a salida-obtenida tras borrar pacientes.
          mv patients.bin $OBT/07_patients.bin 
          mv analysis.bin $OBT/07_analysis.bin
          
          $VAL $cmpBinPatients $OBT/07_patients.bin $SAL/07_patients.bin 2>&1
          if [ $? -eq 0 ]; then
            echo "OK"
            let pokaux++
          fi
          $VAL $cmpBinAnalysis $OBT/07_analysis.bin $SAL/07_analysis.bin 2>&1
          if [ $? -eq 0 ]; then
            echo "OK"
            let pokaux++
          fi
          cp $FICH/analysis.bin analysis.bin  #reponemos analysis.bin
          if [ $pokaux -eq 2 ]; then
	        		let pok++
	       fi
	     elif test "$bn" == "08_statistics"; then
          mv statistics.txt $OBT/08_statistics.txt
          $comparefiles $OBT/08_statistics.txt $SAL/08_statistics.txt 2>&1
          if [ $? -ne 0 ]; then
		        diff -EwB $OBT/08_statistics.txt $SAL/08_statistics.txt 2>&1
	        else
 	          echo "OK"
	          let pok++
	        fi    
        elif test "$bn" == "09_savePatients"; then
           mv patients.bin $OBT/09_patients.bin
	        $VAL $cmpBinPatients $OBT/09_patients.bin $SAL/09_patients.bin 2>&1
           if [ $? -eq 0 ]; then
             echo "OK"
             let pok++
           fi
        else 
      	 echo "OK"
	       let pok++
	     fi
      fi
    fi;

    # rm -f teachers.bin
    echo "--------------------------"
  done
  rm -f patients.bin
  rm -f statistics.txt
  rm -f wrong_patients.txt
  rm -r analysis.bin
  if [ $pok -eq $npruebas ]; then
  echo -e "\nTODAS LAS PRUEBAS DEL AUTOCORRECTOR FUNCIONAN\n"
  else
    echo -e "\nOJO: FALLAN" $[npruebas-pok] "PRUEBAS DEL AUTOCORRECTOR\n"
  fi;
fi;

rm -f $tempfile

