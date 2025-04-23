/*-----------------------------------------------------------------------------
  InverterManager.ino
 -----------------------------------------------------------------------------
 
 This file is part of InverterManager.

  InverterManager is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  InverterManager is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Rospo.  If not, see <http://www.gnu.org/licenses/>.
*/

// Outputs
#define BUTTONPUSHER PB11
#define RELAYCONTROL PB9
#define BUZZER PB5

// Inputs
#define ACSENSE PA1
#define LOADSENSE PA4

// Settings
#define RUNCYCLES 1300

bool ecoflowRunning;
bool relaysOn;
int confusionCount;
int cycleCount;

void setup() {
  pinMode(RELAYCONTROL, OUTPUT);
  pinMode(BUTTONPUSHER, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LOADSENSE, INPUT);
  pinMode(ACSENSE, INPUT);
  digitalWrite( RELAYCONTROL, LOW );
  digitalWrite( BUTTONPUSHER, LOW );
  ecoflowRunning = false;
  relaysOn = false;
  confusionCount = 0;
  sound_bootup();
}

void sound_checking() {
  tone(BUZZER, 700);
  delay(100);
  tone(BUZZER, 700);
  delay(100);
  noTone(BUZZER);
  delay(500);
}

void sound_bootup() {
  tone(BUZZER, 700);
  delay(300);
  noTone(BUZZER);
  delay(100);
  tone(BUZZER, 700);
  delay(300);
  noTone(BUZZER);
}

void sound_siren() {
  tone(BUZZER, 1000);
  delay(900);
  tone(BUZZER, 1500);
  delay(900);
  noTone(BUZZER);
}

void sound_alarm() {
  tone(BUZZER, 500);
  delay(900);
  noTone(BUZZER);
  delay(900);
}

void sound_ascending() {
  tone(BUZZER, 300);
  delay(900);
  tone(BUZZER, 400);
  delay(900);
  tone(BUZZER, 500);
  delay(900);
  noTone(BUZZER);
  delay(900);
}

void sound_sunshine() {
  tone(BUZZER, 587);
  delay(300);
  tone(BUZZER, 784);
  delay(300);
  tone(BUZZER, 880);
  delay(300);
  tone(BUZZER, 988);
  delay(400);
  noTone(BUZZER);
  delay(50);
  tone(BUZZER, 988);
  delay(700);
  noTone(BUZZER);
  delay(900);
}

void runOnDemand() {
  if(ecoflowRunning)
  {
      // Try to confirm it really is running
      if(digitalRead(ACSENSE) == LOW)
      {
        if( ++confusionCount > 5 )
        {
          // panic
          digitalWrite(RELAYCONTROL, LOW);
          while(true)
          {
            sound_alarm();
          }
        } else {
          // try again
          digitalWrite(BUTTONPUSHER, HIGH); // Activate DVH
          delay(150);
          digitalWrite(BUTTONPUSHER, LOW);
          tone(BUZZER,1000);
          delay(1000);
          noTone(BUZZER);
        }
        
      }

      if(cycleCount++ > RUNCYCLES)
      {
        // Check if we need to still be running
        digitalWrite(RELAYCONTROL, LOW); // switch back to sensing
        sound_checking();
        if( digitalRead(LOADSENSE) == HIGH ) // Still loaded
        {
          // Turn back on and reset counter
          digitalWrite(RELAYCONTROL, HIGH);
          cycleCount = 0;
        } else {
          // Keep inverter off
          ecoflowRunning = false;
          confusionCount = 0;
        }
      } else {
        delay(100);
      }
  } else {
    // Check for load, and activate if necessary
    if( digitalRead(LOADSENSE) == HIGH )
    {
      digitalWrite(BUTTONPUSHER, HIGH); // Activate DVH
      digitalWrite(RELAYCONTROL, HIGH); // Switch relay to power load
      ecoflowRunning = true;
      confusionCount = 0;
      delay(150);
      digitalWrite(BUTTONPUSHER, LOW);
      cycleCount = 0;
      sound_sunshine();
      delay(10);
    } else {
      // Try to confirm the inverter is not running
      if(digitalRead(ACSENSE) == HIGH)
      {
         if( ++confusionCount > 5 )
        {
          // panic
          digitalWrite(RELAYCONTROL, LOW);
          while(true)
          {
            sound_alarm();
          }
        } else {
          // try again
          digitalWrite(BUTTONPUSHER, HIGH); // Activate DVH
          delay(150);
          digitalWrite(BUTTONPUSHER, LOW);
          tone(BUZZER,1000);
          delay(1000);
          noTone(BUZZER);
        }
      }
    }
  }
}


void loop()
{
  runOnDemand();
}
