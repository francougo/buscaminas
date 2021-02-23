#include "structs.cpp"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>

using namespace std; 

int main(){
	DB db = AbrirBaseDeDatos();
	crearUsuariosArt(&db, 10);
	for (int i=0;i<db.cantidad_usuarios;i++){
		crearPartidasArtificial(&(db.usuarios[i])); //crea un set de partidas para cada user
		//cout<<"antes facil: "<<db.usuarios[i].partidasfacil[0].puntaje<<endl;
		ordenarPartidas(&db.usuarios[i], 1);
		//cout<<"desp facil: "<<db.usuarios[i].partidasfacil[0].puntaje<<endl;
		ordenarPartidas(&db.usuarios[i], 2);
		ordenarPartidas(&db.usuarios[i], 3);
		
	}
	OrdenarUsuarios(&db);
	cout<<"Antes mxn: "<<endl;
	cout<<db.mejorFacil<<" "<<db.mejorMedio<<" "<<db.mejorDificil<<endl;
	mejorXNiv(&db);             // actualiza el ranking 
	cout<<"Desp mxn: "<<endl;
	cout<<db.mejorFacil<<" "<<db.mejorMedio<<" "<<db.mejorDificil<<endl;
	guardarDB(&db);
	
	
}
	void crearPartidasArtificial (PUsuario user){
		Partida match;
		PPartida Pmatch = &match;
		int a, b;
		char c;
		for (int j = 0; j<30; j++){
			a = 1 + rand()% 3;
			b = rand() % 60000;
			if ((a + b)%3==0){
				c = 'G';
			}else{
				if ((a+b)%3==1){
					c = 'P';
				}else{
					c = 'A';
				}
			}
			GuardarPartida(Pmatch, a, b, c);
			partidaAUsuario (Pmatch, user);
		}
	}
		
		void crearUsuariosArt(PDB db, int cantidad){     // debe usarse en una base de datos vacia unicamente
			Usuario user;
			char numeroU[3] = "aa", n[11]="usuario";
			for (int i=0;i<cantidad;i++){
				strcpy(n, "usuario");
				sprintf(numeroU, "%d", i);
				strcat(n, numeroU);       // usuario01, usuario02, etc
				strcpy(user.nombre, n);
				strcpy(user.contrasena, "cont12#");     // misma contrasena para todos
				user.perdidas = 0;
				user.ganadas = 0;
				user.abandonos = 0;
				user.gan = 0;
				user.perd = 0;
				user.ab = 0;
				user.tlfacil = 0;
				user.tlmedio = 0;
				user.tldificil = 0;
				encriptar(&user, true);
				db->usuarios[i] = user;
				db->cantidad_usuarios++;
			}
		} 

