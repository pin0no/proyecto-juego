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

void cargarmapa(struct posicion);
int puntaje(int mat[M][N]);//funcion para el puntaje

struct posicion
{
	int posicionX = 0;
	int posicionY = 0;
};
typedef posicion movimiento;
using namespace std;

int mat[M][N];
/*int posicionX = 0 , posicionY = 0;*/


ALLEGRO_EVENT_QUEUE* event_queue;

int main()
{
	movimiento jugador; /*movimiento jugador en posiciones X e Y*/



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

	ALLEGRO_BITMAP* menu_null = al_load_bitmap("datos/imagenes/pared.png");/*imagenes a utilizar*/
	ALLEGRO_BITMAP* menu_null1 = al_load_bitmap("datos/imagenes/hola.png");
	/*ALLEGRO_BITMAP* menu_null1 = al_load_bitmap("imagenes/cara_benja.PNG"); */
	
	ALLEGRO_KEYBOARD_STATE* state{};

	cargarmapa(jugador);//funcion para cargar mapa 

	while (true)
	{
		ALLEGRO_EVENT evento;
		/*al_clear_to_color(al_map_rgb(0, 0, 0));*/
		/*al_draw_bitmap(menu_null, 0, 0, 0);*//*primera imagen*/
		

		al_flip_display();/*mostrar las imagenes en pantalla*/
		al_wait_for_event(event_queue, &evento);

		al_draw_bitmap(menu_null1, jugador.posicionX, jugador.posicionY, 0); /*personaje*/
		
		if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (evento.keyboard.keycode)/*hacer una accion para cada tecla pulsada*/
				{
				case ALLEGRO_KEY_D:
				{
					jugador.posicionX = jugador.posicionX + 12;
					break;
				}
				case ALLEGRO_KEY_A: 
				{
					jugador.posicionX = jugador.posicionX - 12;
					break;
				}
				case ALLEGRO_KEY_W:
				{
					jugador.posicionY = jugador.posicionY - 16;
					break;
				}
				case ALLEGRO_KEY_S:
				{
					jugador.posicionY = jugador.posicionY + 16;
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
void cargarmapa(struct posicion)
{
		
	typedef posicion movimiento;

	int i=0, j=0;
	char texto[M][N]{};

	FILE* mapa;

	mapa = fopen("datos/mapas/mapa1.txt", "r");
	
	movimiento jugador;

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
			struct posicion
			{
				int posicionX = i * 16;
				int posicionY = j * 12;

			};
			if (mat[i][j] == 'x')
			{
				al_draw_bitmap(al_load_bitmap("datos/imagenes/pared.png"),  jugador.posicionX, j * jugador.posicionY, 0);
			}
			if (mat[i][j] == 'p')
			{
				//printf("se encontro p = [%d][%d]\n",i,j);
				al_draw_bitmap(al_load_bitmap("datos/imagenes/personaje.png"), i* jugador.posicionX, j* jugador.posicionY, 0);
			}
		}
		//printf("\n");
	}
	printf("matriz\n");
	for (i = 0; i < M; i++)
	{
		for (j = 0; j < N; j++)
		{
			printf("%c", mat[i][j]);
		}
		printf("\n");
	}
}

