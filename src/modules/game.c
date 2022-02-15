#include "game.h"

/* Structural Variables */
int running = 1;
int playing = 0;
int selecting = 1;
int waiting = 0;
int lost = 0;

/*  Display and Event Queue */
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;

/* Timers */
ALLEGRO_TIMER *timer = NULL;

/* Fonts */
ALLEGRO_FONT *size_64 = NULL;
ALLEGRO_FONT *size_32 = NULL;
ALLEGRO_FONT *size_24 = NULL;
ALLEGRO_FONT *size_16 = NULL;

ALLEGRO_BITMAP *img_game_over = NULL;
ALLEGRO_BITMAP *img_press_start = NULL;

/* Sounds */
ALLEGRO_SAMPLE *soundtrack = NULL;

int main()
{
    game_load();

    game_run();

    /* End-Game procedures */
    al_destroy_display(display);

    return 1;
}

void game_start(void)
{
    game_reset();

    playing = 1;
    selecting = 0;
    waiting = 0;
    lost = 0;
}

void game_stop(void)
{
    playing = 0;
    waiting = 1;
}

void game_lose(void)
{
    lost = 1;
    playing = 0;

    /* Saves the record */
    if (score > record)
    {
        FILE *file;
        file = fopen("data.txt", "w");
        printf("%d", score);
        fprintf(file, "%d", score);
        fclose(file);
        record = score;
    }
}

void game_reset(void)
{
    coins = 0;
    score = 0;

    /* Loads the record */
    FILE *record_file = fopen("data.txt", "r");
    fscanf(record_file, "%d", &record);
    fclose(record_file);

    /* Resets the ship */
    ship_reset();

    shots_reset();

    entities_reset();

    planets_reset();

    drops_reset();

    /* Resets the timers*/
    game_reset_timers();
}

void game_run(void)
{

    al_play_sample(soundtrack, 0.1f, 0, 1.0, ALLEGRO_PLAYMODE_LOOP, 0);

    srand(time(NULL));

    game_reset_timers();

    Ship *ship;
    ship = ship_get_ship();

    while (running)
    {
        ALLEGRO_EVENT event;

        /* Waits for an event to happen */
        al_wait_for_event(event_queue, &event);

        /* Closes screen (Press X) */
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            running = 0;
            break;
        }

        /* Timers flips */
        if (event.type == ALLEGRO_EVENT_TIMER)
        {
            /* Display timer (60 flips per second) */
            if (event.timer.source == timer)
            {
                // background
                backgrounds_update();

                if (playing)
                {

                    /* Attemps to create a something else each 3 seconds */
                    if (al_get_timer_count(timer) % (100 * 3) == 0)
                    {
                        planets_create_random_planet();
                    }

                    /* Updates */
                    entities_update();
                    shots_update();
                    ship_update();
                    planets_update();
                    stats_update();
                    drops_update();
                }

                char score_text[100];
                sprintf(score_text, "Record: %d", record);
                if (selecting)
                {
                    al_draw_text(size_64, al_map_rgb(255, 255, 255), 1280 / 2, 720 / 2 + al_get_bitmap_height(img_game_over) / 2, ALLEGRO_ALIGN_CENTER, score_text);
                    al_draw_bitmap(img_press_start, 1280 / 2 - al_get_bitmap_width(img_press_start) / 2, 720 / 2 - al_get_bitmap_height(img_press_start) / 2, 0);
                }
                if (lost)
                {
                    al_draw_bitmap(img_game_over, 1280 / 2 - al_get_bitmap_width(img_game_over) / 2, 720 / 2 - al_get_bitmap_height(img_game_over) / 2, 0);
                    if (score >= record)
                    {
                        al_draw_text(size_64, al_map_rgb(255, 255, 255), 1280 / 2, 720 / 2 + al_get_bitmap_height(img_game_over) * 0.75, ALLEGRO_ALIGN_CENTER, "Congrats, new record!");
                    }
                    al_draw_text(size_64, al_map_rgb(255, 255, 255), 1280 / 2, 720 / 2 + al_get_bitmap_height(img_game_over) / 2, ALLEGRO_ALIGN_CENTER, score_text);
                }
                al_flip_display();
            }
        }

        /* Keyboard Inputs Down */
        if (event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP)
        {
            game_process_keyboard(&event);
        }
    }
}

void game_process_keyboard(ALLEGRO_EVENT *event)
{
    Ship *ship;
    ship = ship_get_ship();

    Shot *shots;
    shots = shots_get_shots();

    /* Keyboard Inputs Down */
    if (event->type == ALLEGRO_EVENT_KEY_DOWN)
    {
        if (playing)
        {
            switch (event->keyboard.keycode)
            {
            case ALLEGRO_KEY_UP:
                ship->object.dirY--;
                break;
            case ALLEGRO_KEY_DOWN:
                ship->object.dirY++;
                break;
            case ALLEGRO_KEY_LEFT:
                ship->object.dirX--;
                break;
            case ALLEGRO_KEY_RIGHT:
                ship->object.dirX++;
                break;
            case ALLEGRO_KEY_SPACE:
                ship->shot_charging = 1;
                break;
            }
        }
    }

    /* Keyboard Inputs Up */
    if (event->type == ALLEGRO_EVENT_KEY_UP)
    {
        if (playing)
        {
            switch (event->keyboard.keycode)
            {
            case ALLEGRO_KEY_UP:
                ship->object.dirY++;
                break;
            case ALLEGRO_KEY_DOWN:
                ship->object.dirY--;
                break;
            case ALLEGRO_KEY_LEFT:
                ship->object.dirX++;
                break;
            case ALLEGRO_KEY_RIGHT:
                ship->object.dirX--;
                break;
            case ALLEGRO_KEY_SPACE:
                ship->shot_charging = 0;
                ship_fire_shot();
                break;
            }
        }

        if (selecting || lost)
        {
            switch (event->keyboard.keycode)
            {
            case ALLEGRO_KEY_ENTER:
                game_start();
                break;
            }
        }
    }
}

void game_load(void)
{
    game_setup_allegro();

    stats_load();

    backgrounds_load();

    entities_load();

    planets_load();

    shots_load();

    ship_load();

    drops_load();

    /* Sounds */
    soundtrack = al_load_sample("assets/soundtrack/default.wav");

    /* Images */
    img_game_over = al_load_bitmap("assets/stats/game_over.png");
    img_press_start = al_load_bitmap("assets/stats/press_start.png");

    al_reserve_samples(100);

    game_reset();
}

void game_load_fonts(void)
{
    // Loads the file vcr.ttf from VCR font and defines that will be used the size of 32 size_64 = al_load_font("vcr.ttf", 64, 1);
    size_64 = al_load_font("fonts/vcr.ttf", 64, 1);
    if (size_64 == NULL)
    {
        fprintf(stderr, "font file does not exist or cannot be accessed!\n");
    }

    // Loads the file vcr.ttf from VCR font and defines that will be used the size of 32
    size_32 = al_load_font("fonts/vcr.ttf", 32, 1);
    if (size_32 == NULL)
    {
        fprintf(stderr, "font file does not exist or cannot be accessed!\n");
    }

    // Loads the file vcr.ttf from VCR font and defines that will be used the size of 24
    size_24 = al_load_font("fonts/vcr.ttf", 24, 1);
    if (size_24 == NULL)
    {
        fprintf(stderr, "font file does not exist or cannot be accessed!\n");
    }

    // Loads the file vcr.ttf from VCR font and defines that will be used the size of 16
    size_16 = al_load_font("fonts/vcr.ttf", 16, 1);
    if (size_16 == NULL)
    {
        fprintf(stderr, "font file does not exist or cannot be accessed!\n");
    }
}

int game_reset_timers(void)
{
    // Creates a timer that increases one unit each 1.0/FPS second
    timer = al_create_timer(1.0 / 100);
    if (!timer)
    {
        fprintf(stderr, "failed to create timer!\n");
        return -1;
    }

    // Initializes the timer
    al_start_timer(timer);

    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    return 1;
}

int game_setup_allegro(void)
{
    // Initializes Allegro
    if (!al_init())
    {
        fprintf(stderr, "failed to initialize allegro!\n");
        return 0;
    }

    // Initializes Allegro's primitive modules
    if (!al_init_primitives_addon())
    {
        fprintf(stderr, "failed to initialize primitives!\n");
        return 0;
    }

    // Initializes the module that allows loading images
    if (!al_init_image_addon())
    {
        fprintf(stderr, "failed to initialize image module!\n");
        return 0;
    }

    // Installs the keyboard
    if (!al_install_keyboard())
    {
        fprintf(stderr, "failed to install keyboard!\n");
        return 0;
    }

    // Installs the mouse
    if (!al_install_mouse())
    {
        fprintf(stderr, "failed to initialize mouse!\n");
        return 0;
    }

    // Installs the audio
    if (!al_install_audio())
    {
        fprintf(stderr, "failed to initialize audio!\n");
        return 0;
    }

    // Inits the audio codec
    if (!al_init_acodec_addon())
    {
        fprintf(stderr, "failed to initialize audio codec!\n");
        return 0;
    }

    // Initializes the font modules
    al_init_font_addon();

    // Initializes allegro's module that understand tff font files
    if (!al_init_ttf_addon())
    {
        fprintf(stderr, "failed to load tff font module!\n");
        return 0;
    }

    game_load_fonts();

    // Creates a display with screen dimensions of SCREEN_W, SCREEN_H pixels
    display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display)
    {
        fprintf(stderr, "failed to create display!\n");
        return -1;
    }

    // Creates events queue
    event_queue = al_create_event_queue();
    if (!event_queue)
    {
        fprintf(stderr, "failed to create event_queue!\n");
        al_destroy_display(display);
        return -1;
    }

    // Register events
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    return 1;
}
