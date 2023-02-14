// A simple example of a receiver. Taken from the original RadioHead library.
// All credits and free beer to: https://www.airspayce.com/mikem/arduino/RadioHead/

#include <Arduino.h>
/*
* Demo for RF remote switch receiver. 
* This example is for the new KaKu / Home Easy type of remotes!
* For details, see NewRemoteReceiver.h!
*
* This sketch shows the received signals on the serial port.
* Connect the receiver to digital pin 2.
*/

#include <NewRemoteReceiver.h>

// Callback function is called only when a valid code is received.
void showCode(NewRemoteCode receivedCode)
{
  // Note: interrupts are disabled. You can re-enable them if needed.

  // Print the received code.
  Serial.print("Addr ");
  Serial.print(receivedCode.address);

  if (receivedCode.groupBit)
  {
    Serial.print(" group");
  }
  else
  {
    Serial.print(" unit ");
    Serial.print(receivedCode.unit);
  }

  switch (receivedCode.switchType)
  {
  case NewRemoteCode::off:
    Serial.print(" off");
    break;
  case NewRemoteCode::on:
    Serial.print(" on");
    break;
  case NewRemoteCode::dim:
    Serial.print(" dim level ");
    Serial.print(receivedCode.dimLevel);
    break;
  }

  Serial.print(", period: ");
  Serial.print(receivedCode.period);
  Serial.println("us.");
}

void setup()
{
  Serial.begin(115200);

  // Initialize receiver on interrupt 0 (= digital pin 2), calls the callback "showCode"
  // after 2 identical codes have been received in a row. (thus, keep the button pressed
  // for a moment)
  //
  // See the interrupt-parameter of attachInterrupt for possible values (and pins)
  // to connect the receiver.
  NewRemoteReceiver::init(0, 2, showCode);
}

/**
 * Demo for RF remote switch receiver.
 * For details, see NewRemoteReceiver.h!
 *
 * Connect the transmitter to digital pin 11 on Arduino or pin 4 on ESP8266 and ESP32.
 *
 * This sketch demonstrates the use of the NewRemoteTransmitter class.
 *
 * When run, this sketch switches some pre-defined devices on and off in a loop.
 *
 * NOTE: the actual receivers have the address and group numbers in this example
 * are only for demonstration! If you want to duplicate an existing remote, please
 * try the "retransmitter"-example instead.
 * 
 * To use this actual example, you'd need to "learn" the used code in the receivers
 * This sketch is unsuited for that.
 * 
 */

#include <NewRemoteTransmitter.h>

// Create a transmitter on address 123, using a digital pin to transmit,
// with a period duration of 260ms (default), repeating the transmitted
// code 2^3=8 times.
#if defined ESP8266 || defined ESP32
NewRemoteTransmitter transmitter(123, 4, 260, 3);
#else
NewRemoteTransmitter transmitter(12384378, 11, 271, 3);
#endif

void loop()
{

  if (Serial.available() > 0)
  {
    String incomingByte = Serial.readStringUntil('\n'); // read the incoming byte:
    if (incomingByte.startsWith("on"))
    {
      transmitter.sendUnit(0, true);
    }

    if (incomingByte.startsWith("off"))
    {
      transmitter.sendUnit(0, false);
    }

    Serial.print("Serial message recieved:");
    Serial.println(incomingByte);
  } // Switch unit 2 off
  // transmitter.sendUnit(0, false);

  // // Wait 5 seconds
  // delay(5000);

  // // Switch unit 2 on
  // transmitter.sendUnit(0, true);

  // // Wait 5 seconds
  // delay(5000);
}
