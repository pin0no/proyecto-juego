#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <Windows.h>
#define ancho 1024
#define alto 768

struct posicion
{
	int posicionX = 0;
	int posicionY = 0;
};
typedef posicion movimiento;
using namespace std;

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

	ALLEGRO_BITMAP* menu_null = al_load_bitmap("imagenes/cesped.jpg");/*imagenes a utilizar*/
	ALLEGRO_BITMAP* menu_null1 = al_load_bitmap("imagenes/hola.png");
	/*ALLEGRO_BITMAP* menu_null1 = al_load_bitmap("imagenes/cara_benja.PNG"); */
	
	ALLEGRO_KEYBOARD_STATE* state{};

	while (true)
	{
		ALLEGRO_EVENT evento;
		/*al_clear_to_color(al_map_rgb(0, 0, 0));*/
		al_draw_bitmap(menu_null, 0, 0, 0);/*primera imagen*/
		al_draw_bitmap(menu_null1, jugador.posicionX, jugador.posicionY, 0);/*segunda imagen*/
		al_flip_display();/*mostrar las imagenes en pantalla*/
		al_wait_for_event(event_queue, &evento);
		

		if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (evento.keyboard.keycode)/*hacer una accion para cada tecla pulsada*/
				{
				case ALLEGRO_KEY_D:
				{
					jugador.posicionX = jugador.posicionX + 30;
					break;
				}
				case ALLEGRO_KEY_A: 
				{
					jugador.posicionX = jugador.posicionX - 30;
					break;
				}
				case ALLEGRO_KEY_W:
				{
					jugador.posicionY = jugador.posicionY - 30;
					break;
				}
				case ALLEGRO_KEY_S:
				{
					jugador.posicionY = jugador.posicionY + 30;
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

