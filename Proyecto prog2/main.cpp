#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <Windows.h>
#define ancho 1024
#define alto 768

using namespace std;

int posicionX = 0/*512*/, posicionY = 0/*384*/;

ALLEGRO_EVENT_QUEUE* event_queue;

int main()
{
	al_init();
	al_install_keyboard();
	al_init_image_addon();
	
	ALLEGRO_DISPLAY* ventana = al_create_display(ancho, alto);

	int ANCHO_W = GetSystemMetrics(SM_CXSCREEN);
	int ALTO_W = GetSystemMetrics(SM_CYSCREEN);

	al_set_window_title(ventana, "primer avance");
	al_set_window_position(ventana, ANCHO_W/2-ancho/2, ALTO_W/2-alto/2);

	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	ALLEGRO_BITMAP* menu_null = al_load_bitmap("imagenes/cesped.jpg");
	ALLEGRO_BITMAP* menu_null1 = al_load_bitmap("imagenes/hola.png");
	/*ALLEGRO_BITMAP* menu_null1 = al_load_bitmap("imagenes/cara_benja.PNG"); */
	
	ALLEGRO_KEYBOARD_STATE* state{};

	while (true)
	{
		ALLEGRO_EVENT evento;
		/*al_clear_to_color(al_map_rgb(0, 0, 0));*/
		al_draw_bitmap(menu_null, 0, 0, 0);
		al_draw_bitmap(menu_null1, posicionX, posicionY, 0);
		al_flip_display();
		al_wait_for_event(event_queue, &evento);
		

		if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (evento.keyboard.keycode)
				{
				case ALLEGRO_KEY_D:
				{
					posicionX = posicionX + 30;	
					break;
				}
				case ALLEGRO_KEY_A: 
				{
					posicionX = posicionX - 30;
					break;
				}
				case ALLEGRO_KEY_W:
				{
					posicionY = posicionY - 30;
					break;
				}
				case ALLEGRO_KEY_S:
				{
					posicionY = posicionY + 30;
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

