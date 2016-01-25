int GreenButton = 17;       //the pin that the pushbutton "C" is attached to
int YellowLED = 13;         // the pin that the YELLOW LED is attached to

int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button      

void setup() 
{
  // put your setup code here, to run once:
  pinMode(GreenButton, INPUT);
  pinMode(YellowLED, OUTPUT);
  DDRD |= (1 << DDD5);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  buttonState = digitalRead(GreenButton);

  if(buttonPushCounter >= 3)
  {
    buttonPushCounter = 0;
  }
 
  if((buttonState != lastButtonState) && (buttonState == LOW))
    {
        buttonPushCounter++;
    }
    
    if(buttonPushCounter == 1)
      {
        digitalWrite(YellowLED, HIGH);
      }
    else if(buttonPushCounter == 2) 
      {
        digitalWrite(YellowLED, HIGH);    // turn the LED on (HIGH is the voltage level)
        delay(1250);                      // wait for a second
        digitalWrite(YellowLED, LOW);    // turn the LED off by making the voltage LOW
        delay(2500);
        PORTD |= (1 << PORTD5); // Turn ON
        delay(500);
        PORTD &= ~(1 << PORTD5); // Turn OFF
        delay(500);
      } 
    else if(buttonPushCounter == 3) 
      {
        digitalWrite(YellowLED, LOW);
      }
      
    lastButtonState = buttonState;
}
