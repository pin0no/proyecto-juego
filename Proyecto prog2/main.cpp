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

#define ancho 1024
#define alto 768
#define M 64
#define N 64
#define MAX 5
#define MAXELEM 5
int puntaje(int puntos);//funcion para el puntaje
int vidas(int corazones);
int gameover(int puntos);

struct posicion
{
	int posicionX=0;
	int posicionY=0;
};
struct muro
{
	int posicionX[M][N];
	int posicionY[M][N];
};
struct enemy 
{
	int posicionX=0;
	int posicionY=0;
};

typedef struct posicion movimiento;
typedef struct enemy zombies ;
movimiento cargarmapa();
movimiento cargarelementos(int index);
zombies cargarenemigo(int index);
struct muro cargarpared();

using namespace std;

int mat[M][N];
/*int posicionX = 0 , posicionY = 0;*/


ALLEGRO_EVENT_QUEUE* event_queue;

int main()
{
	srand(time(0));

	movimiento jugador, elemento[MAXELEM];/*movimiento jugador en posiciones X e Y*/
	zombies enemigo[MAX];
	struct muro pared;

	int i = 0, j = 0, cont = 0, puntos = 0,contenemigos=0;
	int x=0, y=0, bandera=0,gmrv;
	int corazones=3;

	al_init();			/*iniciaciones*/

	al_install_keyboard();//teclado

	al_init_image_addon();//imagenes

	al_init_font_addon();//fuentes de letrras
	al_init_ttf_addon();

	al_init_primitives_addon();//figuras

	al_init_acodec_addon();//audios
	al_install_audio();

	ALLEGRO_DISPLAY* ventana = al_create_display(ancho, alto);/*crear una ventana*/

	int ANCHO_W = GetSystemMetrics(SM_CXSCREEN);/*obtener los pixeles de la pantalla que se utiliza*/
	int ALTO_W = GetSystemMetrics(SM_CYSCREEN);

	al_set_window_title(ventana, "primer avance");/*cambio de titulo*/

	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	
	ALLEGRO_BITMAP* bloque = al_load_bitmap("datos/imagenes/pared.png");/*imagenes a utilizar*/
	ALLEGRO_BITMAP* player = al_load_bitmap("datos/imagenes/personaje.png");
	ALLEGRO_BITMAP* fondo = al_load_bitmap("datos/imagenes/cesped.jpg");
	ALLEGRO_BITMAP* zombi = al_load_bitmap("datos/imagenes/fantasma.png");
	ALLEGRO_BITMAP* objeto = al_load_bitmap("datos/imagenes/corazon.jpg");

	ALLEGRO_FONT* letras = al_load_font("datos/fuentes/AldotheApache.ttf",50,0);
	
	ALLEGRO_COLOR negro = al_map_rgb(0, 0, 0);
	ALLEGRO_COLOR blanco = al_map_rgb(255, 255, 255);
	ALLEGRO_COLOR rojo = al_map_rgb(255, 0, 0);

	ALLEGRO_KEYBOARD_STATE* state{};

	ALLEGRO_SAMPLE* musica = al_load_sample("datos\musica\papaya song.wav");
	ALLEGRO_SAMPLE_ID* sample_id;
	ALLEGRO_AUDIO_STREAM* musica1 = al_load_audio_stream("datos\musica\papaya song.wav", 4, 2048);

	al_reserve_samples(1);

	jugador = cargarmapa();//funcion para cargar mapa
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

	ALLEGRO_EVENT evento;
	
	while (true)
	{
		if (bandera == 0)
		{
			al_clear_to_color(al_map_rgb(0, 0, 0));

			al_draw_bitmap(fondo, 0, 0, 0);
			for (i = 0; i < M; i++)
			{
				for (j = 0; j < N; j++)
				{
					al_draw_bitmap(bloque, pared.posicionX[i][j], pared.posicionY[i][j], 0);//bloques
				}
			}

			for (i = 0; i < MAX; i++)
			{
				al_draw_bitmap(zombi, enemigo[i].posicionX, enemigo[i].posicionY, 0);//enemigo
			}

			for (i = 0; i < MAXELEM; i++)
			{
				al_draw_bitmap(objeto, elemento[i].posicionX, elemento[i].posicionY, 0);
			}


			al_draw_bitmap(player, jugador.posicionX, jugador.posicionY, 0); /*personaje*/
			if (jugador.posicionX > ancho)
			{
				jugador.posicionX = 0;
			}
			if (jugador.posicionX < 0)
			{
				jugador.posicionX = ancho;
			}
			for (i = 0; i < MAXELEM; i++)//suma de puntaje
			{
				if (jugador.posicionX == elemento[i].posicionX && jugador.posicionY == elemento[i].posicionY)
				{
					puntos = puntaje(puntos);
					elemento[i].posicionX = -30;//intentar cambiarlo 
					elemento[i].posicionY = -30;
				}
			}
			for (i = 0; i < MAXELEM; i++)
			{
				if (jugador.posicionX == enemigo[i].posicionX && jugador.posicionY == enemigo[i].posicionY)
				{
					corazones = vidas(corazones);
					jugador.posicionX = x;
					jugador.posicionY = y;
					if (corazones == 0)
					{
						bandera = 1;
					}
				}
			}
			al_draw_rectangle(ancho - 16, 12 * 4, ancho - 16 * 7, 12 * 2, blanco, 30);
			al_draw_textf(letras, negro, ancho - 16 * 8, 12, NULL, "%d", puntos);
			al_draw_textf(letras, rojo, ancho - 16 * 2, 12, NULL, "%d", corazones);

			al_flip_display();/*mostrar las imagenes en pantalla*/
			al_wait_for_event(event_queue, &evento);

			//al_play_sample(musica, 40, 0.0, 2.0, ALLEGRO_PLAYMODE_ONCE,NULL);

			if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
			{
				switch (evento.keyboard.keycode)/*hacer una accion para cada tecla pulsada*/
				{
				case ALLEGRO_KEY_D:
				{
					jugador.posicionX = jugador.posicionX + 16;
					break;
				}
				case ALLEGRO_KEY_A:
				{
					jugador.posicionX = jugador.posicionX - 16;
					break;
				}
				case ALLEGRO_KEY_W:
				{
					jugador.posicionY = jugador.posicionY - 12;
					break;
				}
				case ALLEGRO_KEY_S:
				{
					jugador.posicionY = jugador.posicionY + 12;
					break;
				}
				case ALLEGRO_KEY_ESCAPE:
				{
					al_destroy_sample(musica);
					return 0;
				}
				case ALLEGRO_KEY_ENTER:
				{
					//al_play_sample(musica, 20.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					bandera = 1;
					break;
				}
				default:
					break;
				}

				contenemigos = contenemigos + 1;

				for (i = 0; i < MAX; i++)
				{
					if (contenemigos % 10 == 0)
					{
						j = 1 + rand() % 4;
						if (j == 1)
						{
							enemigo[i].posicionX = enemigo[i].posicionX + 16;
							enemigo[i].posicionY = enemigo[i].posicionY + 12;
						}
						if (j == 2)
						{
							enemigo[i].posicionX = enemigo[i].posicionX - 16;
							enemigo[i].posicionY = enemigo[i].posicionY - 12;
						}
						if (j == 3)
						{
							enemigo[i].posicionX = enemigo[i].posicionX + 16;
							enemigo[i].posicionY = enemigo[i].posicionY - 12;
						}
						if (j == 4)
						{
							enemigo[i].posicionX = enemigo[i].posicionX - 16;
							enemigo[i].posicionY = enemigo[i].posicionY + 12;
						}
					}
				}

			}
		}
		if (bandera == 1)
		{
			gmrv=gameover(puntos);
			if (gmrv== 1)
				return 0;
		}
	}
		
	return 0;
}

movimiento cargarmapa()
{
	movimiento jugador;
	int i=0, j=0;

	FILE* mapa;

	mapa = fopen("datos/mapas/mapa1.txt", "r");
	
	for (i = 0; i < M; i++)
	{
		for (j = 0; j < N; j++)
		{
			fscanf(mapa, "%c", &mat[i][j]);
		}
		//fscanf(mapa, "\n");
	}
	fclose(mapa);
	for (i = 0; i < M; i++)
	{
		for (j = 0; j < N; j++)
		{
			
			//printf("%c", mat[i][j]);
			if (mat[i][j] == 'p')
			{
				jugador.posicionX = i * 16;
				jugador.posicionY = j * 12;
				return jugador;
			}
		}
	}
	
}

zombies cargarenemigo(int index)
{
	zombies enemigo[MAX];
	int i = 0, j = 0,cont = 0;

	FILE* mapa;

	mapa = fopen("datos/mapas/mapa1.txt", "r");

	for (i = 0; i < M; i++)
	{
		for (j = 0; j < N; j++)
		{
			fscanf(mapa, "%c", &mat[i][j]);
		}
		//fscanf(mapa, "\n");
	}
	fclose(mapa);
	for (i = 0; i < M; i++)
	{
		for (j = 0; j < N; j++)
		{

			//printf("%c", mat[i][j]);
			if (mat[i][j] == 'e' && cont <= MAX)
			{
				enemigo[cont].posicionX = i * 16;
				enemigo[cont].posicionY = j * 12;
				cont++;
			}
			
		}
	}
	return enemigo[index];
}

movimiento cargarelementos(int index)
{
	movimiento elemento[MAXELEM];
	int i = 0, j = 0,cont=0;

	FILE* mapa;

	mapa = fopen("datos/mapas/mapa1.txt", "r");

	for (i = 0; i < M; i++)
	{
		for (j = 0; j < N; j++)
		{
			fscanf(mapa, "%c", &mat[i][j]);
		}
	}

	fclose(mapa);

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
	int i = 0, j = 0;

	FILE* mapa;

	mapa = fopen("datos/mapas/mapa1.txt", "r");

	for (i = 0; i < M; i++)
	{
		for (j = 0; j < N; j++)
		{
			fscanf(mapa, "%c", &mat[i][j]);
		}
		//fscanf(mapa, "\n");
	}
	fclose(mapa);
	//printf("paredes funcion\n");
	for (i = 0; i < M; i++)
	{
		for (j = 0; j < N; j++)
		{
			if (mat[i][j] == 'x')
			{
				//printf("mat[i][j] = %c\n", mat[i][j]);
				//printf("i = %d\t j = %d\n", i, j);
				pared.posicionX[i][j] = i * 16;
				pared.posicionY[i][j] = j * 12;
				//printf("posicionX = %d\tposicionY = %d\n", pared[i][j].posicionX, pared[i][j].posicionY);
			}

		}
	} 
	return pared;
}

int puntaje(int puntos)
{
	puntos = puntos + 300;
	return puntos;
}

int vidas(int corazones)
{
	corazones = corazones - 1;
	return  corazones;
}

int gameover(int puntos)
{
	ALLEGRO_FONT* letras = al_load_font("datos/fuentes/AldotheApache.ttf", 50, 0);
	ALLEGRO_FONT* letras1 = al_load_font("datos/fuentes/AldotheApache.ttf", 20, 0);
	ALLEGRO_COLOR blanco = al_map_rgb(255, 255, 255);
	ALLEGRO_COLOR rojo = al_map_rgb(255, 0, 0);
	ALLEGRO_KEYBOARD_STATE* state{};
	ALLEGRO_EVENT evento;

	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_textf(letras, rojo, ancho /2-16*14, alto/4+12*3, NULL, "tus puntos fueron de :%d", puntos);
	al_draw_text(letras, blanco, ancho / 2-16*6, alto / 5, NULL, "GAME OVER");
	al_draw_text(letras1, blanco, ancho / 2 - 16 * 6, alto / 2 + 12 * 6, NULL, "presione escape para salir");
	al_flip_display();
	al_wait_for_event(event_queue, &evento);

	if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		switch (evento.keyboard.keycode)/*hacer una accion para cada tecla pulsada*/
		{
		case ALLEGRO_KEY_ESCAPE:
		{
			return 1;
		}
		default:
			break;
		}
	}
	return 0;
}