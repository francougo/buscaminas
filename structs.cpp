#include "structs.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <iomanip>
#include <conio.h>





using namespace std;
// Para guardar datos de partidas:
// se guarda una partida(inicializa), se ejecuta partidaAUsuario para cargar los datos y
// se ejecuta guardarDB para escribir los datos modificados al archivo (los usuarios se crean
// dentro de la base de datos por lo que no es necesario cargarlos en la misma)

void crearBaseDeDatos(PDB database){
	database -> cantidad_usuarios = 0;
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
			gets(nombre);
			if(strlen(nombre)>7 && strlen(nombre) <11){
				tamanoCorrecto = true;
			}
			while(esAlNum && cont<strlen(nombre)){
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
			}
		}
		
    
    
    cout <<"Su nombre es: "<<nombre<<endl;
		strcpy(user.nombre, nombre);
		correcto = false;
		tamanoCorrecto = false;
		esAlNum = true;
		cout<<"Ingrese su contrasena: ";
		while (!correcto){
			cin >> ws;
			gets(contrasena);
			if (strlen(contrasena) > 0 && strlen(contrasena)<13){
				tamanoCorrecto = true;
			}
			while(esAlNum && cont<strlen(contrasena)){
				if(!isalnum(contrasena[cont])) esAlNum = false;
				else cont++;
			}
			if (esAlNum && tamanoCorrecto){
				correcto = true;
				cout<<"Su contrasena es: "<<contrasena<<endl;
				strcpy(user.contrasena, contrasena);
			}else{
				cout<<"Contraasena invalida, por favor, trate otra vez."<<endl;
			}
		}
		user.perdidas = 0;
		user.ganadas = 0;
		user.abandonos = 0;
		user.tlfacil = 0;
		user.tlmedio = 0;
		user.tldificil = 0;
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
	Pfecha punterodate = &date;
	obtenerFecha(punterodate);
	match -> fecha = punterodate;
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
	date -> mes = mes;
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

// toma una partida y le guarda los datos al usuario ingresado
void partidaAUsuario(PPartida match, PUsuario usr){
		if (match -> tipo == 'G') usr -> gan++;
		else if (match -> tipo == 'P') usr -> perd++;
		else usr -> ab++;
		int total = (usr -> gan) + (usr -> perd) + (usr -> ab);
		usr -> ganadas = usr -> gan * 100.0 / total;	
		usr -> perdidas = usr -> perd * 100.0 / total;
		usr -> abandonos = usr -> ab * 100.0 / total;
		

		// guardado de partida
		int menor_punt = 10000000, pos=-1;
		if (match -> dificultad ==1){
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
		}
		else if (match -> dificultad ==2){
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
		}
		else{
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
		}
}

// guarda la base de datos ingresada en el archivo
void guardarDB(PDB database){
	ofstream archivo;
	archivo.open("usuarios.bin",ios::binary);
	if(!archivo.fail()){
		archivo.write((char*) database, sizeof(*database));
		archivo.close();
		cout<<"Progreso guardado"<<endl;
	}else{
		cout<<"Error al abrir el archivo. guardarDB"<<endl;
	}

}

// busca un usuario por su nombre y contrasenia
Usuario AbrirUsuario (PDB database, bool &sale){
	Usuario user;
	int QU = database -> cantidad_usuarios, cont = 0, i = 0, longitud;
	char nombre[11], n[11], pwrd[13], p[13];
	bool encontrado = false, coincide = false;
	while((!encontrado || !coincide) && i<3){
		while(!encontrado && i<3){
			cin.ignore(1000, '\n');
			cout<<"Ingrese nombre de usuario: ";
			cin >> ws;
			gets(nombre);
			while(!encontrado && cont < QU){
				strcpy(n, (database->usuarios[cont].nombre));
				if ((strcmp(nombre, n))==0) encontrado = true;
				else cont++;
			}
			if(!encontrado){
				cout<<"Usuario inexistente."<<endl;
				i++;
			}
		}
		while(!coincide && i<3){
			strcpy(p, (database -> usuarios[cont].contrasena));
			cout<<"Ingrese su contrasena: ";
			cin>>ws;
			gets(pwrd);
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
		//puts("Error al abrir el archivo.(Abrir base de datos)");
	}
	return database;
}

//calcula puntos con el tiempo
int calculoPTO(int tiempo){
	int ret;
	if (tiempo<=0) ret = 1000000;
	else ret=60000/tiempo;
	return ret;
}

// mejor por nivel, porcentajes de usrs(n) mejor puntaje por nivel por usuario(y fecha del ptje)
void mostrarPTO(PDB database, int opcion){
	system(cls);
	int num, x = 0, m=0, x2=0;
	bool flag1=true, flag2=true;
	char movimiento, rta;

	if (database->cantidad_usuarios!==0){
		switch(opcion){
			case 1:
			cout<<"| Nivel |   Usuario   | Puntaje "<<endl;

			int mayorXusuario=0, mayorXnivel=0, p_usuario=-1;

			if(database->cantidad_usuarios != 0){
				for (int i =0; i<database->cantidad_usuarios; i++){
					if(database->usuarios[i].tlfacil !=0){ // solo si tiene alguna partida jugada
						for (int j=0;j<database->usuarios[i].tlfacil;j++){
							if(database->usuarios[i].partidasfacil[j].puntaje > mayorXusuario){
								mayorXusuario = database->usuarios[i].partidasfacil[j].puntaje;
							}
						}
						if(mayorXusuario>mayorXnivel){
							mayorXnivel = mayorXusuario;
							p_usuario = i;
						}
					}
				}
				if (p_usuario != -1) cout<<"| 1     | "<<database->usuarios[p_usuario].nombre<<" | "<<mayorXnivel<<endl;
				else cout<<"| 1     | "<<"========== | ==="<<endl;
				mayorXusuario=0;
				mayorXnivel=0;
				p_usuario=-1;

				for (int i =0; i<database->cantidad_usuarios; i++){
					if(database->usuarios[i].tlmedio !=0){ // solo si tiene alguna partida jugada
						for (int j=0;j<database->usuarios[i].tlmedio;j++){
							if(database->usuarios[i].partidasmedio[j].puntaje > mayorXusuario){
								mayorXusuario = database->usuarios[i].partidasmedio[j].puntaje;
							}
						}
						if(mayorXusuario>mayorXnivel){
							mayorXnivel = mayorXusuario;
							p_usuario = i;
						}
					}
				}
				if (p_usuario != -1) cout<<"| 2     | "<<database->usuarios[p_usuario].nombre<<" | "<<mayorXnivel<<endl;
				else cout<<"| 2     | "<<"========== | ==="<<endl;
				mayorXusuario=0;
				mayorXnivel=0;
				p_usuario=-1;

				for (int i =0; i<database->cantidad_usuarios; i++){
					if(database->usuarios[i].tldificil !=0){ // solo si tiene alguna partida jugada
						for (int j=0;j<database->usuarios[i].tldificil;j++){
							if(database->usuarios[i].partidasdificil[j].puntaje > mayorXusuario){
								mayorXusuario = database->usuarios[i].partidasdificil[j].puntaje;
							}
						}
						if(mayorXusuario>mayorXnivel){
							mayorXnivel = mayorXusuario;
							p_usuario = i;
						}
					}
				}
				if (p_usuario != -1) cout<<"| 3     | "<<database->usuarios[p_usuario].nombre<<" | "<<mayorXnivel<<endl;
				else cout<<"| 3     | "<<"========== | ==="<<endl;
				mayorXusuario=0;
				mayorXnivel=0;
				p_usuario=-1;
			}else{
				cout<<"| 1     | "<<"========== | ==="<<endl;
				cout<<"| 2     | "<<"========== | ==="<<endl;
				cout<<"| 3     | "<<"========== | ==="<<endl;
			}
			break;

			case 2:
			cout<<"Ingrese la cantidad de usuarios a mostrar (un numero entre 1 y "<<database->cantidad_usuarios<<"): "<<endl;
			cin>>num;
			while (num<1 || num>database->cantidad_usuarios){
				system(cls);
				cout<<"El numero ingresado no es valido: Ingrese un valor entre 1 y "<<database->cantidad_usuarios<<":"<<endl;
				cin>>num;

			}
			
			while (flag1){
				cout<<"   Usuario   | % Ganadas | % Perdidas | % Abandonos"<<endl;
				x = x2;
				while (flag2 && m<32){
					cout<<" "<<database->usuarios[x].nombre<<" | "<<database->usuarios[x].ganadas<<"        | ";
					cout<<database->usuarios[x].perdidas<<"         | "<<database->usuarios[x].abandonos<<endl;
					x++;
					m++;
					if (x==num) flag2=false;
				}
				cout<<"\n\nUtilice 'j' y 'k' para desplazarse o ingrese 'q' para volver al menu";
				m=0;
				movimiento = getch();
				if (movimiento == 'j' && x2<num-1){
					x2++;
				}else if (movimiento == 'k' && x2>0){
					x2--;
				}else if (movimiento == 'q'){
					cout<<"Desea salir al menu: s / n"<<endl;
					cin>>rta;
					if(rta=='s') flag1 = false;

				}
				system(cls);
			}
			break;

			case 3:
			int mejpartf, mejpartm, mejpartd;
			
			while (flag1){
				cout<<" Usuario     | Nivel |  Fecha   | Mejor Puntaje"<<endl;
				x = x2;
				while (flag2 && m<10){
					mejpartf = mejorPartida(&(database->usuarios[x]), 1);
					mejpartm = mejorPartida(&(database->usuarios[x]), 2);
					mejpartd = mejorPartida(&(database->usuarios[x]), 3);
					cout<<database->usuarios[x].nombre<<" | 1     | ";
					if(mejpartf!=-1){
						cout<<database->usuarios[x].partidasfacil[mejpartf].fecha->dia;
						cout<<"/"<<database->usuarios[x].partidasfacil[mejpartf].fecha->mes<<"/"<<database->usuarios[x].partidasfacil[mejpartf].fecha->anyo;
						cout<<" | "<<database->usuarios[x].partidasfacil[mejpartf].puntaje<<endl;
					}else cout<<"-        | -"<<endl;

					if(mejpartm!=-1){
						cout<<"              | 1     | "<<database->usuarios[x].partidasmedio[mejpartm].fecha->dia;
						cout<<"/"<<database->usuarios[x].partidasmedio[mejpartm].fecha->mes<<"/"<<database->usuarios[x].partidasmedio[mejpartm].fecha->anyo;
						cout<<" | "<<database->usuarios[x].partidasmedio[mejpartm].puntaje<<endl;
					}else cout<<"-        | -"<<endl;

					if(mejpartf!=-1){
						cout<<"              | 1     | "<<database->usuarios[x].partidasdificil[mejpartd].fecha->dia;
						cout<<"/"<<database->usuarios[x].partidasdificil[mejpartd].fecha->mes<<"/"<<database->usuarios[x].partidasdificil[mejpartd].fecha->anyo;
						cout<<" | "<<database->usuarios[x].partidasdificil[mejpartd].puntaje<<endl;
					}else cout<<"-        | -"<<endl;

					x++;
					m++;
					if (x==database->cantidad_usuarios) flag2=false;
				}
				cout<<"\n\nUtilice 'j' y 'k' para desplazarse o ingrese 'q' para volver al menu";
				m=0;
				movimiento = getch();
				if (movimiento == 'j' && x2<num-1){
					x2++;
				}else if (movimiento == 'k' && x2>0){
					x2--;
				}else if (movimiento == 'q'){
					cout<<"Desea salir al menu: s / n"<<endl;
					cin>>rta;
					if(rta=='s') flag1 = false;

				}
				system(cls);
			}



		}
	}else cout<<"No hay ningun dato guardado :("<<endl;
}


// devuelve la posicion de la mejor partida 
// devuelve -1 si el usuario no tiene partidas en ese nivel
int mejorPartida(PUsuario usr, int dif){
	int ret=-1;
	int mejpuntos =0;
	if(dif==1){
		for (int i=0;i<usr->tlfacil;i++){
			if(mejpuntos<=usr->partidasfacil[i].puntaje){
				mejpuntos = usr->partidasfacil[i].puntaje;
				ret = i;
			}
		}
	}else if (dif==2){
		for (int i=0;i<usr->tlmedio;i++){
			if(mejpuntos<=usr->partidasmedio[i].puntaje){
				mejpuntos = usr->partidasmedio[i].puntaje;
				ret = i;
			}
		}
	}else {
		for (int i=0;i<usr->tldificil;i++){
			if(mejpuntos<=usr->partidasdificil[i].puntaje){
				mejpuntos = usr->partidasdificil[i].puntaje;
				ret = i;
			}
		}
	}
	return ret;
}

// encriptar = true: encripta, sino desencripta
void encriptar(PUsuario user, bool encriptar){
	char* referencia; // 58 caracteres posibles para la contrasenia
	int pos, j=0;
	bool encontro=false;
	referencia = {a, b, c, d, e, f, g, h, i, j, k, l, m, n, ñ, o, p, q, r, s, t, u, v, w, x, y, z, $, #, %, &, A, B, C, D, E, F, G, H, I, J, K, L, M, N, Ñ, O, P, Q, R, S, T, U, V, W, X, Y, Z}
	if(encriptar){
		for (int i=0;i<13;i++){
			while (j<58 && !encontro){
				if (user->contrasena[i]==referencia[j]){
					pos = j;
					encontro = true;
				}
			}
			encontro=false;
			user->contrasena[i] = referencia[(pos+7)%58];
		}
	}else{
		for (int i=0;i<13;i++){
			while (j<58 && !encontro){
				if (user->contrasena[i]==referencia[j]){
					pos = j;
					encontro = true;
				}
			}
			encontro=false;
			user->contrasena[i] = referencia[(pos-7)%58];
		}
	}
}















