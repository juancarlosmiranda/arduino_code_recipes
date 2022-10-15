/*
Project: Reading and parsing data from PC
Author: Juan Carlos Miranda
Date: October 2022
Description:
  An sketch to parse and processing data from USB.
  Manage LED through serial port.

  The code is based on
  Demo of PC-Arduino comms using Python. 
  * https://forum.arduino.cc/t/serial-input-basics/278284/2 
  * https://forum.arduino.cc/t/demo-of-pc-arduino-comms-using-python/219184/4

Use:

Commands with one parameter:
<CMD_01,ON> turn on RED LED
<CMD_01,OFF> turn off RED LED

Commands without parameter
<CMD_02> blink GREEN LED
<CMD_03> blink 

Load sketc and test data sending commands with "Serial Monitor"
*/

/* Definition of LED ports*/
#define RED 10
#define GREEN 9
#define BLUE 8
#define TIME_LED 200

/* Configuration of serial port*/
#define SERIAL_SPEED 9600

/******************************************/
/*!
* Recognized command definitions
*/
/******************************************/
/*
Command examples received:
<CMD_01,ON> turn on RED LED
<CMD_01,OFF> turn off RED LED

Examples of responses to PC
*/
#define START_CMD '<'
#define END_CMD '>'
#define DELIMITER_CMD ','
#define CMD_01 "CMD_01"
#define CMD_02 "CMD_02"
#define CMD_03 "CMD_03"
#define CMD_04 "CMD_04"

// predefined responses
#define RESP_HEADER "RESP_"
#define START_MESSAGE "<STARTING_ARDUINO>"
#define BUFF_SIZE 40 // size of messages

char input_buffer[BUFF_SIZE] = { 0 };
char message_from_pc[BUFF_SIZE] = { 0 }; // global variable

byte bytes_recvd = 0;
boolean flag_read_in_progress = false;  // true when detect a start marked in string, false when end marker is detected.
boolean flag_new_data_from_pc = false;  // true when data from PC is finally received.


void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  Serial.begin(SERIAL_SPEED); // set the port speed here
  Serial.println(START_MESSAGE);   // send first message, telling I am alive!
}

/*!
* Process data input from serial port
*/
void receive_msg_from_pc() {

  if (Serial.available() > 0) {
    char char_read = Serial.read();  // reading data from serial port

    if (char_read == END_CMD) {       // used to detect the end of the string
      flag_read_in_progress = false;  // flag to show that read is finished
      flag_new_data_from_pc = true;   // serial data string is finally received
      input_buffer[bytes_recvd] = 0;  // close string data
      strcpy(message_from_pc, input_buffer);
      // ----------- parse data HERE ---------
      // only when data string is finished, it parser commands
      // the data is processed in cascade mode
      parse_command(message_from_pc);
      // ----------- parse data HERE ---------
    }

    if (flag_read_in_progress == true) {  // save characters in order of arrival
      input_buffer[bytes_recvd] = char_read;
      bytes_recvd++;

      if (bytes_recvd == BUFF_SIZE) {  // this avoid the overflow in array
        bytes_recvd = BUFF_SIZE - 1;
      }
    }

    if (char_read == START_CMD) {     // used to detect the start of the string
      bytes_recvd = 0;                // init character counter
      flag_read_in_progress = true;   // init flag read data
      flag_new_data_from_pc = false;  // reset flag new data
    }
  }
}

/*!
* Prepare strings to send to PC
*/
void reply_to_pc(char *msg) {
  if (flag_new_data_from_pc == true) {
    flag_new_data_from_pc = false;  //this indicates that data have been processed
    // prepares a response using start and end symbols defined something like: <RESPONSE messages>
    Serial.print(START_CMD);
    Serial.print(RESP_HEADER);
    Serial.print(msg); // echo message here
    Serial.println(END_CMD);    
  }
}

  /*
  Parses commands and splits in order to proocess parameters
  */
void parse_command(char *message_to_parse) {
  char *cmd_token = NULL;
  char cmd_header[] = ".\n";  
  char param_01[] = "OFF\n";
  // char param_02[] = "OFF\n"; // use this in Ccommand sintaxys with 2 parameters
  char resp_msg[] = "R_";

  //get command header and process command parameter

  if (flag_new_data_from_pc == true) {
    cmd_token = strtok(message_to_parse, ",");
    strcpy(cmd_header,cmd_token); //creatoin of header
    Serial.println(cmd_header);
    if (strcmp(cmd_header, CMD_01) == 0) { //* CMD_01
      // --------parameter extraction ----------
      cmd_token=strtok(NULL,",");
      strcpy(param_01, cmd_token);
      strcat(resp_msg,cmd_header);
      // ------------------
      if(strcmp(param_01,"ON")== 0){
        led_on(RED);
      }else if(strcmp(param_01,"OFF")== 0){
        led_off(RED);
      }
      // ------------------
      reply_to_pc(resp_msg);
    } else if (strcmp(cmd_header, CMD_02) == 0) {  //* CMD_02      
      // --------------------
      blink_led(GREEN,TIME_LED);
      // --------------------
      reply_to_pc("CMD_02 parsing");
    } else if (strcmp(cmd_header, CMD_03) == 0) {  //* CMD_03      
      // --------------------
      blink_led(BLUE,TIME_LED);
      // --------------------
      reply_to_pc("CMD_03 parsing");
    } else if (strcmp(cmd_header, CMD_04) == 0) {  //* CMD_04      
      reply_to_pc("CMD_04 parsing");
    }
  }
}  //void parse_command(msg_port)


void blink_led(int pin, int duration) {
  digitalWrite(pin, HIGH);
  delay(duration);
  digitalWrite(pin, LOW);
  delay(duration);
}

void led_on(int pin) {
  digitalWrite(pin, HIGH);
}

void led_off(int pin) {
  digitalWrite(pin, LOW);
}



void loop() {
  receive_msg_from_pc(); 
  // enabl to debug functions
  //blink_led(RED,TIME_LED);
  //delay(1000);
  //blink_led(GREEN,TIME_LED);
  //delay(1000);
  //blink_led(RED,TIME_LED);
  //led_on(RED);
  //delay(1000);
  //led_off(RED);  
  //delay(1000);

}