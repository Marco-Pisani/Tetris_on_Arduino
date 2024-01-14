#include <avr/io.h>
#include <util/delay.h>
#include <avr/cpufunc.h>
#include <avr/interrupt.h>

#include "Tetris.h"


void starting_position()
{
	utilities2.enable = 0;		//disables other functions

	Pixel_color[next_block_row + blocks[next_block].second_third_position.block2_row][next_block_column + blocks[next_block].second_third_position.block2_column] = 0;
	
	Pixel_color[next_block_row + blocks[next_block].second_third_position.block3_row][next_block_column + blocks[next_block].second_third_position.block3_column] = 0;
	
	Pixel_color[next_block_row + blocks[next_block].fourth_position.block4_row][next_block_column + blocks[next_block].fourth_position.block4_column] = 0;

	Pixel_color[next_block_row][next_block_column] = 0;
	
	utilities2.block_type = next_block;

	while( ( next_block = (TCNT0 & 0x7) ) > 6);		//randomly generates a new piece

	second_third = blocks[utilities2.block_type].second_third_position;

	fourth = blocks[utilities2.block_type].fourth_position;

	current_collisions = starting_collisions[utilities2.block_type];

	reference_row = START_ROW;
	reference_column = START_COLUMN;
	
	current_color = colors[utilities2.block_type];
	
	
	//gameover check
	if(Pixel_color[reference_row][reference_column] != background)
	{
		game_over = 1;		//game_over
	}
	else
	{
		if(Pixel_color[reference_row + second_third.block2_row][reference_column + second_third.block2_column] != background)
		{
			game_over = 1;		//game_over
		}
		else
		{
			if(Pixel_color[reference_row + second_third.block3_row][reference_column + second_third.block3_column] != background)
			{
				game_over = 1;		//game_over
			}
			else
			{
				if(Pixel_color[reference_row + fourth.block4_row][reference_column + fourth.block4_column] != background)
				{
					game_over = 1;		//game_over
				}
			}
		}
	}
	


	Pixel_color[reference_row][reference_column] = current_color;
	Pixel_color[reference_row + second_third.block2_row][reference_column + second_third.block2_column] = current_color;
	Pixel_color[reference_row + second_third.block3_row][reference_column + second_third.block3_column] = current_color;
	Pixel_color[reference_row + fourth.block4_row][reference_column + fourth.block4_column] = current_color;
	

	Pixel_color[next_block_row + blocks[next_block].second_third_position.block2_row][next_block_column + blocks[next_block].second_third_position.block2_column] = background;
	
	Pixel_color[next_block_row + blocks[next_block].second_third_position.block3_row][next_block_column + blocks[next_block].second_third_position.block3_column] = background;
	
	Pixel_color[next_block_row + blocks[next_block].fourth_position.block4_row][next_block_column + blocks[next_block].fourth_position.block4_column] = background;

	Pixel_color[next_block_row][next_block_column] = background;
	

	utilities2.reset_position = 0;
	utilities2.enable = 1;
}


void line_check(char line_to_check)
{
	utilities2.enable = 0;
	
	for(char m = 0; m<4; m++)
	{
		for(int p = (LEFT_MARGIN); (p < (RIGHT_MARGIN)) && (utilities1.line_check != 1); p++)
		{
			if(Pixel_color[line_to_check][p] == background)
			{
				utilities1.line_check = 1;	//means the line is not completed
			}
		}

		if(utilities1.line_check == 0)	//line completed
		{
			for(char k = (line_to_check); k > 1; k--)
			{
				for(char h=LEFT_MARGIN; h < (RIGHT_MARGIN); h++)
				{
					Pixel_color[k][h] = Pixel_color[k - 1][h];
				}
			}
			
			lines_completed++;
		}
		utilities1.line_check=0;
	}

	utilities2.enable = 1;
}


void position_update()
{
	utilities2.enable = 0;

	if(current_collisions.collisions.low_block1)
	{
		if(reference_row == (ROWS-1) || Pixel_color[reference_row + 1][reference_column] != background)
		{
			utilities1.row_pointer = (reference_row > utilities1.row_pointer) ? reference_row : utilities1.row_pointer;
			utilities2.reset_position=1;
			utilities2.enable = 1;
		}
	}

	if(current_collisions.collisions.low_block2)
	{
		if( reference_row + second_third.block2_row == (ROWS-1) || Pixel_color[reference_row + second_third.block2_row + 1][reference_column + second_third.block2_column] != background)
	        {
        	        utilities1.row_pointer = (reference_row + second_third.block2_row > utilities1.row_pointer) ? (reference_row + second_third.block2_row) : utilities1.row_pointer;
        	        utilities2.reset_position=1;
        	        utilities2.enable = 1;
        	}
	}

	if(current_collisions.collisions.low_block3)
        {
		if( reference_row + second_third.block3_row == (ROWS-1) || Pixel_color[reference_row + second_third.block3_row + 1][reference_column + second_third.block3_column] != background)
                {
                        utilities1.row_pointer = (reference_row + second_third.block3_row > utilities1.row_pointer) ? (reference_row + second_third.block3_row) : utilities1.row_pointer;
                        utilities2.reset_position=1;
                        utilities2.enable = 1;
                }
        }

	if(current_collisions.collisions.low_block4)
        {
                if( reference_row + fourth.block4_row == (ROWS-1) || Pixel_color[reference_row + fourth.block4_row + 1][reference_column + fourth.block4_column] != background)
                {
                        utilities1.row_pointer = (reference_row + fourth.block4_row > utilities1.row_pointer) ? (reference_row + fourth.block4_row) : utilities1.row_pointer;
                        utilities2.reset_position=1;
                        utilities2.enable = 1;
                }
        }


	if(!utilities2.reset_position)
	{	
		Pixel_color[reference_row + second_third.block2_row][reference_column + second_third.block2_column] = background;
		Pixel_color[reference_row + second_third.block3_row][reference_column + second_third.block3_column] = background;
		Pixel_color[reference_row + fourth.block4_row][reference_column + fourth.block4_column] = background;
		Pixel_color[reference_row++][reference_column] = background;

		Pixel_color[reference_row][reference_column] = current_color;
		Pixel_color[reference_row + second_third.block2_row][reference_column + second_third.block2_column] = current_color;
		Pixel_color[reference_row + second_third.block3_row][reference_column + second_third.block3_column] = current_color;
		Pixel_color[reference_row + fourth.block4_row][reference_column + fourth.block4_column] = current_color;
        }
        
        utilities2.enable = 1;
}


void rotate()
{
	utilities2.enable = 0;
	
	//first empty the blocks
	Pixel_color[reference_row + second_third.block2_row][reference_column + second_third.block2_column] = background;
        Pixel_color[reference_row + second_third.block3_row][reference_column + second_third.block3_column] = background;
        Pixel_color[reference_row + fourth.block4_row][reference_column + fourth.block4_column] = background;
        Pixel_color[reference_row][reference_column] = background;

	//rotation of block coordinates, counter-clockwise
	//rotation matrix
	/*
	
		0	-1
		
		1	 0
	
	*/
	//if the block has coordinates X and Y, the new coordinates are -Y and X
	
	//clock wise
	/*
	
		 0	 1
		
		-1	 0
	
	*/
	//if the block has coordinates X and Y, the new coordinates are Y and -X
	
	temp = 0;
	
	temp = second_third.block2_row;
	second_third.block2_row = -second_third.block2_column;	//X = -Y
	second_third.block2_column = temp;				//Y = X
	
	temp = second_third.block3_row;
	second_third.block3_row = -second_third.block3_column;	//X = -Y
	second_third.block3_column = temp;				//Y = X
	
	temp = fourth.block4_row;
	fourth.block4_row = -fourth.block4_column;		//X = -Y
	fourth.block4_column = temp;				//Y = X

	//now checks if the new blocks aren't already full or out of bounds, if there are errors with the new block then temp = 1

	
	
	temp = 0;
	
	if(Pixel_color[reference_row + second_third.block2_row][reference_column + second_third.block2_column] != background || reference_column + second_third.block2_column > RIGHT_MARGIN-1)
	{
		temp = 1;
	}
			
	if(Pixel_color[reference_row + second_third.block3_row][reference_column + second_third.block3_column] != background || reference_column + second_third.block3_column > RIGHT_MARGIN-1)
	{
		temp = 1;
	}
		
	if(Pixel_color[reference_row + fourth.block4_row][reference_column + fourth.block4_column] != background || reference_column + fourth.block4_column > RIGHT_MARGIN-1)
	{
		temp = 1;
	}
	
	if(temp == 1)		//error, perform a clockwise rotation to restore the values
	{
		temp = 0;
	
		temp = second_third.block2_row;
		second_third.block2_row = second_third.block2_column;		//X = Y
		second_third.block2_column = -temp;			//Y = -X
		
		temp = second_third.block3_row;
		second_third.block3_row = second_third.block3_column;		//X = Y
		second_third.block3_column = -temp;			//Y = -X
		
		temp = fourth.block4_row;
		fourth.block4_row = fourth.block4_column;		//X = Y
		fourth.block4_column = -temp;				//Y = -X
	}
	else			//no errors
	{
		//block positions change
		temp = 0;
		
		temp = (current_collisions.temp >> 12) & 0x0f; 				//saving the high_block values 
		current_collisions.temp = current_collisions.temp << 4;			//shifting the positions values
		current_collisions.temp = (current_collisions.temp | (temp & 0x0f));	//restoring the high_values (now left values)
	}
	
	Pixel_color[reference_row][reference_column] = current_color;
        Pixel_color[reference_row + second_third.block2_row][reference_column + second_third.block2_column] = current_color;
        Pixel_color[reference_row + second_third.block3_row][reference_column + second_third.block3_column] = current_color;
        Pixel_color[reference_row + fourth.block4_row][reference_column + fourth.block4_column] = current_color;
	
	utilities2.enable = 1;
}




void move_right()
{
	if(current_collisions.collisions.right_block1)
        {
                if(reference_column == (RIGHT_MARGIN - 1) || Pixel_color[reference_row][reference_column + 1] != background)
                {
                        return;
                }
        }

        if(current_collisions.collisions.right_block2)
        {
                if(reference_column + second_third.block2_column == (RIGHT_MARGIN - 1) || Pixel_color[reference_row + second_third.block2_row][reference_column + second_third.block2_column + 1] != background)
                {
                        return;
                }
        }

        if(current_collisions.collisions.right_block3)
        {
                if(reference_column + second_third.block3_column == (RIGHT_MARGIN - 1) || Pixel_color[reference_row + second_third.block3_row][reference_column + second_third.block3_column + 1] != background)
                {
                        return;
                }
        }

        if(current_collisions.collisions.right_block4)
        {
                if(reference_column + fourth.block4_column == (RIGHT_MARGIN - 1) || Pixel_color[reference_row + fourth.block4_row][reference_column + fourth.block4_column + 1] != background)
                {
                        return;
                }
        }

		Pixel_color[reference_row + second_third.block2_row][reference_column + second_third.block2_column] = background;
		Pixel_color[reference_row + second_third.block3_row][reference_column + second_third.block3_column] = background;
		Pixel_color[reference_row + fourth.block4_row][reference_column + fourth.block4_column] = background;
		Pixel_color[reference_row][reference_column++] = background;

		Pixel_color[reference_row][reference_column] = current_color;
		Pixel_color[reference_row + second_third.block2_row][reference_column + second_third.block2_column] = current_color;
		Pixel_color[reference_row + second_third.block3_row][reference_column + second_third.block3_column] = current_color;
		Pixel_color[reference_row + fourth.block4_row][reference_column + fourth.block4_column] = current_color;
}

void move_left()
{
	if(current_collisions.collisions.left_block1)
        {
                if(reference_column == (LEFT_MARGIN) || Pixel_color[reference_row][reference_column - 1] != background)
                {
                        return;
                }
        }

        if(current_collisions.collisions.left_block2)
        {
                if(reference_column + second_third.block2_column == (LEFT_MARGIN) || Pixel_color[reference_row + second_third.block2_row][reference_column + second_third.block2_column - 1] != background)
                {
                        return;
                }
        }

        if(current_collisions.collisions.left_block3)
        {
                if(reference_column + second_third.block3_column == (LEFT_MARGIN) || Pixel_color[reference_row + second_third.block3_row][reference_column + second_third.block3_column - 1] != background)
                {
                        return;
                }
        }

        if(current_collisions.collisions.left_block4)
        {
                if(reference_column + fourth.block4_column == (LEFT_MARGIN) || Pixel_color[reference_row + fourth.block4_row][reference_column + fourth.block4_column - 1] != background)
                {
                        return;
                }
        }

		Pixel_color[reference_row + second_third.block2_row][reference_column + second_third.block2_column] = background;
                Pixel_color[reference_row + second_third.block3_row][reference_column + second_third.block3_column] = background;
                Pixel_color[reference_row + fourth.block4_row][reference_column + fourth.block4_column] = background;
                Pixel_color[reference_row][reference_column--] = background;

                Pixel_color[reference_row][reference_column] = current_color;
                Pixel_color[reference_row + second_third.block2_row][reference_column + second_third.block2_column] = current_color;
                Pixel_color[reference_row + second_third.block3_row][reference_column + second_third.block3_column] = current_color;
                Pixel_color[reference_row + fourth.block4_row][reference_column + fourth.block4_column] = current_color;
}



void block_types_setup()
{
//square tetromino, 0
        blocks[0].second_third_position.block2_row = 0;
        blocks[0].second_third_position.block2_column = -1;

        blocks[0].second_third_position.block3_row = -1;
        blocks[0].second_third_position.block3_column = 0;

        blocks[0].fourth_position.block4_row = -1;
        blocks[0].fourth_position.block4_column = -1;

        starting_collisions[0].collisions.low_block1 = 1;
        starting_collisions[0].collisions.low_block2 = 1;
        starting_collisions[0].collisions.low_block3 = 0;
        starting_collisions[0].collisions.low_block4 = 0;

        starting_collisions[0].collisions.left_block1 = 0;
        starting_collisions[0].collisions.left_block2 = 1;
        starting_collisions[0].collisions.left_block3 = 0;
        starting_collisions[0].collisions.left_block4 = 1;

        starting_collisions[0].collisions.right_block1 = 1;
        starting_collisions[0].collisions.right_block2 = 0;
        starting_collisions[0].collisions.right_block3 = 1;
        starting_collisions[0].collisions.right_block4 = 0;

        starting_collisions[0].collisions.high_block1 = 0;
        starting_collisions[0].collisions.high_block2 = 0;
        starting_collisions[0].collisions.high_block3 = 1;
        starting_collisions[0].collisions.high_block4 = 1;

//long tetromino, 1
	blocks[1].second_third_position.block2_row = -1;
        blocks[1].second_third_position.block2_column = 0;

        blocks[1].second_third_position.block3_row = +1;
        blocks[1].second_third_position.block3_column = 0;

        blocks[1].fourth_position.block4_row = +2;
        blocks[1].fourth_position.block4_column = 0;

        starting_collisions[1].collisions.low_block1 = 0;
        starting_collisions[1].collisions.low_block2 = 0;
        starting_collisions[1].collisions.low_block3 = 0;
        starting_collisions[1].collisions.low_block4 = 1;

        starting_collisions[1].collisions.left_block1 = 1;
        starting_collisions[1].collisions.left_block2 = 1;
        starting_collisions[1].collisions.left_block3 = 1;
        starting_collisions[1].collisions.left_block4 = 1;

        starting_collisions[1].collisions.right_block1 = 1;
        starting_collisions[1].collisions.right_block2 = 1;
        starting_collisions[1].collisions.right_block3 = 1;
        starting_collisions[1].collisions.right_block4 = 1;

        starting_collisions[1].collisions.high_block1 = 0;
        starting_collisions[1].collisions.high_block2 = 1;
        starting_collisions[1].collisions.high_block3 = 0;
        starting_collisions[1].collisions.high_block4 = 0;

//T tetromino, 2
	blocks[2].second_third_position.block2_row = 0;
        blocks[2].second_third_position.block2_column = -1;

        blocks[2].second_third_position.block3_row = 0;
        blocks[2].second_third_position.block3_column = 1;

        blocks[2].fourth_position.block4_row = -1;
        blocks[2].fourth_position.block4_column = 0;

        starting_collisions[2].collisions.low_block1 = 1;
        starting_collisions[2].collisions.low_block2 = 1;
        starting_collisions[2].collisions.low_block3 = 1;
        starting_collisions[2].collisions.low_block4 = 0;

        starting_collisions[2].collisions.left_block1 = 0;
        starting_collisions[2].collisions.left_block2 = 1;
        starting_collisions[2].collisions.left_block3 = 0;
        starting_collisions[2].collisions.left_block4 = 1;

        starting_collisions[2].collisions.right_block1 = 0;
        starting_collisions[2].collisions.right_block2 = 0;
        starting_collisions[2].collisions.right_block3 = 1;
        starting_collisions[2].collisions.right_block4 = 1;

        starting_collisions[2].collisions.high_block1 = 0;
        starting_collisions[2].collisions.high_block2 = 1;
        starting_collisions[2].collisions.high_block3 = 1;
        starting_collisions[2].collisions.high_block4 = 1;
        
//skew tetromino, 3
	blocks[3].second_third_position.block2_row = 0;
        blocks[3].second_third_position.block2_column = -1;

        blocks[3].second_third_position.block3_row = -1;
        blocks[3].second_third_position.block3_column = 1;

        blocks[3].fourth_position.block4_row = -1;
        blocks[3].fourth_position.block4_column = 0;

        starting_collisions[3].collisions.low_block1 = 1;
        starting_collisions[3].collisions.low_block2 = 1;
        starting_collisions[3].collisions.low_block3 = 1;
        starting_collisions[3].collisions.low_block4 = 0;

        starting_collisions[3].collisions.left_block1 = 0;
        starting_collisions[3].collisions.left_block2 = 1;
        starting_collisions[3].collisions.left_block3 = 0;
        starting_collisions[3].collisions.left_block4 = 1;

        starting_collisions[3].collisions.right_block1 = 1;
        starting_collisions[3].collisions.right_block2 = 0;
        starting_collisions[3].collisions.right_block3 = 1;
        starting_collisions[3].collisions.right_block4 = 0;

        starting_collisions[3].collisions.high_block1 = 0;
        starting_collisions[3].collisions.high_block2 = 1;
        starting_collisions[3].collisions.high_block3 = 1;
        starting_collisions[3].collisions.high_block4 = 1;
        
//inverse skew tetromino, 4
	blocks[4].second_third_position.block2_row = 0;
        blocks[4].second_third_position.block2_column = 1;

        blocks[4].second_third_position.block3_row = -1;
        blocks[4].second_third_position.block3_column = -1;

        blocks[4].fourth_position.block4_row = -1;
        blocks[4].fourth_position.block4_column = 0;

        starting_collisions[4].collisions.low_block1 = 1;
        starting_collisions[4].collisions.low_block2 = 1;
        starting_collisions[4].collisions.low_block3 = 1;
        starting_collisions[4].collisions.low_block4 = 0;

        starting_collisions[4].collisions.left_block1 = 1;
        starting_collisions[4].collisions.left_block2 = 0;
        starting_collisions[4].collisions.left_block3 = 1;
        starting_collisions[4].collisions.left_block4 = 0;

        starting_collisions[4].collisions.right_block1 = 0;
        starting_collisions[4].collisions.right_block2 = 1;
        starting_collisions[4].collisions.right_block3 = 0;
        starting_collisions[4].collisions.right_block4 = 1;

        starting_collisions[4].collisions.high_block1 = 0;
        starting_collisions[4].collisions.high_block2 = 1;
        starting_collisions[4].collisions.high_block3 = 1;
        starting_collisions[4].collisions.high_block4 = 1;
        
//L tetromino, 5
	blocks[5].second_third_position.block2_row = 0;
        blocks[5].second_third_position.block2_column = -1;

        blocks[5].second_third_position.block3_row = 0;
        blocks[5].second_third_position.block3_column = 1;

        blocks[5].fourth_position.block4_row = -1;
        blocks[5].fourth_position.block4_column = 1;

        starting_collisions[5].collisions.low_block1 = 1;
        starting_collisions[5].collisions.low_block2 = 1;
        starting_collisions[5].collisions.low_block3 = 1;
        starting_collisions[5].collisions.low_block4 = 0;

        starting_collisions[5].collisions.left_block1 = 0;
        starting_collisions[5].collisions.left_block2 = 1;
        starting_collisions[5].collisions.left_block3 = 0;
        starting_collisions[5].collisions.left_block4 = 1;

        starting_collisions[5].collisions.right_block1 = 0;
        starting_collisions[5].collisions.right_block2 = 0;
        starting_collisions[5].collisions.right_block3 = 1;
        starting_collisions[5].collisions.right_block4 = 1;

        starting_collisions[5].collisions.high_block1 = 1;
        starting_collisions[5].collisions.high_block2 = 1;
        starting_collisions[5].collisions.high_block3 = 0;
        starting_collisions[5].collisions.high_block4 = 1;
        
//inverse L tetromino 6
	blocks[6].second_third_position.block2_row = -1;
        blocks[6].second_third_position.block2_column = 0;

        blocks[6].second_third_position.block3_row = 0;
        blocks[6].second_third_position.block3_column = 1;

        blocks[6].fourth_position.block4_row = 0;
        blocks[6].fourth_position.block4_column = 2;

        starting_collisions[6].collisions.low_block1 = 1;
        starting_collisions[6].collisions.low_block2 = 0;
        starting_collisions[6].collisions.low_block3 = 1;
        starting_collisions[6].collisions.low_block4 = 1;

        starting_collisions[6].collisions.left_block1 = 1;
        starting_collisions[6].collisions.left_block2 = 1;
        starting_collisions[6].collisions.left_block3 = 0;
        starting_collisions[6].collisions.left_block4 = 0;

        starting_collisions[6].collisions.right_block1 = 0;
        starting_collisions[6].collisions.right_block2 = 1;
        starting_collisions[6].collisions.right_block3 = 0;
        starting_collisions[6].collisions.right_block4 = 1;

        starting_collisions[6].collisions.high_block1 = 0;
        starting_collisions[6].collisions.high_block2 = 1;
        starting_collisions[6].collisions.high_block3 = 1;
        starting_collisions[6].collisions.high_block4 = 1;
        
        colors[0] = 1;//RED;
        colors[1] = 2;//GREEN;
        colors[2] = 3;
        colors[3] = 4;//YELLOW;
        colors[4] = 5;//PURPLE;
        colors[5] = 6;//TEAL;
        colors[6] = 0;//BLACK;
}





ISR(TIMER1_COMPA_vect)      //interrupt routine
{

	if(line == VISIBLE + FRONT + PULSE)
		PORTB |= 1 << PORTB2;


    if(line < VISIBLE)
    {

	while( j< current_columns)
	{
		PORTD = Pixel_color[i][j++];
	}
	j=0;

	if(line_count == BLOCK_HEIGHT)
	{
		i++;
        	line_count = 0;
	}
	
    }

    	if(line == VISIBLE + FRONT)
		PORTB &= ~(1 << PORTB2);



    if((utilities3.a == speed) && (utilities2.enable))
    {
	position_update();
	utilities3.a=0;
    }


    if(line == LINES)         //line increment
    {
        line = 0;
	i=0;
	line_count = 0;
	utilities3.a++;
    }
    else
    {
	line_count++;
	++line;
    }
}



void setup()
{
	DDRB |= 1 << DDB1;      //OCA1, HSYHC
	DDRB |= 1 << DDB2;      //VSYNC
    
	DDRD |= 1 << PORTD0;	//RED
	DDRD |= 1 << PORTD1;	//GREEN
	DDRD |= 1 << PORTD2;	//BLUE
	
	DDRB &= ~(1 << PORTB3);	//RIGHT
	DDRB &= ~(1 << PORTB4);	//LEFT
	DDRB &= ~(1 << PORTB0);	//ROTATION COUNTER-CLOCKWISE
    //////////////////////////////
    OCR1AH = 0x00;
    OCR1AL = STARTING_PIXEL;		//33;	//33

    ICR1  = HSYNC_PERIOD;//508;//421;	//421;

    TCCR1A |= (1 << COM1A0);
    TCCR1A |= (1 << COM1A1);
    
    TCCR1A &= ~(1 << WGM10);
    TCCR1A |= 1 << WGM11;
    TCCR1B |= 1 << WGM12;
    TCCR1B |= 1 << WGM13;
    
    //PD5(T1) as input clock, rising edge
    TCCR1B &= ~(1 << CS10);     //Timer stop
    TCCR1B &= ~(1 << CS11);
    TCCR1B &= ~(1 << CS12);
    //Interrupt enable
    TIMSK1 |= 1 << OCIE1A;

    PORTB |= 1 << PORTB2;
    TCCR1B |= (1 << CS10);      //Timer start
    
    sei();
    
    utilities3.a=0;
    current_columns = COLUMNS;
    
    TCCR0B |= 5;	//a second timer, used to randomly generate the blocks
    speed = 10;
    lines_completed = 0;
}


void game_colors_setup()
{
	//starting position
	
	for(i = 0; i<ROWS; i++)
	{
		for(j=0; j<COLUMNS; j++)
		{
			Pixel_color[i][j] = 0;
		}
	}
	
	for(i = 0; i<ROWS; i++)
	{
		for(j=LEFT_MARGIN; j<RIGHT_MARGIN; j++)
		{
//			Pixel_color[i][j] = (i+j+1)%8;
			Pixel_color[i][j] = background;
		}
	}

	for(i=0;i<ROWS;i++)
        {
                Pixel_color[i][RIGHT_MARGIN] = 0;
        }

	utilities1.line_check=0;
}



void title_colors_setup()
{

	background = WHITE;
	
	utilities1.line_check=0;
	
	for(i = 0; i<ROWS; i++)
	{
		for(j=0; j<COLUMNS; j++)
		{
			Pixel_color[i][j] = BLACK;
		}
	}

	//P
	Pixel_color[2][0+2] = background;
	Pixel_color[2][1+2] = background;
	Pixel_color[3][2+2] = background;
	Pixel_color[3][0+2] = background;
	Pixel_color[4][0+2] = background;
	Pixel_color[4][1+2] = background;
	Pixel_color[5][0+2] = background;
	Pixel_color[6][0+2] = background;
	
	//R
	Pixel_color[2][4+2] = background;
	Pixel_color[2][5+2] = background;
	Pixel_color[3][6+2] = background;
	Pixel_color[3][4+2] = background;
	Pixel_color[4][4+2] = background;
	Pixel_color[4][5+2] = background;
	Pixel_color[5][4+2] = background;
	Pixel_color[6][4+2] = background;
	Pixel_color[5][6+2] = background;
	Pixel_color[6][6+2] = background;
	
	//E
	Pixel_color[2][8+2] = background;
	Pixel_color[2][9+2] = background;
	Pixel_color[2][8+2] = background;
	Pixel_color[2][10+2] = background;
	Pixel_color[3][8+2] = background;
	Pixel_color[4][8+2] = background;
	Pixel_color[4][9+2] = background;
	Pixel_color[5][8+2] = background;
	Pixel_color[6][8+2] = background;
	Pixel_color[4][10+2] = background;
	Pixel_color[6][10+2] = background;
	Pixel_color[6][9+2] = background;
	
	//S
	Pixel_color[2][12+2] = background;
	Pixel_color[2][13+2] = background;
	Pixel_color[2][12+2] = background;
	Pixel_color[2][14+2] = background;
	Pixel_color[3][12+2] = background;
	Pixel_color[4][12+2] = background;
	Pixel_color[4][13+2] = background;
	Pixel_color[5][14+2] = background;
	Pixel_color[6][12+2] = background;
	Pixel_color[4][14+2] = background;
	Pixel_color[6][14+2] = background;
	Pixel_color[6][13+2] = background;
	
	//S
	Pixel_color[2][16+2] = background;
	Pixel_color[2][17+2] = background;
	Pixel_color[2][17+2] = background;
	Pixel_color[2][18+2] = background;
	Pixel_color[3][16+2] = background;
	Pixel_color[4][16+2] = background;
	Pixel_color[4][17+2] = background;
	Pixel_color[5][18+2] = background;
	Pixel_color[6][16+2] = background;
	Pixel_color[4][18+2] = background;
	Pixel_color[6][18+2] = background;
	Pixel_color[6][17+2] = background;


	//S
	Pixel_color[8][2] = background;
	Pixel_color[9][2] = background;
	Pixel_color[10][2] = background;
	Pixel_color[8][3] = background;
	Pixel_color[8][4] = background;
	Pixel_color[10][3] = background;
	Pixel_color[10][4] = background;
	Pixel_color[11][4] = background;
	Pixel_color[12][4] = background;
	Pixel_color[12][3] = background;
	Pixel_color[12][2] = background;
	
	//T
	Pixel_color[8][6] = background;
	Pixel_color[8][7] = background;
	Pixel_color[8][8] = background;
	Pixel_color[9][7] = background;
	Pixel_color[10][7] = background;
	Pixel_color[11][7] = background;
	Pixel_color[12][7] = background;
	
	//A
	Pixel_color[8][11] = background;
	Pixel_color[9][10] = background;
	Pixel_color[9][12] = background;
	Pixel_color[10][10] = background;
	Pixel_color[10][12] = background;
	Pixel_color[11][10] = background;
	Pixel_color[11][12] = background;
	Pixel_color[12][10] = background;
	Pixel_color[12][12] = background;
	Pixel_color[10][11] = background;
	
	//R
	Pixel_color[8][14] = background;
	Pixel_color[8][15] = background;
	Pixel_color[9][14] = background;
	Pixel_color[10][14] = background;
	Pixel_color[11][14] = background;
	Pixel_color[12][14] = background;
	Pixel_color[9][16] = background;
	Pixel_color[10][15] = background;
	Pixel_color[11][16] = background;
	Pixel_color[12][16] = background;
	
	//T
	Pixel_color[8][18] = background;
	Pixel_color[8][19] = background;
	Pixel_color[8][20] = background;
	Pixel_color[9][19] = background;
	Pixel_color[10][19] = background;
	Pixel_color[11][19] = background;
	Pixel_color[12][19] = background;
	
	for(int g = 14; g<4 + 14; g++)
	{
		for(int y = 2; y<21; y++)
		{
			Pixel_color[g][y] = (g+y)%8 + 1;
		}
	}
}



void game_over_screen()
{
	for(i = 0; i<ROWS; i++)
	{
		for(j=0; j<COLUMNS; j++)
		{
			Pixel_color[i][j] = 0;
		}
	}
	

	//G
	Pixel_color[2][0] = background;
	Pixel_color[2][1] = background;
	Pixel_color[2][2] = background;
	Pixel_color[2][3] = background;
	Pixel_color[2][4] = background;
	Pixel_color[3][0] = background;
	Pixel_color[4][0] = background;
	Pixel_color[5][0] = background;
	Pixel_color[6][0] = background;
	Pixel_color[6][1] = background;
	Pixel_color[6][2] = background;
	Pixel_color[6][3] = background;
	Pixel_color[6][4] = background;
	Pixel_color[5][4] = background;
	Pixel_color[4][4] = background;
	Pixel_color[4][3] = background;
		
	//A
	Pixel_color[2][7] = background;
	Pixel_color[2][8] = background;
	Pixel_color[2][9] = background;
	Pixel_color[3][6] = background;
	Pixel_color[3][10] = background;
	Pixel_color[4][6] = background;
	Pixel_color[5][6] = background;
	Pixel_color[6][6] = background;
	Pixel_color[4][10] = background;
	Pixel_color[5][10] = background;
	Pixel_color[6][10] = background;
	Pixel_color[4][9] = background;
	Pixel_color[4][8] = background;
	Pixel_color[4][7] = background;
	
	//M
	Pixel_color[2][12] = background;
	Pixel_color[3][12] = background;
	Pixel_color[4][12] = background;
	Pixel_color[5][12] = background;
	Pixel_color[6][12] = background;
	Pixel_color[3][13] = background;
	Pixel_color[3][15] = background;
	Pixel_color[2][16] = background;
	Pixel_color[4][14] = background;
	Pixel_color[3][16] = background;
	Pixel_color[4][16] = background;
	Pixel_color[5][16] = background;
	Pixel_color[6][16] = background;
	
	//E
	Pixel_color[2][18] = background;
	Pixel_color[3][18] = background;
	Pixel_color[4][18] = background;
	Pixel_color[5][18] = background;
	Pixel_color[6][18] = background;
	Pixel_color[2][19] = background;
	Pixel_color[2][20] = background;
	Pixel_color[2][21] = background;
	
	Pixel_color[4][19] = background;
	Pixel_color[4][20] = background;
	
	Pixel_color[6][19] = background;
	Pixel_color[6][20] = background;
	Pixel_color[6][21] = background;
	
//	Pixel_color[6][22] = background;
//	Pixel_color[2][22] = background;
//	Pixel_color[4][21] = background;

	//O
	
	Pixel_color[9][0] = background;
	Pixel_color[10][0] = background;
	Pixel_color[11][0] = background;
	Pixel_color[12][0] = background;
	Pixel_color[13][0] = background;
	Pixel_color[9][1] = background;
	Pixel_color[9][2] = background;
	Pixel_color[9][3] = background;
	Pixel_color[9][4] = background;
	Pixel_color[10][4] = background;
	Pixel_color[11][4] = background;
	Pixel_color[12][4] = background;
	Pixel_color[13][4] = background;
	Pixel_color[13][1] = background;
	Pixel_color[13][2] = background;
	Pixel_color[13][3] = background;
	
	//V
	Pixel_color[9][6] = background;
	Pixel_color[10][6] = background;
	Pixel_color[11][7] = background;
	Pixel_color[12][7] = background;
	Pixel_color[13][8] = background;
	Pixel_color[12][9] = background;
	Pixel_color[11][9] = background;
	Pixel_color[10][10] = background;
	Pixel_color[9][10] = background;
	
	//E
	Pixel_color[9][12] = background;
	Pixel_color[9][13] = background;
	Pixel_color[9][14] = background;
	Pixel_color[9][15] = background;
//	Pixel_color[9][16] = background;
	Pixel_color[10][12] = background;
	Pixel_color[11][12] = background;
	Pixel_color[12][12] = background;
	Pixel_color[13][12] = background;
	Pixel_color[13][13] = background;
	Pixel_color[13][14] = background;
	Pixel_color[13][15] = background;
//	Pixel_color[13][16] = background;
	Pixel_color[11][13] = background;
	Pixel_color[11][14] = background;
//	Pixel_color[11][15] = background;
	
	//R
	Pixel_color[9][17] = background;
	Pixel_color[9][18] = background;
	Pixel_color[9][19] = background;
//	Pixel_color[9][20] = background;
	Pixel_color[10][17] = background;
	Pixel_color[11][17] = background;
	Pixel_color[12][17] = background;
	Pixel_color[13][17] = background;
	Pixel_color[10][20] = background;
//	Pixel_color[11][20] = background;
	Pixel_color[11][19] = background;
	Pixel_color[11][18] = background;
	Pixel_color[12][19] = background;
	Pixel_color[13][20] = background;
}



int main()
{
 	utilities2.enable = 0;
	i=0;
	utilities3.a=0;
    
	title_colors_setup();
	setup();


  
    while(utilities2.enable == 0)
    {
    	if((PINB & 0x01) == 0x01)
    		utilities2.enable = 1;	
    }

//start game
	i=0;
	utilities3.a=0;
	current_columns = RIGHT_MARGIN+1;
	
	block_types_setup();
	game_colors_setup();
	
	starting_position();
	utilities2.enable = 1;
	

    while(!game_over)
    {
    
    	//rotate button
	if( utilities2.rotate == 0 )
	{


		_delay_ms(10);

		if((PINB & 0x01) == 0x01 && (utilities2.enable))		
		{
			utilities2.rotate = 1;	//means the button has been pressed
			rotate();
		}

	}

	if( (utilities2.rotate == 1) && (PINB & 0X01) == 0 ) //to check if the button was pressed but now is relesed
        {
                utilities2.rotate = 0;
        }    
        
        
        
	//right button
	if( utilities1.right == 0 )
	{


		_delay_ms(10);

		if((PINB & 0x08) == 0x08 && (utilities2.enable))		
		{
			utilities1.right = 1;	//means the button has been pressed
			move_right();
		}

	}

	if( (utilities1.right == 1) && (PINB & 0X08) == 0 ) //to check if the button was pressed but now is relesed
        {
                utilities1.right = 0;
        }



	//left button
	if( utilities1.left == 0 )
        {


                _delay_ms(10);

                if((PINB & 0x10) == 0x10 && (utilities2.enable))               
                {
                        utilities1.left = 1;       //means the button has been pressed
                        move_left();
                }

        }

        if( (utilities1.left == 1) && (PINB & 0X10) == 0 ) 			//to check if the button was pressed but now is relesed
        {
                utilities1.left = 0;
        }



	if(utilities1.row_pointer)
	{
		line_check(utilities1.row_pointer);
		utilities1.row_pointer = 0;
	}


	if(lines_completed >= DIFFICULTY_INCREASE && speed >= SPEED_LIMIT )
	{
		speed -= 5;
		lines_completed = 0;
	}
	
	
	if(utilities2.reset_position)
		starting_position();
		
	
   }
   	current_columns = COLUMNS;
   	utilities2.enable = 0;
	_delay_ms(250);
	game_over_screen();
		
	//gameover
	for(;;)
	{

	}

        return 0;
}
