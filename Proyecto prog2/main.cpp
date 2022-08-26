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
#define MAXELEM 10
#define NOMBRE 10

int puntaje(int puntos,int valor);//funcion para el puntaje
int vidas(int corazones);
int menuflg(ALLEGRO_BITMAP*inicio[5], ALLEGRO_EVENT evento, ALLEGRO_EVENT_QUEUE* event_queue, int bandera);
int gameover(ALLEGRO_COLOR[3], ALLEGRO_FONT* font[2], ALLEGRO_EVENT evento, ALLEGRO_EVENT_QUEUE* event_queue, int puntos);
int rnk(ALLEGRO_COLOR colors[3], ALLEGRO_FONT* font[2], ALLEGRO_EVENT evento, ALLEGRO_EVENT_QUEUE* event_queue, int puntos, int bandera);

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
};
struct user 
{
	char nombre[30];
	int puntaje=0;
};

typedef struct elements elementos;
typedef struct user usuario;
typedef struct player jugador;
typedef struct enemy zombies ;

//implementar cambio de mapas
//simbolo dentro de mapa que sea una puerta hacia otro nivel 
//solo aparece si puntaje > ciertos puntos 
//crear bandera para crear/cambiar mapas
//an
void setmapa(const char*file_name);
jugador setjugador();
elementos cargarelementos(int index);
zombies cargarenemigo(int index);
struct muro cargarpared();

using namespace std;

char mat[M][N];
int mouseX = -1;
int mouseY = -1;
/*int posicionX = 0 , posicionY = 0;*/




ALLEGRO_SAMPLE* musica = NULL;
ALLEGRO_SAMPLE_INSTANCE* songinstance = NULL;

int main()
{
	srand(time(0));
	jugador jugador;/*movimiento jugador en posiciones X e Y*/
	elementos elemento[MAXELEM];
	zombies enemigo[MAX];
	struct muro pared;
	user usuario;

	int i = 0, j = 0, cont = 1, contenemigos = 0, movimientojugador = 0;
	int x = 0, y = 0, bandera = 0, gmrv, derecha = 0, izquierda = 0, arriba = 0, abajo = 0, png=0;
	int rotacion = 1,nivel=900,stge_nivel=0;

	char buffer[10];
	const char* sala = "datos / mapas / mapa";
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

	
	

	ALLEGRO_BITMAP* bloque = al_load_bitmap("datos/imagenes/pared(1).jpg");/*imagenes a utilizar*/
	ALLEGRO_BITMAP* player = al_load_bitmap("datos/imagenes/pacman.png");
	ALLEGRO_BITMAP* fondo = al_load_bitmap("datos/imagenes/fondo(1).jpg");
	ALLEGRO_BITMAP* zombi = al_load_bitmap("datos/imagenes/pacman.png");
	ALLEGRO_BITMAP* objeto = al_load_bitmap("datos/imagenes/pacman.png");
	ALLEGRO_BITMAP* entrada = al_load_bitmap("datos/imagenes/puerta1.jpg");
	//menu
	ALLEGRO_BITMAP* menu = al_load_bitmap("datos/imagenes/menu.png");//0
	ALLEGRO_BITMAP* jugar = al_load_bitmap("datos/imagenes/jugar.png");//1
	ALLEGRO_BITMAP* opciones = al_load_bitmap("datos/imagenes/opciones.png");//2
	ALLEGRO_BITMAP* ranking = al_load_bitmap("datos/imagenes/ranking.png");//3
	ALLEGRO_BITMAP* salir = al_load_bitmap("datos/imagenes/salir.png");//4
	ALLEGRO_BITMAP* inicio[5] = { al_load_bitmap("datos/imagenes/menu.png"),al_load_bitmap("datos/imagenes/jugar.png"),al_load_bitmap("datos/imagenes/opciones.png"),al_load_bitmap("datos/imagenes/ranking.png"),al_load_bitmap("datos/imagenes/salir.png") };
	//ALLEGRO_BITMAP* menu1 = al_load_bitmap("datos/imagenes/menu1.png");

	ALLEGRO_FONT* letras = al_load_font("datos/fuentes/AldotheApache.ttf", 50, 0);
	ALLEGRO_FONT* letras1 = al_load_font("datos/fuentes/AldotheApache.ttf", 20, 0);
	ALLEGRO_FONT* font[2] = { al_load_font("datos/fuentes/AldotheApache.ttf", 50, 0) ,al_load_font("datos/fuentes/AldotheApache.ttf", 20, 0) };


	ALLEGRO_COLOR negro = al_map_rgb(0, 0, 0);
	ALLEGRO_COLOR blanco = al_map_rgb(255, 255, 255);
	ALLEGRO_COLOR rojo = al_map_rgb(255, 0, 0);
	ALLEGRO_COLOR colors[3]{ negro,blanco,rojo };

	ALLEGRO_KEYBOARD_STATE* state{};

	ALLEGRO_EVENT evento;
	ALLEGRO_EVENT_QUEUE* event_queue;

	event_queue = al_create_event_queue();
	
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());

	al_reserve_samples(10);
	musica = al_load_sample("datos/musica/papaya song.ogg");

	songinstance = al_create_sample_instance(musica);
	al_set_sample_instance_playmode(songinstance, ALLEGRO_PLAYMODE_LOOP);

	al_attach_sample_instance_to_mixer(songinstance, al_get_default_mixer());
	setmapa("datos / mapas / mapa1.txt");//funcion para cargar mapa

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
	
	while (true)
	{
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_wait_for_event(event_queue, &evento);

		if (bandera == -1)
		{
			return 0;
		}
		if (bandera == 0)//menu ||agregar menu con mouse||
		{
			bandera = menuflg(inicio, evento, event_queue, bandera);
			if (bandera == 1)
			{
				al_hide_mouse_cursor(ventana);
			}
		}
		if (bandera == 1)//juego
		{
			//al_play_sample_instance(songinstance);


			al_draw_bitmap(fondo, 0, 0, 0);
			//cargar mapa (enemigos,muralla,etc)
			//if(bandera)
			//siguiente mapa (contador,revisar gmail)
			if (stge_nivel == 1)
			{
				cont++;
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
				itoa(cont, buffer, 10);
				setmapa("datos / mapas / mapa2.txt");
			}

			for (i = 0; i < 2; i++)
			{
				printf("%d\n", usuario.puntaje);

				if (nivel >= usuario.puntaje);
				{
					//printf("hola\n");
					al_draw_bitmap_region(entrada, 8, 12, 8, 14, pared.posicionpuertaX[i], pared.posicionpuertaY[i], 0);
				}
				if (nivel < usuario.puntaje)
				{
					//printf("adios\n");
					al_draw_bitmap_region(entrada, 18, 12, 11, 15, pared.posicionpuertaX[i], pared.posicionpuertaY[i], 0);
					if (jugador.posicionX == pared.posicionpuertaX[i] && jugador.posicionY == pared.posicionpuertaY[i])
					{
						stge_nivel = 1;
					}
				}
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
				//al_draw_bitmap(zombi, enemigo[i].posicionX * 16, enemigo[i].posicionY * 12, 0);//enemigo
				al_draw_bitmap_region(zombi, 456, 64, 15, 15, enemigo[i].posicionX * 16, enemigo[i].posicionY * 12, 0);
			}

			for (i = 0; i < MAXELEM; i++)//objetos
			{
				//al_draw_bitmap(objeto, elemento[i].posicionX, elemento[i].posicionY, 0);
				al_draw_bitmap_region(objeto, 603, 49, 8, 14,  elemento[i].posicionX, elemento[i].posicionY, 0);
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
				rotacion =34;
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


			

			if (jugador.posicionX * 16 > ancho-1 )
			{
				jugador.posicionX = 0;
			}
			if (jugador.posicionX * 16 < -1)
			{
				jugador.posicionX = 64 - 2;
			}

			if (jugador.posicionY * 12 > alto - 1)
			{
				printf("posicionY%d\n", jugador.posicionY);
				jugador.posicionY = 0;
			}
			if (jugador.posicionY * 12 < -1)
			{
				//printf("jugadorY=%d\n", jugador.posicionY);
				printf("posicionY%d\n", jugador.posicionY);
				jugador.posicionY = 64 - 1;
			}

			for (i = 0; i < MAXELEM; i++)//suma de puntaje
			{
				if (jugador.posicionX * 16 == elemento[i].posicionX && jugador.posicionY * 12 == elemento[i].posicionY)
				{
					usuario.puntaje = puntaje(usuario.puntaje,elemento[i].valor);
					elemento[i].posicionX = -30;//intentar cambiarlo 
					elemento[i].posicionY = -30;
				}
			}

			for (i = 0; i < MAX; i++)
			{
				if (jugador.posicionX * 16 == enemigo[i].posicionX * 16 && jugador.posicionY * 12 == enemigo[i].posicionY * 12)//cuando el jugador choca con un enemigo
				{
					jugador.vidas = vidas(jugador.vidas);
					jugador.posicionX = x;
					jugador.posicionY = y;
					if (jugador.vidas == 0)
					{
						//	al_destroy_bitmap(bloque);//liberacion de memoria
						//	al_destroy_bitmap(player);
						//	al_destroy_bitmap(fondo);
						//	al_destroy_bitmap(zombi);
						//	al_destroy_bitmap(objeto);
						al_show_mouse_cursor(ventana);
						bandera = 2;
					}
				}
			}
			
			//count algo ++
			//if count_algo == 20 --> enemigo se mueve y count_algo = 0
			contenemigos = contenemigos + 1;
			//printf("contenemigos = %d\n", contenemigos);
			if (contenemigos == 20)
			{
				for (i = 0; i < MAX; i++)
				{
					switch (j = 1 + rand() % 4)
					{
					case 1:
					{
						if (mat[enemigo[i].posicionX + 1][enemigo[i].posicionY] != 'x')//colision con paredes
						{
							enemigo[i].posicionX = enemigo[i].posicionX + 1;
						}
						break;
					}
					case 2:
					{
						if (mat[enemigo[i].posicionX - 1][enemigo[i].posicionY] != 'x')//colision con paredes
						{
							enemigo[i].posicionX = enemigo[i].posicionX - 1;
						}
						break;
					}
					case 3:
					{
						if (mat[enemigo[i].posicionX][enemigo[i].posicionY - 1] != 'x')//colision con paredes
						{
							enemigo[i].posicionY = enemigo[i].posicionY - 1;
						}
						break;
					}
					case 4:
					{
						if (mat[enemigo[i].posicionX][enemigo[i].posicionY + 1] != 'x')//colision con paredes
						{
							enemigo[i].posicionY = enemigo[i].posicionY + 1;
						}
						break;
					}
					default:
						break;
					}
					contenemigos = 0;

				}
			}
			

			al_draw_rectangle(ancho - 16, 12 * 4, ancho - 16 * 7, 12 * 2, blanco, 30);
			al_draw_textf(letras, negro, ancho - 16 * 8, 12, NULL, "%d",usuario.puntaje);
			al_draw_textf(letras, rojo, ancho - 16 * 2, 12, NULL, "%d", jugador.vidas);
			//al_play_sample(musica, 40, 0.0, 2.0, ALLEGRO_PLAYMODE_ONCE,NULL);

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
					//	al_destroy_bitmap(bloque);
					//	al_destroy_bitmap(player);
					//	al_destroy_bitmap(fondo);
					//	al_destroy_bitmap(zombi);
					//	al_destroy_bitmap(objeto);
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
		if (bandera == 2)//ranking
		{
			bandera = rnk(colors,font,evento,event_queue,usuario.puntaje,bandera);
		}
		if (bandera == 3)//game over
		{
			gmrv = gameover(colors, font, evento, event_queue, usuario.puntaje);
			if (gmrv == 1)
				return 0;
			if (gmrv == 2)
				bandera = 0;
		}
		al_flip_display();
	}
	
	return 0;
}
void setmapa(const char* file_name)
{
	int i = 0, j = 0;
	//char aux[;
	FILE* mapa;

	mapa = fopen(file_name, "r");

	for (i = 0; i < M; i++)
	{
		for (j = 0; j < N; j++)
		{
			fscanf(mapa, "%c", &mat[j][i]);
		}
		//fscanf(mapa, "%c",aux);
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
				pared.posicionpuertaX[cont] = i * 16;
				pared.posicionpuertaY[cont] = j * 12;
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

int menuflg(ALLEGRO_BITMAP* inicio[5], ALLEGRO_EVENT evento, ALLEGRO_EVENT_QUEUE* event_queue, int bandera)
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
			break;
		}
		default:
			break;
		}
	}
	return bandera;
}

int gameover(ALLEGRO_COLOR colors[3], ALLEGRO_FONT* font[2], ALLEGRO_EVENT evento, ALLEGRO_EVENT_QUEUE* event_queue, int puntos)
{
	al_draw_textf(font[0], colors[2], ancho / 2 - 16 * 14, alto / 4 + 12 * 3, NULL, "tus puntos fueron de :%d", puntos);
	al_draw_text(font[0], colors[1], ancho / 2 - 16 * 6, alto / 5, NULL, "GAME OVER");
	al_draw_text(font[1], colors[1], ancho / 2 - 16 * 6, alto / 2 + 12 * 6, NULL, "presione escape para salir");
	al_draw_text(font[1], colors[1], ancho / 2 - 16 * 6, alto / 2 + 12 * 8, NULL, "presione enter para volver al menu");
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

int rnk(ALLEGRO_COLOR colors[3], ALLEGRO_FONT* font[2], ALLEGRO_EVENT evento, ALLEGRO_EVENT_QUEUE* event_queue, int puntos, int bandera)
{
	int i = 0, espacio = ancho / 2 - 16 * 6;
	char name[30]="pinono\n";
	usuario jugador;

	FILE* ranking;
	ALLEGRO_FILE* rank;

	rank = al_fopen("datos/ranking/puntajeallegro.txt", "w+");
	ranking = fopen("datos/ranking/puntaje.txt", "w+");//escritura o creacion del ranking en caso de que no exista

	jugador.puntaje = puntos;
	printf("%d", jugador.puntaje);
	al_draw_text(font[1], colors[1], 16*15, alto / 2 + 12 * 10, NULL, "ingrese nombre usuario: ");
	if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		switch (evento.keyboard.keycode)/*hacer una accion para cada tecla pulsada*/
		{
		case ALLEGRO_KEY_P:
		{
			al_draw_text(font[1], colors[1], espacio, alto / 2 + 12 * 10, 0, "p");
			jugador.nombre[i] = 'p';
			printf("jugador.nombre[%d]=%c\n", i, jugador.nombre[i]);
			i++;
			break;
		}
		case ALLEGRO_KEY_I:
		{
			al_draw_text(font[1], colors[1], espacio, alto / 2 + 12 * 10, 0, "i");
			jugador.nombre[i] = 'i';
			printf("jugador.nombre[%d]=%c\n", i, jugador.nombre[i]);
			i++;
			break;
		}
		case ALLEGRO_KEY_N:
		{
			al_draw_text(font[1], colors[1], espacio, alto / 2 + 12 * 10, 0, "n");
			jugador.nombre[i] = 'n';
			printf("jugador.nombre[%d]=%c\n", i, jugador.nombre[i]);
			i++;
			break;
		}
		case ALLEGRO_KEY_O:
		{
			al_draw_text(font[1], colors[1], espacio, alto / 2 + 12 * 10, 0, "o");
			jugador.nombre[i] = 'o';
			printf("jugador.nombre[%d]=%c\n", i, jugador.nombre[i]);
			i++;

			break;
		}
		case ALLEGRO_KEY_ESCAPE:
		{
			return -1;
		}
		case ALLEGRO_KEY_0:
		{
			bandera = 3;
			break;
		}
		default:
			break;
		}
	}
	while (i < 30)
	{
		printf("i = %d\n", i);
		strcpy(jugador.nombre, name);
		fprintf(ranking, "%c-%d\n", jugador.nombre, puntos);//c++
		al_fputs(rank, jugador.nombre);//allegro
		al_fputc(rank, jugador.puntaje);
		i = i + 1;
	}
	al_fclose(rank);
	fclose(ranking);
	return bandera;
}
