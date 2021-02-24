struct Fecha{
	int dia, mes, anyo;
};

typedef struct Fecha* Pfecha;

struct Partida{
	// la dificultad es un numero entre 1 y 3 inclusive
	// el tipo es G, P o A, ganada perdida o abandonada
	
	int dificultad, puntaje;
	char tipo;
	Fecha fecha;
};

typedef struct Partida* PPartida;

struct Usuario{
	int gan, perd, ab, tlfacil, tlmedio, tldificil;
	char nombre[11], contrasena[13];
	double perdidas, ganadas, abandonos;
	Partida partidasfacil[10], partidasmedio[10], partidasdificil[10];
	// un array dinamico puede venir bien para almacenar los objetos de tipo Partida, 
	//es decir con la 
	// informacion de las partidas de cada dificultad para cada usuario. 
	//Seria un arreglo con 3 espacios
	// y en cada uno se almacenaria un arreglo dinamico.
};

typedef struct Usuario* PUsuario;


struct DB{
	Usuario usuarios[100];
	int cantidad_usuarios = 0;
    int mejorFacil, mejorMedio, mejorDificil;
};

typedef struct DB* PDB;

bool ExisteBDD(PDB database);
void crearBaseDeDatos(PDB database);
void crearUsuario(PDB database);
void GuardarPartida (PPartida match, int dif, int score, char tipo);
void obtenerFecha(Pfecha date);
int QueMesEs(char mes[3]);
void partidaAUsuario(PPartida match, PUsuario usr);
void guardarDB(PDB database);
Usuario AbrirUsuario (PDB database, bool &sale, int &index);
DB AbrirBaseDeDatos();
void OrdenarUsuarios (PDB database);
void mejorXNiv(PDB db);
void ordenarPartidas(PUsuario user, int dificultad);
void strFecha(char fecha[11], Fecha fech);
void DibujarCuad(const char *str, int ancho, char separador='|');
void Puntaje(int opcion, PDB);
void encriptar(PUsuario user, bool encriptar);
void crearPartidasArtificial (PUsuario user);
void crearUsuariosArt(PDB db, int cantidad);
void usuarioADB (Usuario user, PDB database, int index);
