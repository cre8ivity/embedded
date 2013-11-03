/** @file game.c
 *
 * @brief Echos characters back with timing data.
 *
 * Links to libc.
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>

int random(int n);
char* get_name(int n);
void game1();
void game2();
void ani_print(char* str, int ms);
void game2_rec(int sleep_time, int rec_level, int curr_level);


#define ROCK 0
#define PAPER 1
#define SCISSOR 2
#define LIZARD 3
#define SPOCK 4

#define GAME1 1
#define GAME2 2

int main(int argc, char** argv)
{
	
	printf("please select a program:\n");
	printf("1: rock scissor paper\n");
	printf("2: recursive sleep\n");
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
		default:
			printf("please stick to the instruction:\n");
			break;
	}
		
} 



int random(int n){
	//srand((unsigned int)time(NULL));
	int time = time();
	int mode = time % n;
	//int mod = rand() %n;
	return mod;
}

void game1(){
	while(1){
		int ai = random(5);
		char* str;
		str = get_name(ai);
		printf("Please choose one weapon:\n");
		printf("0: Rock 1: Paper 2: Scissor 3: Lizard 4: Spock\n");
		char user_choice;
		char ignore;
		int read_byte = read(0, &user_choice, 1);
		read(0, &ignore, 1);	
		user_choice = user_choice - '0';
		//printf("%d\n",user_choice);
		char* user_str = get_name(user_choice);
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

char* get_name(int n){
	
	switch(n){
		case ROCK:
			return "Rock";
			
		case PAPER:
			return "Paper";
			
		case SCISSOR:
			return "Scissor";
			
		case LIZARD:
			return  "Lizard";
			
		case SPOCK:
			return"Spock";		
	}
}
void game2(){

	ani_print("Welcome\n",1);
	char sleep_time, rec_level;
	printf("Please input sleep time and recursive level: e.g: 5 2\n");
	
	char ignore;
	read(0, &sleep_time, 1);
	read(0, &ignore, 1);
	read(0, &rec_level, 1);
	read(0, &ignore, 1);
	sleep_time = sleep_time - '0';
	rec_level = rec_level - '0';
	

	int i = 1;

	game2_rec(sleep_time,rec_level,i);
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

