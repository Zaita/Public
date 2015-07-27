const int RESET_SWITCH = 2;
const int FLOAT_1 = 3; // Float 1 is the primary float used to activate the pump
const int FLOAT_2 = 4; // Float 2 is the fail-safe switch. An alarm will go off when it's triggered
const int FLOAT_3 = 5; // Float 3 is the reservoir warning switch

const int PUMP = 6; // 12v DC pump
const int BUZZER = 7; // audible alarm

const int GREEN_LED = 10; // Pump is on
const int YELLOW_LED = 9; // Unit is on standby
const int RED_LED = 8; // error state / not operational indicator
const int LED = 13;

const int DELAY_MS = 100; // delay in ms
const unsigned long PUMP_BOUNCE_MS = 10000; // minimum time between pump being activated to stop burnout
const unsigned long PUMP_MAX_ON_MS = 60000; // maximum time pump can be on
const unsigned long STANDBY_MS = 5000; // how long unit remains in standby

// different patterns for the buzzer to 
// indicate what errors occurred
const int BUZZER_PATTERN[3][8] = { 
  { 1, 1, 1, 1, 1, 1, 1, 1 }, // Pump has been on too long alarm
  { 1, 1, 1, 1, 0, 0, 0, 0 }, // Fail safe float has been activated
  { 0, 0, 1, 1, 0, 0, 1, 1 }  // Reservoir float has been activated
};

// Different states for the system
enum State {
  kOperational,
  kPumpOn,
  kPumpMaxOnAlarm,
  kFailsafeAlarm,
  kReservoirAlarm,
  kStandby
};

State current_state = kStandby;
unsigned long standby_timer = 0;
unsigned long pump_on_timer = 0;
unsigned long pump_off_timer = PUMP_BOUNCE_MS + 1;

/**
Setup our initial state
**/
void setup() {
  // Set pins up
  pinMode(RESET_SWITCH, INPUT);
  pinMode(FLOAT_1, INPUT);
  pinMode(FLOAT_2, INPUT);
  pinMode(FLOAT_3, INPUT);
  pinMode(PUMP, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(LED, OUTPUT);
  
  // start serial output for logging
  Serial.begin(9600);
  
  current_state = kStandby;
}

/**
Sound the alarm
**/
void sound_alarm(int alarm_index) {
  static int index = 0;
  index = (index + 1) % 8;
  if (BUZZER_PATTERN[alarm_index][index] == 1) {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(LED, HIGH);        
    digitalWrite(BUZZER, HIGH);  
  } else {
    digitalWrite(RED_LED, LOW);
    digitalWrite(LED, LOW);    
    digitalWrite(BUZZER, LOW);      
  }    
  
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
}
/**
Main execution code.
**/
void loop() {
  delay(DELAY_MS); // Everything is done in 1/10th second increments
  
  if (digitalRead(RESET_SWITCH) == HIGH && current_state != kStandby) {
    Serial.println("Reset Switch: Activated");
    current_state = kStandby;
    standby_timer = 0;    
    pump_on_timer = 0;
    pump_off_timer = PUMP_BOUNCE_MS + 1;
    
    // turn on all LEDs
    digitalWrite(RED_LED, HIGH);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(LED, HIGH);
    
    digitalWrite(PUMP, LOW);
    digitalWrite(BUZZER, LOW);
  
  } else if (current_state == kPumpMaxOnAlarm) {
    /**
    This alarm code is activated when the pump has been on for too long
    **/
    digitalWrite(PUMP, LOW);
    sound_alarm(0);
    Serial.println("Pump max on alarm: Activated");
      
  } else if (current_state == kFailsafeAlarm) {
    /**
    This alarm code is activated when the failsafe float has been activated
    **/
    sound_alarm(1);    
    Serial.println("Failsafe float switch alarm: Activated");

  } else if (current_state == kReservoirAlarm) {
    /**
    This alarm code is activated when the reservoir float switch has been tipped
    **/
    sound_alarm(2);    
    Serial.println("Reservoir empty alarm: Activated");    
  
  } else if (current_state == kStandby) {
    /**
    Unit is in standby mode. Wait until timer expires then move to
    operational state
    **/
    Serial.println("Currently in standby");
    digitalWrite(YELLOW_LED, HIGH);
    standby_timer += DELAY_MS;
    if (standby_timer > STANDBY_MS) {
      current_state = kOperational;
      Serial.println("Changing to operational state");
    }
    
  } else if (digitalRead(FLOAT_3) == HIGH) {    
    current_state = kReservoirAlarm; 
    digitalWrite(PUMP, LOW);        
    Serial.println("Changing to reservoir alarm state");
  
  } else if (digitalRead(FLOAT_2) == LOW) {
    current_state = kFailsafeAlarm; 
    digitalWrite(PUMP, LOW);
    Serial.println("Changing to failsafe alarm state");
    
  } else if (digitalRead(FLOAT_1) == HIGH && current_state != kPumpOn) {
    if (pump_off_timer > PUMP_BOUNCE_MS) {
      current_state = kPumpOn;
      digitalWrite(PUMP, HIGH);
      digitalWrite(GREEN_LED, HIGH);
      pump_on_timer = 0;
      pump_off_timer = 0;
      Serial.println("Switching pump on");
    } else {
      Serial.print("Pump still in bounce. Pump off timer: ");
      Serial.print(pump_off_timer);
      Serial.print("ms / ");
      Serial.print(PUMP_BOUNCE_MS);
      Serial.print("ms\n");
      pump_off_timer += DELAY_MS;
    }
    
  } else if (digitalRead(FLOAT_1) != HIGH && current_state == kPumpOn) {
    /**
    Pump is on, switch it off
    **/
    current_state = kOperational;
    digitalWrite(PUMP, LOW);
    digitalWrite(GREEN_LED, LOW);    
    Serial.println("Switching pump off");
    
  } else if (current_state == kPumpOn) {
    /**
    Ensure the pump has not been running for too long
    **/
    pump_on_timer += DELAY_MS;
    pump_off_timer = 0;
    Serial.print("Pump on for ");
    Serial.print(pump_on_timer);
    Serial.print("ms\n");
    if (pump_on_timer > PUMP_MAX_ON_MS) {
      Serial.print("Pump on timer: ");
      Serial.print(pump_on_timer);
      Serial.print("; Pump max on ms: ");
      Serial.print(PUMP_MAX_ON_MS);
      Serial.print("\n");
      digitalWrite(PUMP, LOW);
      current_state = kPumpMaxOnAlarm;
    }           
  
  } else if (current_state == kOperational) {
    /**
    Standard operational mode
    **/
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(LED, HIGH);
    
    pump_off_timer += DELAY_MS;    
    Serial.println("...");
    
  } else {
    Serial.println("Shouldn't have gotten here... whoops!"); 
    
  }
}
