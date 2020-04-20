const int RESET_SWITCH = 2;
const int FLOAT = 3; // Float for the tank
const int POWER_PIN = 4; // providing power for rst switch
const int PUMP = 7;
const int BUZZER = 12;

const int GREEN_LED = 10;
const int RED_LED = 9;
const int BLUE_LED = 8;
const int LED = 13;

const int DELAY_MS = 100;
const unsigned long PUMP_MAX_ON_MS = 10000; // Maximum time pump can be on
const unsigned long PUMP_BOUNCE_MS = 30000; // Time between pump activations
const unsigned long STANDBY_MS = 5000;

const int BUZZER_PATTERN[8] = { 1, 1, 1, 1, 0, 0, 0, 0 }; // Fail safe float has been activated

// Different states for the system
enum State {
  kStandby,
  kOperational,
  kPumpOn,
  kPumpMaxOnAlarm,
  kError
};

State current_state = kStandby;
unsigned long standby_timer = 0;
unsigned long pump_on_timer = 0;
unsigned long pump_off_timer = PUMP_BOUNCE_MS + 1;


/**
 * Setup our initial state
 **/
void setup() {
    // Set pins up
  pinMode(RESET_SWITCH, INPUT);
  pinMode(FLOAT, INPUT);
  pinMode(POWER_PIN, OUTPUT);  
  pinMode(PUMP, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(LED, OUTPUT);
  
  digitalWrite(POWER_PIN, HIGH);
  current_state = kStandby;
  
  Serial.begin(9600);
}

/**
Sound the alarm
**/
void sound_alarm() {
  static int index = 0;
  index = (index + 1) % 8;
  if (BUZZER_PATTERN[index] == 1) {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(LED, HIGH);        
    digitalWrite(BUZZER, HIGH);  
  } else {
    digitalWrite(RED_LED, LOW);
    digitalWrite(LED, LOW);    
    digitalWrite(BUZZER, LOW);      
  }    

  digitalWrite(GREEN_LED, LOW);
}

/**
 * Send Data to Server
 */
void Send(String value) {
  Serial.println(value);
}

/**
 * Main application logic
 */
void loop() {
  delay(DELAY_MS); // Everything is done in 1/10th second increments
  
  if (digitalRead(RESET_SWITCH) == LOW && current_state != kStandby) {
    Send("Reset Switch: Activated");
    current_state = kStandby;
    standby_timer = 0;    
    pump_on_timer = 0;
    pump_off_timer = PUMP_BOUNCE_MS + 1;
    
    // turn on all LEDs
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(LED, HIGH);
    digitalWrite(BLUE_LED, HIGH);
    
    digitalWrite(PUMP, LOW);
    digitalWrite(BUZZER, LOW);

  } else if (current_state == kPumpMaxOnAlarm) {
    /**
    This alarm code is activated when the pump has been on for too long
    **/
    Send("Pump max on alarm: Activated");
    digitalWrite(PUMP, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(LED, LOW);
    sound_alarm();
    current_state = kError;  
    
  } else if (current_state == kError) {
    Send("Error. Pump on Alarm!!!");
    sound_alarm();
    
  } else if (current_state == kStandby) {
    /**
    Unit is in standby mode. Wait until timer expires then move to
    operational state
    **/
    Send("Currently in standby");
    // turn on all LEDs
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(LED, HIGH);
    digitalWrite(BLUE_LED, HIGH);
    
    standby_timer += DELAY_MS;
    if (standby_timer > STANDBY_MS) {
      current_state = kOperational;
      Send("Changing to operational state");      
    }
  } else if (digitalRead(FLOAT) == LOW && current_state != kPumpOn) {
    if (pump_off_timer > PUMP_BOUNCE_MS) {
      current_state = kPumpOn;
      digitalWrite(PUMP, HIGH);
      digitalWrite(GREEN_LED, HIGH);
      pump_on_timer = 0;
      pump_off_timer = 0;
      Send("Switching pump on");
    } else {
      String cmd = "Pump still in bounce. Pump off timer: ";
      cmd += String(pump_off_timer) + "ms / " + String(PUMP_BOUNCE_MS) + "ms";
      Send(cmd);
      pump_off_timer += DELAY_MS;
    }
    
  } else if (digitalRead(FLOAT) != LOW && current_state == kPumpOn) {
    Send("Switching Pump Off. Float disabled");
    /**
    Pump is on, switch it off
    **/
    current_state = kOperational;
    digitalWrite(PUMP, LOW);
    digitalWrite(GREEN_LED, LOW);    
    Send("Switching pump off");
    
  } else if (current_state == kPumpOn) {
    /**
    Ensure the pump has not been running for too long
    **/
    pump_on_timer += DELAY_MS;
    pump_off_timer = 0;
    
    Send("Pump on for " + String(pump_on_timer) + "ms");  
    if (pump_on_timer > PUMP_MAX_ON_MS) {
      Send("Pump on timer: " + String(pump_on_timer) + "; Pump max on ms: " + String(PUMP_MAX_ON_MS));
      digitalWrite(PUMP, LOW);
      current_state = kPumpMaxOnAlarm;
    }           
  
  } else if (current_state == kOperational) {
    /**
    Standard operational mode
    **/
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    
    pump_off_timer += DELAY_MS;      
    Send("...");
    
  } else {
    Send("Shouldn't have gotten here... whoops!");     
  }  
}




