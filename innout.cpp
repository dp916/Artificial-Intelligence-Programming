// Danny Pham
// Homework 2 - Hot Wheels Chess
// 10-20-18
// In N Out

#include <stdio.h>
#include <stdlib.h>
#include <cstdio>


int goFirst();
int checkPieces(int userMove, int piece);
int swap(int row);
void moveList(int userMove, int depth);
void computerMove(int *move);
void retractMove(int *move, int target, int userMove);
void makeMove(int *move, int userMove, int *target);
int min(int depth, int a, int b);
int max(int depth, int a, int b);
int evaluate();
int checkWinner();
void checkGameover();
void getMove(int *move);
void setup();
void show();


int b[8][7];
int maxdepth = 5;
int moveData[40][800];
int guessMove[40];

int count = 0;


int main()
{
    int userMove = goFirst();
    setup();
    int move[4];
    int x = 0;

    while (1)
    {
        show();
        moveList(userMove, 0);

        printf("Available Moves:");
        int i;
        for (i = 0; i < guessMove[0] * 4; i += 2)
        {
            if (i % 4 == 0)
            {
                printf(" ");
            }

            printf("%c", moveData[0][i] + 65);
            printf("%c", swap(moveData[0][i+1]) + 49);

        }
        printf("\n");
        if (userMove)
        {
            getMove(move);
            checkGameover();
            makeMove(move, userMove, &x);
            checkGameover();
        }
        else
        {
        computerMove(move);
        checkGameover();
        }
        userMove = !userMove;

    }
}
// setup board
void setup()
{
    int i,j;
    for(i = 0; i < 8; i++)
    {
        for(j = 0; j < 7 ; j++)
        {
            b[i][j] = 0;
        }
    }
    //Car C = 1000
    //Rook R = 500
    //CPU = 300
    //Knight N = 350
    //Bipshop B = 100
    //Pawn P = 100

    //Computer
    b[0][0] = 1000;
    b[2][0] = 500; b[2][1] = 500;
    b[3][0] = 300; b[3][1] = 300;
    b[2][3] = 350; b[2][4] = 350;
    b[1][1] = 100; b[2][2] = 100; b[3][3] = 100;b[3][4] = 100;
    b[3][5] = 100; b[3][6] = 100;

    //HUMAN
    b[7][0] = -1000;
    b[5][0] = -500; b[5][1] = -500;
    b[4][0] = -300; b[4][1] = -300;
    b[5][3] = -350; b[5][4] = -350;
    b[6][1] = -100; b[5][2] = -100; b[4][3] = -100; b[4][4] = -100;
    b[4][5] = -100; b[4][6] = -100;
}

//Print current position is displayed on the screen
void show()
{
    printf("\n  -----------------------  COMPUTER\n");
    int i,j;
    for(i = 0; i < 8; i++)
    {
        printf("%i ", 8-i);

        for(j = 0; j < 7; j++)
        {
            switch(b[i][j])
            {
                case 1000:
                    printf(" C ");
                    break;
                case -1000:
                    printf(" c ");
                    break;
                case 300:
                    printf(" N ");
                    break;
                case -300:
                    printf(" n ");
                    break;
                case 500:
                    printf(" R ");
                    break;
                case -500:
                    printf(" r ");
                    break;
                case 350:
                    printf(" B ");
                    break;
                case -350:
                    printf(" b ");
                    break;
                case 100:
                    printf(" P ");
                    break;
                case -100:
                    printf(" p ");
                    break;
                default:
                    printf(" - ");
            }
        }
        printf("\n");
    }

    printf("  ");
    for(i = 0; i < 7; i++)
    {
        char c = 'A';
        printf(" %c ", c + i);
    }
    printf("\n");
    printf("  -----------------------  HUMAN\n");
}

// Decide who goes first
int goFirst()
{
    char value;
    while ((value != 'y') && (value != 'Y') && (value != 'N') && (value != 'n'))
    {
        printf("Do you want to go first? Enter Y or N: ");
        scanf(" %c", &value);
    }
    if ((value == 'n') || (value == 'N'))
    {
        return (int) 0;
    }
    return (int) 1;
}

// prompt user input and change the input to progam friendly values.
void getMove(int *move)
{
    char user_input[5] = {' '};
    int x = 1;
    while (x)
    {
    	printf("Enter your move:  ");
      scanf("%4s", user_input);

        // lower case to upper case
        if (user_input[0] == 'a' || user_input[0] == 'b' || user_input[0] == 'c' || user_input[0] == 'd' || user_input[0] == 'e' || user_input[0] == 'f' || user_input[0] == 'g')
		    {
            user_input[0] = user_input[0] - 32;

        }
        if (user_input[2] == 'a' || user_input[2] == 'b' || user_input[2] == 'c' || user_input[2] == 'd' || user_input[2] == 'e' || user_input[2] == 'f' || user_input[2] == 'g')
		    {

            user_input[2] = user_input[2] - 32;
        }

        //change all ascii to int
        move[0] = (int)user_input[0] - 65; // upper
        move[1] = (int)user_input[1] - 49;
        move[2] = (int)user_input[2] - 65;
        move[3] = (int)user_input[3] - 49;
        int i = 0;

        //compare user user_input to the move generated from moveList
        while(i < guessMove[0] * 4)
        {
            if (moveData[0][i] == move[0] && swap(moveData[0][i+1] == move[1] )&& moveData[0][i+2] == move[2] && swap(moveData[0][i+3] == move[3]))
            {
                x = 0;
            }
            i += 4;

        }
        if (x)
        {

            printf("Please enter a move from above\n");
        }

    }
    //set legal user_input into movep[]
    move[0] = (int) user_input[0] - 65;
    move[1] = swap((int) user_input[1] - 49);
    move[2] = (int) user_input[2] - 65;
    move[3] = swap((int) user_input[3] - 49);

}

int checkWinner()
{
    if (b[3][6] == 1000)
    {
      return 9999;
    }
    else if (b[4][6] == -1000)
    {
      return -9999;
    }
    else

    return 1;
}

void checkGameover()
{
  show();

  if (checkWinner() == 9999)
  {
    printf("\n\n\nComputer winner\n\n\n");
    exit(0);
  }
  if (checkWinner() == -9999)
  {
    printf("\n\n\nHuman winner\n\n\n");
    exit(0);
  }
}

// check if the piece is a correct piece for the player or computer
int checkPieces(int userMove, int piece)
{
    if ((userMove && piece > 0) || (!userMove && piece < 0))
    {
        return 1;
    }
    return 0;
}

int swap(int x)
{
    switch(x)
    {
      case 0: return 7; break;
      case 1: return 6; break;
      case 2: return 5; break;
      case 3: return 4; break;
      case 4: return 3; break;
      case 5: return 2; break;
      case 6: return 1; break;
      case 7: return 0; break;
      default: return x; break;
    }
}

// heuristic
int evaluate()
{
    int i,j;
    int total;
    for(i = 0; i < 8; i++)
    {
        for(j = 0; j < 7; j++)
        {
            total = total + b[i][j];
        }
    }
    return total;
}


//mini
int min(int depth, int a, int b)
{
    count++;

    if (depth > maxdepth)
    {
        return evaluate();
    }

    moveList(1,depth);

    int i,score;
    int target = 0;
    int bestScore = 9999;
    int move[4];

    for(i = 0; i < guessMove[depth] * 4; i = i+4)
    {
      move[0] = moveData[depth][i];
      move[1] = moveData[depth][i+1];
      move[2] = moveData[depth][i+2];
      move[3] = moveData[depth][i+3];

      makeMove(move,1,&target);

      score = max(depth+1,a,bestScore);

      if (score < bestScore)
      {
        bestScore = score;
      }

      retractMove(move, target, 1);
      target = 0;

      if (score <= a)
      {
        return bestScore;
      }
    }
    return bestScore;

}

//max
int max(int depth, int a, int b)
{
    count++;

    if (depth > maxdepth)
    {
        return evaluate();
    }
    moveList(0,depth);

    int i,score;
    int target = 0;
    int bestScore = -9999;
    int move[4];

    for(i = 0; i < guessMove[depth]*4; i = i+4)
    {
      move[0] = moveData[depth][i];
      move[1] = moveData[depth][i+1];
      move[2] = moveData[depth][i+2];
      move[3] = moveData[depth][i+3];

      makeMove(move,0,&target);

      score = min(depth+1,bestScore,b);

      if (score > bestScore)
      {
        bestScore = score;
      }
      retractMove(move, target, 0);

      target = 0;
      if (score >= b)
      {
        return bestScore;
      }
    }
    return bestScore;
}


void makeMove(int *move, int userMove, int *target)
{
	b[move[3]][move[2]] = b[move[1]][move[0]];
	b[move[1]][move[0]] = 0;
    if (userMove && move[3] > 0)
    {

		if (b[move[3]][move[2]] == 100)
    {
			b[move[3]][move[2]] = 0;
      *target = 100;
		}
		else if(b[move[3]][move[2]] == 300)
    {
			b[move[3]][move[2]]= 0;
		  *target = 300;
		}
    else if(b[move[3]][move[2]] == 350)
    {
			b[move[3]][move[2]] = 0;
		 	*target = 350;
		}
		else if(b[move[3]][move[2]] == 500)
    {
			b[move[3]][move[2]] = 0;
		 	*target = 500;
		}
    else if(b[move[3]][move[2]] == 1000)
    {
			b[move[3]][move[2]] = 0;
		 	*target = 1000;
		}
		else
    {
			*target = 0;
		}
	}


	else if (!userMove && move[3] <= 8 )
  {
		if (b[move[3]][move[2]] == -100)
    {
			b[move[3]][move[2]] = 0;
      *target = -100;
		}
		else if (b[move[3]][move[2]] == -300)
    {
			b[move[3]][move[2]] = 0;
		 	*target = -300;
		}
    else if (b[move[3]][move[2]] == -350)
    {
			b[move[3]][move[2]] = 0;
		 	*target = -350;
		}
		else if(b[move[3]][move[2]] == -500)
    {
			b[move[3]][move[2]] = 0;
		 	*target = -500;
		}
    else if(b[move[3]][move[2]] == -1000)
    {
			b[move[3]][move[2]] = 0;
		 	*target = -1000;
		}
		else
    {
			*target = 0;
		}
		}

}

//computer move
void computerMove(int *move)
{
    int bestScore = -9999;
    int bestMove[4];
    int i,score;
    int target = 0;

    moveList(0, 0);

    for(i = 0; i < guessMove[0]*4; i+=4)
    {
        target=0;
        move[0] = moveData[0][i];
        move[1] = moveData[0][i+1];
        move[2] = moveData[0][i+2];
        move[3] = moveData[0][i+3];

        makeMove(move, 0, &target);

        score = min(1,-9999,9999);

        if (score > bestScore)
        {
            bestScore = score;
            bestMove[0] = move[0];
            bestMove[1] = move[1];
            bestMove[2] = move[2];
            bestMove[3] = move[3];
        }
        retractMove(move, target, 0);
    }
    printf("Computer Move: %c%c%c%c", bestMove[0]+65, (swap(bestMove[1])+49),bestMove[2]+65 ,(swap(bestMove[3])+49));
    makeMove(bestMove, 0, &target);
    printf(" (%c%c%c%c)\n", bestMove[0]+65, (bestMove[1])+49,bestMove[2]+65 ,(bestMove[3])+49);

}

//retract move for minimax
void retractMove(int *move, int target, int userMove)
{
    int a1,a2,a3,a4;
    a1 = move[0];
    a2 = move[1];
    a3 = move[2];
    a4 = move[3];

    b[a2][a1] = b[a4][a3];
    b[a4][a3] = 0;

    if (userMove && target != 0)
    {
        b[a4][a3] = target;
    }
    if(!userMove && target != 0)
    {
        b[a4][a3] = target;
    }
}

void moveList(int userMove, int depth)
{
  int count = 0;
  int i,j;
  int size;
  int valid;
  if (userMove)
  {

    for(i=0;i<8;i++)
    {
      for(j=0;j<7;j++)
      {

// HUMAN PAWN

        if(b[i][j] == -100)
        {
        // UP 1
          if (i > 0 && b[i-1][j] == 0 )
          {
            moveData[depth][count * 4] = j;
            moveData[depth][count * 4 + 1] = i;
            moveData[depth][count * 4 + 2] = j;
            moveData[depth][count * 4 + 3] = i-1;
            count++;
          }

          // UP 1 RIGHT 1

          if (b[i-1][j+1] == 0)
          {
            moveData[depth][count * 4 + 1] = i;
            moveData[depth][count * 4] = j;
            moveData[depth][count * 4 + 3] = i-1;
            moveData[depth][count * 4 + 2] = j+1;
            count++;
          }

          // UP 1 LEFT 1
          if (b[i-1][j-1] == 0)
          {
            moveData[depth][count * 4 + 1] = i;
            moveData[depth][count * 4] = j;
            moveData[depth][count * 4 + 3] = i-1;
            moveData[depth][count * 4 + 2] = j-1;
            count++;
          }

        }

// HUMAN BISHOP

        else if(b[i][j] == -350)
        {
          int valid = 0;
          size = 1;
          while(!valid)
          {
          //check the border
            if (b[i-size][j-size] != 0 || i-size < 0 || j-size < 0)
            {
              valid = 1;
            }
            //check top left
            if(b[i-size][j-size] == 0 && i > 0 && j-size >= 0 && !valid)
            {
              moveData[depth][count * 4] = j;
              moveData[depth][count * 4 + 1] = i;
              moveData[depth][count * 4 + 2] = j-size;
              moveData[depth][count * 4 + 3] = i-size;
              count++;
            }
            size++;
          }
          valid = 0;
          size = 1;
          while(!valid)
          {
          //check top right
            if (b[i-size][j+size] != 0 ||  i-size < 0 || j-size < 0)
            {
              valid = 1;
            }
            if (b[i-size][j+size] == 0 && i > 0 && j+size <= 6 && !valid)
            {
              moveData[depth][count * 4] = j;
              moveData[depth][count * 4 + 1] = i;
              moveData[depth][count * 4 + 2] = j+size;
              moveData[depth][count * 4 + 3] = i-size;
              count++;
            }
            size++;
          }
          valid = 0;
          size = 1;
          while(!valid)
          {
            if (b[i+size][j+size] != 0 || i-size < 0 || j+size > 6)
            {
              valid = 1;
            }
            //check bottom right
            if (b[i+size][j+size] == 0 && i > 0 && j < 6 && !valid)
            {
              moveData[depth][count * 4] = j;
              moveData[depth][count * 4 + 1] = i;
              moveData[depth][count * 4 + 2] = j+size;
              moveData[depth][count * 4 + 3] = i+size;
              count++;
            }
            size++;
          }
          valid = 0;
          size = 1;
          while (!valid)
          {
          //check bottom left
            if (b[i+size][j-size] != 0 || j-size < 0 || i+size > 8)
            {
              valid = 1;
            }
            if (b[i+size][j-size] == 0 && i > 0 && j < 6 && !valid )
            {
              moveData[depth][count * 4] = j;
              moveData[depth][count * 4 + 1] = i;
              moveData[depth][count * 4 + 2] = j-size;
              moveData[depth][count * 4 + 3] = i+size;
              count++;
            }
            size++;
          }
        }

// HUMAN KING

        else if(b[i][j] == -1000)
        {
        //check right
          if (b[i][j+1] == 0 && b[i][j] != -1000)
          {
            moveData[depth][count * 4 + 1] = i;
            moveData[depth][count * 4] = j;
            moveData[depth][count * 4 + 3] = i;
            moveData[depth][count * 4 + 2] = j+1;
            count++;
          }
          //check top right
          if (b[i-1][j+1] == 0)
          {
            moveData[depth][count * 4 + 1] = i;
            moveData[depth][count * 4] = j;
            moveData[depth][count * 4 + 3] = i-1;
            moveData[depth][count * 4 + 2] = j+1;
            count++;
          }

        }

// HUMAN KNIGHT

        else if(b[i][j] == -300)
        {
        //check top left UP
          if (b[i-2][j-1] == 0 && i-2 >= 0 && j-1 >= 0)
          {
            moveData[depth][count * 4 + 1] = i;
            moveData[depth][count * 4] = j;
            moveData[depth][count * 4 + 3] = i-2;
            moveData[depth][count * 4 + 2] = j-1;
            count++;
          }
          //check top right UP
          if (b[i-2][j+1] == 0 && i-2 >= 0 && j-1 <= 6)
          {
            moveData[depth][count * 4 + 1] = i;
            moveData[depth][count * 4] = j;
            moveData[depth][count * 4 + 3] = i-2;
            moveData[depth][count * 4 + 2] = j+1;
            count++;
          }
          //check top left Down
          if (b[i-1][j-2] == 0 && i-1 >= 0 && j-2 >= 0)
          {
            moveData[depth][count * 4 + 1] = i;
            moveData[depth][count * 4] = j;
            moveData[depth][count * 4 + 3] = i-1;
            moveData[depth][count * 4 + 2] = j-2;
            count++;
          }
          //check top right Down
          if (b[i-1][j+2] == 0 && i-1 >= 0 && j+2 <= 6)
          {
            moveData[depth][count * 4 + 1] = i;
            moveData[depth][count * 4] = j;
            moveData[depth][count * 4 + 3] = i-1;
            moveData[depth][count * 4 + 2] = j+2;
            count++;
          }
          //check bottom left Up
          if (b[i+1][j-2] == 0 && i+1 <= 8 && j-2 >= 0)
          {
            moveData[depth][count * 4 + 1] = i;
            moveData[depth][count * 4] = j;
            moveData[depth][count * 4 + 3] = i+1;
            moveData[depth][count * 4 + 2] = j-2;
            count++;
          }
          //check bottom left Down
          if (b[i+2][j-1] == 0 && i+2 <= 8 && j-1 >= 0)
          {
            moveData[depth][count * 4 + 1] = i;
            moveData[depth][count * 4] = j;
            moveData[depth][count * 4 + 3] = i+2;
            moveData[depth][count * 4 + 2] = j-1;
            count++;
          }
          //check bottom right Up
          if (b[i+1][j+2] == 0 && i+1 <= 8 && j+2 <= 6)
          {
            moveData[depth][count * 4 + 1] = i;
            moveData[depth][count * 4] = j;
            moveData[depth][count * 4 + 3] = i+1;
            moveData[depth][count * 4 + 2] = j+2;
            count++;
          }
          //check bottom right Bottom
          if (b[i+2][j+1] == 0 && i+2 <= 8 && j+1 <= 6)
          {
            moveData[depth][count * 4 + 1] = i;
            moveData[depth][count * 4] = j;
            moveData[depth][count * 4 + 3] = i+2;
            moveData[depth][count * 4 + 2] = j+1;
            count++;
          }
        }
// HUMAN ROOK
			  else if(b[i][j] == -500)
        {
        //if there is a piece on path
          int valid = 0;
          //check vertical
          for (size=i;size>0;size--)
          {
            if (b[size-1][j] ==0 && !valid)
            {
              moveData[depth][count * 4] = j;
              moveData[depth][count * 4 + 1] = i;
              moveData[depth][count * 4 + 2] = j;
              moveData[depth][count * 4 + 3] = size-1;
              count++;
            }
            else
            {
              valid = 1;
            }
          }
          //check horizontal left
          valid = 0;
          for (size = j-1; size >= 0; size--)
          {
            if (checkPieces(userMove, b[i-1][size]) && b[i][size] == 0 && !valid)
            {
              moveData[depth][count * 4] = j;
              moveData[depth][count * 4 + 1] = i;
              moveData[depth][count * 4 + 2] = size;
              moveData[depth][count * 4 + 3] = i;
              count++;
              break;
            }
            if (b[i][size] != 0)
            {
              valid=1;
              break;
            }
          }
          //check horizontal right
          valid=0;
          for (size=j+1;size<7;size++)
          {
            if (checkPieces(userMove, b[i-1][size]) && b[i][size] == 0 && !valid)
            {
              moveData[depth][count * 4] = j;
              moveData[depth][count * 4 + 1] = i;
              moveData[depth][count * 4 + 2] = size;
              moveData[depth][count * 4 + 3] = i;
              count++;
              break;
            }
            if (b[i][size] != 0)
            {
              valid=1;
            }
          }
        }
      }
    }
  }


  //Computer same with inverted +/-

  else
  {
    for (i = 0; i < 8; i++)
    {
            for (j = 0; j < 7; j++)
            {

// CPU PAWN

              if(b[i][j] == 100)
              {
                // check top Left

                if(b[i+1][j+1] == 0  && i+1 <= 8 && j+1 <= 7)
                {
                  moveData[depth][count * 4] = j;
                  moveData[depth][count * 4 + 1] = i;
                  moveData[depth][count * 4 + 2] = j+1;
                  moveData[depth][count * 4 + 3] = i+1;
                  count++;
                }

                //Check top Right

                if (b[i+1][j-1] == 0  && i > 0 && j < 6)
                {
                  moveData[depth][count * 4] = j;
                  moveData[depth][count * 4 + 1] = i;
                  moveData[depth][count * 4 + 2] = j-1;
                  moveData[depth][count * 4 + 3] = i+1;
                  count++;
                }

                //Check forward
                if (i > 0 && b[i+1][j] == 0)
                {
                  moveData[depth][count * 4] = j;
                  moveData[depth][count * 4 + 1] = i;
                  moveData[depth][count * 4 + 2] = j;
                  moveData[depth][count * 4 + 3] = i+1;
                  count++;
                }
              }
// CPU BISHOP
              else if (b[i][j] == 350)
              {
                int valid = 0;
                size = 1;
                while (!valid)
                {
                  // Check top Left
                  if (b[i+size][j+size] != 0 || j+size > 6 || i+size >8)
                  {
                    valid = 1;
                  }
                  if (b[i+size][j+size] == 0 &&  i+size <= 8 && j+size<=6 )
                  {
                    moveData[depth][count * 4] = j;
                    moveData[depth][count * 4 + 1] = i;
                    moveData[depth][count * 4 + 2] = j+size;
                    moveData[depth][count * 4 + 3] = i+size;
                    count++;
                  }
                  size++;
                }

                valid = 0;
                size = 1;

                while (!valid)
                {
                  //Check Top Right
                  if (b[i+size][j-size] != 0 || i+size > 8 || j-size > 6)
                  {
                    valid = 1;
                  }
                  if (b[i+size][j-size] == 0 && i+size<=8 && j-size>=0 && !valid)
                  {
                    moveData[depth][count * 4] = j;
                    moveData[depth][count * 4 + 1] = i;
                    moveData[depth][count * 4 + 2] = j-size;
                    moveData[depth][count * 4 + 3] = i+size;
                    count++;
                  }
                  size++;
                }
                valid = 0;
                size = 1;
                while (!valid)
                {
                  //Check Bottom right
                  if (b[i-size][j-size] != 0 || j-size > 6 ||i-size>8 )
                  {
                    valid = 1;
                  }
                  if (b[i-size][j-size] == 0 && i-size >=0 && j-size>=0 && !valid)
                  {
                    moveData[depth][count * 4] = j;
                    moveData[depth][count * 4 + 1] = i;
                    moveData[depth][count * 4 + 2] = j-size;
                    moveData[depth][count * 4 + 3] = i-size;
                    count++;
                  }
                  size++;
                }
                valid = 0;
                size = 1;
                while (!valid)
                {
                  //Check Bottom Left
                  if (b[i-size][j+size] != 0 || i-size > 8 || j-size < 0)
                  {
                    valid = 1;
                  }
                  if (b[i-size][j+size] == 0 && i-size >=0 && j+size <=6 &&!valid)
                  {
                    moveData[depth][count * 4] = j;
                    moveData[depth][count * 4 + 1] = i;
                    moveData[depth][count * 4 + 2] = j+size;
                    moveData[depth][count * 4 + 3] = i-size;
                    count++;
                  }
                  size++;
                }

              }//End UPC Bishop move
// CPU KING
              else if (b[i][j] == 1000)
              {
                //check topleft
                if (b[i+1][j+1] == 0 && i+1 <= 8 && j <= 6)
                {
                  moveData[depth][count * 4 + 1] = i;
                  moveData[depth][count * 4] = j;
                  moveData[depth][count * 4 + 3] = i+1;
                  moveData[depth][count * 4 + 2] = j+1;
                  count++;
                }
                //check topRight X
                if (b[i+1][j-1] == 0 && i+1 <=8  && j -1 >= 0)
                {
                  moveData[depth][count * 4 + 1] = i;
                  moveData[depth][count * 4] = j;
                  moveData[depth][count * 4 + 3] = i+1;
                  moveData[depth][count * 4 + 2] = j-1;
                  count++;
                }
                //check forward |
                if (i > 0 && b[i+1][j] == 0)
                {
                  moveData[depth][count * 4] = j;
                  moveData[depth][count * 4 + 1] = i;
                  moveData[depth][count * 4 + 2] = j;
                  moveData[depth][count * 4 + 3] = i+1;
                  count++;
                  }

              }
// CPU KNIGHT
              else if (b[i][j] == 300)
              {
                //check top left UP
                if (b[i+2][j+1] == 0 && i+2 <= 8 && j+1 <= 6)
                {
                  moveData[depth][count * 4 + 1] = i;
                  moveData[depth][count * 4] = j;
                  moveData[depth][count * 4 + 3] = i+2;
                  moveData[depth][count * 4 + 2] = j+1;
                  count++;
                }
                //Check top Right UP
                if (b[i+2][j-1] == 0 && i+2 <= 8 && j-1 >= 0)
                {
                  moveData[depth][count * 4 + 1] = i;
                  moveData[depth][count * 4] = j;
                  moveData[depth][count * 4 + 3] = i+2;
                  moveData[depth][count * 4 + 2] = j-1;
                  count++;
                }
                //check top left Down
                if (b[i+1][j+2] == 0 && i+1 <= 8 && j+2 <= 6)
                {
                  moveData[depth][count * 4 + 1] = i;
                  moveData[depth][count * 4] = j;
                  moveData[depth][count * 4 + 3] = i+1;
                  moveData[depth][count * 4 + 2] = j+2;
                  count++;
                }
                //check top Right Down
                if (b[i+1][j-2] == 0 && i+1 <= 8 && j-2 >= 0)
                {
                  moveData[depth][count * 4 + 1] = i;
                  moveData[depth][count * 4] = j;
                  moveData[depth][count * 4 + 3] = i+1;
                  moveData[depth][count * 4 + 2] = j-2;
                  count++;
                }
                //check Bottom Left Up
                if (b[i-1][j+2] == 0 && i-1 >= 0 && j+2 <= 6)
                {
                  moveData[depth][count * 4 + 1] = i;
                  moveData[depth][count * 4] = j;
                  moveData[depth][count * 4 + 3] = i-1;
                  moveData[depth][count * 4 + 2] = j+2;
                  count++;
                }
                //check BottomLeft Bottow
                if (b[i-2][j+1] == 0 && i-2 >= 0 && j+1 <= 6)
                {
                  moveData[depth][count * 4 + 1] = i;
                  moveData[depth][count * 4] = j;
                  moveData[depth][count * 4 + 3] = i-2;
                  moveData[depth][count * 4 + 2] = j+1;
                  count++;
                }
                //check Bottom Right Up
                if (b[i-1][j-2] == 0 && i-1 >= 0 && j-2 >= 0)
                {
                  moveData[depth][count * 4 + 1] = i;
                  moveData[depth][count * 4] = j;
                  moveData[depth][count * 4 + 3] = i-1;
                  moveData[depth][count * 4 + 2] = j-2;
                  count++;
                }
                //check Bottom Right Bottom
                if (b[i-2][j-1] == 0 && i-2 >= 0 && j-1 >=0)
                {
                  moveData[depth][count * 4 + 1] = i;
                  moveData[depth][count * 4] = j;
                  moveData[depth][count * 4 + 3] = i-2;
                  moveData[depth][count * 4 + 2] = j-1;
                  count++;
                }

// CPU ROOK
				        else if (b[i][j] == 500)
                {
                //if piece on path
                  int valid = 0;
                  //check vertical
                  for (size = i; size <=8; size++)
                  {
                    if (b[size+1][j] == 0 && !valid)
                    {
                      moveData[depth][count * 4] = j;
                      moveData[depth][count * 4 + 1] = i;
                      moveData[depth][count * 4 + 2] = j;
                      moveData[depth][count * 4 + 3] = size+1;
                      count++;
                    }
                    else
                    {
                      valid = 1;
                    }
                  }
                  //check horizontal left
                  valid = 0;
                  for (size = j+1 ; size<7; size++)
                  {
                    if (checkPieces(userMove, b[i-1][size]) && b[i][size] == 0 && !valid)
                    {
                      moveData[depth][count * 4] = j;
                      moveData[depth][count * 4 + 1] = i;
                      moveData[depth][count * 4 + 2] = size;
                      moveData[depth][count * 4 + 3] = i;
                      count++;
                      break;
                    }
                    if (b[i][size] != 0)
                    {
                      valid = 1;
                      break;
                    }
                  }
                  //check horizontal right
                  valid = 0;
                  for (size =j-1; size>=0; size--)
                  {
                    if (checkPieces(userMove, b[i-1][size]) && b[i][size] == 0 && !valid)
                    {
                      moveData[depth][count * 4] = j;
                      moveData[depth][count * 4 + 1] = i;
                      moveData[depth][count * 4 + 2] = size;
                      moveData[depth][count * 4 + 3] = i;
                      count++;
                      break;
                    }
                    if (b[i][size] != 0)
                    {
                      valid = 1;
                      break;
                    }
                  }
                }
        }
      }
    }
  }
  guessMove[depth] = count;
}
