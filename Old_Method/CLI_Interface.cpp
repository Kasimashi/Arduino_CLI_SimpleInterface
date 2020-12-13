#include <Arduino.h>
#include <string.h>

#define LINE_BUF_SIZE 128   //Maximum input string length
#define ARG_BUF_SIZE 64     //Maximum argument string length
#define MAX_NUM_ARGS 8      //Maximum number of arguments

bool error_flag = false;

char line[LINE_BUF_SIZE];
char args[MAX_NUM_ARGS][ARG_BUF_SIZE];

//Function declarations
void cmd_help();
int cmd_exit();
void my_cli();
void cli_init();
int execute();
void parse_line();
void read_line();
void help_exit();
void help_help();
void quit();

//List of functions pointers corresponding to each command
int (*commands_func[])(){
    &cmd_help,
    &cmd_exit
};

//List of command names
const char *commands_str[] = {
    "help",
    "exit"
};

int num_commands = sizeof(commands_str) / sizeof(char *);

int cmd_exit(){
    Serial.println("Exiting CLI.");

    while(1);
}

void help_help(){
    Serial.println("The following commands are available:");

    for(int i=0; i<num_commands; i++){
        Serial.print("  ");
        Serial.println(commands_str[i]);
    }
    Serial.println("");
    Serial.println("You can for instance type \"help led\" for more info on the LED command.");
}



void help_exit(){
    Serial.println("This will exit the CLI. To restart the CLI, restart the program.");
}

void quit(){
    Serial.println("QUIT THE PROGRAM.");
}

void cmd_help(){
    if(args[1] == NULL){
        help_help();
    }
    else if(strcmp(args[1], commands_str[0]) == 0){
        help_help();
    }
    else if(strcmp(args[1], commands_str[1]) == 0){
        help_exit();
    }
    else{
        help_help();
    }
}

void cmd_quit(){
    if(args[1] == NULL){
    	quit();
    }
    else{
    	quit();
    }
}



void read_line(){
    String line_string ="";
    char ch="";

	while ((ch!=13)){ //Tant que l'utilisateur n'a pas validé ou n'a pas fait CTRL+C on lit.
		if (Serial.available() >0){ //Si on détecte une entrée série.
			ch = Serial.read(); //Lecture du caractère d'entrée.
			switch(ch){
				case(3): //CTRL+C
					return;
					break;
				case(13): //ENTER
					break;
				default:
					Serial.print(ch);
					line_string += (char)ch;
			}

		}
	}
	Serial.println("");
	if(line_string.length() < LINE_BUF_SIZE){
	  line_string.toCharArray(line, LINE_BUF_SIZE);
	  /*
	   *
	   * DEBUG TIPS.
	   *
	  Serial.print("Line String :");
	  Serial.print(line_string);
	  Serial.println("|");
	  */
	}
	else{
	  Serial.println("Input string too long.");
	  error_flag = true;
	}
}

void parse_line(){
    char *argument;
    int counter = 0;


    argument = strtok(line, " ");

    while((argument != NULL)){
        if(counter < MAX_NUM_ARGS){
            if(strlen(argument) < ARG_BUF_SIZE){
                strcpy(args[counter],argument);
                argument = strtok(NULL, " ");
                counter++;
            }
            else{
                Serial.println("Input string too long.");
                error_flag = true;
                break;
            }
        }
        else{
            break;
        }
    }
}

int execute(){
    for(int i=0; i<num_commands; i++){ // On parcours la liste des commandes
        if(strcmp(args[0], commands_str[i]) == 0){ //Si la commande correspond à une commande de la liste
            return(*commands_func[i])(); //ON appelle la fonction
        }
    }

    Serial.println("Invalid command. Type \"help\" for more.");
    return 0;
}

void cli_init(){
    Serial.println("Welcome to this simple Arduino command line interface (CLI).");
    Serial.println("Type \"help\" to see a list of commands.");

    my_cli();
}


void my_cli(){
    Serial.print("> ");

    read_line();
    if(!error_flag){
        parse_line();
    }
    if(!error_flag){
        execute();
    }
    memset(line, 0, LINE_BUF_SIZE); //Suppression de l'ancienne commande
    memset(args, 0, sizeof(args[0][0]) * MAX_NUM_ARGS * ARG_BUF_SIZE); //Suppression des anciens arguments

    error_flag = false;
    my_cli(); //Rebouclage de la CLI
}

void setup() {
    Serial.begin(115200);

    cli_init();
}

void loop() {

}
