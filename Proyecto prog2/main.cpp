#include <stdio.h>
#include <string.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <Windows.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#define ancho 1008
#define alto 768
#define M 64
#define N 64
#define MAX 5
#define MAXELEM 300
#define NOMBRE 50
#define MAXRANKING 10

int puntaje(int puntos,int valor);//funcion para el puntaje
int vidas(int corazones);
int menuflg(ALLEGRO_BITMAP*inicio[6], ALLEGRO_EVENT evento, ALLEGRO_EVENT_QUEUE* event_queue, int bandera);
int gameover(ALLEGRO_COLOR[3], ALLEGRO_FONT* font[3], ALLEGRO_EVENT evento, ALLEGRO_EVENT_QUEUE* event_queue, int puntos);


struct player
{
	int posicionX=0;
	int posicionY=0;
	int vidas = 3;
};
struct elements
{
	int posicionX, posicionY;
	int valor=300;
};
struct muro
{
	int posicionX[M][N];
	int posicionY[M][N];
	int posicionpuertaX[2];
	int posicionpuertaY[2];
};
struct enemy 
{
	int posicionX=0;
	int posicionY=0;
	int originalX = 0;
	int originalY = 0;
	int personalidad = 0;
};
struct user 
{
	char nombre[30]="NA";
	int puntaje=0;
};

typedef struct elements elementos;
typedef struct user usuario;
typedef struct player jugador;
typedef struct enemy zombies ;

//ctr+k->c
void setmapa(const char*file_name);
int leerrnk(usuario rnk[MAXRANKING]);
int verrnk(ALLEGRO_BITMAP* inicio[6],ALLEGRO_COLOR colors[3], ALLEGRO_FONT* font[3], ALLEGRO_EVENT evento, ALLEGRO_EVENT_QUEUE* event_queue, usuario rnk[MAXRANKING]);
int guardarrnk(ALLEGRO_COLOR colors[3], ALLEGRO_FONT* font[3], ALLEGRO_EVENT evento, ALLEGRO_EVENT_QUEUE* event_queue, int puntos, int bandera,usuario rnk[MAXRANKING],int id);
void ordenar(usuario rank[MAXRANKING]);
void guardarrnk2( usuario rnk[MAXRANKING]);
int personalidad(zombies enemigo[], int i);
jugador setjugador();
elementos cargarelementos(int index);
zombies cargarenemigo(int index);
struct muro cargarpared();

using namespace std;

usuario rank[MAXRANKING];
char mat[M][N];
int mouseX = -1;
int mouseY = -1;
/*int posicionX = 0 , posicionY = 0;*/




ALLEGRO_SAMPLE* musica = NULL;
ALLEGRO_SAMPLE_INSTANCE* songinstance = NULL;
////////////////////////////////////////////////////////////////////    CODIGO    //////////////////////////////////////////////////////////////////////////////////////////
int main()
{
	srand(time(0));
	jugador jugador;/*movimiento jugador en posiciones X e Y*/
	elementos elemento[MAXELEM];
	zombies enemigo[MAX];
	muro pared;

	int i = 0, j = 0, cont = 1, contenemigos = 0, movimientojugador = 0;
	int x = 0, y = 0, bandera = 0, gmrv, derecha = 0, izquierda = 0, arriba = 0, abajo = 0, png = 0;
	int rotacion = 1, nivel = 10000, nivel_cont = 1, stge_nivel = 0, id = 0, azar, saverank = 0, puntos = 0, index_user = 0;
	int keycode, escribir_nombre=0;

	char buffer;
	char username[NOMBRE]="";
	int tecla=0;
	const char* sala = "datos/mapas/mapa";
	char* dir;
	al_init();			/*iniciaciones*/
	
	al_install_keyboard();//teclado

	al_install_mouse();//mouse

	al_init_image_addon();//imagenes

	al_init_font_addon();//fuentes de letras
	al_init_ttf_addon();

	al_init_primitives_addon();//figuras

	al_init_acodec_addon();//audios
	al_install_audio();

	ALLEGRO_DISPLAY* ventana = al_create_display(ancho, alto);/*crear una ventana*/

	int ANCHO_W = GetSystemMetrics(SM_CXSCREEN);/*obtener los pixeles de la pantalla que se utiliza*/
	int ALTO_W = GetSystemMetrics(SM_CYSCREEN);

	al_set_window_title(ventana, "primer avance");/*cambio de titulo*/

	
	
	/////////////////////////////////////////////////////////    IMAGENES     ////////////////////////////////////////////////////////////////////////
	ALLEGRO_BITMAP* bloque = al_load_bitmap("datos/imagenes/pared(1).jpg");/*imagenes a utilizar*/
	ALLEGRO_BITMAP* player = al_load_bitmap("datos/imagenes/pacman.png");
	ALLEGRO_BITMAP* fondo = al_load_bitmap("datos/imagenes/fondo(1).jpg");
	ALLEGRO_BITMAP* zombi = al_load_bitmap("datos/imagenes/pacman.png");
	ALLEGRO_BITMAP* objeto = al_load_bitmap("datos/imagenes/pacman.png");
	ALLEGRO_BITMAP* entrada = al_load_bitmap("datos/imagenes/puerta1.jpg");
	ALLEGRO_BITMAP* corazones = al_load_bitmap("datos/imagenes/vidas.jpg");
	/////////////////////////////////////////////////////////    MENU     ////////////////////////////////////////////////////////////////////////
	ALLEGRO_BITMAP* menu = al_load_bitmap("datos/imagenes/menu.png");//0
	ALLEGRO_BITMAP* jugar = al_load_bitmap("datos/imagenes/jugar.png");//1
	ALLEGRO_BITMAP* opciones = al_load_bitmap("datos/imagenes/opciones.png");//2
	ALLEGRO_BITMAP* ranking = al_load_bitmap("datos/imagenes/ranking.png");//3
	ALLEGRO_BITMAP* salir = al_load_bitmap("datos/imagenes/salir.png");//4
	ALLEGRO_BITMAP* puntajes = al_load_bitmap("datos/imagenes/puntajes.png");//5
	ALLEGRO_BITMAP* inicio[6] = { al_load_bitmap("datos/imagenes/menu.png"),al_load_bitmap("datos/imagenes/jugar.png"),al_load_bitmap("datos/imagenes/opciones.png"),al_load_bitmap("datos/imagenes/ranking.png"),al_load_bitmap("datos/imagenes/salir.png"), al_load_bitmap("datos/imagenes/puntajes.png") };
	
	//////////////////////////////////////////////////////       LETRAS     ////////////////////////////////////////////////////////////////////////
	ALLEGRO_FONT* letras = al_load_font("datos/fuentes/AldotheApache.ttf", 50, 0);
	ALLEGRO_FONT* letras1 = al_load_font("datos/fuentes/AldotheApache.ttf", 20, 0);
	ALLEGRO_FONT* letras3 = al_load_font("datos/fuentes/AldotheApache.ttf", 40, 0);
	ALLEGRO_FONT* font[3] = { al_load_font("datos/fuentes/AldotheApache.ttf", 50, 0) ,al_load_font("datos/fuentes/AldotheApache.ttf", 20, 0),al_load_font("datos/fuentes/AldotheApache.ttf", 40, 0) };

	////////////////////////////////////////////////////////     COLORES     //////////////////////////////////////////////////////////////////////////////////////////
	ALLEGRO_COLOR negro = al_map_rgb(0, 0, 0);
	ALLEGRO_COLOR blanco = al_map_rgb(255, 255, 255);
	ALLEGRO_COLOR rojo = al_map_rgb(255, 0, 0);
	ALLEGRO_COLOR azul = al_map_rgb(0, 0,255);
	ALLEGRO_COLOR colors[3]{ negro,blanco,rojo };

	ALLEGRO_KEYBOARD_STATE* state{};

	ALLEGRO_EVENT evento;
	ALLEGRO_EVENT_QUEUE* event_queue;
	ALLEGRO_TIMER* fps = al_create_timer(1.0 / 5);

	event_queue = al_create_event_queue();
	
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(fps));
	al_start_timer(fps);

	al_reserve_samples(10);
	musica = al_load_sample("datos/musica/papaya song.ogg");

	songinstance = al_create_sample_instance(musica);
	al_set_sample_instance_playmode(songinstance, ALLEGRO_PLAYMODE_LOOP);

	al_attach_sample_instance_to_mixer(songinstance, al_get_default_mixer());

	setmapa("datos/mapas/mapa1.txt");//funcion para cargar mapa
	id=leerrnk(rank);

	jugador = setjugador();
	x = jugador.posicionX;
	y = jugador.posicionY;

	for (i = 0; i < MAX; i++)
	{
		enemigo[i] = cargarenemigo(i);
	}
	
	for (i = 0; i < MAXELEM; i++)
	{
		elemento[i] = cargarelementos(i);
	}

	pared = cargarpared();


	//al_play_sample_instance(songinstance);
	al_convert_mask_to_alpha(zombi, negro);
	al_convert_mask_to_alpha(objeto, negro);
	al_convert_mask_to_alpha(player, negro);
	///////////////////////////////////////////////////////////////// WHILE  ////////////////////////////////////////////////////////////////////////
	while (true)
	{
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_wait_for_event(event_queue, &evento);

		if (bandera == -1)
		{
			return 0;
		}
		if (bandera == 0)//menu 
		{
			al_show_mouse_cursor(ventana);
			bandera = menuflg(inicio, evento, event_queue, bandera);
			if (bandera == 1)
			{
				al_hide_mouse_cursor(ventana);
			}
			cont = 1;
		}
		if (bandera == 1)//juego
		{
			//al_play_sample_instance(songinstance);
			al_draw_bitmap(fondo, 0, 0, 0);
			//printf("cont=%d\n", cont);

			for (i = 0; i < 2; i++)
			{
				if (nivel >= puntos);
				{
					al_draw_bitmap_region(entrada, 8, 12, 8, 14, pared.posicionpuertaX[i] * 16, pared.posicionpuertaY[i] * 12, 0);
				}
				if (nivel < puntos)
				{
					al_draw_bitmap_region(entrada, 18, 12, 11, 15, pared.posicionpuertaX[i] * 16, pared.posicionpuertaY[i] * 12, 0);

					if (jugador.posicionX == pared.posicionpuertaX[i] && jugador.posicionY == pared.posicionpuertaY[i])
					{
						stge_nivel = 1;
						nivel = nivel * nivel_cont;
					}
				}
			}

			if (stge_nivel == 1)
			{
				cont++;

				switch (cont)
				{
				case 2:
				{
					printf("mapa2\n");
					setmapa("datos/mapas/mapa2.txt");
					break;
				}
				case 3:
				{

					printf("mapa3\n");
					setmapa("datos/mapas/mapa3.txt");
					break;
				}
				case 4:
				{
					printf("mapa4\n");
					setmapa("datos/mapas/mapa4.txt");
					break;
				}
				case 5:
				{
					printf("mapa5\n");
					setmapa("datos/mapas/mapa5.txt");
					break;
				}
				case 6:
				{
					printf("mapa6\n");
					setmapa("datos/mapas/mapa6.txt");
					break;
				}
				default:
					break;
				}
				jugador = setjugador();
				for (i = 0; i < MAX; i++)
				{
					enemigo[i] = cargarenemigo(i);
				}
				for (i = 0; i < MAXELEM; i++)
				{
					elemento[i] = cargarelementos(i);
				}
				pared = cargarpared();
				stge_nivel = 0;
			}


			for (i = 0; i < M; i++)
			{
				for (j = 0; j < N; j++)
				{
					al_draw_bitmap(bloque, pared.posicionX[i][j], pared.posicionY[i][j], 0);//bloques
				}
			}

			for (i = 0; i < MAX; i++)
			{
				al_draw_bitmap_region(zombi, 456, 64, 15, 15, enemigo[i].posicionX * 16, enemigo[i].posicionY * 12, 0);
			}

			for (i = 0; i < MAXELEM; i++)//objetos
			{
				//al_draw_bitmap(objeto, elemento[i].posicionX, elemento[i].posicionY, 0);
				al_draw_bitmap_region(objeto, 603, 49, 8, 14, elemento[i].posicionX, elemento[i].posicionY, 0);
			}

			switch (png)//dibujo del jugador
			{
			case 0:
			{
				rotacion = 1;
				break;
			}
			case 1:
			{
				rotacion = 17;
				break;
			}
			case 2:
			{
				rotacion = 34;
				break;
			}
			case 3:
			{
				rotacion = 49;
				break;
			}
			break;
			}

			al_draw_bitmap_region(player, 473, rotacion, 12, 12, jugador.posicionX * 16, jugador.posicionY * 12, 0);//abajo




			if (jugador.posicionX * 16 > ancho - 1)
			{
				jugador.posicionX = 0;
			}
			if (jugador.posicionX * 16 < -1)
			{
				jugador.posicionX = 64 - 2;
			}

			if (jugador.posicionY * 12 > alto - 1)
			{
				//printf("posicionY%d\n", jugador.posicionY);
				jugador.posicionY = 0;
			}
			if (jugador.posicionY * 12 < -1)
			{
				//printf("jugadorY=%d\n", jugador.posicionY);
				//printf("posicionY%d\n", jugador.posicionY);
				jugador.posicionY = 64 - 1;
			}

			for (i = 0; i < MAXELEM; i++)//suma de puntaje
			{
				if (jugador.posicionX * 16 == elemento[i].posicionX && jugador.posicionY * 12 == elemento[i].posicionY)
				{
					puntos = puntaje(puntos, elemento[i].valor);
					elemento[i].posicionX = -30;
					elemento[i].posicionY = -30;
				}
			}

			for (i = 0; i < MAX; i++)
			{
				if (jugador.posicionX * 16 == enemigo[i].posicionX * 16 && jugador.posicionY * 12 == enemigo[i].posicionY * 12)//cuando el jugador choca con un enemigo
				{
					for (j = 0; j < MAX; j++)
					{
						enemigo[j].posicionX = enemigo[j].originalX;
						enemigo[j].posicionY = enemigo[j].originalY;
					}
					jugador.vidas = vidas(jugador.vidas);//copiar esto para el movimiento enemigo
					jugador.posicionX = x;
					jugador.posicionY = y;
					if (jugador.vidas == 0)
					{
						al_show_mouse_cursor(ventana);
						saverank = 0;
						bandera = 2;
					}
				}
			}

			contenemigos = contenemigos + 1;
			//printf("contenemigos = %d\n", contenemigos);
			if (contenemigos == 5)
			{
				for (i = 0; i < MAX; i++)
				{
					switch (enemigo[i].personalidad)
					{
					//aleatorio
					case 0:
					{
						azar = rand()%4;
						switch (azar)
						{
						case 0:
						{
							if (mat[enemigo[i].posicionX + 1][enemigo[i].posicionY] != 'x')//colision con paredes
							{
								enemigo[i].posicionX = enemigo[i].posicionX + 1;
							}
							break;
						}
						case 1:
						{
							if (mat[enemigo[i].posicionX - 1][enemigo[i].posicionY] != 'x')
							{
								enemigo[i].posicionX = enemigo[i].posicionX - 1;
							}
							break;
						} 
						case 2:
						{
							if (mat[enemigo[i].posicionX][enemigo[i].posicionY+1] != 'x')
							{
								enemigo[i].posicionY = enemigo[i].posicionY + 1;
							}
							break;
						}
						case 3:
						{
							if (mat[enemigo[i].posicionX][enemigo[i].posicionY - 1] != 'x')
							{
								enemigo[i].posicionY = enemigo[i].posicionY - 1;
							}
							break;
						}
						default:
							break;
						}
					
						
						break;
					}
					//buscar
					case 1:
					{
						for (j = 0; j < MAX; j++)
						{
							if (enemigo[i].posicionX * 16 == enemigo[j].posicionX * 16 && enemigo[i].posicionY * 12 == enemigo[j].posicionY * 12 && i!=j)//cuando el jugador choca con un enemigo
							{
								enemigo[j].personalidad = 0;
							}
						}

						if (enemigo[i].posicionX > jugador.posicionX && mat[enemigo[i].posicionX - 1][enemigo[i].posicionY] != 'x')
						{
							enemigo[i].posicionX--;
						}

						else if (enemigo[i].posicionX < jugador.posicionX && mat[enemigo[i].posicionX + 1][enemigo[i].posicionY] != 'x')
						{
							enemigo[i].posicionX++;
						}

						if (enemigo[i].posicionY > jugador.posicionY && mat[enemigo[i].posicionX][enemigo[i].posicionY-1] != 'x')
						{
							enemigo[i].posicionY--;
						}

						else if (enemigo[i].posicionY < jugador.posicionY && mat[enemigo[i].posicionX][enemigo[i].posicionY + 1] != 'x')
						{
							enemigo[i].posicionY++;
						}
						break;
					}
					//busca+
					case 2:
					{
						if (mat[enemigo[i].posicionX - 1][enemigo[i].posicionY] != 'x')//colision con paredes
						{
							enemigo[i].posicionX = enemigo[i].posicionX - 1;
						}
						break;
					}
					//lineal || puertas
					case 3:
					{
						if (mat[enemigo[i].posicionX][enemigo[i].posicionY - 1] != 'x')//colision con paredes
						{
							enemigo[i].posicionY = enemigo[i].posicionY - 1;
						}
						break;
					}
					default:
						break;
					}
					contenemigos = 0;

				}
			}


			al_convert_mask_to_alpha(corazones, azul);

			al_draw_textf(letras3, negro, ancho - 16 * 11, 12, NULL, "%d", puntos);

			if (jugador.vidas>=1)
			{
				al_draw_bitmap_region(corazones, 4, 0, 28, 28, ancho - 16 * 2, 12, 0);
			}
			if (jugador.vidas>=2)
			{
				al_draw_bitmap_region(corazones, 4, 0, 28, 28, ancho - 16 * 4, 12, 0);
			}
			if (jugador.vidas==3)
			{
				al_draw_bitmap_region(corazones, 4, 0, 28, 28, ancho - 16 * 6, 12, 0);
			}

			if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
			{
				switch (evento.keyboard.keycode)/*hacer una accion para cada tecla pulsada*/
				{
				case ALLEGRO_KEY_D:
				{
					if (mat[jugador.posicionX + 1][jugador.posicionY] != 'x')//colision con paredes
					{
						png = 0;
						jugador.posicionX = jugador.posicionX + 1;
						movimientojugador = 1;
					}
					break;
				}
				case ALLEGRO_KEY_A:
				{
					if (mat[jugador.posicionX - 1][jugador.posicionY] != 'x')//colision con paredes
					{
						png = 1;
						jugador.posicionX = jugador.posicionX - 1;
						movimientojugador = 1;
					}
					break;
				}
				case ALLEGRO_KEY_W:
				{
					if (mat[jugador.posicionX][jugador.posicionY - 1] != 'x')//colision con paredes
					{
						png = 2;
						jugador.posicionY = jugador.posicionY - 1;
						movimientojugador = 1;
					}
					break;
				}
				case ALLEGRO_KEY_S:
				{
					if (mat[jugador.posicionX][jugador.posicionY + 1] != 'x')//colision con paredes
					{
						png = 3;
						jugador.posicionY = jugador.posicionY + 1;
						movimientojugador = 1;
					}
					break;
				}
				case ALLEGRO_KEY_ESCAPE:
				{
					return 0;
				}
				case ALLEGRO_KEY_ENTER:
				{
					al_show_mouse_cursor(ventana);
					bandera = 2;
					break;
				}
				case ALLEGRO_KEY_0:
				{
					bandera = 2;
					al_stop_sample_instance(songinstance);
					break;
				}
				default:
					break;
				}
			}
		}
		if (bandera == 2)//nombre
		{
			al_clear_to_color(negro);
			if (saverank == 0)
			{
				if (puntos > rank[MAXRANKING-1].puntaje )
				{
					//al_draw_text(letras, blanco, 16 * 15, alto / 2 + 12 * 10, NULL, "ingrese nombre usuario: ");

					index_user = 0;

					while (escribir_nombre == 0) 
					{

						al_clear_to_color(negro);
						al_draw_bitmap(puntajes, 0, 0, 0);
						al_wait_for_event(event_queue, &evento);
						if (evento.type == ALLEGRO_EVENT_KEY_DOWN) 
						{
							if (evento.keyboard.keycode >= ALLEGRO_KEY_A && evento.keyboard.keycode <= ALLEGRO_KEY_Z) 
							{
								username[index_user] = username[index_user] + 'a' + (evento.keyboard.keycode - ALLEGRO_KEY_A);
								printf("%c", username[index_user]);
								index_user++;
							}
						}
						if (evento.type == ALLEGRO_EVENT_KEY_DOWN) 
						{
							if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER) 
							{
								escribir_nombre = 1;
							}
						}
						al_draw_text(letras, blanco, 16 * 5, alto / 2 - 12 * 8, 0, "Escriba su nombre:");
						al_draw_textf(letras, blanco, ancho / 2 - 16 * index_user, alto / 2 + 12 * 6, 0, "%s", username);
						al_flip_display();
					}
					
					strcpy(rank[MAXRANKING-1].nombre, username);
					rank[MAXRANKING - 1].puntaje = puntos;
					printf("%s %d\n", rank[MAXRANKING - 1].nombre, rank[MAXRANKING - 1].puntaje);
					ordenar(rank);
					guardarrnk2(rank);
				}

				saverank++;
			}
			if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
			{
				switch (evento.keyboard.keycode)/*hacer una accion para cada tecla pulsada*/
				{
				case ALLEGRO_KEY_1:
				{
					bandera = 3;
				}
				default:
					break;
				}
			}
		}
		if (bandera == 3)//game over
		{
			gmrv = gameover(colors, font, evento, event_queue, puntos);
			if (gmrv == 1)
				return 0;
			if (gmrv == 2)
				bandera = 0;
		}
		if (bandera == 4)/*ranking*/
		{
			bandera = verrnk(inicio,colors, font, evento, event_queue, rank);
		}
		al_flip_display();
	}
	
	return 0;
}
void setmapa(const char* file_name)
{
	int i = 0, j = 0;
	//char aux;
	FILE* mapa;

	mapa = fopen(file_name, "r");

	for (i = 0; i < M; i++)
	{
		for (j = 0; j < N; j++)
		{
			fscanf(mapa, "%c", &mat[j][i]);
		}
		//fscanf(mapa, "%c",&aux);
	}
	if (mapa == NULL)
	{
		printf("no se abrio el archivo");
		exit(-1);
	}
	fclose(mapa);
}

jugador setjugador()
{
	jugador jugador;
	int i=0, j=0;
	
	for (i = 0; i < M; i++)
	{
		for (j = 0; j < N; j++)
		{
			if (mat[i][j] == 'p')
			{
				jugador.posicionX = i ;
				jugador.posicionY = j ;
				return jugador;
			}
		}
	}
	
}
//arreglar funcion enemigos || devolver un arreglo de estructura
zombies cargarenemigo(int index)
{
	zombies enemigo[MAX];
	int i = 0, j = 0,cont = 0;

	srand(time(0));

	for (i = 0; i < M; i++)
	{
		for (j = 0; j < N; j++)
		{

			//printf("%c", mat[i][j]);
			if (mat[i][j] == 'e' && cont <= MAX)
			{
				enemigo[cont].personalidad = 0;
				enemigo[cont].posicionX = i ;
				enemigo[cont].posicionY = j ;
				enemigo[cont].originalX = i;
				enemigo[cont].originalY = j;

				cont++;
			}
			
		}
	}
	return enemigo[index];
}

elementos cargarelementos(int index)
{
	elementos elemento[MAXELEM];
	int i = 0, j = 0,cont=0;

	for (i = 0; i < M; i++)
	{
		for (j = 0; j < N; j++)
		{
			if (mat[i][j] == 'v' && cont <MAXELEM)
			{
				elemento[cont].posicionX = i * 16;
				elemento[cont].posicionY = j * 12;
				cont++;
			}
		}
	}
	return elemento[index];
}

struct muro cargarpared()
{
	struct muro pared ;
	int i = 0, j = 0,cont=0;

	for (i = 0; i < M; i++)
	{
		for (j = 0; j < N; j++)
		{
			if (mat[i][j] == 'x')
			{
				pared.posicionX[i][j] = i * 16;
				pared.posicionY[i][j] = j * 12;
			}

		}
	} 
	for (i = 0; i < M; i++)
	{
		for (j = 0; j < N; j++)
		{
			if (mat[i][j] == 'm' && cont < 2)
			{
				pared.posicionpuertaX[cont] = i ;
				pared.posicionpuertaY[cont] = j ;
				cont++;
			}
		}
	}
	return pared;
}

int puntaje(int puntos,int valor)
{
	user usuario;
	usuario.puntaje = puntos + valor;
	return usuario.puntaje;
}

int vidas(int corazones)
{
	corazones = corazones - 1;
	return  corazones;
}

int menuflg(ALLEGRO_BITMAP* inicio[6], ALLEGRO_EVENT evento, ALLEGRO_EVENT_QUEUE* event_queue, int bandera)
{
	al_draw_bitmap(inicio[0], 0, 0, 0);

	if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		switch (evento.keyboard.keycode)
		{
		case ALLEGRO_KEY_K:
		{
			bandera = 1;
			break;
		}
		case ALLEGRO_KEY_ESCAPE:
		{
			return -1;
		}
		case ALLEGRO_KEY_0:
		{
			return 2;
		}
		default:
			break;
		}
	}

	if (evento.type == ALLEGRO_EVENT_MOUSE_AXES)
	{
		mouseX = evento.mouse.x;
		mouseY = evento.mouse.y;
		//printf("x = %d\ty = %d\n", mouseX, mouseY);
	}

	if (mouseX < 983 && mouseX > 679 && mouseY < 195 && mouseY > 133)//si pasa el mouse hace un cambio 
	{
		al_draw_bitmap(inicio[1], 0, 0, 0);//juego
	}
	if (mouseX < 981 && mouseX > 678 && mouseY < 322 && mouseY > 261)
	{
		al_draw_bitmap(inicio[2], 0, 0, 0);//opciones
	}
	if (mouseX < 325 && mouseX > 22 && mouseY < 663 && mouseY > 602)
	{
		al_draw_bitmap(inicio[3], 0, 0, 0);//ranking
	}
	if (mouseX < 981 && mouseX > 678 && mouseY < 454 && mouseY > 392)
	{
		al_draw_bitmap(inicio[4], 0, 0, 0);//salir
	}
	if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
	{
		switch (evento.mouse.button)
		{
		case 1:
		{
			if (mouseX < 983 && mouseX > 679 && mouseY < 195 && mouseY > 133)//jugar
			{
				bandera = 1;
			}
			if (mouseX < 981 && mouseX > 678 && mouseY < 454 && mouseY > 392)//salir
			{
				bandera = -1;
			}
			if (mouseX < 325 && mouseX > 22 && mouseY < 663 && mouseY > 602)//ranking
			{
				bandera = 4;
			}
			break;
		}
		default:
			break;
		}
	}
	return bandera;
}

int gameover(ALLEGRO_COLOR colors[3], ALLEGRO_FONT* font[3], ALLEGRO_EVENT evento, ALLEGRO_EVENT_QUEUE* event_queue, int puntos)
{
	al_draw_textf(font[0], colors[2], ancho / 2 - 16 * 14, alto / 4 + 12 * 3, NULL, "tus puntos fueron de :%d", puntos);
	al_draw_text(font[0], colors[1], ancho / 2 - 16 * 6, alto / 5, NULL, "GAME OVER");
	al_draw_text(font[1], colors[1], ancho / 2 - 16 * 6, alto / 2 + 12 * 6, NULL, "presione escape para salir");
	al_draw_text(font[1], colors[1], ancho / 2 - 16 * 6, alto / 2 + 12 * 8, NULL, "presione tab para volver al menu");
	if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		switch (evento.keyboard.keycode)/*hacer una accion para cada tecla pulsada*/
		{
		case ALLEGRO_KEY_ESCAPE:
		{
			return 1;
		}
		case ALLEGRO_KEY_TAB:
		{
			return 2;
		}
		default:
			break;
		}
	}
	return 0;
}

int leerrnk(usuario rnk[MAXRANKING])
{
	int id = 0;
	FILE* ranking;
	ranking = fopen("datos/ranking/rank.txt", "r+");
	if (ranking == NULL)
	{
		printf("no se abrio el archivo");
		exit(-1);
	}
	for (id = 0; id < MAXRANKING; id++)
	{
		fscanf(ranking, "%s", rank[id].nombre);
		fscanf(ranking, "%d", &rank[id].puntaje);
	}
	fclose(ranking);
	return id;
}

//ranking(primero iniciar,segundo leer, tercero guardar)
//int leerrnk2(usuario rnk[MAXRANKING] )
//{
//	int id=0;
//	FILE* ranking;
//	ranking = fopen("datos/ranking/rank.txt", "r+");
//	if (ranking == NULL)
//	{
//		printf("no se abrio el archivo");
//		exit(-1);
//	}
//	while (!feof(ranking))
//	{
//		fscanf(ranking, "%s", rank[id].nombre);
//		fscanf(ranking, "%d", &rank[id].puntaje);
//		//printf("%d.-%s %d\n", id, rank[id].nombre, rank[id].puntaje);
//		id++;
//	}
//	
//	fclose(ranking);
//	return id;
//}

int verrnk(ALLEGRO_BITMAP* inicio[6],ALLEGRO_COLOR colors[3], ALLEGRO_FONT* font[3], ALLEGRO_EVENT evento, ALLEGRO_EVENT_QUEUE* event_queue,usuario rnk[MAXRANKING])
{
	int i=1,j=0,id=0;
	al_draw_bitmap(inicio[5], 0, 0, 0);
	for (id = 0; id < MAXRANKING; id++)
	{
		j++;
		printf("%s\t%d\n", rank[id].nombre, rank[id].puntaje);

		al_draw_text(font[0], colors[1],  16 * 8, 12 * 8, NULL, "presione escape para volver al menu");
		al_draw_textf(font[2], colors[1], ancho / 2 - 16 * 8, alto / 4 + 12 * i, NULL, "%d.-%s\t%d",j,rank[id].nombre,rank[id].puntaje);
		i = i + 4;
	}
	if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		switch (evento.keyboard.keycode)/*hacer una accion para cada tecla pulsada*/
		{
		case ALLEGRO_KEY_ESCAPE:
		{
			return 0;
		}
		default:
			break;
		}
	}
	return 4;
}
//----- 500 "----- = nombres"
//----- 450
//----- 400
//#define MAXRRANK 5
//main()
//	struct rank ranking[MAXRANK]
//	loadranking(ranking)
int guardarrnk(ALLEGRO_COLOR colors[3], ALLEGRO_FONT* font[3], ALLEGRO_EVENT evento, ALLEGRO_EVENT_QUEUE* event_queue, int puntos, int bandera, usuario rnk[MAXRANKING],int id)
{
	int i = 0, espacio = ancho / 2 - 16 * 6;

	al_clear_to_color(colors[0]);
	al_draw_text(font[1], colors[1], 16*15, alto / 2 + 12 * 10, NULL, "ingrese nombre usuario: ");

	if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		switch (evento.keyboard.keycode)/*hacer una accion para cada tecla pulsada*/
		{
		case ALLEGRO_KEY_1:
		{
			return 3;
		}
		default:
			break;
		}
	}
	return bandera;
}

void guardarrnk2( usuario rnk[MAXRANKING])
{
	int i = 0;
	FILE* ranking;
	ranking = fopen("datos/ranking/rank.txt", "w");

	if (ranking == NULL)
	{
		printf("no se abrio el archivo");
		exit(-1);
	}
	for (i = 0; i < MAXRANKING; i++)
	{
		fprintf(ranking, "%s ", rank[i].nombre);
		fprintf(ranking, " %d\n", rank[i].puntaje);
	}
	fclose(ranking);
}

void ordenar(usuario rank[MAXRANKING])
{
	int id = 0, i = 1;
	usuario aux;
	for (i = MAXRANKING; i > 0; i--)
	{
		if (rank[i].puntaje > rank[i - 1].puntaje)
		{
			aux = rank[i - 1];
			rank[i - 1] = rank[i];
			rank[i] = aux;
		}
	}
}