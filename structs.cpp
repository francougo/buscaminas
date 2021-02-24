#include "structs.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <iomanip>
#include <windows.h>

using namespace std;
// Para guardar datos de partidas:
// se guarda una partida(inicializa), se ejecuta partidaAUsuario para cargar los datos y
// se ejecuta guardarDB para escribir los datos modificados al archivo (los usuarios se crean
// dentro de la base de datos por lo que no es necesario cargarlos en la misma)

void crearBaseDeDatos(PDB database){
	database -> cantidad_usuarios = 0;
    database -> mejorFacil=-1;
    database -> mejorMedio=-1;
    database -> mejorDificil=-1;
}

// inicializa a un usuario con el nombre y contrasenia que el usuario ingrese
void crearUsuario(PDB database){
	int cont = 0, QUsuarios = database -> cantidad_usuarios;
	Usuario user;
	PUsuario Puser = &user;
	char nombre[11], n[11], contrasena[13];
	bool correcto = false, tamanoCorrecto = false, esAlNum = true, esUnic = true;
	if (QUsuarios < 100){
		cout<<"Ingrese su nombre: ";
		while(!correcto){
			cin >> ws;
			fgets(nombre, 11, stdin);
			for (int i=0; i<11; i++) if (nombre[i]=='\n') nombre[i] = '\0';
            cin.ignore(1000, '\n');
			if(strlen(nombre)>7 && strlen(nombre) <11){
				tamanoCorrecto = true;
			}
			while(esAlNum && cont<strlen(nombre)-1){
				if(!isalnum(nombre[cont])) esAlNum = false;
				else cont++;
			}
			cont = 0;
			if(QUsuarios > 0){
				while(esUnic && cont < QUsuarios){
					strcpy(n, database -> usuarios[cont].nombre);
					if(strcmp(nombre, n)==0) esUnic = false;
					else cont++;
				}
				cont = 0;
			}
			if (tamanoCorrecto && esAlNum && esUnic){
				correcto = true;
			}else{
				cout<<"Nombre invalido. Por favor, intente otra vez."<<endl;
				tamanoCorrecto = false;
				esAlNum = true;
				esUnic = true;
			}
		}
    cout <<"Su nombre es: "<<nombre<<endl;
		strcpy(user.nombre, nombre);
		correcto = false;
		tamanoCorrecto = false;
		esAlNum = true;
		cout<<"Ingrese su contrasena: ";
		while (!correcto){
			cont = 0;
			cin >> ws;
			fgets(contrasena, 13, stdin);
			for (int i=0; i<13; i++) if (contrasena[i]=='\n') contrasena[i] = '\0';
            cin.ignore(1000, '\n');
			if (strlen(contrasena) > 7 && strlen(contrasena)<13){
				tamanoCorrecto = true;
			}
			while(esAlNum && cont<strlen(contrasena)-1){
				if(!isalnum(contrasena[cont])&&contrasena[cont]!='$'&&contrasena[cont]!='%'&&contrasena[cont]!='&'&&contrasena[cont]!='#') esAlNum = false;
				else cont++;
			}
			if (esAlNum && tamanoCorrecto){
				correcto = true;
				cout<<"Su contrasena es: "<<contrasena<<endl;
				strcpy(user.contrasena, contrasena);
			}else{
				cout<<"Contrasena invalida, por favor, trate otra vez."<<endl;
				correcto = false;
				tamanoCorrecto = false;
				esAlNum = true;
			}
		}
		system("pause");
		user.perdidas = 0;
		user.ganadas = 0;
		user.abandonos = 0;
		user.tlfacil = 0;
		user.tlmedio = 0;
		user.tldificil = 0;
		user.gan = 0;
		user.perd = 0;
		user.ab = 0;
		encriptar(Puser, true);
		database -> usuarios[QUsuarios] = user;
		database -> cantidad_usuarios++;
		}
}

void GuardarPartida (PPartida match, int dif, int score, char tipo){
	// Cambie la funcion para aprovechar que pasaste todo a punteros,
	// ahora lo que hace es generar una fecha nueva con cada nueva partida
	// y despues le asigna al puntero de fecha de la partida el valor de memoria
	// de la nueva fecha uwu
	Fecha date;
	obtenerFecha(&date); 
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
	date->dia = dia;
	for (int i = 0; i<3; i++){
		nombreMes[i] = fecha[i+4];
	}
	mes = QueMesEs(nombreMes);
	date->mes = mes;
	dig0 = (int)fecha[20] - 48;
	dig1 = (int)fecha[21] -48;
	dig2 = (int)fecha[22] -48;
	dig3 = (int)fecha[23] -48;
	anio = dig0 * 1000 + dig1 * 100 + dig2 * 10 + dig3;
	date->anyo = anio;
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

// toma una partida y le guarda los datos al usuario ingresado
void partidaAUsuario(PPartida match, PUsuario usr){
		if (match -> tipo == 'G') usr -> gan++;
		else if (match -> tipo == 'P') usr -> perd++;
		else usr -> ab++;
		int total = (usr -> gan) + (usr -> perd) + (usr -> ab);
		if (total != 0){
			usr -> ganadas = (usr -> gan * 100.0) / total;	
			usr -> perdidas = (usr -> perd * 100.0) / total;
			usr -> abandonos = (usr -> ab * 100.0) / total;
		}
		// guardado de partida
		int menor_punt = 10000000, pos=-1;
		if (match -> dificultad ==1){
            //cout<<"Antes pAU 1: ";
            //for (int i=0;i<usr->tlfacil;i++) cout<<usr->partidasfacil[i].puntaje<<" ";
            //cout<<endl;

			if (usr->tlfacil==10) {
				for (int i=0;i<10;i++){
					if (usr->partidasfacil[i].puntaje < menor_punt){
						menor_punt = usr->partidasfacil[i].puntaje;
						pos = i;
					}
				}
				if (match->puntaje>menor_punt) usr->partidasfacil[pos] = *match;
			}else{
				usr->partidasfacil[usr->tlfacil] = *match;
				usr->tlfacil++;
			}
            //cout<<"Despues pAU 1: ";
            //for (int i=0;i<usr->tlfacil;i++) cout<<usr->partidasfacil[i].puntaje<<" ";
            //cout<<endl;

		}
		else if (match -> dificultad ==2){
            //cout<<"Antes pAU 2: ";
            //for (int i=0;i<usr->tlmedio;i++) cout<<usr->partidasmedio[i].puntaje<<" ";
            //cout<<endl;

			if (usr->tlmedio ==10){
				for (int j=0;j<10;j++){
					if (usr->partidasmedio[j].puntaje < menor_punt){
						menor_punt = usr->partidasmedio[j].puntaje;
						pos = j;
					}
				}
				if (match->puntaje>menor_punt) usr->partidasmedio[pos] = *match;
			}else{
				usr->partidasmedio[usr->tlmedio] = *match;
				usr->tlmedio++;
			}
            //cout<<"Despues pAU 2: ";
            //for (int i=0;i<usr->tlmedio;i++) cout<<usr->partidasmedio[i].puntaje<<" ";
            //cout<<endl;

		}
		else{
            //cout<<"Antes pAU 3: ";
            //for (int i=0;i<usr->tldificil;i++) cout<<usr->partidasdificil[i].puntaje<<" ";
            //cout<<endl;

			if (usr->tldificil==10){
				for (int k=0;k<10;k++){
					if (usr->partidasdificil[k].puntaje < menor_punt){
						menor_punt = usr->partidasdificil[k].puntaje;
						pos = k;
					}
				}
				if (match->puntaje>menor_punt) usr->partidasdificil[pos] = *match;
			}else{
				usr->partidasdificil[usr->tldificil] = *match;
				usr->tldificil++;
			}
            //cout<<"Despues pAU 3: ";
            //for (int i=0;i<usr->tldificil;i++) cout<<usr->partidasdificil[i].puntaje<<" ";
            //cout<<endl;
        
		}
}

// guarda la base de datos ingresada en el archivo
void guardarDB(PDB database){
	ofstream archivo;
	archivo.open("usuarios.bin",ios::binary);
	if(!archivo.fail()){
		archivo.write((char*) database, sizeof(*database));
		archivo.close();
	}else{
		cout<<"Error al abrir base de datos."<<endl;
		system("pause");
	}
}

// busca un usuario por su nombre y contrasenia
Usuario AbrirUsuario (PDB database, bool &sale, int &index){
	Usuario user;
    PUsuario Puser;

	int QU = database -> cantidad_usuarios, cont = 0, i = 0;
	char nombre[11], n[11], pwrd[13], p[13];
	bool encontrado = false, coincide = false;
	while((!encontrado || !coincide) && i<3){
		while(!encontrado && i<3){
			cont = 0;
			cin.ignore(1000, '\n');
			cout<<"Ingrese nombre de usuario: ";
			cin >> ws;
			fgets(nombre, 11, stdin);
			for (int j=0; j<11; j++) if (nombre[j]=='\n') nombre[j] = '\0';
            cin.ignore(1000, '\n');
			while(!encontrado && cont < QU){
				strcpy(n, (database->usuarios[cont].nombre));
				if ((strcmp(nombre, n))==0){
					encontrado = true;
					index = cont;
				}else cont++;
			}
			if(!encontrado){
				cout<<"Usuario inexistente."<<endl;
				i++;
			}
		}
		while(!coincide && i<3){
            Puser = &(database -> usuarios[cont]);
            encriptar(Puser, false);
			strcpy(p, (database -> usuarios[cont].contrasena));
            encriptar(Puser, true);
			cout<<"Ingrese su contrasena: ";
			cin>>ws;
			fgets(pwrd, 13, stdin);
			for (int j=0; j<13; j++) if (pwrd[j]=='\n') pwrd[j] = '\0';
            cin.ignore(1000, '\n');
			if (strcmp(pwrd, p)==0) coincide = true;
			else{
				i++;
				cout<<"Contrasena incorrecta, por favor, intente de nuevo."<<endl;
			}
		}	
		if (encontrado && coincide) user = database -> usuarios[cont];
	}
	if (i==3) sale = true;
	else sale = false;
	
	return user;
}

//se ejecuta al comienzo del programa
// intenta leer un archivo, lo crea si no existe y lee o crea una base de datos para cargar
// en memoria
DB AbrirBaseDeDatos(){
	DB database;
	PDB aux = &database;
	ifstream archivo;
	archivo.open("usuarios.bin",ios::binary);
	if(!archivo.fail()){
		// cambie esta parte para que si no logra leer la base de datos devuelva una nueva
		archivo.read((char*) &database, sizeof(database));
		if (archivo.eof()){
			crearBaseDeDatos(aux);
			database = *aux;
		}
		archivo.close();
		
	}else{
		// se toma en cuenta que si fail=1 es porque el archivo no existe y se crea uno
		
		archivo.close();
		crearBaseDeDatos(aux);
		ofstream arch;
		arch.open("usuarios.bin", ios::binary);
		arch.write((char*) aux, sizeof(*aux));
		database = *aux;
		arch.close();

	}
	return database;
}

//calcula puntos con el tiempo
int calculoPTO(int tiempo){
	int ret;
	if (tiempo==0){
		ret = 1000000;
	}else{
		if (tiempo < 0){
			ret = 0;
		}else{
			ret = 60000 / tiempo;
		}
	}
	return ret;
}

// encriptar = true: encripta, sino desencripta
void encriptar(PUsuario user, bool encriptar){
	char referencia[68]= {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', (char)164, 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '$', '#', '%', '&', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', (char)165, 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
	int pos, j=0;
	bool encontro=false;
	if(encriptar){
		for (int i=0;i<strlen(user->contrasena);i++){
			while (j<68 && !encontro){
				if (user->contrasena[i]==referencia[j]){
					pos = j;
					encontro = true;
				}
				j++;
			}
			j=0;
			encontro=false;
			user->contrasena[i] = referencia[(pos+7)%68];
		}
	}else{
		for (int i=0;i<strlen(user->contrasena);i++){
			while (j<68 && !encontro){
				if (user->contrasena[i]==referencia[j]){
					pos = j;
					encontro = true;
				}
				j++;
			}
			j=0;
			encontro=false;
			if (pos > 6){
				user->contrasena[i] = referencia[(pos-7)%68];
			}else{
				user->contrasena[i] = referencia[68-7+pos];
			}
		}
	}
}

void DibujarCuad(const char *str, int ancho, char separador){
        // Imprime una string agrega separadores y determina
        // el tamano del cuadrado con ancho 
        cout<<separador<<' '<<str;
        for (int i=0;i<(ancho-(int)(strlen(str)));i++){
                cout<<' ';}
        // resultado
        // 'sep' str'espacios''fin'
}

void Puntaje(int opcion, PDB db){
        int an1, an2, an3, an4;
        int num, pos=0, x = 0, linea = 0, maxLineas = 27;// maximo de lineas a mostrar al mismo tiempo
        bool flag1 = true,flag2 = true; // banderas de bucles
        char buffer[100];

        switch(opcion){
                case 1:
                char nomb[11];
                an1=7;an2=11;an3=7;
                // previamente se deben transformar los puntajes a string 
                system("cls");
                DibujarCuad("Nivel",an1);DibujarCuad("Usuario",an2);DibujarCuad("Puntaje",an3);
                cout<<endl;

                if (db->mejorFacil != -1){
                        sprintf(buffer, "%d", db->usuarios[db->mejorFacil].partidasfacil[0].puntaje);
                        strcpy(nomb, db->usuarios[db->mejorFacil].nombre);
                }
                else {
                        strcpy(buffer, "-");
                        strcpy(nomb, "-");
                }
                DibujarCuad("Facil",an1);DibujarCuad(nomb,an2);
                DibujarCuad(buffer ,an3);
                cout<<endl;
                buffer[0] = '\0';

                if (db->mejorMedio != -1){
                        sprintf(buffer, "%d", db->usuarios[db->mejorMedio].partidasmedio[0].puntaje);
                        strcpy(nomb, db->usuarios[db->mejorMedio].nombre);
                }
                else {
                        strcpy(buffer, "-");
                        strcpy(nomb, "-");
                }
                DibujarCuad("Medio",an1);DibujarCuad(nomb,an2);
                DibujarCuad(buffer ,an3);
                cout<<endl;
   
                if (db->mejorDificil != -1){
                        sprintf(buffer, "%d", db->usuarios[db->mejorDificil].partidasdificil[0].puntaje);
                        strcpy(nomb, db->usuarios[db->mejorDificil].nombre);
                }
                else {
                        strcpy(buffer, "-");
                        strcpy(nomb, "-");
                }
                DibujarCuad("Dificil",an1);DibujarCuad(nomb,an2);
                DibujarCuad(buffer ,an3);
                cout<<endl;
                buffer[0] = '\0';
                buffer[0] = '\0';

                
                system("pause");
                break;
                case 2:
                system("cls");
                if (db->cantidad_usuarios <=0){
                        cout<<"Nada que mostrar: No hay usuarios registrados"<<endl;
                        system("pause");
                }else{

                        an1 = 11;an2 = 9;an3 = 10;an4 = 11; // anchos de los cuadrados a imprimir

                        // validacion
                        cout<<"Ingrese la cantidad de usuarios a mostrar (un numero entre 1 y "<<db->cantidad_usuarios<<"): "<<endl;
			            while (!(cin>>num) || (num<1 || num>db->cantidad_usuarios)){ 
                                system("cls");
                                cout<<"Caracter incorrecto, ingrese un numero entre 1 y "<<db->cantidad_usuarios<<endl;
                                cin.clear();
                                cin.ignore(200, '\n');
                        }
                        while (flag1){
                                system("cls");
                                DibujarCuad("Usuario", an1);
                                DibujarCuad("% Ganadas", an2);
                                DibujarCuad("% Perdidas", an3);
                                DibujarCuad("% Abandonos", an4);
                                cout<<endl;
                                x = pos;
                                flag2 = true; // para subir luego de llegar al ultimo elemento
                                // Imprimir porcentajes
                                while (flag2 && linea<maxLineas){
                                        DibujarCuad(db->usuarios[x].nombre, an1);
                                        // toma el indice x almacenado en el ranking para utilizarlo en el arreglo
                                        // de usuarios y mostrarlos de mayor a menor % de ganadas
        
                                        sprintf(buffer, "%.1f", db->usuarios[x].ganadas);
                                        DibujarCuad(buffer, an2);
                                        buffer[0] = '\0';
        
                                        sprintf(buffer, "%.1f", db->usuarios[x].perdidas);
                                        DibujarCuad(buffer, an3);
                                        buffer[0] = '\0';
        
                                        sprintf(buffer, "%.1f", db->usuarios[x].abandonos);
                                        DibujarCuad(buffer, an4);
                                        buffer[0] = '\0';
        
                                        cout<<endl;
                                        ++x;
                                        linea++;
                                        if (x==num) flag2 = false;
                                }
                                linea = 0;
                                // movimiento
                                cout<<"Presione 'j' o 'k' para scrolear o 'q' para salir:"<<endl;
                                system("pause");
                                if (GetAsyncKeyState(0x4A)) if ((pos+maxLineas)<num){
                                        pos++;
                                }
                                if (GetAsyncKeyState(0x4B)) if (pos>0){
                                        pos--;
                                }
                                // salir al menu
                                if (GetAsyncKeyState(0x51)){
                                        flag1 = false;
                                }
                        }
                }
                break;
                case 3:
                system("cls");
                if (db->cantidad_usuarios <=0){
                        cout<<"Nada que mostrar: No hay usuarios registrados"<<endl;
                        system("pause");
                }else{
                        num = db->cantidad_usuarios; pos=0; x = 0; linea = 0;
                        an1 = 11;an2 = 7;an3 = 11;an4 = 11; 
                        flag1 = true;flag2 = true;
                        char fecha[11];
        
                        while (flag1){
                                system("cls");
                                DibujarCuad("Usuario", an1);DibujarCuad("Nivel", an2);
                                DibujarCuad("Fecha", an3);DibujarCuad("Mejor Puntaje", an4);
                                cout<<endl;
                                x = pos;
                                flag2 = true;
                                while(flag2 && linea<(maxLineas/3)){
                                        if (db->usuarios[x].tlfacil > 0){ // solo si el usuario tiene partidas
                                                strFecha(fecha, db->usuarios[x].partidasfacil[0].fecha);
                                                sprintf(buffer, "%d", db->usuarios[x].partidasfacil[0].puntaje);
                                        }
                                        else {
                                                strcpy(fecha, "-");
                                                strcpy(buffer, "-");
                                        }
                                        DibujarCuad(db->usuarios[x].nombre, an1);DibujarCuad("Facil", an2);
                                        DibujarCuad(fecha, an3);DibujarCuad(buffer, an4);
                                        cout<<"  "<<x;
                                        cout<<endl;
        
                                        if (db->usuarios[x].tlmedio > 0){
                                                strFecha(fecha, db->usuarios[x].partidasmedio[0].fecha);
                                                sprintf(buffer, "%d", db->usuarios[x].partidasmedio[0].puntaje);
                                        }
                                        else {
                                                strcpy(fecha, "-");
                                                strcpy(buffer, "-");
                                        }
                                        DibujarCuad("-", an1);DibujarCuad("Medio", an2);
                                        DibujarCuad(fecha, an3);DibujarCuad(buffer, an4);
                                        cout<<endl;
        
                                        if (db->usuarios[x].tldificil > 0){
                                                strFecha(fecha, db->usuarios[x].partidasdificil[0].fecha);
                                                sprintf(buffer, "%d", db->usuarios[x].partidasdificil[0].puntaje);
                                        }
                                        else {
                                                strcpy(fecha, "-");
                                                strcpy(buffer, "-");
                                        }
                                        DibujarCuad("-", an1);DibujarCuad("Dificil", an2);
                                        DibujarCuad(fecha, an3);DibujarCuad(buffer, an4);
                                        cout<<endl;

                                        ++x;
                                        linea++;
                                        if (x==num) flag2 = false;
                                }
                                linea = 0;
                                cout<<"Presione 'j' o 'k' para scrolear o 'q' para salir:"<<endl;
                                system("pause");
                                if (GetAsyncKeyState(0x4A)) if ((pos+maxLineas/3)<num){
                                        pos++;
                                }
                                if (GetAsyncKeyState(0x4B)) if (pos>0){
                                        pos--;
                                }
                                // salir al menu
                                if (GetAsyncKeyState(0x51)){
                                        flag1 = false;
                                }
                        }
                }
        }
} 
void OrdenarUsuarios (PDB database){
	Usuario aux;
	int mayor, contador = 0;
	double maximo;
	if (database -> cantidad_usuarios > 0){
		for (int i = 0; i < (database -> cantidad_usuarios - 1); i++){
			mayor = i;
			maximo = database -> usuarios[mayor].ganadas;
			for (int j = i; j < database -> cantidad_usuarios; j++){
				if (database -> usuarios[j].ganadas > maximo){
					mayor = j;
					maximo = database -> usuarios[mayor].ganadas;
				}
			}
			aux = database -> usuarios[i];
			database -> usuarios[i] = database -> usuarios[mayor];
			database -> usuarios[mayor] = aux;
		}
	}
}

void mejorXNiv(PDB db){// setea los valores de mejores puntajes en db
        int mejPuntaje = 0, indice = -1;
        int qUsuarios = db -> cantidad_usuarios;
        for (int i=0;i<qUsuarios;i++){
                if (db -> usuarios[i].partidasfacil[0].puntaje > mejPuntaje){
                        mejPuntaje = db -> usuarios[i].partidasfacil[0].puntaje;
                        indice = i;
                }
        }
        db->mejorFacil = indice;
        mejPuntaje = 0; indice = -1;
        for (int i=0;i<qUsuarios;i++){
                if (db -> usuarios[i].partidasmedio[0].puntaje > mejPuntaje){
                        mejPuntaje = db -> usuarios[i].partidasmedio[0].puntaje;
                        indice = i;
                }
        }
        db->mejorMedio = indice;
        mejPuntaje = 0; indice = -1;
 
        for (int i=0;i<qUsuarios;i++){
                if (db -> usuarios[i].partidasdificil[0].puntaje > mejPuntaje){
                        mejPuntaje = db -> usuarios[i].partidasdificil[0].puntaje;
                        indice = i;
                }
        }
        db->mejorDificil = indice;
}
                        
void ordenarPartidas(PUsuario user, int dificultad){
        // Coloca la partida con el mayor puntaje en el indice 0 de cada arreglo 
        Partida aux;
        int pIndex = 0, usrMax = 0;
        switch(dificultad){
                case 1:
				if (user -> tlfacil > 0){
					for (int j=0;j<user->tlfacil;j++){
						cout<<"bucle "<<j<<endl;
							if (user->partidasfacil[j].puntaje > usrMax){
									pIndex = j;
									usrMax = user->partidasfacil[j].puntaje;
							}
					}
					cout<<693<<endl;
					aux = user->partidasfacil[pIndex];
					cout<<695<<endl;
					user->partidasfacil[pIndex] = user->partidasfacil[0];
					cout<<697<<endl;
					user->partidasfacil[0] = aux;
				}
                break;
                case 2:
				if (user-> tlmedio > 0){
                for (int j=0;j<user->tlmedio;j++){
                        if (user->partidasmedio[j].puntaje > usrMax){
                                pIndex = j;
                                usrMax = user->partidasmedio[j].puntaje;
                        }
                }
                aux = user->partidasmedio[pIndex];
                user->partidasmedio[pIndex] = user->partidasmedio[0];
                user->partidasmedio[0] = aux;
				}
        
                break;
                case 3:
				if (user-> tldificil > 0){
                for (int j=0;j<user->tldificil;j++){
                        if (user->partidasdificil[j].puntaje > usrMax){
                                pIndex = j;
                                usrMax = user->partidasdificil[j].puntaje;
                        }
                }
                aux = user->partidasdificil[pIndex];
                user->partidasdificil[pIndex] = user->partidasdificil[0];
                user->partidasdificil[0] = aux;
				}

        }
}

void strFecha(char fecha[11], Fecha fech){
        char dia[3],mes[3],anio[5];
        sprintf(dia, "%d", fech.dia);
        sprintf(mes, "%d", fech.mes);
        sprintf(anio, "%d", fech.anyo);
        fecha[0] = '\0';
        strcat(fecha, dia);
        strcat(fecha, "/");
        strcat(fecha, mes);
        strcat(fecha, "/");
        strcat(fecha, anio);
}

void usuarioADB (Usuario user, PDB database, int index){
	database -> usuarios[index] = user;
}



