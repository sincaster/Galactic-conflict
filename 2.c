#include<stdio.h>
#include<stdlib.h>
#include<allegro5/allegro5.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_image.h>
#include<allegro5/allegro_primitives.h>

void pls_init(bool test, const char *description){
	if(test) return;

	printf("couldn't initialize %s\n", description);
	exit(1);
}

int shotnum = 0;
int reqsh = 1;
int xship = 750;
int n = 0;
int yship = 750;
int yshot[10000];
int pos;

int main(){
	pls_init(al_init(), "allegro");
	pls_init(al_install_keyboard(), "keyboard");
	pls_init(al_init_image_addon(), "image addon");
	pls_init(al_init_primitives_addon(), "primitives");

	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 40.0);
	pls_init(timer, "timer");

	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	pls_init(queue, "queue");

	ALLEGRO_DISPLAY* disp = al_create_display(1500, 850);
	pls_init(disp, "display");

	ALLEGRO_FONT* font = al_create_builtin_font();
	pls_init(font, "font");

	ALLEGRO_BITMAP* spaceship = al_load_bitmap("humanship2.png");
	pls_init(spaceship, "spaceship");

	ALLEGRO_BITMAP* shot = al_load_bitmap("shot.png");
	pls_init(shot, "fire");

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	bool done = false;
	bool introdone = false;
	bool redraw = true;
	int step = 0;
	ALLEGRO_EVENT event;

	#define KEY_SEEN     1
	#define KEY_RELEASED 2

	unsigned char key[ALLEGRO_KEY_MAX];
	memset(key, 0, sizeof(key));

	al_start_timer(timer);
	while(!introdone){
		al_wait_for_event(queue, &event);
		switch(event.type){
			case ALLEGRO_EVENT_TIMER:
				if(!(yshot[n]<669) && step > 1)
					pos = xship;
		 		if(key[ALLEGRO_KEY_LEFT] && step > 0){
					xship-=10;
				        if(xship<0)
						xship +=10;
				}
				if(key[ALLEGRO_KEY_RIGHT] && step > 0){
					xship+=10;
				        if(xship>1435)
						xship -=10;							
				}

				if(key[ALLEGRO_KEY_ESCAPE] && step > 0){
					introdone = true;
					done = true;
				}
				for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
					key[i] &= KEY_SEEN;
				if (step > 3)
					yshot[n]-=10;

				redraw = true;
				break;
			case ALLEGRO_EVENT_KEY_DOWN:
				if((event.keyboard.keycode == ALLEGRO_KEY_SPACE) && step>3)
					shotnum++;
				if((event.keyboard.keycode == ALLEGRO_KEY_ENTER)){
					step++;
					if(step == 3)
						introdone = true;
				}
				key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
				break;
			case ALLEGRO_EVENT_KEY_UP:
				key[event.keyboard.keycode] &= KEY_RELEASED;
				break;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				introdone = true;
				done = true;
				break;
			case ALLEGRO_EVENT_KEY_CHAR:
				break;
		}
	
		if(redraw && al_is_event_queue_empty(queue)){	
			al_clear_to_color(al_map_rgb(0, 0, 0));
	                al_draw_scaled_bitmap(spaceship, 0, 0, al_get_bitmap_width(spaceship), al_get_bitmap_height(spaceship),
			xship, yship, al_get_bitmap_width(spaceship) / 2, al_get_bitmap_width(spaceship) / 2, 0);    

			if(step==0){
				al_draw_text(font, al_map_rgb(255, 255, 255), 200, 200, 0, "Welcome to the tutorial of Galactic Conflict");
				al_draw_text(font, al_map_rgb(255, 255, 0), 200, 250, 0, "press Enter to continue when you have read all on the current screen to continue");
				al_draw_text(font, al_map_rgb(255, 255, 255), 100, 400, 0, "you will use the left and right arrows to move the spaceship around\n\n");
			}
			if(step==1){
				al_draw_text(font, al_map_rgb(255, 255, 255), 200, 200, 0, "By the way, from now on you can press escape to exit");
				al_draw_text(font, al_map_rgb(255, 255, 0), 200, 250, 0, "Now, you can try moving around! \n\n (enter to continue)");
				al_draw_text(font, al_map_rgb(255, 255, 0), 100, 450, 0, "Oh and you will be using the space button to shoot!");
			}
			if(step==2){
                                al_draw_text(font, al_map_rgb(255, 255, 0), 200, 200, 0, "Try shooting now! Even though the shooting mechanic still needs some uh work");
				al_draw_text(font, al_map_rgb(255, 255, 0), 200, 250, 0, "So that's all for now really, lol, I think you already in this");
				al_draw_text(font, al_map_rgb(255, 255, 0), 100, 450, 0, "tutorial the full experience but enter to continue so there's no text!");
			}

			if(shotnum==reqsh && step>1){
				reqsh++;
				n++;
				yshot[n] = 670;
				al_draw_bitmap(shot, xship, yshot[n], 0);
			}
			
			if(yshot[n]>0 && step > 1)
				al_draw_bitmap(shot, pos, yshot[n], 0); 

			al_flip_display();
			redraw = false;
		}
	}

	while(!done){
		al_wait_for_event(queue, &event);
		switch(event.type){
			case ALLEGRO_EVENT_TIMER:
				if(!(yshot[n]<669))
					pos = xship;

		 		if(key[ALLEGRO_KEY_LEFT]){
					xship-=10;
				        if(xship<0)
						xship +=10;
				}
				if(key[ALLEGRO_KEY_RIGHT]){
					xship+=10;
				        if(xship>1435)
						xship -=10;							
				}

				if(key[ALLEGRO_KEY_ESCAPE])
					done = true;

				for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
					key[i] &= KEY_SEEN;

				yshot[n]-=10;
				redraw = true;
				break;
			case ALLEGRO_EVENT_KEY_DOWN:
				if(event.keyboard.keycode == ALLEGRO_KEY_SPACE)
					shotnum++;
				key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
				break;
			case ALLEGRO_EVENT_KEY_UP:
				key[event.keyboard.keycode] &= KEY_RELEASED;
				break;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				done = true;
				break;
			case ALLEGRO_EVENT_KEY_CHAR:
				break;
		}
	
		if(redraw && al_is_event_queue_empty(queue)){	
			al_clear_to_color(al_map_rgb(0, 0, 0));
	                al_draw_scaled_bitmap(spaceship, 0, 0, al_get_bitmap_width(spaceship), al_get_bitmap_height(spaceship),
			xship, yship, al_get_bitmap_width(spaceship) / 2, al_get_bitmap_width(spaceship) / 2, 0);    

			al_draw_text(font, al_map_rgb(0, 0, 255), 0, 0, 0, "Galactic Conflict");

			if(shotnum==reqsh){
				reqsh++;
				n++;
				yshot[n] = 670;
				al_draw_bitmap(shot, xship, yshot[n], 0);
			}
			
			if(yshot[n]>0)
				al_draw_bitmap(shot, pos, yshot[n], 0); 

			al_flip_display();
			redraw = false;
		}
	}

	al_destroy_font(font);
	al_destroy_display(disp);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);
	al_destroy_bitmap(shot);
	al_destroy_bitmap(spaceship);
	return 0;
}

