/*
Project: Reading and parsing data from PC
Author: Juan Carlos Miranda
Date: October 2022
Description:
  An sketch to parse and processing data from USB. It returns an ECHO response. 
  The code is based on
  Demo of PC-Arduino comms using Python. 
  * https://forum.arduino.cc/t/serial-input-basics/278284/2 
  * https://forum.arduino.cc/t/demo-of-pc-arduino-comms-using-python/219184/4

Use:

*/
#define RED 6
#define GREEN 5
#define BLUE 3
#define BUFF_SIZE 40

/******************************************/
/*!
* Recognized command definitions
*/
/******************************************/
#define START_CMD '<'
#define END_CMD '>'
#define CMD_01 "CMD_01"
#define CMD_02 "CMD_02"
#define CMD_03 "CMD_03"
#define CMD_04 "CMD_04"

// predefined responses
#define RESP_HEADER "RESPONSE"

char input_buffer[BUFF_SIZE] = { 0 };
char message_from_pc[BUFF_SIZE] = { 0 };

byte bytes_recvd = 0;
boolean flag_read_in_progress = false;  // true when detect a start marked in string, false when end marker is detected.
boolean flag_new_data_from_pc = false;  // true when data from PC is finally received.



void setup() {
  Serial.begin(9600); // set the port speed here
  Serial.println("<STARTING_ARDUINO>");   // send first message, telling I am alive!
}

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
      parse_command();
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

void parse_command() {
  /*
  Parses commands and splits in order to proocess parameters
  */
  //get command header and process command parameter
  if (flag_new_data_from_pc == true) {
    if (strcmp(message_from_pc, CMD_01) == 0) { //* CMD_01     
      reply_to_pc("CMD_01 parsing");
    } else if (strcmp(message_from_pc, CMD_02) == 0) {  //* CMD_02      
      reply_to_pc("CMD_02 parsing");
    } else if (strcmp(message_from_pc, CMD_03) == 0) {  //* CMD_03      
      reply_to_pc("CMD_03 parsing");
    } else if (strcmp(message_from_pc, CMD_04) == 0) {  //* CMD_04      
      reply_to_pc("CMD_04 parsing");
    }
  }
}  //void parse_command(msg_port)



void loop() {
  receive_msg_from_pc(); 
}