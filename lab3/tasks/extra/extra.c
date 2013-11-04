/** @file extra.c
 *
 * @brief Echos characters back with timing data.
 *
 * Links to libc.
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
//#include <time.h>
int random(int n);
void get_name(int n, char* str);
void game1();
void game2();
void ani_print(char* str, int ms);
void game2_rec(int sleep_time, int rec_level, int curr_level);
int judge(int ai, int user);

void game3();
void print_game3(int* keys);
void generate_random_array(int* keys, int n);
int get_pos(char c);

#define ROCK 0
#define PAPER 1
#define SCISSOR 2
#define LIZARD 3
#define SPOCK 4

#define GAME1 1
#define GAME2 2
#define GAME3 3

int main(int argc, char** argv)
{
    
    printf("please select a program:\n");
    printf("1: rock scissor paper\n");
    printf("2: recursive sleep\n");
    printf("3: Whack-A-Mole\n");
    char choice;
    char ignore;
    read(0, &choice, 1);
    read(0, &ignore, 1);
    choice = choice - '0';
    //printf("%d",choice);
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
        default:
            printf("please stick to the instruction:\n");
            break;
    }
    return 0;
} 



int random(int n){
    //srand((unsigned int)time(NULL));
    int current_time = time() / 10;
    int mode = current_time % n;
    //int mode = rand() %n;
    return mode;
}

void game1(){
    while(1){
        int ai = random(5);
        char str[10];
        get_name(ai, str);
        printf("Please choose one weapon:\n");
        printf("0: Rock 1: Paper 2: Scissor 3: Lizard 4: Spock\n");
        char user_choice;
        char ignore;
        read(0, &user_choice, 1);
        read(0, &ignore, 1);    
        
        if (user_choice > '4' || user_choice < '0') {
            break;
        }
        user_choice = user_choice - '0';
        //printf("%d\n",user_choice);
        char user_str[10];
        get_name(user_choice, user_str);
        int result = judge(ai,user_choice);

        if(result == 1 ){
            printf("You win! AI: %s, you: %s\n",str,user_str);
        } else if(result == -1){
            printf("You loss!AI: %s, you: %s\n",str,user_str);
        } else {
            printf("Tie!AI: %s, you: %s\n",str,user_str);
        }
    }


}

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
void game2(){
    char str[] = "Welcome\n";
    ani_print(str, 200);
    char sleep_time, rec_level;
    printf("Please input sleep time and recursive level: e.g: 5 2\n");
    
    char ignore;
    read(0, &sleep_time, 1);
    read(0, &ignore, 1);
    read(0, &rec_level, 1);
    read(0, &ignore, 1);
    sleep_time = sleep_time - '0';
    rec_level = rec_level - '0';
    int sleep;
    sleep = sleep_time *1000;
    int i = 1;

    game2_rec(sleep,rec_level,i);
    printf("You are AWAKE!!\n");


}

void game2_rec(int sleep_time, int rec_level, int curr_level){
    
    
    printf("You are in level %d.\n",curr_level);
    int level = curr_level;
    sleep(sleep_time);
    curr_level++;
    if(curr_level > rec_level){
        return;
    }
    game2_rec(sleep_time,rec_level,curr_level);
    printf("You are back in level %d.\n",level);
    
}

void ani_print(char* str, int ms){

    int len = strlen(str);
    int i;
    for(i = 0;i< len;i++){
        write(1,&str[i],1);
        sleep(ms);
    }
    return;


}

void game3(){

    int keys[9] = {0,0,0,0,0,0,0,0,0};

    generate_random_array(keys,4);
    print_game3(keys);
    char input;
    time_t start = time(NULL);
    while(read(0,&input,1)>0){
        int pos = get_pos(input);
        if(pos < 0)
            continue;
        keys[pos] = 0;
        time_t end = time(NULL);
        if((end - start )>2){
            generate_random_array(keys,1);
            start = time(NULL);
        }
        int i = 0;
        
        print_game3(keys);
    }   
}

void generate_random_array(int* keys, int n){
    
    int count = 0;
    while(count < n){
        int i = random(8);
        if(keys[i] == 1)
            continue;
        else{
            keys[i] =1;
            count++;
        }
    }
}

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
void print_game3(int* keys){
    

    char line0[] = "+--+--+--+";
    char line1[] = "|  |  |  |";
    char line2[] = "|  |  |  |";
    char line3[] = "|  |  |  |";

    line1[1] = keys[0]?'x': ' ';    
    line1[4] = keys[1]?'x': ' ';
    line1[7] = keys[2]?'x': ' ';
    line2[1] = keys[3]?'x': ' ';
    line2[4] = keys[4]?'x': ' ';
    line2[7] = keys[5]?'x': ' ';
    line3[1] = keys[6]?'x': ' ';
    line3[4] = keys[7]?'x': ' ';
    line3[7] = keys[8]?'x': ' ';
    const char* CLEAR_SCREE_ANSI = "\e[1;1H\e[2J";
    write(1,CLEAR_SCREE_ANSI,12);
    printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n",line0,line1,line0,line2,line0,line3,line0);
}
