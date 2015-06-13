/*
  Created by Bruno Calou Alves, May, 2015.
  Read LICENSE for more information.
*/

#include "log.h"

Log my_log;

void setup()
{
  //Begin the serial communication
  Serial.begin(9600);

  //Set the log target (it's serial by default)
  my_log.setTarget(&Serial);

  //Uncomment to disable the log
  //my_log.disable();

  //Hide the tag, the priority or both.
  //my_log.hideTag();
  //my_log.hidePriority();
  //Hide header just hides the tags and the priority
  my_log.hideHeader();
}

void loop()
{

  //Print whatever you like
  my_log.info("info id", "message");
  my_log.assert("assert id", 6);
  my_log.warn("warn id", 5.9);
  my_log.debug("debug id", 1533L);
  my_log.error("error id", '6');
  my_log.verbose("verbose id", 42);

  //You can use the println method as well
  my_log.println(INFO, "cool info", "Printing");
  my_log.println(INFO, "cool info", "Cool");
  my_log.println(INFO, "cool info", "Info");

  //C++ style
  my_log << "No header" << " at all" << log_endl;
  my_log << DEBUG << "debug id" << "My debug" << " stuff" << log_endl;

  //Regular arduino print and println methods
  my_log.print("Regular Arduino");
  my_log.print("  Print");
  my_log.println();

  my_log.println("Arduino println");


  delay(500);

}
