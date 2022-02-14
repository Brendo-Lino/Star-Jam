#include "game.h"

/* Score and coins */
int score = 0;
int record = 0;
int coins = 0;

/* Stats */
ALLEGRO_BITMAP *img_heart_full = NULL;
ALLEGRO_BITMAP *img_heart_empty = NULL;
ALLEGRO_BITMAP *img_flipping_coin_sheet = NULL;
ALLEGRO_BITMAP *img_bar_charging_sheet = NULL;
ALLEGRO_BITMAP *img_coin = NULL;

void stats_load(void)
{
    img_heart_full = al_load_bitmap("assets/stats/heart_full.png");
    img_heart_empty = al_load_bitmap("assets/stats/heart_empty.png");
    img_flipping_coin_sheet = al_load_bitmap("assets/stats/flipping_coin_sheet.png");
    img_coin = al_load_bitmap("assets/stats/coin.png");
    img_bar_charging_sheet = al_load_bitmap("assets/stats/charging_bar_sheet.png");
}

void stats_update(void)
{
    Ship *ship;
    ship = ship_get_ship();

    /* Hearts */
    int hearts_x = 0;
    for (int i = 1; i < 6; i++)
    {
        ALLEGRO_BITMAP *img_heart;
        if (ship->health >= i)
        {
            img_heart = img_heart_full;
        }
        else
        {
            img_heart = img_heart_empty;
        }
        hearts_x = SCREEN_W / 100 * 2 + al_get_bitmap_width(img_heart) * i + i * al_get_bitmap_width(img_heart) / 4;
        al_draw_bitmap(img_heart, hearts_x, SCREEN_H / 100 * 2, 0);
    }

    /* Score */
    char score_text[100];
    sprintf(score_text, "SCORE:%d", score);
    al_draw_text(size_32, al_map_rgb(255, 255, 255), hearts_x + 70, SCREEN_H / 100 * 3, ALLEGRO_ALIGN_LEFT, score_text);

    /* Record */
    char record_text[100];
    sprintf(record_text, "RECORD: %d", record);
    al_draw_text(size_32, al_map_rgb(255, 255, 255), hearts_x + 280, SCREEN_H / 100 * 3, ALLEGRO_ALIGN_LEFT, record_text);

    /* Coins */
    char coins_text[100];
    sprintf(coins_text, "%dx", coins);
    al_draw_text(size_24, al_map_rgb(255, 255, 255), SCREEN_W / 100 * 99, SCREEN_H / 100 * 4, ALLEGRO_ALIGN_RIGHT, coins_text);
    al_draw_bitmap(img_coin, SCREEN_W / 100 * 100, SCREEN_H / 100 * 2, 0);

    /* Charging bar */
    al_draw_bitmap_region(img_bar_charging_sheet, ((ship->shot_charge <= 5 ? ship->shot_charge : 5) * 152), 0, 152, 36, 550, 655, 0);
}
