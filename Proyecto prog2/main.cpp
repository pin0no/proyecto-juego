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

ALLEGRO_SAMPLE* musica = NULL;
ALLEGRO_SAMPLE_INSTANCE* songinstance = NULL;

int main()
{
	srand(time(0));

	movimiento jugador, elemento[MAXELEM];/*movimiento jugador en posiciones X e Y*/
	zombies enemigo[MAX];
	struct muro pared;

	int i = 0, j = 0, cont = 0, puntos = 0, contenemigos = 0, movimientojugador = 0;
	int x = 0, y = 0, bandera = 0, gmrv, derecha = 0, izquierda = 0, arriba = 0, abajo = 0;
	int corazones = 3;

	al_init();			/*iniciaciones*/

	al_install_keyboard();//teclado

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

	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	ALLEGRO_BITMAP* bloque = al_load_bitmap("datos/imagenes/pared.png");/*imagenes a utilizar*/
	ALLEGRO_BITMAP* player = al_load_bitmap("datos/imagenes/personaje.png");
	ALLEGRO_BITMAP* fondo = al_load_bitmap("datos/imagenes/cesped.jpg");
	ALLEGRO_BITMAP* zombi = al_load_bitmap("datos/imagenes/fantasma.png");
	ALLEGRO_BITMAP* objeto = al_load_bitmap("datos/imagenes/corazon.jpg");

	ALLEGRO_FONT* letras = al_load_font("datos/fuentes/AldotheApache.ttf", 50, 0);
	ALLEGRO_FONT* letras1 = al_load_font("datos/fuentes/AldotheApache.ttf", 20, 0);


	ALLEGRO_COLOR negro = al_map_rgb(0, 0, 0);
	ALLEGRO_COLOR blanco = al_map_rgb(255, 255, 255);
	ALLEGRO_COLOR rojo = al_map_rgb(255, 0, 0);

	ALLEGRO_KEYBOARD_STATE* state{};


	al_reserve_samples(10);

	musica = al_load_sample("datos/musica/papaya song.ogg");

	songinstance = al_create_sample_instance(musica);
	al_set_sample_instance_playmode(songinstance, ALLEGRO_PLAYMODE_LOOP);

	al_attach_sample_instance_to_mixer(songinstance, al_get_default_mixer());

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

	//al_play_sample_instance(songinstance);
	
	while (true)
	{
		if (bandera == 0)//menu ||agregar menu con mouse||
		{
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_text(letras1, blanco, ancho / 2 - 16 * 6, alto / 2 + 12 * 6, NULL, "presione k para iniciar el juego  presione l para salir del juego");
			al_flip_display();

			al_wait_for_event(event_queue, &evento);

			if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
			{
				switch (evento.keyboard.keycode)
				{
				case ALLEGRO_KEY_K:
				{
					bandera = 1;
					break;
				}
				case ALLEGRO_KEY_L:
				{
					return 0;
				}
				default:
					break;
				}
			}
		}
		if (bandera == 1)//juego
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
				al_draw_bitmap(zombi, enemigo[i].posicionX * 16, enemigo[i].posicionY * 12, 0);//enemigo
			}

			for (i = 0; i < MAXELEM; i++)//objetos
			{
				al_draw_bitmap(objeto, elemento[i].posicionX, elemento[i].posicionY, 0);
			}

			al_draw_bitmap(player, jugador.posicionX*16, jugador.posicionY*12, 0); /*personaje*/
			if (jugador.posicionX*16 > ancho-1)
			{
				jugador.posicionX = 0;
			}
			if (jugador.posicionX*16 < -1)
			{
				jugador.posicionX = ancho-16;
			}

			for (i = 0; i < MAXELEM; i++)//suma de puntaje
			{
				if (jugador.posicionX*16 == elemento[i].posicionX && jugador.posicionY *12== elemento[i].posicionY)
				{
					puntos = puntaje(puntos);
					elemento[i].posicionX = -30;//intentar cambiarlo 
					elemento[i].posicionY = -30;
				}
			}

			for (i = 0; i < MAXELEM; i++)
			{
				if (jugador.posicionX*16 == enemigo[i].posicionX * 16 && jugador.posicionY*12 == enemigo[i].posicionY * 12)//cuando el jugador choca con un enemigo
				{
					corazones = vidas(corazones);
					jugador.posicionX = x;
					jugador.posicionY = y;
					if (corazones == 0)
					{
						al_destroy_bitmap(bloque);//liberacion de memoria
						al_destroy_bitmap(player);
						al_destroy_bitmap(fondo);
						al_destroy_bitmap(zombi);
						al_destroy_bitmap(objeto);
						bandera = 2;
					}
				}
			}
			
			if (movimientojugador == 1)
			{
				contenemigos = contenemigos + 1;
				for (i = 0; i < MAX; i++)
				{
					if (contenemigos % 10 == 0)
					{
						if (derecha != enemigo[i].posicionX)//necesito ayuda en esto que es lograr que los enemigos escogan una direccion dependiendo cual numero obtengan y de ahi moverse en esa direccion hasta tener una colision
						{
							
							switch (j = 1 + rand() % 4)
							{
							case 1:
							{
								if (mat[enemigo[i].posicionX + 1][enemigo[i].posicionY] != 'x')//colision con paredes
								{
									//printf("enemigo[i] = %d\n\n", enemigo[i].posicionX);
									enemigo[i].posicionX = enemigo[i].posicionX + 1;
									derecha = enemigo[i].posicionX;
									//printf("enemigo[i] = %d\ni = %d\nderecha = %d\n", enemigo[i].posicionX, i, derecha);
									break;
								}
								break;
							}

							case 2:
							{
								if (mat[enemigo[i].posicionX - 1][enemigo[i].posicionY] != 'x')//colision con paredes
								{
									enemigo[i].posicionX = enemigo[i].posicionX - 1;
									break;
								}
								break;
							}

							case 3:
							{
								if (mat[enemigo[i].posicionX][enemigo[i].posicionY - 1] != 'x')//colision con paredes
								{
									enemigo[i].posicionY = enemigo[i].posicionY - 1;
									break;
								}
								break;
							}

							case 4:
							{
								if (mat[enemigo[i].posicionX][enemigo[i].posicionY + 1] != 'x')//colision con paredes
								{
									enemigo[i].posicionY = enemigo[i].posicionY + 1;
									break;
								}
								break;
							}

							default:
								break;
							}
						}
						
						if (derecha == enemigo[i].posicionX)
						{
							//printf("enemigo[i] = %d\ni = %d\nderecha = %d\n", enemigo[i].posicionX, i, derecha);
							enemigo[i].posicionX = enemigo[i].posicionX + 1;
							derecha = enemigo[i].posicionX;

							if (mat[enemigo[i].posicionX + 1][enemigo[i].posicionY] != 'x')
							{
								derecha = 0;
							}
						}
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
					if (mat[jugador.posicionX +1][jugador.posicionY] != 'x')//colision con paredes
					{
						jugador.posicionX = jugador.posicionX + 1;
						movimientojugador = 1;
						break;
					}
					movimientojugador = 1;
					break;
				}
				case ALLEGRO_KEY_A:
				{
					if (mat[jugador.posicionX -1][jugador.posicionY] != 'x')//colision con paredes
					{
						jugador.posicionX = jugador.posicionX - 1;
						movimientojugador = 1;
						break;
					}
					movimientojugador = 1;
					break;
				}
				case ALLEGRO_KEY_W:
				{
					if (mat[jugador.posicionX][jugador.posicionY - 1] != 'x')//colision con paredes
					{
						jugador.posicionY = jugador.posicionY - 1;
						movimientojugador = 1;
						break;
					}
					movimientojugador = 1;
					break;
				}
				case ALLEGRO_KEY_S:
				{
					if (mat[jugador.posicionX][jugador.posicionY + 1] != 'x')//colision con paredes
					{
						jugador.posicionY = jugador.posicionY + 1;
						movimientojugador = 1;
						break;
					}
					movimientojugador = 1;
					break;
				}
				case ALLEGRO_KEY_ESCAPE:
				{
					return 0;
				}
				case ALLEGRO_KEY_ENTER:
				{
					al_destroy_bitmap(bloque);
					al_destroy_bitmap(player);
					al_destroy_bitmap(fondo);
					al_destroy_bitmap(zombi);
					al_destroy_bitmap(objeto);

					bandera = 2;
					break;
				}
				case ALLEGRO_KEY_0:
				{
					al_stop_sample_instance(songinstance);
					break;
				}
				default:
					break;
				}
			}
		}
		if (bandera == 2)//game over
		{
			gmrv=gameover(puntos);
			if (gmrv == 1)
				return 0;
			if (gmrv == 2)
				bandera = 0;
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
			if (mat[i][j] == 'p')
			{
				jugador.posicionX = i ;
				jugador.posicionY = j ;
				return jugador;
			}
		}
	}
	
}

zombies cargarenemigo(int index)
{
	zombies enemigo[MAX];
	int i = 0, j = 0,cont = 0;

	for (i = 0; i < M; i++)
	{
		for (j = 0; j < N; j++)
		{

			//printf("%c", mat[i][j]);
			if (mat[i][j] == 'e' && cont <= MAX)
			{
				enemigo[cont].posicionX = i ;
				enemigo[cont].posicionY = j ;
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
	al_draw_text(letras1, blanco, ancho / 2 - 16 * 6, alto / 2 + 12 * 8, NULL, "presione enter para volver al menu");
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
		case ALLEGRO_KEY_ENTER:
		{
			return 2;
		}
		default:
			break;
		}
	}
	return 0;
}