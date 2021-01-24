#include <vectrex/bios.h>

#pragma vx_copyright "2021"
#pragma vx_title_pos 0,-80
#pragma vx_title_size -8, 80
#pragma vx_title "g VEC RUNNER"
#pragma vx_music vx_music_1

#define DEFAULT_SCALE 0x7F
#define DEFAULT_INTENSITY 0x5F

typedef enum { false, true } bool;

const int8_t ship_max_x = 124;//70;
const int8_t ship_max_y = 124;//84;
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
	int8_t intensity;
	int8_t radius;
};

typedef enum { UP, DOWN, LEFT, RIGHT } joystick_direction;
bool joystick1[] = { false, false, false, false };
bool buttons1[] = { false, false, false, false };

static struct game_object player;

void vectrex_init() {
    intensity(DEFAULT_INTENSITY);
    set_scale(DEFAULT_SCALE);

    // stop_music();
    // stop_sound();
}

bool can_ship_move_down() {
	int8_t bottom = player.y - player.radius;
	if ((bottom - player.speed) >= -ship_max_y) {
		return true;
	}
	return false;
}

bool can_ship_move_left() {
	int8_t left = player.x - player.radius;
	if ((left - player.speed) >= -ship_max_x) {
		return true;
	}
	return false;
}

bool can_ship_move_right() {
	int8_t right = player.x + player.radius;
	if ((right + player.speed) <= ship_max_x) {
		return true;
	}
	return false;
}

bool can_ship_move_up() {
	int8_t top = player.y + player.radius;
	if ((top + player.speed) <= ship_max_y) {
		return true;
	}
	return false;
}

void game_init() {
	player.x = 0;
	player.y = 0;
	player.speed = ship_speed;
	player.intensity = 0x7F;
	player.radius = ship_radius;
}

void game_input() {
  uint8_t i, joystick_result1, buttons_result;
	joystick_result1 = read_joystick(1);
  buttons_result = read_buttons();
  char j1t[] = { '1', ':', '_', '_', '_', '_', 0x00 };

  for(i = 0; i < 4; i++) {
    joystick1[i] = (joystick_result1 & (1<<i)) ? true : false;
    j1t[2+i] = (joystick_result1 & (1<<i)) ? 'X' : '_';
  }

  buttons1[0] = (buttons_result & JOY1_BTN1_MASK) ? true : false;
  buttons1[1] = (buttons_result & JOY1_BTN2_MASK) ? true : false;
  buttons1[2] = (buttons_result & JOY1_BTN3_MASK) ? true : false;
  buttons1[3] = (buttons_result & JOY1_BTN4_MASK) ? true : false;

  intensity(0x7f);
  print_str_c(-0x10, -0x50, j1t); 

  if (joystick1[UP] && can_ship_move_up()) {
    player.y += ship_speed;
    print_str_c(-0x20, -0x50, "UP");
  }
  else if (joystick1[DOWN] && can_ship_move_down()) {
    player.y -= ship_speed;
    print_str_c(-0x20, -0x50, "DOWN");
  }

  if (joystick1[LEFT] && can_ship_move_left()) {
    player.x -= ship_speed;
    print_str_c(-0x30, -0x50, "LEFT");
  }
  else if (joystick1[RIGHT] && can_ship_move_right()) {
    player.x += ship_speed;
    print_str_c(-0x30, -0x50, "RIGHT");
  }
}

int main() {
	vectrex_init();
	game_init();

	while(1) {
		wait_retrace();

		game_input();
    print_str_c(0x20, -0x50, "TEST STUFF");
    reset_beam();

		intensity(player.intensity);
		move(player.y, player.x);
		lines(4, shipVL);
	}

	return 0;
}
