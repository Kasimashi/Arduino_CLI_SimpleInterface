#include <Arduino.h>

// Include Library
#include "Shell/SimpleCLI.h"

// Create CLI Object
SimpleCLI cli;

// Commands
Command cmdPing;
Command cmdMycommand;
Command cmdEcho;
Command cmdRm;
Command cmdLs;
Command cmdBoundless;
Command cmdSingle;
Command cmdHelp;

void setup() {
    Serial.begin(9600);
    Serial.println("Welcome to this simple Arduino command line interface (CLI).");
	Serial.println("Type \"help\" to see a list of commands.");

    cmdPing = cli.addCmd("ping");
    cmdPing.addArg("n", "10");
    cmdPing.setDescription("Responds with a ping n-times");

    cmdMycommand = cli.addCmd("mycommand");
    cmdMycommand.addArg("o");
    cmdMycommand.setDescription("Says hi to o");

    cmdEcho = cli.addCmd("echo");
    cmdEcho.addPosArg("text", "something");
    cmdEcho.setDescription("Echos what you said");

    cmdRm = cli.addCmd("rm");
    cmdRm.addPosArg("file");
    cmdRm.setDescription("Removes specified file (but not actually)");

    cmdLs = cli.addCmd("ls");
    cmdLs.addFlagArg("a");
    cmdLs.setDescription("Lists files in directory (-a for all)");

    cmdBoundless = cli.addBoundlessCmd("boundless");
    cmdBoundless.setDescription("A boundless command that echos your input");

    cmdSingle = cli.addSingleArgCmd("single");
    cmdSingle.setDescription("A single command that echos your input");

    cmdHelp = cli.addCommand("help");
    cmdHelp.setDescription("Get help!");
}

void loop() {

	String input ="";
	char ch="";
	Serial.print("# ");
	while ((ch!=13)){ //Tant que l'utilisateur n'a pas valid� ou n'a pas fait CTRL+C on lit.
		if (Serial.available() >0){ //Si on d�tecte une entr�e s�rie.
			ch = Serial.read(); //Lecture du caract�re d'entr�e.
			switch(ch){
				case(3): //CTRL+C
					return;
					break;
				case(13): //ENTER
					break;
				case(8593): //Fl�che du haut
					Serial.println("Fl�che du haut! ");
					break;
				default:
					Serial.print(ch);
					Serial.print(ch,DEC);
					input += (char)ch;
			}

		}
	}
	Serial.println();
	String last_input = input;
	if (input.length() > 0) {
		cli.parse(input);
	}

    if (cli.available()) {
        Command c = cli.getCmd();

        int argNum = c.countArgs();

        Serial.print("> ");
        Serial.print(c.getName());
        Serial.print(' ');

        for (int i = 0; i<argNum; ++i) {
            Argument arg = c.getArgument(i);
            // if(arg.isSet()) {
            Serial.print(arg.toString());
            Serial.print(' ');
            // }
        }

        Serial.println();

        if (c == cmdPing) {
            Serial.print(c.getArgument("n").getValue() + "x ");
            Serial.println("Pong!");
        } else if (c == cmdMycommand) {
            Serial.println("Hi " + c.getArgument("o").getValue());
        } else if (c == cmdEcho) {
            Argument str = c.getArgument(0);
            Serial.println(str.getValue());
        } else if (c == cmdRm) {
            Serial.println("Remove directory " + c.getArgument(0).getValue());
        } else if (c == cmdLs) {
            Argument a   = c.getArgument("a");
            bool     set = a.isSet();
            if (a.isSet()) {
                Serial.println("Listing all directories");
            } else {
                Serial.println("Listing directories");
            }
        } else if (c == cmdBoundless) {
            Serial.print("Boundless: ");

            for (int i = 0; i<argNum; ++i) {
                Argument arg = c.getArgument(i);
                if (i>0) Serial.print(",");
                Serial.print("\"");
                Serial.print(arg.getValue());
                Serial.print("\"");
            }
        } else if (c == cmdSingle) {
            Serial.println("Single \"" + c.getArg(0).getValue() + "\"");
        } else if (c == cmdHelp) {
            Serial.println(cli.toString());
        }
    }

    if (cli.errored()) {
        CommandError cmdError = cli.getError();
        Serial.println();
        Serial.print("ERROR: ");
        Serial.println(cmdError.toString());

        if (cmdError.hasCommand()) {
            Serial.print("Did you mean \"");
            Serial.print(cmdError.getCommand().toString());
            Serial.println("\"?");
        }
    }
}
