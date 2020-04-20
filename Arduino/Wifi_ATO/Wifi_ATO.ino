const int RESET_SWITCH = 2;
const int FLOAT = 3; // Float for the tank
const int PUMP = 7;
const int BUZZER = 5;

const int GREEN_LED = 8;
const int RED_LED = 9;
const int BLUE_LED = 10;
const int LED = 13;

const int DELAY_MS = 100;
const unsigned long PUMP_MAX_ON_MS = 30000; // Maximum time pump can be on
const unsigned long PUMP_BOUNCE_MS = 60000; // Time between pump activations
const unsigned long STANDBY_MS = 10000;

const char* SSID = "Tartarus";
const char* PASS = "theworldspinsquickly";
const char* IP = "192.168.0.251";
const char* PORT = "9001";
const char* DEVICE = "DISPLAY_ATO";

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
 * Connect to the Wifi
 */
bool connectWifi() {
  Serial.println("AT+CWMODE=1");
  delay(2000);
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  Serial.println(cmd);
  delay(5000);
  if(Serial.find("OK"))
    return true;
  else
    return false;  
}

/**
 * Setup our initial state
 **/
void setup() {
    // Set pins up
  pinMode(RESET_SWITCH, INPUT);
  pinMode(FLOAT, INPUT);
  pinMode(PUMP, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(LED, OUTPUT);
  
  Serial.begin(115200);
  Serial.println("AT");
  delay(2500);
  if (Serial.find("OK")) {
    Serial.println("Wifi Module OK");  
    if (connectWifi()) {
      Serial.println("Wifi Connected");
      digitalWrite(BLUE_LED, HIGH);
    } else
      Serial.println("Failed to connect to Wifi");
  } else 
    Serial.println("Wifi Module not responding");
    
  current_state = kStandby;
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
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP + String("\",") + PORT;
  Serial.println(cmd);
  digitalWrite(BLUE_LED, LOW);
  delay(2000);
  if(Serial.find("Error")){
    return;
  }
  
  digitalWrite(BLUE_LED, HIGH);
  cmd = String(DEVICE) + ": " + String(value);
  cmd += "\r\n";
  Serial.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  if(Serial.find(">")){
    Serial.print(cmd);
  }else{
    Serial.println("AT+CIPCLOSE");
    digitalWrite(BLUE_LED, LOW);
  }
}

/**
 * Main application logic
 */
void loop() {
  delay(DELAY_MS); // Everything is done in 1/10th second increments
  
  if (digitalRead(RESET_SWITCH) == HIGH && current_state != kStandby) {
    Send("Reset Switch: Activated");
    current_state = kStandby;
    standby_timer = 0;    
    pump_on_timer = 0;
    pump_off_timer = PUMP_BOUNCE_MS + 1;
    
    // turn on all LEDs
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(LED, HIGH);
    
    digitalWrite(PUMP, LOW);
    digitalWrite(BUZZER, LOW);
    
    digitalWrite(BLUE_LED, LOW);
  } else if (current_state == kPumpMaxOnAlarm) {
    /**
    This alarm code is activated when the pump has been on for too long
    **/
    Send("Pump max on alarm: Activated");
    digitalWrite(PUMP, LOW);
    sound_alarm();
    current_state = kError;  
  } else if (current_state == kError) {
    Send("Error");
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
    
    standby_timer += DELAY_MS;
    if (standby_timer > STANDBY_MS) {
      current_state = kOperational;
      Send("Changing to operational state");
    }
  } else if (digitalRead(FLOAT) == HIGH && current_state != kPumpOn) {
    Send("X");
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
    
  } else if (digitalRead(FLOAT) != HIGH && current_state == kPumpOn) {
    Send("Y");
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
    digitalWrite(LED, HIGH);
    
    pump_off_timer += DELAY_MS;      
    Send("...");
    
  } else {
    Send("Shouldn't have gotten here... whoops!");     
  }  
}




