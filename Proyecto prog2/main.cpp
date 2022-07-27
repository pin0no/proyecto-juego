#include <stdio.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <Windows.h>
#define ancho 1024
#define alto 768
#define M 64
#define N 64

int puntaje(int mat[M][N]);//funcion para el puntaje

struct posicion
{
	int posicionX = 0;
	int posicionY = 0;
};
struct muro
{
	int posicionX = 0;
	int posicionY = 0;
};
typedef posicion movimiento;

int cargarmapa(movimiento jugador);
int cargarpared(muro pared[M][N]);

using namespace std;

int mat[M][N];
/*int posicionX = 0 , posicionY = 0;*/


ALLEGRO_EVENT_QUEUE* event_queue;

int main()
{
	movimiento jugador; /*movimiento jugador en posiciones X e Y*/
	struct muro pared[M][N];
	
	int i=0, j=0;

	al_init();			/*iniciaciones*/
	al_install_keyboard();
	al_init_image_addon();
	
	ALLEGRO_DISPLAY* ventana = al_create_display(ancho, alto);/*crear una ventana*/

	int ANCHO_W = GetSystemMetrics(SM_CXSCREEN);/*obtener los pixeles de la pantalla que se utiliza*/
	int ALTO_W = GetSystemMetrics(SM_CYSCREEN);

	al_set_window_title(ventana, "primer avance");/*cambio de titulo*/
	al_set_window_position(ventana, ANCHO_W/2-ancho/2, ALTO_W/2-alto/2);/*centrar la ventana*/

	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	ALLEGRO_BITMAP* bloque = al_load_bitmap("datos/imagenes/pared.png");/*imagenes a utilizar*/
	ALLEGRO_BITMAP* player = al_load_bitmap("datos/imagenes/personaje.png");
	/*ALLEGRO_BITMAP* menu_null1 = al_load_bitmap("imagenes/cara_benja.PNG"); */
	
	ALLEGRO_KEYBOARD_STATE* state{};

	cargarmapa(jugador);//funcion para cargar mapa ¿se devuelven los datos?
	printf("datos jugador\n");
	printf("posicionX = %d\tposicionY = %d\n", jugador.posicionX, jugador.posicionY);

	cargarpared(pared);
	printf("paredes\n");
	//for (i = 0; i < M; i++)
	{
	//	for (j = 0; j < N; j++)
		{
			//al_draw_bitmap(bloque, pared[i][j].posicionX * 16, pared[i][j].posicionY * 12, 0);
			printf("i = %d\t j = %d\n", i, j);
			printf("posicionX = %d\tposicionY = %d\n", pared[i][j].posicionX, pared[i][j].posicionY);
		}
	}
	while (true)
	{
		ALLEGRO_EVENT evento;
		al_clear_to_color(al_map_rgb(0, 0, 0));
		//al_draw_bitmap(menu_null, 0, 0, 0);/*primera imagen*/

		//al_draw_bitmap(al_load_bitmap("datos/imagenes/personaje.png"), jugador.posicionX * 16, jugador.posicionY * 12, 0);
		
		
		//al_draw_bitmap(al_load_bitmap("datos/imagenes/pared.png"), i * 16, j * 12, 0);
		al_draw_bitmap(player, jugador.posicionX , jugador.posicionY , 0); /*personaje*/
		//printf("jugador\n");
		//printf("posicionX = %d\tposicionY = %d\n", jugador.posicionX, jugador.posicionY);

		al_flip_display();/*mostrar las imagenes en pantalla*/
		al_wait_for_event(event_queue, &evento);

		
		
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
					return 0;
				}
				default:
					break;
				}
		}
	}
	return 0;
}
int cargarmapa(movimiento jugador)
{
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
				jugador.posicionX = i * 16 ;
				jugador.posicionY = j * 12;
				printf("datos jugador funcion\n");
				printf("posicionX = %d\tposicionY = %d\n", jugador.posicionX, jugador.posicionY);

				return jugador.posicionX,jugador.posicionY;
			}
		}
	}
	return 0;
}
int cargarpared(muro pared[M][N])
{
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
	printf("paredes funcion\n");
	for (i = 0; i < M; i++)
	{
		for (j = 0; j < N; j++)
		{

			//printf("%c", mat[i][j]);

			if (mat[i][j] == 'x')
			{
				//printf("mat[i][j] = %c\n", mat[i][j]);
				//printf("i = %d\t j = %d\n", i, j);
				
				pared[i][j].posicionX = i;
				pared[i][j].posicionY = j;
				//printf("posicionX = %d\tposicionY = %d\n", pared[i][j].posicionX, pared[i][j].posicionY);
			}

		}
	} 
	for (i = 0; i < M; i++)
	{
		for (j = 0; j < N; j++)
		{
			if (mat[i][j] == 'x')
			{
				printf("i = %d\t j = %d\n", i, j);
				return pared[i][j].posicionX, pared[i][j].posicionY;
			}
		}
	}
	
}

