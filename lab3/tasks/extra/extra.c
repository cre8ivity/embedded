/** @file extra.c
 *
 * @brief Echos characters back with timing data.
 *
 * Authors: DANLEI ZHOU 1 <danleiz> 
 *          ZHUOLIN LIU 2 <zhuolinl>
 *          QIAN MAO 3 <qianm>
 *          
 * Links to libc.
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>

void game1();
void game2();
void game3();

int random(int n);
void get_name(int n, char* str);
int judge(int ai, int user);

void ani_print(char* str, int ms);
void game2_rec(int sleep_time, int rec_level, int curr_level);

void print_game3(int* keys);
void generate_random_array(int* keys, int n,int forbidden);
int get_pos(char c);

#define ROCK 0
#define PAPER 1
#define SCISSOR 2
#define LIZARD 3
#define SPOCK 4

#define GAME1 1
#define GAME2 2
#define GAME3 3
#define EXIT 4

/*
 * main function to display usage and redirect to each program
 */
int main(int argc, char** argv)
{
    while(1){
        // print usage
        printf("please select a program:\n");
        printf("1: rock scissor paper\n");
        printf("2: recursive sleep\n");
        printf("3: Whack-A-Mole\n");
        printf("4: Exit\n");
        char choice;
        char ignore;

        // wait user to input game index
        read(STDIN_FILENO, &choice, 1);
        read(STDIN_FILENO, &ignore, 1);
        choice = choice - '0';
        switch(choice){
            case GAME1:
                game1();
                break;
            case GAME2:
                game2();
                break;
            case GAME3:
                game3();
                break;
            case EXIT:
                return 0;
            default:
                printf("please stick to the instruction:\n");
                break;
        }

    }
    return 0;
} 


/*
 * function to get a random integer between 0-n
 * based on time
 */
int random(int n){

    int current_time = time() / 10;
    int mode = current_time % n;
    return mode;
}

/*
 * game1 main function 
 */
void game1(){
    while(1){
        // ai weapon and get it
        int ai = random(5);
        char str[10];
        get_name(ai, str);

        // print usage
        printf("Please choose one weapon:\n");
        printf("0: Rock 1: Paper 2: Scissor 3: Lizard 4: Spock\n");

        char user_choice;
        char ignore;

        // wait user to get his weapon
        read(STDIN_FILENO, &user_choice, 1);
        read(STDIN_FILENO, &ignore, 1);    
        
        // error input
        if (user_choice > '4' || user_choice < '0') {
            break;
        }
        user_choice = user_choice - '0';
        char user_str[10];

        // get user weapon
        get_name(user_choice, user_str);

        // get result and print
        int result = judge(ai,user_choice);
        if(result == 1 ){
            printf("You win! AI: %s, you: %s\n",str,user_str);
        } else if(result == -1){
            printf("You loss! AI: %s, you: %s\n",str,user_str);
        } else {
            printf("Tie! AI: %s, you: %s\n",str,user_str);
        }
    }
}

/*
 * decide the result of game1
 */
int judge(int ai, int user){
    int result = -1;
    switch(ai){
        case ROCK:
            if(user == PAPER || user == SPOCK)
                result = 1;
            else if(user == ROCK)
                result = 0;
            break;

        case PAPER:
            if(user == SCISSOR || user == LIZARD)
                result = 1;
            else if(user == PAPER)
                result = 0;
            break;
            
        case SCISSOR:
            if(user == SPOCK || user == ROCK)
                result = 1;
            else if(user == SCISSOR)
                result = 0;
            break;

        case LIZARD:
            if(user == SCISSOR || user == ROCK)
                result = 1;
            else if(user == LIZARD)
                result = 0;
            break;
            
        case SPOCK:
            if(user == PAPER || user == LIZARD)
                result = 1;
            else if(user == SPOCK)
                result = 0;
            break;
    }
    return result;

}

/*
 * get the name of each weapon number, used for display result
 */
void get_name(int n, char* str){
    switch(n){
        case ROCK:
            memcpy(str, "Rock", 10);
            break;       
        case PAPER:
            memcpy(str, "Paper", 10);
            break;     
        case SCISSOR:
            memcpy(str, "Scissor", 10);
            break;   
        case LIZARD:
            memcpy(str, "Lizard", 10);
            break; 
        case SPOCK:
            memcpy(str, "Spock", 10);
            break;
    }
}

/*
 * game 2 main function
 */
void game2(){
    // print welcome message using sleep
    char str[] = "Welcome to inception\n";
    ani_print(str, 200);

    // wait and get user input
    char sleep_time, rec_level;
    printf("Please input sleep time and recursive level: e.g: 5 2\n");    
    char ignore;
    read(STDIN_FILENO, &sleep_time, 1);
    read(STDIN_FILENO, &ignore, 1);
    read(STDIN_FILENO, &rec_level, 1);
    read(STDIN_FILENO, &ignore, 1);
    sleep_time = sleep_time - '0';
    rec_level = rec_level - '0';

    int sleep;
    sleep = sleep_time * 1000;
    int i = 1;

    // go the the inception
    game2_rec(sleep,rec_level, i);
    printf("You are AWAKE!!\n");
}

/*
 * game2 recursive helper, return when curr_level hit the bottom level.
 */
void game2_rec(int sleep_time, int rec_level, int curr_level){
    // print sleep level 
    printf("You are in level %d.\n", curr_level);
    int level = curr_level;
    sleep(sleep_time);
    curr_level++;
    if(curr_level > rec_level){
        return;
    }
    // recursive sleep
    game2_rec(sleep_time, rec_level, curr_level);
    printf("You are back in level %d.\n", level);
}

/*
 * helper function, which can dymanically display a string in a given speed.
 */
void ani_print(char* str, int ms){

    int len = strlen(str);
    int i;
    for(i = 0;i < len;i++){
        write(STDOUT_FILENO, &str[i], 1);
        sleep(ms);
    }
    return;
}

/*
 * Whack a Mole(3*3) main function,
 * in the beginning, there are four moles. and a new mole will generate each 0.5s.
 * user can hit q,w,e,a,s,d,z,x,c for according squares to whack the mole. 
 */
void game3(){

    int keys[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    // generate the initial moles layout
    generate_random_array(keys, 4, 9);
    // print inital matrix
    print_game3(keys);
    char input;
    int count = 4;
    int start = time();

    // read and handle user input
    while(read(STDIN_FILENO, &input, 1)>0){
        int pos = get_pos(input);
        if(pos < 0)
            break;
        if(keys[pos] == 1){
            count--;
            keys[pos] = 0;
        }

        int end = time();
        //generate a user moles if time over 0.5 s
        if((end - start ) > 500 && count < 8 ){
            generate_random_array(keys, 1, pos);
            count++;
            start = time();
        }
        // print new matrix
        print_game3(keys);
    }   
}

/*
 * helper function to randomly generate n more moles, 
 * the forbidden position cannot be occupied.
 * fill up the place of the random output
 */
void generate_random_array(int* keys, int n, int forbidden){
    
    int count = 0;
    while(count < n){
        int i = random(8);
        // if user just bit a mole, the mole will not 
        // re-appear in that hole
        if(keys[i] == 1 || i == forbidden)
            continue;
        else{
            keys[i] =1;
            count++;
        }
    }
}

/*
 * helper function to match user input to certain spot. 
 * will return -1 when wrong key is hit.
 */
int get_pos(char c){
    switch(c){
        case 'q':
            return 0;
        case 'w':
            return 1;
        case 'e':
            return 2;
        case 'a':
            return 3;
        case 's':
            return 4;
        case 'd':
            return 5;
        case 'z':
            return 6;
        case 'x':
            return 7;
        case 'c':
            return 8;
        default:
            return -1;
    }
}

/*
 * helper function to print the game
 */
void print_game3(int* keys){
    
    char line0[] = "+--+--+--+";
    char line1[] = "|  |  |  |";
    char line2[] = "|  |  |  |";
    char line3[] = "|  |  |  |";
    char hit = 'x';
    char space = ' ';

    line1[1] = keys[0]? hit : space;    
    line1[4] = keys[1]? hit: space;
    line1[7] = keys[2]? hit: space;
    line2[1] = keys[3]? hit: space;
    line2[4] = keys[4]? hit: space;
    line2[7] = keys[5]? hit: space;
    line3[1] = keys[6]? hit: space;
    line3[4] = keys[7]? hit: space;
    line3[7] = keys[8]? hit: space;

    //clear the screen
    const char* CLEAR_SCREE_ANSI = "\e[1;1H\e[2J";
    write(1,CLEAR_SCREE_ANSI, 12);
    //print the new matrix
    printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n", line0, line1, line0, line2, line0, line3, line0);
}
