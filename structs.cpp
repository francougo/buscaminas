#include "structs.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

void crearBaseDeDatos(PDB database){
	database -> cantidad_usuarios = 0;
	database -> usuarios = new Usuario[100];
}
void crearUsuario(PDB database){
	int cont = 0;
	PUsuario user = &(database -> usuarios[database -> cantidad_usuarios]);
	char nombre[11], n[11], contrasena[13];
		bool correcto = false;
	if ((database -> cantidad_usuarios) < 100){
		cout<<"Ingrese su nombre: ";
		gets(nombre);
		while (!correcto){
			if (strlen(nombre) >= 8 && strlen(nombre)<11){
				correcto = true;
				for (int i = 0; i<siezof(nombre); i++){
					if(!isalnum(nombre[i]) correcto = false;
				}
				while(correcto && cont < (database -> cantidad_usuarios)){
					n = database -> usuarios[cont].nombre;
					if(strcmp(nombre, n)==0) correcto = false;
					else cont ++;
				}
			}else{
				cout<<"Error. Vuelva a ingresar su nombre: ";
				gets(nombre);
				cout<<endl;
			}
		}
		cout <<"Su nombre es: "<<nombre;
		correcto = false;
		cout<<"Ingrese su contrasena: ";
		gets(contrasena);
		while (!correcto){
			if (strlen(contrasena) > 0 && strlen(contrasena)<13){
				cout <<"Su contrasena es: "<<contrasena;
				correcto = true;
				for (int i = 0; i<siezof(contrasena); i++){
					if(!isalnum(contrasena[i]) correcto = false;
				}
			}else{
				cout<<"Error. Vuelva a ingresar su contrasena."<<endl;
				gets(contrasena);
			}
		}
		for (int i = 0; i<strlen(nombre); i++){
			user -> nombre[i] = nombre[i];
		}
		for (int i = 0; i<strlen(contrasena); i++){
			user -> contrasena[i] = contrasena[i];
		}
		user -> partidasfacil = new Partida[10];
		user -> partidasmedio = new Partida[10];
		user -> partidasdificil = new Partida[10];
		user -> perdidas = 0;
		user -> ganadas = 0;
		user -> abandonos = 0;
		database -> cantidad_usuarios++;
		}else{
			cout<<"Error. Ya se alcanzo la cantidad maxima de usuarios."<<endl;
		}		

}
void GuardarPartida (PPartida match, int dif, int score, char tipo){
	// Cambie la funcion para aprovechar que pasaste todo a punteros,
	// ahora lo que hace es generar una fecha nueva con cada nueva partida
	// y despues le asigna al puntero de fecha de la partida el valor de memoria
	// de la nueva fecha uwu
	Pfecha date;
	obtenerFecha(date);
	match -> fecha = date;
	match -> dificultad = dif;
	match -> puntaje = score;
	match -> tipo = tipo;
}
void obtenerFecha(Pfecha date){
	
	int dig0, dig1, dig2, dig3, mes, anio, dia;
	char fecha[25], nombreMes[3];
	time_t current_time;
	current_time=time(NULL);
	ctime(&current_time);
	strcpy(fecha, ctime(&current_time));
	dig0 = (int)fecha[8] - 48;
	dig1 = (int)fecha[9] -48;
	dia = dig0 * 10 + dig1;
	date -> dia = dia;
	for (int i = 0; i<3; i++){
		nombreMes[i] = fecha[i+4];
	}
	mes = QueMesEs(nombreMes);
	date -> mes = mes
	dig0 = (int)fecha[20] - 48;
	dig1 = (int)fecha[21] -48;
	dig2 = (int)fecha[22] -48;
	dig3 = (int)fecha[23] -48;
	anio = dig0 * 1000 + dig1 * 100 + dig2 * 10 + dig3;
	date -> anyo = anio;
}
	
int QueMesEs(char mes[3]){
	int rdo = -1;
	if (mes[0]=='D'){
		rdo = 12;
	}else{
		if (mes[0]=='N'){
			rdo = 11;
		}else{
			if (mes[0]=='O'){
				rdo = 10;
			}else{
				if (mes[0]=='S'){
					rdo = 9;
				}else{
					if (mes[0]=='F'){
						rdo = 2;
					}else{
						if(mes[0]=='A'){
							if (mes[1]=='p') rdo = 4;
							else rdo = 8;
						}else{
							if(mes[0]=='M'){
								if(mes[2]=='r') rdo = 3;
								else rdo = 5;
							}else{
								if(mes[0]=='J'){
									if(mes[1]=='a') rdo = 1;
									else{
										if (mes[2]=='n') rdo = 6;
										else rdo = 7;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return  rdo;
}

void partidaAUsuario(PPartida match, PUsuario usr){
		if (match -> tipo == 'G') usr -> gan++;
		else if (match -> tipo == 'P') usr -> perd++;
		else usr -> ab++;

		int total = usr -> gan + usr -> perd + usr -> ab;

		usr -> ganadas = usr -> gan * 100.0 / total;	
		usr -> perdidas = usr -> perd * 100.0 / total;
		usr -> abandonos = usr -> ab * 100.0 / total;
}



void guardarDB(PDB database){
	// tiene que crear si no lo hay un archivo binario y guardarle la base de datos
	// en caso de que el archivo ya exista se le sobreescribe la base de datos,
	// asi se puede llamar a la misma funcion cada vez que se quiera guardar algo


}









































DB AbrirBaseDeDatos(){

	DB database;
	ifstream archivo;
	archivo.open("basededatos.dat",ios::binary);
	if(!archivo.fail()){
		archivo.read((PDB) database, sizeof(database));
		archivo.close();
	}else{
		puts("Error al abrir el archivo.");
	}
	return database;
}



