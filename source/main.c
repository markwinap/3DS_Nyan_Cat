#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <3ds.h>
#include <sf2d.h>

//Slider Stuff
#define CONFIG_3D_SLIDERSTATE (*(float*)0x1FF81080)

//Audio Stuff WIP
u8* buffer;
u32 size;
#define SAMPLERATE 32000

static u64 last_time = 0;
static u64 last_time2 = 0;
static u64 new_time = 0;
static u64 new_time2 = 0;

static u64 counter_frame = 0;

int interaxial = 20;

int catposx = 192;
int catposy = 68;
int rainposx = 192;
int rainposy = 76;
int rainbow_frame = 1;
int cat_frame = 1;
int star_frame = 1;

int star_pos1;
int star_pos2;
int star_pos3;
int star_pos4;
int star_pos5;
int star_pos6;
int star_pos7;
int star_pos8;
int topx = 400;
int topy = 240;
int bottom = 320;

int numx = 151;
int numy = 100;

int cat_selected = 1;
int play_state = 0;

int first_cat = 1;
int last_cat = 16;

sf2d_texture *tex2;
sf2d_texture *tex1;

//int other = 0;
//static u64 new_time3 = 0;
//static u64 last_time3 = 0;

//Cat2 780*80 6 frames 130*80 70ms
//rainbow speed 300ms 2 frames only
//star 56*14 4 frames 14*14 70ms
//font 272*42 17*21 cada character
//font2 352*54 22*27 cada character
//3ds screen top resolution 400*240
//3ds screen bottom resolution 320*240
extern const struct {
	unsigned int 	 width;
	unsigned int 	 height;
	unsigned int 	 bytes_per_pixel;
	unsigned char	 pixel_data[];
} ballon_img;

extern const struct {
	unsigned int 	 width;
	unsigned int 	 height;
	unsigned int 	 bytes_per_pixel;
	unsigned char	 pixel_data[];
} cloud_img;


extern const struct {
	unsigned int 	 width;
	unsigned int 	 height;
	unsigned int 	 bytes_per_pixel;
	unsigned char	 pixel_data[];
} mummy_img;


extern const struct {
	unsigned int 	 width;
	unsigned int 	 height;
	unsigned int 	 bytes_per_pixel;
	unsigned char	 pixel_data[];
} dub_img;

extern const struct {
	unsigned int 	 width;
	unsigned int 	 height;
	unsigned int 	 bytes_per_pixel;
	unsigned char	 pixel_data[];
} technyancolor_img;



extern const struct {
	unsigned int 	 width;
	unsigned int 	 height;
	unsigned int 	 bytes_per_pixel;
	unsigned char	 pixel_data[];
} skull_img;


extern const struct {
	unsigned int 	 width;
	unsigned int 	 height;
	unsigned int 	 bytes_per_pixel;
	unsigned char	 pixel_data[];
} tacnayn_img;

extern const struct {
	unsigned int 	 width;
	unsigned int 	 height;
	unsigned int 	 bytes_per_pixel;
	unsigned char	 pixel_data[];
} jazz_img;

extern const struct {
	unsigned int 	 width;
	unsigned int 	 height;
	unsigned int 	 bytes_per_pixel;
	unsigned char	 pixel_data[];
} sad_img;

extern const struct {
	unsigned int 	 width;
	unsigned int 	 height;
	unsigned int 	 bytes_per_pixel;
	unsigned char	 pixel_data[];
} zombie_img;

extern const struct {
	unsigned int 	 width;
	unsigned int 	 height;
	unsigned int 	 bytes_per_pixel;
	unsigned char	 pixel_data[];
} grumpy_img;


extern const struct {
	unsigned int 	 width;
	unsigned int 	 height;
	unsigned int 	 bytes_per_pixel;
	unsigned char	 pixel_data[];
} america_img;


extern const struct {
	unsigned int 	 width;
	unsigned int 	 height;
	unsigned int 	 bytes_per_pixel;
	unsigned char	 pixel_data[];
} j5_img;


extern const struct {
	unsigned int 	 width;
	unsigned int 	 height;
	unsigned int 	 bytes_per_pixel;
	unsigned char	 pixel_data[];
} pikanyan_img;

extern const struct {
	unsigned int 	 width;
	unsigned int 	 height;
	unsigned int 	 bytes_per_pixel;
	unsigned char	 pixel_data[];
} jamaicnyan_img;


extern const struct {
	unsigned int 	 width;
	unsigned int 	 height;
	unsigned int 	 bytes_per_pixel;
	unsigned char	 pixel_data[];
} cat2_img;

extern const struct {
	unsigned int 	 width;
	unsigned int 	 height;
	unsigned int 	 bytes_per_pixel;
	unsigned char	 pixel_data[];
} gb_img;


extern const struct {
	unsigned int 	 width;
	unsigned int 	 height;
	unsigned int 	 bytes_per_pixel;
	unsigned char	 pixel_data[];
} star_img;

extern const struct {
	unsigned int 	 width;
	unsigned int 	 height;
	unsigned int 	 bytes_per_pixel;
	unsigned char	 pixel_data[];
} font_img;

extern const struct {
	unsigned int 	 width;
	unsigned int 	 height;
	unsigned int 	 bytes_per_pixel;
	unsigned char	 pixel_data[];
} nyaned_img;

extern const struct {
	unsigned int 	 width;
	unsigned int 	 height;
	unsigned int 	 bytes_per_pixel;
	unsigned char	 pixel_data[];
} seconds_img;

static void draw_rainbow_dub(int rainx, int rainy, int line1, int line2, int line3, int line4, int line5, int line6);
static void draw_rainbow_tacnayn(int rainx, int rainy);
static void draw_rainbow_jazz(int rainx, int rainy);
static void draw_rainbow_america(int rainx, int rainy);
static void draw_rainbow_pikanyan(int rainx, int rainy);
static void draw_rainbow_jamaicnyan(int rainx, int rainy);
static void draw_rainbow(int rainx, int rainy);
static void draw_rainbow_gb(int rainx, int rainy);
static void draw_number(const sf2d_texture *mmm, int num, int numx, int numy);

static void audio_load(const char *audio);
static void audio_stop(void);

int main()
{

	// Set the random seed based on the time
	srand(time(NULL));

	sf2d_init();

	sf2d_set_vblank_wait(1);
	sf2d_set_clear_color(RGBA8(0x0F, 0x4D, 0x8F, 0xFF));//Background

	sf2d_set_3D(1);//Enable 3D

	sf2d_texture *font = sf2d_create_texture_mem_RGBA8(font_img.pixel_data, font_img.width, font_img.height, GPU_RGBA8, SF2D_PLACE_RAM);
	sf2d_texture *seconds = sf2d_create_texture_mem_RGBA8(seconds_img.pixel_data, seconds_img.width, seconds_img.height, GPU_RGBA8, SF2D_PLACE_RAM);
	sf2d_texture *nyaned = sf2d_create_texture_mem_RGBA8(nyaned_img.pixel_data, nyaned_img.width, nyaned_img.height, GPU_RGBA8, SF2D_PLACE_RAM);


	csndInit();//start Audio Lib

	play_state = 1;
	cat_selected = rand() % last_cat; //Initiate with a random cat selected
	last_time = osGetTime(); //get current time
	last_time2 = osGetTime(); //get current time 2

	

		while (aptMainLoop()) {

			int right_eye = CONFIG_3D_SLIDERSTATE * interaxial;
			hidScanInput();
			if (hidKeysDown() & KEY_START) break;

			if (hidKeysDown() & KEY_DUP){
				audio_stop();
				audio_stop();
				cat_selected = cat_selected + 2;
					play_state = 1;
			}

			if (hidKeysDown() & KEY_DDOWN){
				audio_stop();
				audio_stop();
				cat_selected = cat_selected -2;
					play_state = 1;	
			}

			if (hidKeysDown() & KEY_DLEFT){
				audio_stop();
				audio_stop();
				cat_selected--;
				play_state = 1;
			}

			if (hidKeysDown() & KEY_DRIGHT){
				audio_stop();
				audio_stop();
				cat_selected++;
				play_state = 1;
			}
			if (hidKeysDown() & KEY_A){
				cat_selected = rand() % last_cat;
				audio_stop();
				audio_stop();
				play_state = 1;
			}
			if (hidKeysDown() & KEY_X){
				cat_selected = last_cat;
				audio_stop();
				audio_stop();
				play_state = 1;
			}
			if (hidKeysDown() & KEY_Y){
				cat_selected = first_cat;
				audio_stop();
				audio_stop();
				play_state = 1;
			}
			if (cat_selected >= last_cat + 1){
				cat_selected = first_cat;
			}
			if (cat_selected <= 0){
				cat_selected = last_cat;
			}


			//////////////////////

			//ballon_img

			if (cat_selected == 16 && play_state == 1){
				sf2d_set_clear_color(RGBA8(0x0F, 0x4D, 0x8F, 0xFF));//Background

				sf2d_free_texture(tex1);
				sf2d_free_texture(tex2);


				tex2 = sf2d_create_texture_mem_RGBA8(ballon_img.pixel_data, ballon_img.width, ballon_img.height, GPU_RGBA8, SF2D_PLACE_RAM);
				tex1 = sf2d_create_texture_mem_RGBA8(cloud_img.pixel_data, cloud_img.width, cloud_img.height, GPU_RGBA8, SF2D_PLACE_RAM);


				audio_load("audio/balloon_raw.bin");
				play_state = 0;
			}

			//dub_img

			if (cat_selected == 15 && play_state == 1){
				sf2d_set_clear_color(RGBA8(0x0C, 0x0C, 0x0C, 0xFF));//Background
				
				sf2d_free_texture(tex1);
				sf2d_free_texture(tex2);

				
				tex2 = sf2d_create_texture_mem_RGBA8(dub_img.pixel_data, dub_img.width, dub_img.height, GPU_RGBA8, SF2D_PLACE_RAM);
				tex1 = sf2d_create_texture_mem_RGBA8(star_img.pixel_data, star_img.width, star_img.height, GPU_RGBA8, SF2D_PLACE_RAM);


				audio_load("audio/dub_raw.bin");
				play_state = 0;
			}


			//technyancolor_img
			if (cat_selected == 14 && play_state == 1){
				sf2d_set_clear_color(RGBA8(0x0C, 0x0C, 0x0C, 0xFF));//Background

				sf2d_free_texture(tex1);
				sf2d_free_texture(tex2);

				tex1 = sf2d_create_texture_mem_RGBA8(star_img.pixel_data, star_img.width, star_img.height, GPU_RGBA8, SF2D_PLACE_RAM);
				tex2 = sf2d_create_texture_mem_RGBA8(technyancolor_img.pixel_data, technyancolor_img.width, technyancolor_img.height, GPU_RGBA8, SF2D_PLACE_RAM);

				audio_load("audio/technyancolor_raw.bin");


				play_state = 0;
			}



			//tacnayn_img

			if (cat_selected == 13 && play_state == 1){
				sf2d_set_clear_color(RGBA8(0x77, 0x00, 0x0D, 0xFF));//Background

				sf2d_free_texture(tex1);
				sf2d_free_texture(tex2);
				
				tex1 = sf2d_create_texture_mem_RGBA8(skull_img.pixel_data, skull_img.width, skull_img.height, GPU_RGBA8, SF2D_PLACE_RAM);
				tex2 = sf2d_create_texture_mem_RGBA8(tacnayn_img.pixel_data, tacnayn_img.width, tacnayn_img.height, GPU_RGBA8, SF2D_PLACE_RAM);

				audio_load("audio/tacnayn_raw.bin");


				play_state = 0;
			}

			// jazz_img

			if (cat_selected == 12 && play_state == 1){
					sf2d_set_clear_color(RGBA8(0x43, 0x16, 0x43, 0xFF));//Background

					sf2d_free_texture(tex1);
					sf2d_free_texture(tex2);

					tex1 = sf2d_create_texture_mem_RGBA8(star_img.pixel_data, star_img.width, star_img.height, GPU_RGBA8, SF2D_PLACE_RAM);
					tex2 = sf2d_create_texture_mem_RGBA8(jazz_img.pixel_data, jazz_img.width, jazz_img.height, GPU_RGBA8, SF2D_PLACE_RAM);

					audio_load("audio/jazz_raw.bin");


					play_state = 0;
				}


			// sad_img

			if (cat_selected == 11 && play_state == 1){
				sf2d_set_clear_color(RGBA8(0x00, 0x16, 0x1E, 0xFF));//Background

				sf2d_free_texture(tex1);
				sf2d_free_texture(tex2);

				tex1 = sf2d_create_texture_mem_RGBA8(star_img.pixel_data, star_img.width, star_img.height, GPU_RGBA8, SF2D_PLACE_RAM);
				tex2 = sf2d_create_texture_mem_RGBA8(sad_img.pixel_data, sad_img.width, sad_img.height, GPU_RGBA8, SF2D_PLACE_RAM);

				audio_load("audio/sad_raw.bin");


				play_state = 0;

			}
			// slomo
			if (cat_selected == 10 && play_state == 1){
				sf2d_set_clear_color(RGBA8(0x0F, 0x4D, 0x8F, 0xFF));//Background

				sf2d_free_texture(tex1);
				sf2d_free_texture(tex2);

				tex1 = sf2d_create_texture_mem_RGBA8(star_img.pixel_data, star_img.width, star_img.height, GPU_RGBA8, SF2D_PLACE_RAM);
				tex2 = sf2d_create_texture_mem_RGBA8(cat2_img.pixel_data, cat2_img.width, cat2_img.height, GPU_RGBA8, SF2D_PLACE_RAM);

				audio_load("audio/slomo_raw.bin");


				play_state = 0;

			}


			//zombie_img
			if (cat_selected == 9 && play_state == 1){
				sf2d_set_clear_color(RGBA8(0x0F, 0x4D, 0x8F, 0xFF));//Background

				sf2d_free_texture(tex1);
				sf2d_free_texture(tex2);
				
				tex1 = sf2d_create_texture_mem_RGBA8(star_img.pixel_data, star_img.width, star_img.height, GPU_RGBA8, SF2D_PLACE_RAM);
				tex2 = sf2d_create_texture_mem_RGBA8(zombie_img.pixel_data, zombie_img.width, zombie_img.height, GPU_RGBA8, SF2D_PLACE_RAM);

				audio_load("audio/original_raw.bin");


				play_state = 0;

			}



			//grumpy_img
			if (cat_selected == 8 && play_state == 1){
				sf2d_set_clear_color(RGBA8(0x0F, 0x4D, 0x8F, 0xFF));//Background

				sf2d_free_texture(tex1);
				sf2d_free_texture(tex2);

				tex1 = sf2d_create_texture_mem_RGBA8(star_img.pixel_data, star_img.width, star_img.height, GPU_RGBA8, SF2D_PLACE_RAM);
				tex2 = sf2d_create_texture_mem_RGBA8(grumpy_img.pixel_data, grumpy_img.width, grumpy_img.height, GPU_RGBA8, SF2D_PLACE_RAM);

				audio_load("audio/original_raw.bin");

				play_state = 0;

			}

			//america_img
			if (cat_selected == 7 && play_state == 1){
				sf2d_set_clear_color(RGBA8(0x0F, 0x4D, 0x8F, 0xFF));//Background

				sf2d_free_texture(tex1);
				sf2d_free_texture(tex2);

				tex1 = sf2d_create_texture_mem_RGBA8(star_img.pixel_data, star_img.width, star_img.height, GPU_RGBA8, SF2D_PLACE_RAM);
				tex2 = sf2d_create_texture_mem_RGBA8(america_img.pixel_data, america_img.width, america_img.height, GPU_RGBA8, SF2D_PLACE_RAM);

				audio_load("audio/america_raw.bin");

				play_state = 0;

			}

			//j5_img
			if (cat_selected == 6 && play_state == 1){
				sf2d_set_clear_color(RGBA8(0x0F, 0x4D, 0x8F, 0xFF));//Background

				sf2d_free_texture(tex1);
				sf2d_free_texture(tex2);

				tex1 = sf2d_create_texture_mem_RGBA8(star_img.pixel_data, star_img.width, star_img.height, GPU_RGBA8, SF2D_PLACE_RAM);
				tex2 = sf2d_create_texture_mem_RGBA8(j5_img.pixel_data, j5_img.width, j5_img.height, GPU_RGBA8, SF2D_PLACE_RAM);

				audio_load("audio/j5_raw.bin");
				play_state = 0;

			}


			//pikanyan_img
			if (cat_selected == 5 && play_state == 1){
				sf2d_set_clear_color(RGBA8(0x0F, 0x4D, 0x8F, 0xFF));//Background

				sf2d_free_texture(tex1);
				sf2d_free_texture(tex2);

				tex1 = sf2d_create_texture_mem_RGBA8(star_img.pixel_data, star_img.width, star_img.height, GPU_RGBA8, SF2D_PLACE_RAM);
				tex2 = sf2d_create_texture_mem_RGBA8(pikanyan_img.pixel_data, pikanyan_img.width, pikanyan_img.height, GPU_RGBA8, SF2D_PLACE_RAM);

				audio_load("audio/pikanyan_raw.bin");
				play_state = 0;

			}




			//jamaicnyan
			if (cat_selected == 4 && play_state == 1){
				sf2d_set_clear_color(RGBA8(0x0F, 0x4D, 0x8F, 0xFF));//Background

				sf2d_free_texture(tex1);
				sf2d_free_texture(tex2);

				tex1 = sf2d_create_texture_mem_RGBA8(star_img.pixel_data, star_img.width, star_img.height, GPU_RGBA8, SF2D_PLACE_RAM);
				tex2 = sf2d_create_texture_mem_RGBA8(jamaicnyan_img.pixel_data, jamaicnyan_img.width, jamaicnyan_img.height, GPU_RGBA8, SF2D_PLACE_RAM);

				audio_load("audio/jamaicnyan_raw.bin");
				play_state = 0;

			}


			//gb
			if (cat_selected == 3 && play_state == 1){
				sf2d_set_clear_color(RGBA8(0x0F, 0x4D, 0x8F, 0xFF));//Background

				sf2d_free_texture(tex1);
				sf2d_free_texture(tex2);

				tex1 = sf2d_create_texture_mem_RGBA8(star_img.pixel_data, star_img.width, star_img.height, GPU_RGBA8, SF2D_PLACE_RAM);
				tex2 = sf2d_create_texture_mem_RGBA8(gb_img.pixel_data, gb_img.width, gb_img.height, GPU_RGBA8, SF2D_PLACE_RAM);

				audio_load("audio/gb_raw.bin");
				play_state = 0;

			}
			//mummy
			if (cat_selected == 2 && play_state == 1){
				sf2d_set_clear_color(RGBA8(0x0F, 0x4D, 0x8F, 0xFF));//Background

				sf2d_free_texture(tex1);
				sf2d_free_texture(tex2);

				tex1 = sf2d_create_texture_mem_RGBA8(star_img.pixel_data, star_img.width, star_img.height, GPU_RGBA8, SF2D_PLACE_RAM);
				tex2 = sf2d_create_texture_mem_RGBA8(mummy_img.pixel_data, mummy_img.width, mummy_img.height, GPU_RGBA8, SF2D_PLACE_RAM);
				
				audio_load("audio/original_raw.bin");
				play_state = 0;
			}


			if (cat_selected == 1 && play_state == 1){
				sf2d_set_clear_color(RGBA8(0x0F, 0x4D, 0x8F, 0xFF));//Background

				sf2d_free_texture(tex1);
				sf2d_free_texture(tex2);

				tex1 = sf2d_create_texture_mem_RGBA8(star_img.pixel_data, star_img.width, star_img.height, GPU_RGBA8, SF2D_PLACE_RAM);
				tex2 = sf2d_create_texture_mem_RGBA8(cat2_img.pixel_data, cat2_img.width, cat2_img.height, GPU_RGBA8, SF2D_PLACE_RAM);

				audio_load("audio/original_raw.bin");
				play_state = 0;

			}

			////////////////////////////////
			new_time = osGetTime() - last_time;
			if (new_time >= 70){
				rainbow_frame++;
				cat_frame++;
				star_frame++;
				last_time = osGetTime();
			}
			new_time2 = osGetTime() - last_time2;
			if (new_time2 >= 100){
				counter_frame++;
				last_time2 = osGetTime();
			}

			sf2d_start_frame(GFX_TOP, GFX_LEFT);


			if (cat_selected == 1 || cat_selected == 2 || cat_selected == 6 || cat_selected == 8 || cat_selected == 9 || cat_selected == 10 || cat_selected == 14){

					if (rainbow_frame <= 5){
						draw_rainbow(rainposx, rainposy);
						draw_rainbow(rainposx - 32, rainposy - 4);
						draw_rainbow(rainposx - 64, rainposy);
						draw_rainbow(rainposx - 96, rainposy + 4); // FIX
						draw_rainbow(rainposx - 128, rainposy + 8);
						draw_rainbow(rainposx - 160, rainposy + 4); // FIX
						draw_rainbow(rainposx - 192, rainposy);
					}
					if (rainbow_frame > 5 && rainbow_frame <= 10){
						draw_rainbow(rainposx, rainposy + 8);
						draw_rainbow(rainposx - 32, rainposy + 12);
						draw_rainbow(rainposx - 64, rainposy + 8);
						draw_rainbow(rainposx - 96, rainposy + 4); // FIX
						draw_rainbow(rainposx - 128, rainposy);
						draw_rainbow(rainposx - 160, rainposy + 4); // FIX
						draw_rainbow(rainposx - 192, rainposy + 8);
					}
					if (rainbow_frame > 10){
						draw_rainbow(rainposx, rainposy);
						draw_rainbow(rainposx - 32, rainposy - 4);
						draw_rainbow(rainposx - 64, rainposy);
						draw_rainbow(rainposx - 96, rainposy + 4); // FIX
						draw_rainbow(rainposx - 128, rainposy + 8);
						draw_rainbow(rainposx - 160, rainposy + 4); // FIX
						draw_rainbow(rainposx - 192, rainposy);


						rainbow_frame = 1;
					}
				}


			if (cat_selected == 3){

					if (rainbow_frame <= 5){
						draw_rainbow_gb(rainposx, rainposy);
						draw_rainbow_gb(rainposx - 32, rainposy - 4);
						draw_rainbow_gb(rainposx - 64, rainposy);
						draw_rainbow_gb(rainposx - 96, rainposy + 4); // FIX
						draw_rainbow_gb(rainposx - 128, rainposy + 8);
						draw_rainbow_gb(rainposx - 160, rainposy + 4); // FIX
						draw_rainbow_gb(rainposx - 192, rainposy);
					}
					if (rainbow_frame > 5 && rainbow_frame <= 10){
						draw_rainbow_gb(rainposx, rainposy + 8);
						draw_rainbow_gb(rainposx - 32, rainposy + 12);
						draw_rainbow_gb(rainposx - 64, rainposy + 8);
						draw_rainbow_gb(rainposx - 96, rainposy + 4); // FIX
						draw_rainbow_gb(rainposx - 128, rainposy);
						draw_rainbow_gb(rainposx - 160, rainposy + 4); // FIX
						draw_rainbow_gb(rainposx - 192, rainposy + 8);
					}
					if (rainbow_frame > 10){
						draw_rainbow_gb(rainposx, rainposy);
						draw_rainbow_gb(rainposx - 32, rainposy - 4);
						draw_rainbow_gb(rainposx - 64, rainposy);
						draw_rainbow_gb(rainposx - 96, rainposy + 4); // FIX
						draw_rainbow_gb(rainposx - 128, rainposy + 8);
						draw_rainbow_gb(rainposx - 160, rainposy + 4); // FIX
						draw_rainbow_gb(rainposx - 192, rainposy);


						rainbow_frame = 1;
					}
				}

			if (cat_selected == 4){

					if (rainbow_frame <= 5){
						draw_rainbow_jamaicnyan(rainposx, rainposy);
						draw_rainbow_jamaicnyan(rainposx - 32, rainposy - 4);
						draw_rainbow_jamaicnyan(rainposx - 64, rainposy);
						draw_rainbow_jamaicnyan(rainposx - 96, rainposy + 4); // FIX
						draw_rainbow_jamaicnyan(rainposx - 128, rainposy + 8);
						draw_rainbow_jamaicnyan(rainposx - 160, rainposy + 4); // FIX
						draw_rainbow_jamaicnyan(rainposx - 192, rainposy);
					}
					if (rainbow_frame > 5 && rainbow_frame <= 10){
						draw_rainbow_jamaicnyan(rainposx, rainposy + 8);
						draw_rainbow_jamaicnyan(rainposx - 32, rainposy + 12);
						draw_rainbow_jamaicnyan(rainposx - 64, rainposy + 8);
						draw_rainbow_jamaicnyan(rainposx - 96, rainposy + 4); // FIX
						draw_rainbow_jamaicnyan(rainposx - 128, rainposy);
						draw_rainbow_jamaicnyan(rainposx - 160, rainposy + 4); // FIX
						draw_rainbow_jamaicnyan(rainposx - 192, rainposy + 8);
					}
					if (rainbow_frame > 10){
						draw_rainbow_jamaicnyan(rainposx, rainposy);
						draw_rainbow_jamaicnyan(rainposx - 32, rainposy - 4);
						draw_rainbow_jamaicnyan(rainposx - 64, rainposy);
						draw_rainbow_jamaicnyan(rainposx - 96, rainposy + 4); // FIX
						draw_rainbow_jamaicnyan(rainposx - 128, rainposy + 8);
						draw_rainbow_jamaicnyan(rainposx - 160, rainposy + 4); // FIX
						draw_rainbow_jamaicnyan(rainposx - 192, rainposy);


						rainbow_frame = 1;
					}
				}

			if (cat_selected == 7){

					if (rainbow_frame <= 5){
						draw_rainbow_america(rainposx, rainposy);
						draw_rainbow_america(rainposx - 32, rainposy - 4);
						draw_rainbow_america(rainposx - 64, rainposy);
						draw_rainbow_america(rainposx - 96, rainposy + 4); // FIX
						draw_rainbow_america(rainposx - 128, rainposy + 8);
						draw_rainbow_america(rainposx - 160, rainposy + 4); // FIX
						draw_rainbow_america(rainposx - 192, rainposy);
					}
					if (rainbow_frame > 5 && rainbow_frame <= 10){
						draw_rainbow_america(rainposx, rainposy + 8);
						draw_rainbow_america(rainposx - 32, rainposy + 12);
						draw_rainbow_america(rainposx - 64, rainposy + 8);
						draw_rainbow_america(rainposx - 96, rainposy + 4); // FIX
						draw_rainbow_america(rainposx - 128, rainposy);
						draw_rainbow_america(rainposx - 160, rainposy + 4); // FIX
						draw_rainbow_america(rainposx - 192, rainposy + 8);
					}
					if (rainbow_frame > 10){
						draw_rainbow_america(rainposx, rainposy);
						draw_rainbow_america(rainposx - 32, rainposy - 4);
						draw_rainbow_america(rainposx - 64, rainposy);
						draw_rainbow_america(rainposx - 96, rainposy + 4); // FIX
						draw_rainbow_america(rainposx - 128, rainposy + 8);
						draw_rainbow_america(rainposx - 160, rainposy + 4); // FIX
						draw_rainbow_america(rainposx - 192, rainposy);


						rainbow_frame = 1;
					}
				}

			if (cat_selected == 5){

					if (rainbow_frame <= 5){
						draw_rainbow_pikanyan(rainposx, rainposy);
						draw_rainbow_pikanyan(rainposx - 32, rainposy - 4);
						draw_rainbow_pikanyan(rainposx - 64, rainposy);
						draw_rainbow_pikanyan(rainposx - 96, rainposy + 4); // FIX
						draw_rainbow_pikanyan(rainposx - 128, rainposy + 8);
						draw_rainbow_pikanyan(rainposx - 160, rainposy + 4); // FIX
						draw_rainbow_pikanyan(rainposx - 192, rainposy);
					}
					if (rainbow_frame > 5 && rainbow_frame <= 10){
						draw_rainbow_pikanyan(rainposx, rainposy + 8);
						draw_rainbow_pikanyan(rainposx - 32, rainposy + 12);
						draw_rainbow_pikanyan(rainposx - 64, rainposy + 8);
						draw_rainbow_pikanyan(rainposx - 96, rainposy + 4); // FIX
						draw_rainbow_pikanyan(rainposx - 128, rainposy);
						draw_rainbow_pikanyan(rainposx - 160, rainposy + 4); // FIX
						draw_rainbow_pikanyan(rainposx - 192, rainposy + 8);
					}
					if (rainbow_frame > 10){
						draw_rainbow_pikanyan(rainposx, rainposy);
						draw_rainbow_pikanyan(rainposx - 32, rainposy - 4);
						draw_rainbow_pikanyan(rainposx - 64, rainposy);
						draw_rainbow_pikanyan(rainposx - 96, rainposy + 4); // FIX
						draw_rainbow_pikanyan(rainposx - 128, rainposy + 8);
						draw_rainbow_pikanyan(rainposx - 160, rainposy + 4); // FIX
						draw_rainbow_pikanyan(rainposx - 192, rainposy);


						rainbow_frame = 1;
					}
				}

			if (cat_selected == 12){

					if (rainbow_frame <= 5){
						draw_rainbow_jazz(rainposx, rainposy);
						draw_rainbow_jazz(rainposx - 32, rainposy - 4);
						draw_rainbow_jazz(rainposx - 64, rainposy);
						draw_rainbow_jazz(rainposx - 96, rainposy + 4); // FIX
						draw_rainbow_jazz(rainposx - 128, rainposy + 8);
						draw_rainbow_jazz(rainposx - 160, rainposy + 4); // FIX
						draw_rainbow_jazz(rainposx - 192, rainposy);
					}
					if (rainbow_frame > 5 && rainbow_frame <= 10){
						draw_rainbow_jazz(rainposx, rainposy + 8);
						draw_rainbow_jazz(rainposx - 32, rainposy + 12);
						draw_rainbow_jazz(rainposx - 64, rainposy + 8);
						draw_rainbow_jazz(rainposx - 96, rainposy + 4); // FIX
						draw_rainbow_jazz(rainposx - 128, rainposy);
						draw_rainbow_jazz(rainposx - 160, rainposy + 4); // FIX
						draw_rainbow_jazz(rainposx - 192, rainposy + 8);
					}
					if (rainbow_frame > 10){
						draw_rainbow_jazz(rainposx, rainposy);
						draw_rainbow_jazz(rainposx - 32, rainposy - 4);
						draw_rainbow_jazz(rainposx - 64, rainposy);
						draw_rainbow_jazz(rainposx - 96, rainposy + 4); // FIX
						draw_rainbow_jazz(rainposx - 128, rainposy + 8);
						draw_rainbow_jazz(rainposx - 160, rainposy + 4); // FIX
						draw_rainbow_jazz(rainposx - 192, rainposy);


						rainbow_frame = 1;
					}
				}

			if (cat_selected == 13){

					if (rainbow_frame <= 5){
						draw_rainbow_tacnayn(rainposx - 20, rainposy);
						draw_rainbow_tacnayn(rainposx + 15, rainposy - 4);
						draw_rainbow_tacnayn(rainposx + 50, rainposy);
						draw_rainbow_tacnayn(rainposx + 85, rainposy + 4); // FIX
						draw_rainbow_tacnayn(rainposx + 120, rainposy + 8);
						draw_rainbow_tacnayn(rainposx + 155, rainposy + 4); // FIX
						draw_rainbow_tacnayn(rainposx + 190, rainposy);
					}
					if (rainbow_frame > 5 && rainbow_frame <= 10){
						draw_rainbow_tacnayn(rainposx - 20, rainposy + 8);
						draw_rainbow_tacnayn(rainposx + 15, rainposy + 12);
						draw_rainbow_tacnayn(rainposx + 50, rainposy + 8);
						draw_rainbow_tacnayn(rainposx + 85, rainposy + 4); // FIX
						draw_rainbow_tacnayn(rainposx + 120, rainposy);
						draw_rainbow_tacnayn(rainposx + 155, rainposy + 4); // FIX
						draw_rainbow_tacnayn(rainposx + 190, rainposy + 8);
					}
					if (rainbow_frame > 10){
						draw_rainbow_tacnayn(rainposx - 20, rainposy);
						draw_rainbow_tacnayn(rainposx + 15, rainposy - 4);
						draw_rainbow_tacnayn(rainposx + 50, rainposy);
						draw_rainbow_tacnayn(rainposx + 85, rainposy + 4); // FIX
						draw_rainbow_tacnayn(rainposx + 120, rainposy + 8);
						draw_rainbow_tacnayn(rainposx + 155, rainposy + 4); // FIX
						draw_rainbow_tacnayn(rainposx + 190, rainposy);

						rainbow_frame = 1;
					}
				}

			if (cat_selected == 15){

					if (rainbow_frame == 1){
						draw_rainbow_dub(rainposx + 15, rainposy - 2, 4, 6, 5, 2, 4, 1);
					}
					if (rainbow_frame == 2){
						draw_rainbow_dub(rainposx + 15, rainposy - 2, 4, 6, 5, 2, 4, 1);
					}
					if (rainbow_frame == 3){
						draw_rainbow_dub(rainposx + 15, rainposy - 2, 4, 1, 3, 3, 5, 2);
					}
					if (rainbow_frame == 4){
						draw_rainbow_dub(rainposx + 15, rainposy - 2, 3, 0, 4, 4, 4, 3);
					}
					if (rainbow_frame == 5){
						draw_rainbow_dub(rainposx + 15, rainposy - 2, 5, 1, 6, 3, 3, 6);
					}
					if (rainbow_frame == 6){
						draw_rainbow_dub(rainposx + 15, rainposy - 2, 1, 2, 5, 2, 2, 5);
					}
					if (rainbow_frame == 7){
						draw_rainbow_dub(rainposx + 15, rainposy - 2, 2, 3, 5, 3, 1, 6);

					}
					if (rainbow_frame == 8){
						draw_rainbow_dub(rainposx + 15, rainposy - 2, 4, 2, 4, 4, 1, 6);

					}
					if (rainbow_frame == 9){
						draw_rainbow_dub(rainposx + 15, rainposy - 2, 5, 4, 3, 3, 2, 4);

					}
					if (rainbow_frame == 10){
						draw_rainbow_dub(rainposx + 15, rainposy - 2, 6, 5, 4, 2, 3, 3);

					}
					if (rainbow_frame == 11){
						draw_rainbow_dub(rainposx + 15, rainposy - 2, 6, 3, 6, 1, 1, 2);

					}
					if (rainbow_frame == 12){
						draw_rainbow_dub(rainposx + 15, rainposy - 2, 5, 4, 3, 3, 2, 4);

					}
					if (rainbow_frame > 13){

						draw_rainbow_dub(rainposx + 15, rainposy - 2, 4, 6, 5, 2, 4, 1);
						rainbow_frame = 1;
					}


				}


				//Draw cat normal

			if (cat_selected == 1 || cat_selected == 2 || cat_selected == 3 || cat_selected == 4 || cat_selected == 5 || cat_selected == 6 || cat_selected == 7 || cat_selected == 8 || cat_selected == 9 || cat_selected == 11 || cat_selected == 12 || cat_selected == 14 || cat_selected == 15) {
					if (cat_frame == 1){
						sf2d_draw_texture_part(tex2, catposx, catposy, 0, 0, 130, 80);
					}
					if (cat_frame == 2){
						sf2d_draw_texture_part(tex2, catposx, catposy, 130, 0, 130, 80);
					}
					if (cat_frame == 3){
						sf2d_draw_texture_part(tex2, catposx, catposy, 260, 0, 130, 80);
					}
					if (cat_frame == 4){
						sf2d_draw_texture_part(tex2, catposx, catposy, 390, 0, 130, 80);
					}
					if (cat_frame == 5){
						sf2d_draw_texture_part(tex2, catposx, catposy, 520, 0, 130, 80);
					}
					if (cat_frame == 6){
						sf2d_draw_texture_part(tex2, catposx, catposy, 650, 0, 130, 80);
					}
					if (cat_frame >= 7){
						sf2d_draw_texture_part(tex2, catposx, catposy, 0, 0, 130, 80);

						cat_frame = 1;
					}
				}




			if (cat_selected == 10){

					if (cat_frame == 1){
						sf2d_draw_texture_part(tex2, catposx, catposy, 0, 0, 130, 80);
					}
					if (cat_frame == 2){
						sf2d_draw_texture_part(tex2, catposx, catposy, 0, 0, 130, 80);
					}
					if (cat_frame == 3){
						sf2d_draw_texture_part(tex2, catposx, catposy, 130, 0, 130, 80);
					}
					if (cat_frame == 4){
						sf2d_draw_texture_part(tex2, catposx, catposy, 130, 0, 130, 80);
					}
					if (cat_frame == 5){
						sf2d_draw_texture_part(tex2, catposx, catposy, 260, 0, 130, 80);
					}
					if (cat_frame == 6){
						sf2d_draw_texture_part(tex2, catposx, catposy, 260, 0, 130, 80);
					}
					if (cat_frame == 7){
						sf2d_draw_texture_part(tex2, catposx, catposy, 390, 0, 130, 80);
					}
					if (cat_frame == 8){
						sf2d_draw_texture_part(tex2, catposx, catposy, 390, 0, 130, 80);
					}
					if (cat_frame == 9){
						sf2d_draw_texture_part(tex2, catposx, catposy, 520, 0, 130, 80);
					}
					if (cat_frame == 10){
						sf2d_draw_texture_part(tex2, catposx, catposy, 520, 0, 130, 80);
					}
					if (cat_frame == 11){
						sf2d_draw_texture_part(tex2, catposx, catposy, 650, 0, 130, 80);
					}
					if (cat_frame == 12){
						sf2d_draw_texture_part(tex2, catposx, catposy, 650, 0, 130, 80);
					}
					if (cat_frame >= 13){
						sf2d_draw_texture_part(tex2, catposx, catposy, 0, 0, 130, 80);

						cat_frame = 1;
					}
				}

			if (cat_selected == 13) {
					if (cat_frame == 1){
						sf2d_draw_texture_part(tex2, catposx - 114, catposy, 0, 0, 130, 80);
					}
					if (cat_frame == 2){
						sf2d_draw_texture_part(tex2, catposx - 114, catposy, 130, 0, 130, 80);
					}
					if (cat_frame == 3){
						sf2d_draw_texture_part(tex2, catposx - 114, catposy, 260, 0, 130, 80);
					}
					if (cat_frame == 4){
						sf2d_draw_texture_part(tex2, catposx - 114, catposy, 390, 0, 130, 80);
					}
					if (cat_frame == 5){
						sf2d_draw_texture_part(tex2, catposx - 114, catposy, 520, 0, 130, 80);
					}
					if (cat_frame == 6){
						sf2d_draw_texture_part(tex2, catposx - 114, catposy, 650, 0, 130, 80);
					}
					if (cat_frame >= 7){
						sf2d_draw_texture_part(tex2, catposx - 114, catposy, 0, 0, 130, 80);

						cat_frame = 1;
					}
				}
			//ballon cloud
			if (cat_selected == 16) {
				if (star_frame <= 3){
					sf2d_draw_texture_scale(tex1, 0, -96, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 100, 0, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 250, -96, 1.6, 1.6);//nube derecha
				}
				if (star_frame <= 6 && star_frame >= 4){
					sf2d_draw_texture_scale(tex1, 0, -68, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 100, 28, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 250, -68, 1.6, 1.6);
				}
				if (star_frame <= 9 && star_frame >= 7){
					sf2d_draw_texture_scale(tex1, 0, -40, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 100, 56, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 250, -40, 1.6, 1.6);
				}
				if (star_frame <= 12 && star_frame >= 10){
					sf2d_draw_texture_scale(tex1, 0, -12, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 100, 84, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 250, -12, 1.6, 1.6);
				}
				if (star_frame <= 15 && star_frame >= 13){
					sf2d_draw_texture_scale(tex1, 0, 16, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 100, 112, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 250, 16, 1.6, 1.6);
				}
				if (star_frame <= 18 && star_frame >= 16){
					sf2d_draw_texture_scale(tex1, 0, 44, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 100, 140, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 250, 44, 1.6, 1.6);
				}
				if (star_frame <= 21 && star_frame >= 19){
					sf2d_draw_texture_scale(tex1, 0, 72, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 100, 168, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 250, 72, 1.6, 1.6);
				}
				if (star_frame <= 24 && star_frame >= 21){
					sf2d_draw_texture_scale(tex1, 0, 100, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 100, 196, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 250, 100, 1.6, 1.6);
				}
				if (star_frame <= 27 && star_frame >= 24){
					sf2d_draw_texture_scale(tex1, 0, 128, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 100, 224, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 250, 128, 1.6, 1.6);
				}
				if (star_frame <= 30 && star_frame >= 27){
					sf2d_draw_texture_scale(tex1, 0, 156, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 100, -84, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 250, 156, 1.6, 1.6);
				}
				if (star_frame <= 33 && star_frame >= 30){
					sf2d_draw_texture_scale(tex1, 0, 184, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 100, -56, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 250, 184, 1.6, 1.6);
				}
				if (star_frame <= 36 && star_frame >= 33){
					sf2d_draw_texture_scale(tex1, 0, 212, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 100, -28, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 250, 212, 1.6, 1.6);
				}
				if (star_frame >= 37){
					sf2d_draw_texture_scale(tex1, 0, 212, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 100, 0, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 250, 212, 1.6, 1.6);

					star_frame = 1;
				}
			}

			///ballon cat
			if (cat_selected == 16) {
				if (cat_frame == 1){
					sf2d_draw_texture_rotate_cut_scale(tex2, catposx - 50, catposy - 45, 0.0, 0, 0, 66, 130, 1.5, 1.5);
				}
				if (cat_frame == 2){
					sf2d_draw_texture_rotate_cut_scale(tex2, catposx - 50, catposy - 45, 0.0, 66, 0, 66, 130, 1.5, 1.5);
				}
				if (cat_frame == 3){
					sf2d_draw_texture_rotate_cut_scale(tex2, catposx - 50, catposy - 45, 0.0, 132, 0, 66, 130, 1.5, 1.5);
				}
				if (cat_frame == 4){
					sf2d_draw_texture_rotate_cut_scale(tex2, catposx - 50, catposy - 45, 0.0, 198, 0, 66, 130, 1.5, 1.5);
				}
				if (cat_frame == 5){
					sf2d_draw_texture_rotate_cut_scale(tex2, catposx - 50, catposy - 45, 0.0, 264, 0, 66, 130, 1.5, 1.5);
				}
				if (cat_frame == 6){
					sf2d_draw_texture_rotate_cut_scale(tex2, catposx - 50, catposy - 45, 0.0, 330, 0, 66, 130, 1.5, 1.5);
				}
				if (cat_frame >= 7){
					sf2d_draw_texture_rotate_cut_scale(tex2, catposx - 50, catposy - 45, 0.0, 0, 0, 66, 130, 1.5, 1.5);

					cat_frame = 1;
				}
			}



				///STARS///////////////////



			if (cat_selected == 13) {
					if (star_frame == 1){
						sf2d_draw_texture(tex1, star_pos1, star_pos2);
						sf2d_draw_texture(tex1, star_pos3, star_pos4);
						sf2d_draw_texture(tex1, star_pos5, star_pos6);
						sf2d_draw_texture(tex1, star_pos7, star_pos8);
					}

					if (star_frame == 2){
						sf2d_draw_texture(tex1, star_pos1 + 10, star_pos2);
						sf2d_draw_texture(tex1, star_pos3 + 10, star_pos4);
						sf2d_draw_texture(tex1, star_pos5 + 10, star_pos6);
						sf2d_draw_texture(tex1, star_pos7 + 10, star_pos8);
					}

					if (star_frame == 3){
						sf2d_draw_texture(tex1, star_pos1 + 20, star_pos2);
						sf2d_draw_texture(tex1, star_pos3 + 20, star_pos4);
						sf2d_draw_texture(tex1, star_pos5 + 20, star_pos6);
						sf2d_draw_texture(tex1, star_pos7 + 20, star_pos8);
					}

					if (star_frame == 4){
						sf2d_draw_texture(tex1, star_pos1 + 30, star_pos2);
						sf2d_draw_texture(tex1, star_pos3 + 30, star_pos4);
						sf2d_draw_texture(tex1, star_pos5 + 30, star_pos6);
						sf2d_draw_texture(tex1, star_pos7 + 30, star_pos8);
					}

					if (star_frame >= 5){
						star_frame = 1;
						star_pos1 = rand() % topx;
						star_pos2 = rand() % topy;
						star_pos3 = rand() % topx;
						star_pos4 = rand() % topy;
						star_pos5 = rand() % topx;
						star_pos6 = rand() % topy;
						star_pos7 = rand() % topx;
						star_pos8 = rand() % topy;
					}

				}


				//Drawnormal stars

			if (cat_selected == 1 || cat_selected == 2 || cat_selected == 3 || cat_selected == 4 || cat_selected == 5 || cat_selected == 6 || cat_selected == 7 || cat_selected == 8 || cat_selected == 9 || cat_selected == 10 || cat_selected == 11 || cat_selected == 12 || cat_selected == 14 || cat_selected == 15){
					if (star_frame == 1){

						sf2d_draw_texture_part(tex1, star_pos1, star_pos2, 0, 0, 14, 14);
						sf2d_draw_texture_part(tex1, star_pos3, star_pos3, 0, 0, 14, 14);
						sf2d_draw_texture_part(tex1, star_pos5, star_pos6, 0, 0, 14, 14);
						sf2d_draw_texture_part(tex1, star_pos7, star_pos8, 0, 0, 14, 14);
					}
					if (star_frame == 2){
						sf2d_draw_texture_part(tex1, star_pos1 - 5, star_pos2, 14, 0, 14, 14);
						sf2d_draw_texture_part(tex1, star_pos3 - 5, star_pos4, 14, 0, 14, 14);
						sf2d_draw_texture_part(tex1, star_pos5 - 5, star_pos6, 14, 0, 14, 14);
						sf2d_draw_texture_part(tex1, star_pos7 - 5, star_pos8, 14, 0, 14, 14);
					}
					if (star_frame == 3){
						sf2d_draw_texture_part(tex1, star_pos1 - 10, star_pos2, 28, 0, 14, 14);
						sf2d_draw_texture_part(tex1, star_pos3 - 10, star_pos4, 28, 0, 14, 14);
						sf2d_draw_texture_part(tex1, star_pos5 - 10, star_pos6, 28, 0, 14, 14);
						sf2d_draw_texture_part(tex1, star_pos7 - 10, star_pos8, 28, 0, 14, 14);
					}
					if (star_frame == 4){
						sf2d_draw_texture_part(tex1, star_pos1 - 15, star_pos2, 42, 0, 14, 14);
						sf2d_draw_texture_part(tex1, star_pos3 - 15, star_pos4, 42, 0, 14, 14);
						sf2d_draw_texture_part(tex1, star_pos5 - 15, star_pos6, 42, 0, 14, 14);
						sf2d_draw_texture_part(tex1, star_pos7 - 15, star_pos8, 42, 0, 14, 14);
					}
					if (star_frame >= 5){
						star_frame = 1;
						star_pos1 = rand() % topx;
						star_pos2 = rand() % topy;
						star_pos3 = rand() % topx;
						star_pos4 = rand() % topy;
						star_pos5 = rand() % topx;
						star_pos6 = rand() % topy;
						star_pos7 = rand() % topx;
						star_pos8 = rand() % topy;
					}
				}

			sf2d_end_frame();


			///Start GFX_RIGHT EYE////
			sf2d_start_frame(GFX_TOP, GFX_RIGHT);


			if (cat_selected == 1 || cat_selected == 2 || cat_selected == 6 || cat_selected == 8 || cat_selected == 9 || cat_selected == 10 || cat_selected == 14){

				if (rainbow_frame <= 5){
					draw_rainbow(rainposx - right_eye, rainposy);
					draw_rainbow((rainposx - 32) - right_eye, rainposy - 4);
					draw_rainbow((rainposx - 64) - right_eye, rainposy);
					draw_rainbow((rainposx - 96) - right_eye, rainposy + 4); // FIX
					draw_rainbow((rainposx - 128) - right_eye, rainposy + 8);
					draw_rainbow((rainposx - 160) - right_eye, rainposy + 4); // FIX
					draw_rainbow((rainposx - 192) - right_eye, rainposy);
				}
				if (rainbow_frame > 5 && rainbow_frame <= 10){
					draw_rainbow(rainposx - right_eye, rainposy + 8);
					draw_rainbow((rainposx - 32) - right_eye, rainposy + 12);
					draw_rainbow((rainposx - 64) - right_eye, rainposy + 8);
					draw_rainbow((rainposx - 96) - right_eye, rainposy + 4); // FIX
					draw_rainbow((rainposx - 128) - right_eye, rainposy);
					draw_rainbow((rainposx - 160) - right_eye, rainposy + 4); // FIX
					draw_rainbow((rainposx - 192) - right_eye, rainposy + 8);
				}
				if (rainbow_frame > 10){
					draw_rainbow(rainposx - right_eye, rainposy);
					draw_rainbow((rainposx - 32) - right_eye, rainposy - 4);
					draw_rainbow((rainposx - 64) - right_eye, rainposy);
					draw_rainbow((rainposx - 96) - right_eye, rainposy + 4); // FIX
					draw_rainbow((rainposx - 128) - right_eye, rainposy + 8);
					draw_rainbow((rainposx - 160) - right_eye, rainposy + 4); // FIX
					draw_rainbow((rainposx - 192) - right_eye, rainposy);


					rainbow_frame = 1;
				}
			}


			if (cat_selected == 3){

				if (rainbow_frame <= 5){
					draw_rainbow_gb(rainposx - right_eye, rainposy);
					draw_rainbow_gb((rainposx - 32) - right_eye, rainposy - 4);
					draw_rainbow_gb((rainposx - 64) - right_eye, rainposy);
					draw_rainbow_gb((rainposx - 96) - right_eye, rainposy + 4); // FIX
					draw_rainbow_gb((rainposx - 128) - right_eye, rainposy + 8);
					draw_rainbow_gb((rainposx - 160) - right_eye, rainposy + 4); // FIX
					draw_rainbow_gb((rainposx - 192) - right_eye, rainposy);
				}
				if (rainbow_frame > 5 && rainbow_frame <= 10){
					draw_rainbow_gb(rainposx - right_eye, rainposy + 8);
					draw_rainbow_gb((rainposx - 32) - right_eye, rainposy + 12);
					draw_rainbow_gb((rainposx - 64) - right_eye, rainposy + 8);
					draw_rainbow_gb((rainposx - 96) - right_eye, rainposy + 4); // FIX
					draw_rainbow_gb((rainposx - 128) - right_eye, rainposy);
					draw_rainbow_gb((rainposx - 160) - right_eye, rainposy + 4); // FIX
					draw_rainbow_gb((rainposx - 192) - right_eye, rainposy + 8);
				}
				if (rainbow_frame > 10){
					draw_rainbow_gb(rainposx - right_eye, rainposy);
					draw_rainbow_gb((rainposx - 32) - right_eye, rainposy - 4);
					draw_rainbow_gb((rainposx - 64) - right_eye, rainposy);
					draw_rainbow_gb((rainposx - 96) - right_eye, rainposy + 4); // FIX
					draw_rainbow_gb((rainposx - 128) - right_eye, rainposy + 8);
					draw_rainbow_gb((rainposx - 160) - right_eye, rainposy + 4); // FIX
					draw_rainbow_gb((rainposx - 192) - right_eye, rainposy);


					rainbow_frame = 1;
				}
			}

			if (cat_selected == 4){

				if (rainbow_frame <= 5){
					draw_rainbow_jamaicnyan(rainposx - right_eye, rainposy);
					draw_rainbow_jamaicnyan((rainposx - 32) - right_eye, rainposy - 4);
					draw_rainbow_jamaicnyan((rainposx - 64) - right_eye, rainposy);
					draw_rainbow_jamaicnyan((rainposx - 96) - right_eye, rainposy + 4); // FIX
					draw_rainbow_jamaicnyan((rainposx - 128) - right_eye, rainposy + 8);
					draw_rainbow_jamaicnyan((rainposx - 160) - right_eye, rainposy + 4); // FIX
					draw_rainbow_jamaicnyan((rainposx - 192), rainposy);
				}
				if (rainbow_frame > 5 && rainbow_frame <= 10){
					draw_rainbow_jamaicnyan(rainposx - right_eye, rainposy + 8);
					draw_rainbow_jamaicnyan((rainposx - 32) - right_eye, rainposy + 12);
					draw_rainbow_jamaicnyan((rainposx - 64) - right_eye, rainposy + 8);
					draw_rainbow_jamaicnyan((rainposx - 96) - right_eye, rainposy + 4); // FIX
					draw_rainbow_jamaicnyan((rainposx - 128) - right_eye, rainposy);
					draw_rainbow_jamaicnyan((rainposx - 160) - right_eye, rainposy + 4); // FIX
					draw_rainbow_jamaicnyan((rainposx - 192) - right_eye, rainposy + 8);
				}
				if (rainbow_frame > 10){
					draw_rainbow_jamaicnyan(rainposx - right_eye, rainposy);
					draw_rainbow_jamaicnyan((rainposx - 32) - right_eye, rainposy - 4);
					draw_rainbow_jamaicnyan((rainposx - 64) - right_eye, rainposy);
					draw_rainbow_jamaicnyan((rainposx - 96) - right_eye, rainposy + 4); // FIX
					draw_rainbow_jamaicnyan((rainposx - 128) - right_eye, rainposy + 8);
					draw_rainbow_jamaicnyan((rainposx - 160) - right_eye, rainposy + 4); // FIX
					draw_rainbow_jamaicnyan((rainposx - 192) - right_eye, rainposy);


					rainbow_frame = 1;
				}
			}

			if (cat_selected == 7){

				if (rainbow_frame <= 5){
					draw_rainbow_america(rainposx - right_eye, rainposy);
					draw_rainbow_america((rainposx - 32) - right_eye, rainposy - 4);
					draw_rainbow_america((rainposx - 64) - right_eye, rainposy);
					draw_rainbow_america((rainposx - 96) - right_eye, rainposy + 4); // FIX
					draw_rainbow_america((rainposx - 128) - right_eye, rainposy + 8);
					draw_rainbow_america((rainposx - 160) - right_eye, rainposy + 4); // FIX
					draw_rainbow_america((rainposx - 192) - right_eye, rainposy);
				}
				if (rainbow_frame > 5 && rainbow_frame <= 10){
					draw_rainbow_america(rainposx - right_eye, rainposy + 8);
					draw_rainbow_america((rainposx - 32) - right_eye, rainposy + 12);
					draw_rainbow_america((rainposx - 64) - right_eye, rainposy + 8);
					draw_rainbow_america((rainposx - 96) - right_eye, rainposy + 4); // FIX
					draw_rainbow_america((rainposx - 128) - right_eye, rainposy);
					draw_rainbow_america((rainposx - 160) - right_eye, rainposy + 4); // FIX
					draw_rainbow_america((rainposx - 192) - right_eye, rainposy + 8);
				}
				if (rainbow_frame > 10){
					draw_rainbow_america(rainposx - right_eye, rainposy);
					draw_rainbow_america((rainposx - 32) - right_eye, rainposy - 4);
					draw_rainbow_america((rainposx - 64) - right_eye, rainposy);
					draw_rainbow_america((rainposx - 96) - right_eye, rainposy + 4); // FIX
					draw_rainbow_america((rainposx - 128) - right_eye, rainposy + 8);
					draw_rainbow_america((rainposx - 160) - right_eye, rainposy + 4); // FIX
					draw_rainbow_america((rainposx - 192) - right_eye, rainposy);


					rainbow_frame = 1;
				}
			}

			if (cat_selected == 5){

				if (rainbow_frame <= 5){
					draw_rainbow_pikanyan(rainposx - right_eye, rainposy);
					draw_rainbow_pikanyan((rainposx - 32) - right_eye, rainposy - 4);
					draw_rainbow_pikanyan((rainposx - 64) - right_eye, rainposy);
					draw_rainbow_pikanyan((rainposx - 96) - right_eye, rainposy + 4); // FIX
					draw_rainbow_pikanyan((rainposx - 128) - right_eye, rainposy + 8);
					draw_rainbow_pikanyan((rainposx - 160) - right_eye, rainposy + 4); // FIX
					draw_rainbow_pikanyan((rainposx - 192) - right_eye, rainposy);
				}
				if (rainbow_frame > 5 && rainbow_frame <= 10){
					draw_rainbow_pikanyan(rainposx - right_eye, rainposy + 8);
					draw_rainbow_pikanyan((rainposx - 32) - right_eye, rainposy + 12);
					draw_rainbow_pikanyan((rainposx - 64) - right_eye, rainposy + 8);
					draw_rainbow_pikanyan((rainposx - 96) - right_eye, rainposy + 4); // FIX
					draw_rainbow_pikanyan((rainposx - 128) - right_eye, rainposy);
					draw_rainbow_pikanyan((rainposx - 160) - right_eye, rainposy + 4); // FIX
					draw_rainbow_pikanyan((rainposx - 192) - right_eye, rainposy + 8);
				}
				if (rainbow_frame > 10){
					draw_rainbow_pikanyan(rainposx - right_eye, rainposy);
					draw_rainbow_pikanyan((rainposx - 32) - right_eye, rainposy - 4);
					draw_rainbow_pikanyan((rainposx - 64) - right_eye, rainposy);
					draw_rainbow_pikanyan((rainposx - 96) - right_eye, rainposy + 4); // FIX
					draw_rainbow_pikanyan((rainposx - 128) - right_eye, rainposy + 8);
					draw_rainbow_pikanyan((rainposx - 160) - right_eye, rainposy + 4); // FIX
					draw_rainbow_pikanyan((rainposx - 192) - right_eye, rainposy);


					rainbow_frame = 1;
				}
			}

			if (cat_selected == 12){

				if (rainbow_frame <= 5){
					draw_rainbow_jazz(rainposx - right_eye, rainposy);
					draw_rainbow_jazz((rainposx - 32) - right_eye, rainposy - 4);
					draw_rainbow_jazz((rainposx - 64) - right_eye, rainposy);
					draw_rainbow_jazz((rainposx - 96) - right_eye, rainposy + 4); // FIX
					draw_rainbow_jazz((rainposx - 128) - right_eye, rainposy + 8);
					draw_rainbow_jazz((rainposx - 160) - right_eye, rainposy + 4); // FIX
					draw_rainbow_jazz((rainposx - 192) - right_eye, rainposy);
				}
				if (rainbow_frame > 5 && rainbow_frame <= 10){
					draw_rainbow_jazz(rainposx - right_eye, rainposy + 8);
					draw_rainbow_jazz((rainposx - 32) - right_eye, rainposy + 12);
					draw_rainbow_jazz((rainposx - 64) - right_eye, rainposy + 8);
					draw_rainbow_jazz((rainposx - 96) - right_eye, rainposy + 4); // FIX
					draw_rainbow_jazz((rainposx - 128) - right_eye, rainposy);
					draw_rainbow_jazz((rainposx - 160) - right_eye, rainposy + 4); // FIX
					draw_rainbow_jazz((rainposx - 192) - right_eye, rainposy + 8);
				}
				if (rainbow_frame > 10){
					draw_rainbow_jazz(rainposx - right_eye, rainposy);
					draw_rainbow_jazz((rainposx - 32) - right_eye, rainposy - 4);
					draw_rainbow_jazz((rainposx - 64) - right_eye, rainposy);
					draw_rainbow_jazz((rainposx - 96) - right_eye, rainposy + 4); // FIX
					draw_rainbow_jazz((rainposx - 128) - right_eye, rainposy + 8);
					draw_rainbow_jazz((rainposx - 160) - right_eye, rainposy + 4); // FIX
					draw_rainbow_jazz((rainposx - 192) - right_eye, rainposy);


					rainbow_frame = 1;
				}
			}

			if (cat_selected == 13){

				if (rainbow_frame <= 5){
					draw_rainbow_tacnayn((rainposx - 20) - right_eye, rainposy);
					draw_rainbow_tacnayn((rainposx + 15) - right_eye, rainposy - 4);
					draw_rainbow_tacnayn((rainposx + 50) - right_eye, rainposy);
					draw_rainbow_tacnayn((rainposx + 85) - right_eye, rainposy + 4); // FIX
					draw_rainbow_tacnayn((rainposx + 120) - right_eye, rainposy + 8);
					draw_rainbow_tacnayn((rainposx + 155) - right_eye, rainposy + 4); // FIX
					draw_rainbow_tacnayn((rainposx + 190) - right_eye, rainposy);
				}
				if (rainbow_frame > 5 && rainbow_frame <= 10){
					draw_rainbow_tacnayn((rainposx - 20) - right_eye, rainposy + 8);
					draw_rainbow_tacnayn((rainposx + 15) - right_eye, rainposy + 12);
					draw_rainbow_tacnayn((rainposx + 50) - right_eye, rainposy + 8);
					draw_rainbow_tacnayn((rainposx + 85) - right_eye, rainposy + 4); // FIX
					draw_rainbow_tacnayn((rainposx + 120) - right_eye, rainposy);
					draw_rainbow_tacnayn((rainposx + 155) - right_eye, rainposy + 4); // FIX
					draw_rainbow_tacnayn((rainposx + 190) - right_eye, rainposy + 8);
				}
				if (rainbow_frame > 10){
					draw_rainbow_tacnayn((rainposx - 20) - right_eye, rainposy);
					draw_rainbow_tacnayn((rainposx + 15) - right_eye, rainposy - 4);
					draw_rainbow_tacnayn((rainposx + 50) - right_eye, rainposy);
					draw_rainbow_tacnayn((rainposx + 85) - right_eye, rainposy + 4); // FIX
					draw_rainbow_tacnayn((rainposx + 120) - right_eye, rainposy + 8);
					draw_rainbow_tacnayn((rainposx + 155) - right_eye, rainposy + 4); // FIX
					draw_rainbow_tacnayn((rainposx + 190) - right_eye, rainposy);

					rainbow_frame = 1;
				}
			}

			if (cat_selected == 15){

				if (rainbow_frame == 1){
					draw_rainbow_dub((rainposx + 15) - right_eye, rainposy - 2, 4, 6, 5, 2, 4, 1);
				}
				if (rainbow_frame == 2){
					draw_rainbow_dub((rainposx + 15) - right_eye, rainposy - 2, 4, 6, 5, 2, 4, 1);
				}
				if (rainbow_frame == 3){
					draw_rainbow_dub((rainposx + 15) - right_eye, rainposy - 2, 4, 1, 3, 3, 5, 2);
				}
				if (rainbow_frame == 4){
					draw_rainbow_dub((rainposx + 15) - right_eye, rainposy - 2, 3, 0, 4, 4, 4, 3);
				}
				if (rainbow_frame == 5){
					draw_rainbow_dub((rainposx + 15) - right_eye, rainposy - 2, 5, 1, 6, 3, 3, 6);
				}
				if (rainbow_frame == 6){
					draw_rainbow_dub((rainposx + 15) - right_eye, rainposy - 2, 1, 2, 5, 2, 2, 5);
				}
				if (rainbow_frame == 7){
					draw_rainbow_dub((rainposx + 15) - right_eye, rainposy - 2, 2, 3, 5, 3, 1, 6);

				}
				if (rainbow_frame == 8){
					draw_rainbow_dub((rainposx + 15) - right_eye, rainposy - 2, 4, 2, 4, 4, 1, 6);

				}
				if (rainbow_frame == 9){
					draw_rainbow_dub((rainposx + 15) - right_eye, rainposy - 2, 5, 4, 3, 3, 2, 4);

				}
				if (rainbow_frame == 10){
					draw_rainbow_dub((rainposx + 15) - right_eye, rainposy - 2, 6, 5, 4, 2, 3, 3);

				}
				if (rainbow_frame == 11){
					draw_rainbow_dub((rainposx + 15) - right_eye, rainposy - 2, 6, 3, 6, 1, 1, 2);

				}
				if (rainbow_frame == 12){
					draw_rainbow_dub((rainposx + 15) - right_eye, rainposy - 2, 5, 4, 3, 3, 2, 4);

				}
				if (rainbow_frame >= 13){

					draw_rainbow_dub((rainposx + 15) - right_eye, rainposy - 2, 4, 6, 5, 2, 4, 1);
					rainbow_frame = 1;
				}


			}




			if (cat_selected == 10){
				if (cat_frame == 1){
					sf2d_draw_texture_part(tex2, catposx - right_eye, catposy, 0, 0, 130, 80);
				}
				if (cat_frame == 2){
					sf2d_draw_texture_part(tex2, catposx - right_eye, catposy, 0, 0, 130, 80);
				}
				if (cat_frame == 3){
					sf2d_draw_texture_part(tex2, catposx - right_eye, catposy, 130, 0, 130, 80);
				}
				if (cat_frame == 4){
					sf2d_draw_texture_part(tex2, catposx - right_eye, catposy, 130, 0, 130, 80);
				}
				if (cat_frame == 5){
					sf2d_draw_texture_part(tex2, catposx - right_eye, catposy, 260, 0, 130, 80);
				}
				if (cat_frame == 6){
					sf2d_draw_texture_part(tex2, catposx - right_eye, catposy, 260, 0, 130, 80);
				}
				if (cat_frame == 7){
					sf2d_draw_texture_part(tex2, catposx - right_eye, catposy, 390, 0, 130, 80);
				}
				if (cat_frame == 8){
					sf2d_draw_texture_part(tex2, catposx - right_eye, catposy, 390, 0, 130, 80);
				}
				if (cat_frame == 9){
					sf2d_draw_texture_part(tex2, catposx - right_eye, catposy, 520, 0, 130, 80);
				}
				if (cat_frame == 10){
					sf2d_draw_texture_part(tex2, catposx - right_eye, catposy, 520, 0, 130, 80);
				}
				if (cat_frame == 11){
					sf2d_draw_texture_part(tex2, catposx - right_eye, catposy, 650, 0, 130, 80);
				}
				if (cat_frame == 12){
					sf2d_draw_texture_part(tex2, catposx - right_eye, catposy, 650, 0, 130, 80);
				}
				if (cat_frame >= 13){
					sf2d_draw_texture_part(tex2, catposx - right_eye, catposy, 0, 0, 130, 80);

					cat_frame = 1;
				}
			}

			if (cat_selected == 13) {
				if (cat_frame == 1){
					sf2d_draw_texture_part(tex2, (catposx - 114) - right_eye, catposy, 0, 0, 130, 80);
				}
				if (cat_frame == 2){
					sf2d_draw_texture_part(tex2, (catposx - 114) - right_eye, catposy, 130, 0, 130, 80);
				}
				if (cat_frame == 3){
					sf2d_draw_texture_part(tex2, (catposx - 114) - right_eye, catposy, 260, 0, 130, 80);
				}
				if (cat_frame == 4){
					sf2d_draw_texture_part(tex2, (catposx - 114) - right_eye, catposy, 390, 0, 130, 80);
				}
				if (cat_frame == 5){
					sf2d_draw_texture_part(tex2, (catposx - 114) - right_eye, catposy, 520, 0, 130, 80);
				}
				if (cat_frame == 6){
					sf2d_draw_texture_part(tex2, (catposx - 114) - right_eye, catposy, 650, 0, 130, 80);
				}
				if (cat_frame >= 7){
					sf2d_draw_texture_part(tex2, (catposx - 114) - right_eye, catposy, 0, 0, 130, 80);

					cat_frame = 1;
				}
			}

			//ballon cloud
			if (cat_selected == 16) {
				if (star_frame <= 3){
					sf2d_draw_texture_scale(tex1, 0, -96, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 100, 0, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 250, -96, 1.6, 1.6);//nube derecha
				}
				if (star_frame <= 6 && star_frame >= 4){
					sf2d_draw_texture_scale(tex1, 0, -68, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 100, 28, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 250, -68, 1.6, 1.6);
				}
				if (star_frame <= 9 && star_frame >= 7){
					sf2d_draw_texture_scale(tex1, 0, -40, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 100, 56, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 250, -40, 1.6, 1.6);
				}
				if (star_frame <= 12 && star_frame >= 10){
					sf2d_draw_texture_scale(tex1, 0, -12, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 100, 84, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 250, -12, 1.6, 1.6);
				}
				if (star_frame <= 15 && star_frame >= 13){
					sf2d_draw_texture_scale(tex1, 0, 16, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 100, 112, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 250, 16, 1.6, 1.6);
				}
				if (star_frame <= 18 && star_frame >= 16){
					sf2d_draw_texture_scale(tex1, 0, 44, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 100, 140, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 250, 44, 1.6, 1.6);
				}
				if (star_frame <= 21 && star_frame >= 19){
					sf2d_draw_texture_scale(tex1, 0, 72, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 100, 168, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 250, 72, 1.6, 1.6);
				}
				if (star_frame <= 24 && star_frame >= 21){
					sf2d_draw_texture_scale(tex1, 0, 100, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 100, 196, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 250, 100, 1.6, 1.6);
				}
				if (star_frame <= 27 && star_frame >= 24){
					sf2d_draw_texture_scale(tex1, 0, 128, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 100, 224, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 250, 128, 1.6, 1.6);
				}
				if (star_frame <= 30 && star_frame >= 27){
					sf2d_draw_texture_scale(tex1, 0, 156, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 100, -84, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 250, 156, 1.6, 1.6);
				}
				if (star_frame <= 33 && star_frame >= 30){
					sf2d_draw_texture_scale(tex1, 0, 184, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 100, -56, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 250, 184, 1.6, 1.6);
				}
				if (star_frame <= 36 && star_frame >= 33){
					sf2d_draw_texture_scale(tex1, 0, 212, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 100, -28, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 250, 212, 1.6, 1.6);
				}
				if (star_frame >= 37){
					sf2d_draw_texture_scale(tex1, 0, 212, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 100, 0, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 250, 212, 1.6, 1.6);

					star_frame = 1;
				}
			}

			///ballon cat
			if (cat_selected == 16) {
				if (cat_frame == 1){
					sf2d_draw_texture_rotate_cut_scale(tex2, (catposx - 50) - right_eye, catposy - 45, 0.0, 0, 0, 66, 130, 1.5, 1.5);
				}
				if (cat_frame == 2){
					sf2d_draw_texture_rotate_cut_scale(tex2, (catposx - 50) - right_eye, catposy - 45, 0.0, 66, 0, 66, 130, 1.5, 1.5);
				}
				if (cat_frame == 3){
					sf2d_draw_texture_rotate_cut_scale(tex2, (catposx - 50) - right_eye, catposy - 45, 0.0, 132, 0, 66, 130, 1.5, 1.5);
				}
				if (cat_frame == 4){
					sf2d_draw_texture_rotate_cut_scale(tex2, (catposx - 50) - right_eye, catposy - 45, 0.0, 198, 0, 66, 130, 1.5, 1.5);
				}
				if (cat_frame == 5){
					sf2d_draw_texture_rotate_cut_scale(tex2, (catposx - 50) - right_eye, catposy - 45, 0.0, 264, 0, 66, 130, 1.5, 1.5);
				}
				if (cat_frame == 6){
					sf2d_draw_texture_rotate_cut_scale(tex2, (catposx - 50) - right_eye, catposy - 45, 0.0, 330, 0, 66, 130, 1.5, 1.5);
				}
				if (cat_frame >= 7){
					sf2d_draw_texture_rotate_cut_scale(tex2, (catposx - 50) - right_eye, catposy - 45, 0.0, 0, 0, 66, 130, 1.5, 1.5);

					cat_frame = 1;
				}
			}

			//Draw cat normal

			if (cat_selected == 1 || cat_selected == 2 || cat_selected == 3 || cat_selected == 4 || cat_selected == 5 || cat_selected == 6 || cat_selected == 7 || cat_selected == 8 || cat_selected == 9 || cat_selected == 11 || cat_selected == 12 || cat_selected == 14 || cat_selected == 15) {
				if (cat_frame == 1){
					sf2d_draw_texture_part(tex2, catposx - right_eye, catposy, 0, 0, 130, 80);
				}
				if (cat_frame == 2){
					sf2d_draw_texture_part(tex2, catposx - right_eye, catposy, 130, 0, 130, 80);
				}
				if (cat_frame == 3){
					sf2d_draw_texture_part(tex2, catposx - right_eye, catposy, 260, 0, 130, 80);
				}
				if (cat_frame == 4){
					sf2d_draw_texture_part(tex2, catposx - right_eye, catposy, 390, 0, 130, 80);
				}
				if (cat_frame == 5){
					sf2d_draw_texture_part(tex2, catposx - right_eye, catposy, 520, 0, 130, 80);
				}
				if (cat_frame == 6){
					sf2d_draw_texture_part(tex2, catposx - right_eye, catposy, 650, 0, 130, 80);
				}
				if (cat_frame >= 7){
					sf2d_draw_texture_part(tex2, catposx - right_eye, catposy, 0, 0, 130, 80);

					cat_frame = 1;
				}
			}




			///stars


			if (cat_selected == 13) {
				if (star_frame == 1){
					sf2d_draw_texture(tex1, star_pos1, star_pos2);
					sf2d_draw_texture(tex1, star_pos3, star_pos4);
					sf2d_draw_texture(tex1, star_pos5, star_pos6);
					sf2d_draw_texture(tex1, star_pos7, star_pos8);
				}

				if (star_frame == 2){
					sf2d_draw_texture(tex1, star_pos1 + 10, star_pos2);
					sf2d_draw_texture(tex1, star_pos3 + 10, star_pos4);
					sf2d_draw_texture(tex1, star_pos5 + 10, star_pos6);
					sf2d_draw_texture(tex1, star_pos7 + 10, star_pos8);
				}

				if (star_frame == 3){
					sf2d_draw_texture(tex1, star_pos1 + 20, star_pos2);
					sf2d_draw_texture(tex1, star_pos3 + 20, star_pos4);
					sf2d_draw_texture(tex1, star_pos5 + 20, star_pos6);
					sf2d_draw_texture(tex1, star_pos7 + 20, star_pos8);
				}

				if (star_frame == 4){
					sf2d_draw_texture(tex1, star_pos1 + 30, star_pos2);
					sf2d_draw_texture(tex1, star_pos3 + 30, star_pos4);
					sf2d_draw_texture(tex1, star_pos5 + 30, star_pos6);
					sf2d_draw_texture(tex1, star_pos7 + 30, star_pos8);
				}

				if (star_frame >= 5){
					star_frame = 1;
					star_pos1 = rand() % topx;
					star_pos2 = rand() % topy;
					star_pos3 = rand() % topx;
					star_pos4 = rand() % topy;
					star_pos5 = rand() % topx;
					star_pos6 = rand() % topy;
					star_pos7 = rand() % topx;
					star_pos8 = rand() % topy;
				}

			}


			//Drawnormal stars

			if (cat_selected == 1 || cat_selected == 2 || cat_selected == 3 || cat_selected == 4 || cat_selected == 5 || cat_selected == 6 || cat_selected == 7 || cat_selected == 8 || cat_selected == 9 || cat_selected == 10 || cat_selected == 11 || cat_selected == 12 || cat_selected == 14 || cat_selected == 15){
				if (star_frame == 1){

					sf2d_draw_texture_part(tex1, star_pos1, star_pos2, 0, 0, 14, 14);
					sf2d_draw_texture_part(tex1, star_pos3, star_pos3, 0, 0, 14, 14);
					sf2d_draw_texture_part(tex1, star_pos5, star_pos6, 0, 0, 14, 14);
					sf2d_draw_texture_part(tex1, star_pos7, star_pos8, 0, 0, 14, 14);
				}
				if (star_frame == 2){
					sf2d_draw_texture_part(tex1, star_pos1 - 5, star_pos2, 14, 0, 14, 14);
					sf2d_draw_texture_part(tex1, star_pos3 - 5, star_pos4, 14, 0, 14, 14);
					sf2d_draw_texture_part(tex1, star_pos5 - 5, star_pos6, 14, 0, 14, 14);
					sf2d_draw_texture_part(tex1, star_pos7 - 5, star_pos8, 14, 0, 14, 14);
				}
				if (star_frame == 3){
					sf2d_draw_texture_part(tex1, star_pos1 - 10, star_pos2, 28, 0, 14, 14);
					sf2d_draw_texture_part(tex1, star_pos3 - 10, star_pos4, 28, 0, 14, 14);
					sf2d_draw_texture_part(tex1, star_pos5 - 10, star_pos6, 28, 0, 14, 14);
					sf2d_draw_texture_part(tex1, star_pos7 - 10, star_pos8, 28, 0, 14, 14);
				}
				if (star_frame == 4){
					sf2d_draw_texture_part(tex1, star_pos1 - 15, star_pos2, 42, 0, 14, 14);
					sf2d_draw_texture_part(tex1, star_pos3 - 15, star_pos4, 42, 0, 14, 14);
					sf2d_draw_texture_part(tex1, star_pos5 - 15, star_pos6, 42, 0, 14, 14);
					sf2d_draw_texture_part(tex1, star_pos7 - 15, star_pos8, 42, 0, 14, 14);
				}
				if (star_frame >= 5){
					star_frame = 1;
					star_pos1 = rand() % topx;
					star_pos2 = rand() % topy;
					star_pos3 = rand() % topx;
					star_pos4 = rand() % topy;
					star_pos5 = rand() % topx;
					star_pos6 = rand() % topy;
					star_pos7 = rand() % topx;
					star_pos8 = rand() % topy;
				}
			}
			sf2d_end_frame();


			///BOTTOM SCREEN
			sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			///stars
			if (cat_selected == 16) {
				if (star_frame <= 3){
					sf2d_draw_texture_scale(tex1, -40, -96, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 60, 0, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 210, -96, 1.6, 1.6);//nube derecha
				}
				if (star_frame <= 6 && star_frame >= 4){
					sf2d_draw_texture_scale(tex1, -40, -68, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 60, 28, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 210, -68, 1.6, 1.6);
				}
				if (star_frame <= 9 && star_frame >= 7){
					sf2d_draw_texture_scale(tex1, -40, -40, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 60, 56, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 210, -40, 1.6, 1.6);
				}
				if (star_frame <= 12 && star_frame >= 10){
					sf2d_draw_texture_scale(tex1, -40, -12, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 60, 84, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 210, -12, 1.6, 1.6);
				}
				if (star_frame <= 15 && star_frame >= 13){
					sf2d_draw_texture_scale(tex1, -40, 16, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 60, 112, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 210, 16, 1.6, 1.6);
				}
				if (star_frame <= 18 && star_frame >= 16){
					sf2d_draw_texture_scale(tex1, -40, 44, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 60, 140, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 210, 44, 1.6, 1.6);
				}
				if (star_frame <= 21 && star_frame >= 19){
					sf2d_draw_texture_scale(tex1, -40, 72, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 60, 168, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 210, 72, 1.6, 1.6);
				}
				if (star_frame <= 24 && star_frame >= 21){
					sf2d_draw_texture_scale(tex1, -40, 100, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 60, 196, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 210, 100, 1.6, 1.6);
				}
				if (star_frame <= 27 && star_frame >= 24){
					sf2d_draw_texture_scale(tex1, -40, 128, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 60, 224, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 210, 128, 1.6, 1.6);
				}
				if (star_frame <= 30 && star_frame >= 27){
					sf2d_draw_texture_scale(tex1, -40, 156, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 60, -84, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 210, 156, 1.6, 1.6);
				}
				if (star_frame <= 33 && star_frame >= 30){
					sf2d_draw_texture_scale(tex1, -40, 184, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 60, -56, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 210, 184, 1.6, 1.6);
				}
				if (star_frame <= 36 && star_frame >= 33){
					sf2d_draw_texture_scale(tex1, -40, 212, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 60, -28, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 210, 212, 1.6, 1.6);
				}
				if (star_frame >= 37){
					sf2d_draw_texture_scale(tex1, -40, 212, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 60, 0, 1.6, 1.6);
					sf2d_draw_texture_scale(tex1, 210, 212, 1.6, 1.6);

					star_frame = 1;
				}
			}
				if (cat_selected == 13) {
					if (star_frame == 1){
						sf2d_draw_texture(tex1, star_pos1, star_pos2);
						sf2d_draw_texture(tex1, star_pos3, star_pos4);
						sf2d_draw_texture(tex1, star_pos5, star_pos6);
						sf2d_draw_texture(tex1, star_pos7, star_pos8);
					}

					if (star_frame == 2){
						sf2d_draw_texture(tex1, star_pos1 + 10, star_pos2);
						sf2d_draw_texture(tex1, star_pos3 + 10, star_pos4);
						sf2d_draw_texture(tex1, star_pos5 + 10, star_pos6);
						sf2d_draw_texture(tex1, star_pos7 + 10, star_pos8);
					}

					if (star_frame == 3){
						sf2d_draw_texture(tex1, star_pos1 + 20, star_pos2);
						sf2d_draw_texture(tex1, star_pos3 + 20, star_pos4);
						sf2d_draw_texture(tex1, star_pos5 + 20, star_pos6);
						sf2d_draw_texture(tex1, star_pos7 + 20, star_pos8);
					}

					if (star_frame == 4){
						sf2d_draw_texture(tex1, star_pos1 + 30, star_pos2);
						sf2d_draw_texture(tex1, star_pos3 + 30, star_pos4);
						sf2d_draw_texture(tex1, star_pos5 + 30, star_pos6);
						sf2d_draw_texture(tex1, star_pos7 + 30, star_pos8);
					}

					if (star_frame >= 5){
						star_frame = 1;
						star_pos1 = rand() % topx;
						star_pos2 = rand() % topy;
						star_pos3 = rand() % topx;
						star_pos4 = rand() % topy;
						star_pos5 = rand() % topx;
						star_pos6 = rand() % topy;
						star_pos7 = rand() % topx;
						star_pos8 = rand() % topy;
					}

				}


				//Drawnormal stars

				if (cat_selected == 1 || cat_selected == 2 || cat_selected == 3 || cat_selected == 4 || cat_selected == 5 || cat_selected == 6 || cat_selected == 7 || cat_selected == 8 || cat_selected == 9 || cat_selected == 10 || cat_selected == 11 || cat_selected == 12 || cat_selected == 14 || cat_selected == 15){
					if (star_frame == 1){

						sf2d_draw_texture_part(tex1, star_pos1, star_pos2, 0, 0, 14, 14);
						sf2d_draw_texture_part(tex1, star_pos3, star_pos3, 0, 0, 14, 14);
						sf2d_draw_texture_part(tex1, star_pos5, star_pos6, 0, 0, 14, 14);
						sf2d_draw_texture_part(tex1, star_pos7, star_pos8, 0, 0, 14, 14);
					}
					if (star_frame == 2){
						sf2d_draw_texture_part(tex1, star_pos1 - 5, star_pos2, 14, 0, 14, 14);
						sf2d_draw_texture_part(tex1, star_pos3 - 5, star_pos4, 14, 0, 14, 14);
						sf2d_draw_texture_part(tex1, star_pos5 - 5, star_pos6, 14, 0, 14, 14);
						sf2d_draw_texture_part(tex1, star_pos7 - 5, star_pos8, 14, 0, 14, 14);
					}
					if (star_frame == 3){
						sf2d_draw_texture_part(tex1, star_pos1 - 10, star_pos2, 28, 0, 14, 14);
						sf2d_draw_texture_part(tex1, star_pos3 - 10, star_pos4, 28, 0, 14, 14);
						sf2d_draw_texture_part(tex1, star_pos5 - 10, star_pos6, 28, 0, 14, 14);
						sf2d_draw_texture_part(tex1, star_pos7 - 10, star_pos8, 28, 0, 14, 14);
					}
					if (star_frame == 4){
						sf2d_draw_texture_part(tex1, star_pos1 - 15, star_pos2, 42, 0, 14, 14);
						sf2d_draw_texture_part(tex1, star_pos3 - 15, star_pos4, 42, 0, 14, 14);
						sf2d_draw_texture_part(tex1, star_pos5 - 15, star_pos6, 42, 0, 14, 14);
						sf2d_draw_texture_part(tex1, star_pos7 - 15, star_pos8, 42, 0, 14, 14);
					}
					if (star_frame >= 5){
						star_frame = 1;
						star_pos1 = rand() % topx;
						star_pos2 = rand() % topy;
						star_pos3 = rand() % topx;
						star_pos4 = rand() % topy;
						star_pos5 = rand() % topx;
						star_pos6 = rand() % topy;
						star_pos7 = rand() % topx;
						star_pos8 = rand() % topy;
					}
				}





				if (counter_frame < 100){
					draw_number(font, counter_frame % 10, numx + 5, numy);
					sf2d_draw_texture_part(font, numx, numy, 308, 0, 22, 27); //point and center
					draw_number(font, counter_frame / 10 % 10, numx - 17, numy);
				}
				else if (counter_frame < 1000){
					draw_number(font, counter_frame % 10, numx + 23, numy);
					sf2d_draw_texture_part(font, numx + 17, numy, 308, 0, 22, 27); //point
					draw_number(font, counter_frame / 10 % 10, numx, numy);//center
					draw_number(font, counter_frame / 100 % 10, numx - 17, numy);
				}
				else if (counter_frame < 10000){
					draw_number(font, counter_frame % 10, numx + 23, numy);
					sf2d_draw_texture_part(font, numx + 17, numy, 308, 0, 22, 27); //point
					draw_number(font, counter_frame / 10 % 10, numx, numy);//center
					draw_number(font, counter_frame / 100 % 10, numx - 17, numy);
					draw_number(font, counter_frame / 1000 % 10, numx - 34, numy);
				}
				else if (counter_frame < 100000){
					draw_number(font, counter_frame % 10, numx + 39, numy);
					sf2d_draw_texture_part(font, numx + 34, numy, 308, 0, 22, 27); //point
					draw_number(font, counter_frame / 10 % 10, numx + 17, numy);
					draw_number(font, counter_frame / 100 % 10, numx, numy);//center
					draw_number(font, counter_frame / 1000 % 10, numx - 17, numy);
					draw_number(font, counter_frame / 10000 % 10, numx - 34, numy);
				}
				else if (counter_frame < 1000000){
					draw_number(font, counter_frame % 10, numx + 39, numy);
					sf2d_draw_texture_part(font, numx + 34, numy, 308, 0, 22, 27); //point
					draw_number(font, counter_frame / 10 % 10, numx + 17, numy);
					draw_number(font, counter_frame / 100 % 10, numx, numy);//center
					draw_number(font, counter_frame / 1000 % 10, numx - 17, numy);
					draw_number(font, counter_frame / 10000 % 10, numx - 34, numy);
					draw_number(font, counter_frame / 100000 % 10, numx - 51, numy);
				}
				else if (counter_frame < 10000000){
					draw_number(font, counter_frame % 10, numx + 56, numy);
					sf2d_draw_texture_part(font, numx + 51, numy, 308, 0, 22, 27); //point
					draw_number(font, counter_frame / 10 % 10, numx + 34, numy);
					draw_number(font, counter_frame / 100 % 10, numx + 17, numy);
					draw_number(font, counter_frame / 1000 % 10, numx, numy);//center
					draw_number(font, counter_frame / 10000 % 10, numx - 17, numy);
					draw_number(font, counter_frame / 100000 % 10, numx - 34, numy);
					draw_number(font, counter_frame / 1000000 % 10, numx - 51, numy);
				}
				else if (counter_frame < 100000000){
					draw_number(font, counter_frame % 10, numx + 56, numy);
					sf2d_draw_texture_part(font, numx + 51, numy, 308, 0, 22, 27); //point
					draw_number(font, counter_frame / 10 % 10, numx + 34, numy);
					draw_number(font, counter_frame / 100 % 10, numx + 17, numy);
					draw_number(font, counter_frame / 1000 % 10, numx, numy);//center
					draw_number(font, counter_frame / 10000 % 10, numx - 17, numy);
					draw_number(font, counter_frame / 100000 % 10, numx - 34, numy);
					draw_number(font, counter_frame / 1000000 % 10, numx - 51, numy);
					draw_number(font, counter_frame / 10000000 % 10, numx - 68, numy);
				}
				else if (counter_frame < 1000000000){
					draw_number(font, counter_frame % 10, numx + 56, numy);
					sf2d_draw_texture_part(font, numx + 51, numy, 308, 0, 22, 27); //point
					draw_number(font, counter_frame / 10 % 10, numx + 34, numy);
					draw_number(font, counter_frame / 100 % 10, numx + 17, numy);
					draw_number(font, counter_frame / 1000 % 10, numx, numy);//center
					draw_number(font, counter_frame / 10000 % 10, numx - 17, numy);
					draw_number(font, counter_frame / 100000 % 10, numx - 34, numy);
					draw_number(font, counter_frame / 1000000 % 10, numx - 51, numy);
					draw_number(font, counter_frame / 10000000 % 10, numx - 68, numy);
					draw_number(font, counter_frame / 100000000 % 10, numx - 85, numy);
				}
				sf2d_draw_texture(nyaned, 44, numy - 18); //nyaned
				sf2d_draw_texture(seconds, 94, numy + 22); //seconds

			sf2d_end_frame();
			sf2d_swapbuffers();

		}

		//sound

		audio_stop();
		audio_stop();
		csndExit();


			sf2d_free_texture(tex1);
			sf2d_free_texture(tex2);
			sf2d_free_texture(font);
			sf2d_free_texture(seconds);
			sf2d_free_texture(nyaned);
		
		

		sf2d_fini();
		return 0;
}


void draw_rainbow_dub(int rainx, int rainy, int line1, int line2, int line3, int line4, int line5, int line6){

	//6633ff
	//0099ff
	//33ff00
	//ffff00
	//ff9900
	//ff0000
	if (line1 == 1){
		sf2d_draw_rectangle(rainx, rainy + 8, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
	}
	if (line1 == 2){
		sf2d_draw_rectangle(rainx, rainy + 8, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 20, rainy + 8, 16, 8, RGBA8(0x00, 0x99, 0xFF, 0xFF));
	}
	if (line1 == 3){
		sf2d_draw_rectangle(rainx, rainy + 8, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 20, rainy + 8, 16, 8, RGBA8(0x00, 0x99, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 40, rainy + 8, 16, 8, RGBA8(0x33, 0xFF, 0x00, 0xFF));

	}
	if (line1 == 4){
		sf2d_draw_rectangle(rainx, rainy + 8, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 20, rainy + 8, 16, 8, RGBA8(0x00, 0x99, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 40, rainy + 8, 16, 8, RGBA8(0x33, 0xFF, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 60, rainy + 8, 16, 8, RGBA8(0xFF, 0xFF, 0x00, 0xFF));

	}
	if (line1 == 5){
		sf2d_draw_rectangle(rainx, rainy + 8, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 20, rainy + 8, 16, 8, RGBA8(0x00, 0x99, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 40, rainy + 8, 16, 8, RGBA8(0x33, 0xFF, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 60, rainy + 8, 16, 8, RGBA8(0xFF, 0xFF, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 80, rainy + 8, 16, 8, RGBA8(0xFF, 0x99, 0x00, 0xFF));

	}
	if (line1 == 6){
		sf2d_draw_rectangle(rainx, rainy + 8, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 20, rainy + 8, 16, 8, RGBA8(0x00, 0x99, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 40, rainy + 8, 16, 8, RGBA8(0x33, 0xFF, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 60, rainy + 8, 16, 8, RGBA8(0xFF, 0xFF, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 80, rainy + 8, 16, 8, RGBA8(0xFF, 0x99, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 100, rainy + 8, 16, 8, RGBA8(0xFF, 0x00, 0x00, 0xFF));

	}
	

	//draw line2
	if (line2 == 1){
		sf2d_draw_rectangle(rainx, rainy + 18, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));

	}
	if (line2 == 2){
		sf2d_draw_rectangle(rainx, rainy + 18, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 20, rainy + 18, 16, 8, RGBA8(0x00, 0x99, 0xFF, 0xFF));
	}
	if (line2 == 3){
		sf2d_draw_rectangle(rainx, rainy + 18, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 20, rainy + 18, 16, 8, RGBA8(0x00, 0x99, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 40, rainy + 18, 16, 8, RGBA8(0x33, 0xFF, 0x00, 0xFF));
	}
	if (line2 == 4){
		sf2d_draw_rectangle(rainx, rainy + 18, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 20, rainy + 18, 16, 8, RGBA8(0x00, 0x99, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 40, rainy + 18, 16, 8, RGBA8(0x33, 0xFF, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 60, rainy + 18, 16, 8, RGBA8(0xFF, 0xFF, 0x00, 0xFF));
	}
	if (line2 == 5){
		sf2d_draw_rectangle(rainx, rainy + 18, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 20, rainy + 18, 16, 8, RGBA8(0x00, 0x99, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 40, rainy + 18, 16, 8, RGBA8(0x33, 0xFF, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 60, rainy + 18, 16, 8, RGBA8(0xFF, 0xFF, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 80, rainy + 18, 16, 8, RGBA8(0xFF, 0x99, 0x00, 0xFF));
	}
	if (line2 == 6){
		sf2d_draw_rectangle(rainx, rainy + 18, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 20, rainy + 18, 16, 8, RGBA8(0x00, 0x99, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 40, rainy + 18, 16, 8, RGBA8(0x33, 0xFF, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 60, rainy + 18, 16, 8, RGBA8(0xFF, 0xFF, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 80, rainy + 18, 16, 8, RGBA8(0xFF, 0x99, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 100, rainy + 18, 16, 8, RGBA8(0xFF, 0x00, 0x00, 0xFF));
	}

	if (line3 == 1){
		sf2d_draw_rectangle(rainx, rainy + 28, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
	}
	if (line3 == 2){
		sf2d_draw_rectangle(rainx, rainy + 28, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 20, rainy + 28, 16, 8, RGBA8(0x00, 0x99, 0xFF, 0xFF));
	}
	if (line3 == 3){
		sf2d_draw_rectangle(rainx, rainy + 28, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 20, rainy + 28, 16, 8, RGBA8(0x00, 0x99, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 40, rainy + 28, 16, 8, RGBA8(0x33, 0xFF, 0x00, 0xFF));
	}
	if (line3 == 4){
		sf2d_draw_rectangle(rainx, rainy + 28, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 20, rainy + 28, 16, 8, RGBA8(0x00, 0x99, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 40, rainy + 28, 16, 8, RGBA8(0x33, 0xFF, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 60, rainy + 28, 16, 8, RGBA8(0xFF, 0xFF, 0x00, 0xFF));
	}
	if (line3 == 5){
		sf2d_draw_rectangle(rainx, rainy + 28, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 20, rainy + 28, 16, 8, RGBA8(0x00, 0x99, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 40, rainy + 28, 16, 8, RGBA8(0x33, 0xFF, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 60, rainy + 28, 16, 8, RGBA8(0xFF, 0xFF, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 80, rainy + 28, 16, 8, RGBA8(0xFF, 0x99, 0x00, 0xFF));
	}
	if (line3 == 6){
		sf2d_draw_rectangle(rainx, rainy + 28, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 20, rainy + 28, 16, 8, RGBA8(0x00, 0x99, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 40, rainy + 28, 16, 8, RGBA8(0x33, 0xFF, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 60, rainy + 28, 16, 8, RGBA8(0xFF, 0xFF, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 80, rainy + 28, 16, 8, RGBA8(0xFF, 0x99, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 100, rainy + 28, 16, 8, RGBA8(0xFF, 0x00, 0x00, 0xFF));
	}
	//line4
	if (line4 == 1){
		sf2d_draw_rectangle(rainx, rainy + 38, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
	}
	if (line4 == 2){
		sf2d_draw_rectangle(rainx, rainy + 38, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 20, rainy + 38, 16, 8, RGBA8(0x00, 0x99, 0xFF, 0xFF));
	}
	if (line4 == 3){
		sf2d_draw_rectangle(rainx, rainy + 38, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 20, rainy + 38, 16, 8, RGBA8(0x00, 0x99, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 40, rainy + 38, 16, 8, RGBA8(0x33, 0xFF, 0x00, 0xFF));
	}
	if (line4 == 4){
		sf2d_draw_rectangle(rainx, rainy + 38, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 20, rainy + 38, 16, 8, RGBA8(0x00, 0x99, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 40, rainy + 38, 16, 8, RGBA8(0x33, 0xFF, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 60, rainy + 38, 16, 8, RGBA8(0xFF, 0xFF, 0x00, 0xFF));
	}
	if (line4 == 5){
		sf2d_draw_rectangle(rainx, rainy + 38, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 20, rainy + 38, 16, 8, RGBA8(0x00, 0x99, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 40, rainy + 38, 16, 8, RGBA8(0x33, 0xFF, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 60, rainy + 38, 16, 8, RGBA8(0xFF, 0xFF, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 80, rainy + 38, 16, 8, RGBA8(0xFF, 0x99, 0x00, 0xFF));
	}
	if (line4 == 6){
		sf2d_draw_rectangle(rainx, rainy + 38, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 20, rainy + 38, 16, 8, RGBA8(0x00, 0x99, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 40, rainy + 38, 16, 8, RGBA8(0x33, 0xFF, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 60, rainy + 38, 16, 8, RGBA8(0xFF, 0xFF, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 80, rainy + 38, 16, 8, RGBA8(0xFF, 0x99, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 100, rainy + 38, 16, 8, RGBA8(0xFF, 0x00, 0x00, 0xFF));
	}
	//line5
	if (line5 == 1){
		sf2d_draw_rectangle(rainx, rainy + 48, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
	}
	if (line5 == 2){
		sf2d_draw_rectangle(rainx, rainy + 48, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 20, rainy + 48, 16, 8, RGBA8(0x00, 0x99, 0xFF, 0xFF));
	}
	if (line5 == 3){
		sf2d_draw_rectangle(rainx, rainy + 48, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 20, rainy + 48, 16, 8, RGBA8(0x00, 0x99, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 40, rainy + 48, 16, 8, RGBA8(0x33, 0xFF, 0x00, 0xFF));
	}
	if (line5 == 4){
		sf2d_draw_rectangle(rainx, rainy + 48, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 20, rainy + 48, 16, 8, RGBA8(0x00, 0x99, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 40, rainy + 48, 16, 8, RGBA8(0x33, 0xFF, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 60, rainy + 48, 16, 8, RGBA8(0xFF, 0xFF, 0x00, 0xFF));
	}
	if (line5 == 5){
		sf2d_draw_rectangle(rainx, rainy + 48, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 20, rainy + 48, 16, 8, RGBA8(0x00, 0x99, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 40, rainy + 48, 16, 8, RGBA8(0x33, 0xFF, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 60, rainy + 48, 16, 8, RGBA8(0xFF, 0xFF, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 80, rainy + 48, 16, 8, RGBA8(0xFF, 0x99, 0x00, 0xFF));
	}
	if (line5 == 6){
		sf2d_draw_rectangle(rainx, rainy + 48, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 20, rainy + 48, 16, 8, RGBA8(0x00, 0x99, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 40, rainy + 48, 16, 8, RGBA8(0x33, 0xFF, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 60, rainy + 48, 16, 8, RGBA8(0xFF, 0xFF, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 80, rainy + 48, 16, 8, RGBA8(0xFF, 0x99, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 100, rainy + 48, 16, 8, RGBA8(0xFF, 0x00, 0x00, 0xFF));
	}
	//line6
	if (line5 == 1){
		sf2d_draw_rectangle(rainx, rainy + 58, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
	}
	if (line5 == 2){
		sf2d_draw_rectangle(rainx, rainy + 58, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 20, rainy + 58, 16, 8, RGBA8(0x00, 0x99, 0xFF, 0xFF));
	}
	if (line5 == 3){
		sf2d_draw_rectangle(rainx, rainy + 58, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 20, rainy + 58, 16, 8, RGBA8(0x00, 0x99, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 40, rainy + 58, 16, 8, RGBA8(0x33, 0xFF, 0x00, 0xFF));
	}
	if (line5 == 4){
		sf2d_draw_rectangle(rainx, rainy + 58, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 20, rainy + 58, 16, 8, RGBA8(0x00, 0x99, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 40, rainy + 58, 16, 8, RGBA8(0x33, 0xFF, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 60, rainy + 58, 16, 8, RGBA8(0xFF, 0xFF, 0x00, 0xFF));
	}
	if (line5 == 5){
		sf2d_draw_rectangle(rainx, rainy + 58, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 20, rainy + 58, 16, 8, RGBA8(0x00, 0x99, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 40, rainy + 58, 16, 8, RGBA8(0x33, 0xFF, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 60, rainy + 58, 16, 8, RGBA8(0xFF, 0xFF, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 80, rainy + 58, 16, 8, RGBA8(0xFF, 0x99, 0x00, 0xFF));
	}
	if (line5 == 6){
		sf2d_draw_rectangle(rainx, rainy + 58, 16, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 20, rainy + 58, 16, 8, RGBA8(0x00, 0x99, 0xFF, 0xFF));
		sf2d_draw_rectangle(rainx - 40, rainy + 58, 16, 8, RGBA8(0x33, 0xFF, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 60, rainy + 58, 16, 8, RGBA8(0xFF, 0xFF, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 80, rainy + 58, 16, 8, RGBA8(0xFF, 0x99, 0x00, 0xFF));
		sf2d_draw_rectangle(rainx - 100, rainy + 58, 16, 8, RGBA8(0xFF, 0x00, 0x00, 0xFF));
	}


}


void draw_rainbow_tacnayn(int rainx, int rainy){
	sf2d_draw_rectangle(rainx, rainy + 8, 35, 8, RGBA8(0x7B, 0x7B, 0x79, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 16, 35, 8, RGBA8(0x98, 0x98, 0x9A, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 24, 35, 8, RGBA8(0xBA, 0xBD, 0xBC, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 32, 35, 8, RGBA8(0xFE, 0xFE, 0xFE, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 40, 35, 8, RGBA8(0x7C, 0x7C, 0x7C, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 48, 35, 8, RGBA8(0x9F, 0x96, 0x96, 0xFF));
}

void draw_rainbow_jazz(int rainx, int rainy){
	sf2d_draw_rectangle(rainx, rainy + 8, 35, 8, RGBA8(0xD6, 0x29, 0x42, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 16, 35, 8, RGBA8(0xD6, 0x6B, 0x29, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 24, 35, 8, RGBA8(0xD6, 0xB5, 0x29, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 32, 35, 8, RGBA8(0x73, 0xD6, 0x29, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 40, 35, 8, RGBA8(0x29, 0xAD, 0xCE, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 48, 35, 8, RGBA8(0x5A, 0x52, 0xD5, 0xFF));
}

void draw_rainbow_america(int rainx, int rainy){
	sf2d_draw_rectangle(rainx, rainy + 8, 35, 8, RGBA8(0xFF, 0x00, 0x00, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 16, 35, 8, RGBA8(0xFF, 0xFF, 0xFF, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 24, 35, 8, RGBA8(0xFF, 0x00, 0x00, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 32, 35, 8, RGBA8(0xFF, 0xFF, 0xFF, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 40, 35, 8, RGBA8(0xFF, 0x00, 0x00, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 48, 35, 8, RGBA8(0xFF, 0xFF, 0xFF, 0xFF));
}

void draw_rainbow_pikanyan(int rainx, int rainy){
	sf2d_draw_rectangle(rainx, rainy + 8, 35, 8, RGBA8(0xF7, 0xE6, 0x52, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 16, 35, 8, RGBA8(0xD6, 0xD6, 0xD6, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 24, 35, 8, RGBA8(0xF7, 0xE6, 0x52, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 32, 35, 8, RGBA8(0xD6, 0xD6, 0xD6, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 40, 35, 8, RGBA8(0xF7, 0xE6, 0x52, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 48, 35, 8, RGBA8(0xD6, 0xD6, 0xD6, 0xFF));
}


void draw_rainbow_jamaicnyan(int rainx, int rainy){
	sf2d_draw_rectangle(rainx, rainy + 8, 32, 8, RGBA8(0xFF, 0x00, 0x00, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 16, 32, 8, RGBA8(0x33, 0x99, 0x00, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 24, 32, 8, RGBA8(0xFF, 0xFF, 0x00, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 32, 32, 8, RGBA8(0xFF, 0x00, 0x00, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 40, 32, 8, RGBA8(0x33, 0x99, 0x00, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 48, 32, 8, RGBA8(0xFF, 0xFF, 0x00, 0xFF));
}

void draw_rainbow(int rainx, int rainy){
	sf2d_draw_rectangle(rainx, rainy + 8, 32, 8, RGBA8(0xFF, 0x00, 0x00, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 16, 32, 8, RGBA8(0xFF, 0x99, 0x00, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 24, 32, 8, RGBA8(0xFF, 0xFF, 0x00, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 32, 32, 8, RGBA8(0x33, 0xFF, 0x00, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 40, 32, 8, RGBA8(0x00, 0x99, 0xFF, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 48, 32, 8, RGBA8(0x66, 0x33, 0xFF, 0xFF));
}

void draw_rainbow_gb(int rainx, int rainy){
	sf2d_draw_rectangle(rainx, rainy + 8, 32, 8, RGBA8(0x19, 0x31, 0x31, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 16, 32, 8, RGBA8(0x21, 0x4A, 0x31, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 24, 32, 8, RGBA8(0x52, 0x7B, 0x6B, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 32, 32, 8, RGBA8(0x4A, 0x52, 0x3A, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 40, 32, 8, RGBA8(0x52, 0x7B, 0x3A, 0xFF));
	sf2d_draw_rectangle(rainx, rainy + 48, 32, 8, RGBA8(0xAD, 0xC5, 0x42, 0xFF));
}


void draw_number(const sf2d_texture *mmm, int num, int numx, int numy){
	if (num == 0){
		sf2d_draw_texture_part(mmm, numx, numy, 0, 27, 22, 27);
	}
	if (num == 1){
		sf2d_draw_texture_part(mmm, numx, numy, 22, 27, 22, 27);
	}
	if (num == 2){
		sf2d_draw_texture_part(mmm, numx, numy, 44, 27, 22, 27);
	}
	if (num == 3){
		sf2d_draw_texture_part(mmm, numx, numy, 66, 27, 22, 27);
	}
	if (num == 4){
		sf2d_draw_texture_part(mmm, numx, numy, 88, 27, 22, 27);
		//sf2d_free_texture(font);
	}
	if (num == 5){
		sf2d_draw_texture_part(mmm, numx, numy, 110, 27, 22, 27);
	}
	if (num == 6){
		sf2d_draw_texture_part(mmm, numx, numy, 132, 27, 22, 27);
	}
	if (num == 7){
		sf2d_draw_texture_part(mmm, numx, numy, 154, 27, 22, 27);
	}
	if (num == 8){
		sf2d_draw_texture_part(mmm, numx, numy, 176, 27, 22, 27);
	}
	if (num == 9){
		sf2d_draw_texture_part(mmm, numx, numy, 198, 27, 22, 27);
	}

}


void audio_load(const char *audio){

	FILE *file = fopen(audio, "rb");

	// seek to end of file
	fseek(file, 0, SEEK_END);

	// file pointer tells us the size
	off_t size = ftell(file);

	// seek back to start
	fseek(file, 0, SEEK_SET);

	//allocate a buffer
	buffer = linearAlloc(size);

	//read contents !
	off_t bytesRead = fread(buffer, 1, size, file);
	//u8 test = &buffer;

	//close the file because we like being nice and tidy
	fclose(file);

	csndPlaySound(8, SOUND_FORMAT_16BIT | SOUND_REPEAT, 32000, 1, 0, buffer, buffer, size);
	linearFree(buffer);
}
void audio_stop(void){
	csndExecCmds(true);
	CSND_SetPlayState(0x8, 0);
	memset(buffer, 0, size);
	GSPGPU_FlushDataCache(NULL, buffer, size);
	linearFree(buffer);
}
