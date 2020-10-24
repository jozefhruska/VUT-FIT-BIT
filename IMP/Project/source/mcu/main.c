/*******************************************************************************
   main.c: Morse code decoder
   Author: Jozef Hruška <xhrusk25@stud.fit.vutbr.cz> (100%)
*******************************************************************************/

#define CHARACTER_SPACE 32
#define CHARACTER_HASH 35
#define CHARACTER_STAR 42
#define CHARACTER_DASH 45
#define CHARACTER_DOT 46

#define CHARACTER_0 48
#define CHARACTER_1 49
#define CHARACTER_2 50
#define CHARACTER_3 51
#define CHARACTER_4 52
#define CHARACTER_5 53
#define CHARACTER_6 54
#define CHARACTER_7 55
#define CHARACTER_8 56
#define CHARACTER_9 57
#define CHARACTER_A 65
#define CHARACTER_B 66
#define CHARACTER_C 67
#define CHARACTER_D 68
#define CHARACTER_E 69
#define CHARACTER_F 70
#define CHARACTER_G 71
#define CHARACTER_H 72
#define CHARACTER_I 73
#define CHARACTER_J 74
#define CHARACTER_K 75
#define CHARACTER_L 76
#define CHARACTER_M 77
#define CHARACTER_N 78
#define CHARACTER_O 79
#define CHARACTER_P 80
#define CHARACTER_Q 81
#define CHARACTER_R 82
#define CHARACTER_S 83
#define CHARACTER_T 84
#define CHARACTER_U 85
#define CHARACTER_V 86
#define CHARACTER_W 87
#define CHARACTER_X 88
#define CHARACTER_Y 89
#define CHARACTER_Z 90

#define MAX_CHARACTERS 5
#define DISPLAY_LINE_LENGTH 16

#include <fitkitlib.h>
#include <keyboard/keyboard.h>
#include <lcd/display.h>

#include <stdio.h>
#include <string.h>

/* Signs
============================================================================= */
const char SIGN_0[] = {CHARACTER_DASH, CHARACTER_DASH, CHARACTER_DASH, CHARACTER_DASH, CHARACTER_DASH, 0};
const char SIGN_1[] = {CHARACTER_DOT, CHARACTER_DASH, CHARACTER_DASH, CHARACTER_DASH, CHARACTER_DASH, 0};
const char SIGN_2[] = {CHARACTER_DOT, CHARACTER_DOT, CHARACTER_DASH, CHARACTER_DASH, CHARACTER_DASH, 0};
const char SIGN_3[] = {CHARACTER_DOT, CHARACTER_DOT, CHARACTER_DOT, CHARACTER_DASH, CHARACTER_DASH, 0};
const char SIGN_4[] = {CHARACTER_DOT, CHARACTER_DOT, CHARACTER_DOT, CHARACTER_DOT, CHARACTER_DASH, 0};
const char SIGN_5[] = {CHARACTER_DOT, CHARACTER_DOT, CHARACTER_DOT, CHARACTER_DOT, CHARACTER_DOT, 0};
const char SIGN_6[] = {CHARACTER_DOT, CHARACTER_DOT, CHARACTER_DOT, CHARACTER_DOT, CHARACTER_DOT, 0};
const char SIGN_7[] = {CHARACTER_DASH, CHARACTER_DOT, CHARACTER_DOT, CHARACTER_DOT, CHARACTER_DOT, 0};
const char SIGN_8[] = {CHARACTER_DASH, CHARACTER_DASH, CHARACTER_DOT, CHARACTER_DOT, CHARACTER_DOT, 0};
const char SIGN_9[] = {CHARACTER_DASH, CHARACTER_DASH, CHARACTER_DASH, CHARACTER_DOT, CHARACTER_DOT, 0};
const char SIGN_A[] = {CHARACTER_DOT, CHARACTER_DASH, 0, 0, 0};
const char SIGN_B[] = {CHARACTER_DASH, CHARACTER_DOT, CHARACTER_DOT, CHARACTER_DOT, 0};
const char SIGN_C[] = {CHARACTER_DASH, CHARACTER_DOT, CHARACTER_DASH, CHARACTER_DOT, 0};
const char SIGN_D[] = {CHARACTER_DASH, CHARACTER_DOT, CHARACTER_DOT, 0, 0};
const char SIGN_E[] = {CHARACTER_DOT, 0, 0, 0, 0};
const char SIGN_F[] = {CHARACTER_DOT, CHARACTER_DOT, CHARACTER_DASH, CHARACTER_DOT, 0};
const char SIGN_G[] = {CHARACTER_DASH, CHARACTER_DASH, CHARACTER_DOT, 0, 0};
const char SIGN_H[] = {CHARACTER_DOT, CHARACTER_DOT, CHARACTER_DOT, CHARACTER_DOT, 0};
const char SIGN_I[] = {CHARACTER_DOT, CHARACTER_DOT, 0, 0, 0};
const char SIGN_J[] = {CHARACTER_DOT, CHARACTER_DASH, CHARACTER_DASH, CHARACTER_DASH, 0};
const char SIGN_K[] = {CHARACTER_DASH, CHARACTER_DOT, CHARACTER_DASH, 0, 0};
const char SIGN_L[] = {CHARACTER_DOT, CHARACTER_DASH, CHARACTER_DOT, CHARACTER_DOT, 0};
const char SIGN_M[] = {CHARACTER_DASH, CHARACTER_DASH, 0, 0, 0};
const char SIGN_N[] = {CHARACTER_DASH, CHARACTER_DOT, 0, 0, 0};
const char SIGN_O[] = {CHARACTER_DASH, CHARACTER_DASH, CHARACTER_DASH, 0, 0};
const char SIGN_P[] = {CHARACTER_DOT, CHARACTER_DASH, CHARACTER_DASH, CHARACTER_DOT, 0};
const char SIGN_Q[] = {CHARACTER_DASH, CHARACTER_DASH, CHARACTER_DOT, CHARACTER_DASH, 0};
const char SIGN_R[] = {CHARACTER_DOT, CHARACTER_DASH, CHARACTER_DOT, 0, 0};
const char SIGN_S[] = {CHARACTER_DOT, CHARACTER_DOT, CHARACTER_DOT, 0, 0};
const char SIGN_T[] = {CHARACTER_DASH, 0, 0, 0, 0};
const char SIGN_U[] = {CHARACTER_DOT, CHARACTER_DOT, CHARACTER_DASH, 0, 0};
const char SIGN_V[] = {CHARACTER_DOT, CHARACTER_DOT, CHARACTER_DOT, CHARACTER_DASH, 0};
const char SIGN_W[] = {CHARACTER_DOT, CHARACTER_DASH, CHARACTER_DASH, 0, 0};
const char SIGN_X[] = {CHARACTER_DASH, CHARACTER_DOT, CHARACTER_DOT, CHARACTER_DASH, 0};
const char SIGN_Y[] = {CHARACTER_DASH, CHARACTER_DOT, CHARACTER_DASH, CHARACTER_DASH, 0};
const char SIGN_Z[] = {CHARACTER_DASH, CHARACTER_DASH, CHARACTER_DOT, CHARACTER_DOT, 0};

/* Global variables
============================================================================= */
char last_received_character;
int is_sign_timer_running;
short current_speed;

char character_count_top;
char character_count_bottom;
char buffer_lcd_top[DISPLAY_LINE_LENGTH];
char buffer_lcd_bottom[MAX_CHARACTERS];

/* Keyboard control
============================================================================= */
void print_user_help(void)
{
  term_send_str("Help");
  term_send_crlf();
}

/* Clears screen and sets it's new content
============================================================================= */
void refresh_display()
{
  LCD_clear();
  LCD_append_string(buffer_lcd_top);

  int i;
  for (i = character_count_top; i < DISPLAY_LINE_LENGTH; i++)
  {
    LCD_append_char(CHARACTER_SPACE);
  }

  LCD_append_string(buffer_lcd_bottom);

  term_send_str("> Display refreshed.");
  term_send_crlf();
}

/* Clears and starts a new timer
============================================================================= */
void start_new_timer(short value)
{
  TACTL = TACLR;
  CCR0 = value;
  TACTL = TASSEL_1 + MC_1;
}

/* Decodes a morse code sign
============================================================================= */
char decode_sign()
{
  if (strcmp(SIGN_0, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_0;
  }

  if (strcmp(SIGN_1, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_1;
  }

  if (strcmp(SIGN_2, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_2;
  }

  if (strcmp(SIGN_3, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_0;
  }

  if (strcmp(SIGN_3, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_A;
  }

  if (strcmp(SIGN_4, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_4;
  }

  if (strcmp(SIGN_5, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_5;
  }

  if (strcmp(SIGN_6, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_6;
  }

  if (strcmp(SIGN_7, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_7;
  }

  if (strcmp(SIGN_8, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_8;
  }

  if (strcmp(SIGN_9, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_9;
  }

  if (strcmp(SIGN_A, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_A;
  }

  if (strcmp(SIGN_B, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_B;
  }

  if (strcmp(SIGN_C, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_C;
  }

  if (strcmp(SIGN_D, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_D;
  }

  if (strcmp(SIGN_E, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_E;
  }

  if (strcmp(SIGN_F, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_F;
  }

  if (strcmp(SIGN_G, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_G;
  }

  if (strcmp(SIGN_H, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_H;
  }

  if (strcmp(SIGN_I, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_I;
  }

  if (strcmp(SIGN_J, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_J;
  }

  if (strcmp(SIGN_K, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_K;
  }

  if (strcmp(SIGN_L, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_L;
  }

  if (strcmp(SIGN_M, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_M;
  }

  if (strcmp(SIGN_N, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_N;
  }

  if (strcmp(SIGN_O, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_O;
  }

  if (strcmp(SIGN_P, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_P;
  }

  if (strcmp(SIGN_Q, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_Q;
  }

  if (strcmp(SIGN_R, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_R;
  }

  if (strcmp(SIGN_S, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_S;
  }

  if (strcmp(SIGN_T, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_T;
  }

  if (strcmp(SIGN_U, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_U;
  }

  if (strcmp(SIGN_V, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_V;
  }

  if (strcmp(SIGN_W, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_W;
  }

  if (strcmp(SIGN_X, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_X;
  }

  if (strcmp(SIGN_Y, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_Y;
  }

  if (strcmp(SIGN_Z, buffer_lcd_bottom) == 0)
  {
    return CHARACTER_Z;
  }

  return 0;
}

/* Decode morse sign
============================================================================= */
void decode_start()
{
  /* Clear active timer */
  TACTL = TACLR;

  term_send_str("> Decoding...");
  term_send_crlf();

  char result = decode_sign();
  if (result != 0)
  {
    term_send_str("> Decoded: \'");
    term_send_char(result);
    term_send_char('\'');
    term_send_crlf();

    buffer_lcd_top[character_count_top] = result;
    character_count_top++;
  }

  character_count_bottom = 0;
  memset(buffer_lcd_bottom, 0, MAX_CHARACTERS);

  refresh_display();
}

/* Keyboard control
============================================================================= */
int keyboard_idle()
{
  char character;
  character = key_decode(read_word_keyboard_4x4());

  /* Start decoding if maximum number of characters is reached */
  if (character_count_bottom == MAX_CHARACTERS)
  {
    delay_ms(500);
    decode_start();
  }

  if (character_count_top == DISPLAY_LINE_LENGTH)
  {
    character_count_top = 0;
    memset(buffer_lcd_top, 0, DISPLAY_LINE_LENGTH);
  }

  if (character != last_received_character)
  {
    last_received_character = character;

    if (character != 0)
    {
      /* Clear LCD on user's request */
      if (character == CHARACTER_HASH)
      {
        /* Clear active timer */
        TACTL = TACLR;

        LCD_clear();

        character_count_top = 0;
        memset(buffer_lcd_top, 0, DISPLAY_LINE_LENGTH);

        character_count_bottom = 0;
        memset(buffer_lcd_bottom, 0, MAX_CHARACTERS);

        term_send_str("> Display cleared.");
        term_send_crlf();

        return 0;
      }

      /* Set speed 1 */
      if (character == CHARACTER_A)
      {
        current_speed = 0x2000;
        term_send_str("> Speed set: 1.");
        term_send_crlf();
      }

      /* Set speed 2 */
      if (character == CHARACTER_B)
      {
        current_speed = 0x4000;
        term_send_str("> Speed set: 2.");
        term_send_crlf();
      }

      /* Set speed 3 */
      if (character == CHARACTER_C)
      {
        current_speed = 0x6000;
        term_send_str("> Speed set: 3.");
        term_send_crlf();
      }

      /* Set speed 4 */
      if (character == CHARACTER_D)
      {
        current_speed = 0x8000;
        term_send_str("> Speed set: 4.");
        term_send_crlf();
      }

      /* Delete the last character */
      if (character == CHARACTER_0 && character_count_top != 0) {
        buffer_lcd_top[character_count_top - 1] = 0;
        character_count_top--;

        term_send_str("> Deleted last character.");
        term_send_crlf();

        refresh_display();
      }

      /* Handle signal key */
      if (character == CHARACTER_STAR)
      {
        /* Start sign timer */
        start_new_timer(current_speed);
        is_sign_timer_running = 1;

        return 0;
      }
    }
    else
    {
      if (is_sign_timer_running)
      {
        is_sign_timer_running = 0;

        buffer_lcd_bottom[character_count_bottom] = CHARACTER_DOT;
        character_count_bottom++;

        refresh_display();

        term_send_str("> Inserted '.' (dot).");
        term_send_crlf();

        /* Start decode timer */
        start_new_timer(current_speed * 3);
      }
    }
  }

  return 0;
}

/* Decoding of user commands (no commands supported)
============================================================================= */
unsigned char decode_user_cmd(char *cmd_ucase, char *cmd)
{
  return CMD_UNKNOWN;
}

/* FPGA initialization
============================================================================= */
void fpga_initialized()
{
  LCD_init();
  LCD_clear();
  LCD_append_string("Morse decoder");
}

/* Timer interrupt
============================================================================= */
interrupt(TIMERA0_VECTOR) Timer_A(void)
{
  char character;
  character = key_decode(read_word_keyboard_4x4());

  if (character != 0)
  {
    is_sign_timer_running = 0;

    buffer_lcd_bottom[character_count_bottom] = CHARACTER_DASH;
    character_count_bottom++;

    refresh_display();

    term_send_str("> Inserted '-' (dash).");
    term_send_crlf();

    /* Start decode timer */
    start_new_timer(0xF000);
  }
  else
  {
    /* Clear active timer */
    TACTL = TACLR;

    decode_start();
  }
}

/* Main
============================================================================= */
int main(void)
{
  is_sign_timer_running = 0;
  last_received_character = 0;
  current_speed = 0x4000;

  character_count_top = 0;
  character_count_bottom = 0;

  initialize_hardware();
  keyboard_init();

  /* Hide initial message */
  delay_ms(2000);
  LCD_clear();

  /* Activate LEDs */
  set_led_d6(1);
  set_led_d5(1);

  /* Timer setup */
  CCTL0 = CCIE;

  while (1)
  {
    delay_ms(5);

    keyboard_idle();
    terminal_idle();
  }
}
