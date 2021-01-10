#include <vectrex.h>

#pragma vx_copyright "2021"
#pragma vx_title_pos 0,-80
#pragma vx_title_size -8, 80
#pragma vx_title "g VEC RUNNER"
#pragma vx_music vx_music_1

#define DEFAULT_SCALE 0x7F
#define DEFAULT_INTENSITY 0x7F

const int8_t ship_max_x = 70;
const int8_t ship_max_y = 84;
const uint8_t ship_radius = 8;
const uint8_t ship_size = 2;
const uint8_t ship_speed = 2;

int8_t shipVL[] = {
	-1 * ship_size, -1 * ship_size,
	2 * ship_size, 1 * ship_size,
	-2 * ship_size, 1 * ship_size,
	1 * ship_size, -1 * ship_size
};


struct game_object {
	int8_t x;
	int8_t y;
	int8_t speed;
	int8_t scale;
	int8_t radius;
};


static struct game_object player;

void vectrex_init() {
    set_beam_intensity(DEFAULT_INTENSITY);
    set_scale(DEFAULT_SCALE);

    stop_music();
    stop_sound();

    controller_enable_1_x();
    controller_enable_1_y();
    controller_disable_2_x();
    controller_disable_2_y();
}

BOOL can_ship_move_down() {
	int8_t bottom = player.y - player.radius;
	if (bottom - player.speed >= -ship_max_y) {
		return TRUE;
	}
	return FALSE;
}

BOOL can_ship_move_left() {
	int8_t left = player.x - player.radius;
	if (left - player.speed >= -ship_max_x) {
		return TRUE;
	}
	return FALSE;
}

BOOL can_ship_move_right() {
	int8_t right = player.x + player.radius;
	if (right + player.speed <= ship_max_x) {
		return TRUE;
	}
	return FALSE;
}

BOOL can_ship_move_up() {
	int8_t top = player.y + player.radius;
	if (top + player.speed <= ship_max_y) {
		return TRUE;
	}
	return FALSE;
}

void game_init() {
	player.x = 0;
	player.y = 0;
	player.speed = ship_speed;
	player.scale = 0x7F;
	player.radius = ship_radius;
}

void game_input() {
    controller_check_joysticks();

    if (controller_joystick_1_up() && can_ship_move_up()) {
        player.y += ship_speed;
    } 
    else if (controller_joystick_1_down() && can_ship_move_down()) {
        player.y -= ship_speed;
    } 

    if (controller_joystick_1_left() && can_ship_move_left()) {
        player.x -= ship_speed;
    } 
    else if (controller_joystick_1_right() && can_ship_move_right()) {
		player.x += ship_speed;
    } 
}

int main() {
	vectrex_init();
	game_init();

	while(1) {
		wait_recal();
		intensity_a(0x7f);

		game_input();

		moveto_d(player.y, player.x);
		draw_vl_a(4, shipVL);
	}

	return 0;
}
