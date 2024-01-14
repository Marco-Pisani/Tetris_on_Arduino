 //  0.0625L   //duration of a cycle in us
//  20        //duration of visible area of a line, in us,    (20 us 320 cycles)
//  1         //duration of horizontal front porch in us      (1 us 16 cycles)
//  2.1875    //duration of horizontal back porch in us       (2.2 us, 35 cycles)
//  3.1875    //duration of HSYNC pulse in us                 (3.2 us, 51 cyles)

//  105.625     duration of VSYNC pulse in us                 (105.6 us, 1690 cycles, 4 lines)
//  15840       duration of visible area of a frame           (15840 us, 253440 cycles, 600 lines)
//  26.375      duration of vertical front porch              (26.4 us,  422 cycles, 1 line)
//  607.1875    duration of vertical back porch               (607.2 us, 9715 cycles, 23 lines)

#define CYCLE 0.0625L

#define VISIBLE 480
#define FRONT 10
#define PULSE 2
#define BACK 33
#define LINES VISIBLE+FRONT+PULSE+BACK -1


#define BLOCK_HEIGHT 25
#define HSYNC_PERIOD 508

#define ROWS 19
#define COLUMNS 24//20

#define BLACK 	0
#define RED 	1
#define GREEN 	2
#define YELLOW 	GREEN+BLUE
#define BLUE 	4
#define PURPLE 	RED+BLUE
#define TEAL 	GREEN+RED
#define WHITE 	7

#define STARTING_PIXEL_GAME 0x90
#define STARTING_PIXEL 0x37 

#define PLAYER_COLOR PURPLE
#define BG WHITE

#define LEFT_MARGIN 7
#define RIGHT_MARGIN LEFT_MARGIN+10

unsigned char next_block_row =9;
unsigned char next_block_column=2;

//coordinates of the reference piece
#define START_ROW 1
#define START_COLUMN 4+LEFT_MARGIN

#define DIFFICULTY_INCREASE 1
#define SPEED_LIMIT 15

unsigned char game_over = 0;

unsigned char lines_completed;
unsigned char speed;

unsigned char colors[7];	//to set the colors of each tetromino

register unsigned char current_color asm("r12");	//contains the color of the current tetromino
register unsigned char background asm("r15");		//background color
register unsigned char current_columns asm("r16");
register unsigned char temp asm("r13");			//helps during rotation

register unsigned char reference_row asm("r8");		//row of the reference block
register unsigned char reference_column asm("r14");	//column of the reference block

 

	typedef struct secondary_blocks
	{
		//second block
		char block2_row		:2;
		char block2_column	:2;

		//third block
		char block3_row		:2;
		char block3_column	:2;

	}secondary_blocks;
	register secondary_blocks second_third asm("r9");	//relative to the current tetromino

	typedef struct fourth_block
	{
		//fourth block, 3 bits so it can assume the value -2 or +2
		char block4_row		:3;
		char block4_column	:3;

	}fourth_block;
	register fourth_block fourth asm("r10");		//relative to the current tetromino



//to define the parameters of each block type
typedef struct block_type
{
	secondary_blocks second_third_position;
	fourth_block fourth_position;
}block_type;

block_type blocks[7];	//vector containing the parameters of each block type




typedef struct position_checking		//if the bit is 1 it means that block is the lowest/rightmost/leftmost/highest
{
	unsigned short left_block1 :1;
	unsigned short left_block2 :1;
	unsigned short left_block3 :1;
	unsigned short left_block4 :1;

	unsigned short low_block1 :1;
	unsigned short low_block2 :1;
	unsigned short low_block3 :1;
	unsigned short low_block4 :1;

	unsigned short right_block1 :1;
	unsigned short right_block2 :1;
	unsigned short right_block3 :1;
	unsigned short right_block4 :1;

	unsigned short high_block1 :1;
	unsigned short high_block2 :1;
	unsigned short high_block3 :1;
	unsigned short high_block4 :1;

}position_checking;

typedef union pippo
{
	position_checking collisions;
	unsigned short temp;
}pippo;

pippo starting_collisions[7];	//collision of tetromino at starting point, one for each type of tetromino
pippo current_collisions;	//positions of our current block




//to implement buttons
typedef struct buttons
{
	unsigned char right 	  :1;
	unsigned char left  	  :1;
	unsigned char line_check  :1;
	unsigned char row_pointer :5;
}buttons;
register buttons utilities1 asm("r7");
//////////////////////////////////////////

typedef struct utilities2
{
	unsigned char block_type	:4;
	unsigned char reset_position	:1;
	unsigned char enable		:1;		//used to enable/disable certain functions
	unsigned char rotate		:1;		//enables rotation
}utilities2_type;
register utilities2_type utilities2 asm ("r11");


volatile short line = 0;
register unsigned char i asm("r3");		//rows in RGB_color
register unsigned char j asm("r4");		//columns in RGB_color
register unsigned char line_count asm("r5");	//to help counting lines for the i variable

typedef struct utilities
{
	unsigned char a :7;
	unsigned char move_check:1;
}utilities3_type;
register utilities3_type utilities3 asm("r6");		//for the position_update() functions

unsigned char next_block;

//color matrix
volatile char Pixel_color[ROWS+1][COLUMNS];
