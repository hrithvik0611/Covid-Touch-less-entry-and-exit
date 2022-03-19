//** Created by Hrithvik Maheshwari on 14-03-2022 3:09 AM **\\

#include <LiquidCrystal.h> // LCD library
#include <Adafruit_MLX90614.h> // Temperature sensor library
#include <avr/io.h> // Inpu-Output library

Adafruit_MLX90614 mlx = Adafruit_MLX90614(); // initializing temperature sensor
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7; // LCD pin declaration
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); // LCD initializing
const int ir_1 = 9; // IR 1 pin declaration
const int ir_2 = 8; // IR 2 pin declaration
const int buzz = 10; // Buzzer pin declaration
const int motorPin_1 = 11;// Pin 1 L293D
const int motorPin_2 = 12;// Pin 2 L293D

void setup()
{
  lcd.begin(16, 2); // starting LCD
  lcd.setCursor(4, 0); // Setting cursor
  lcd.print("Welcome"); // printing welcome message on LCD
  delay(3000);
  pinMode(9, INPUT); // IR 1 pin initializing
  pinMode(8, INPUT); // IR 2 pin initializing
  pinMode(motorPin_1, OUTPUT); // motor pin initialization
  pinMode(motorPin_2, OUTPUT);
  pinMode(buzz, OUTPUT); // buzzer initialization
  digitalWrite(buzz, LOW);
  Serial.begin(9600);
  mlx.begin(); // starting temperature sensor
}

void loop()
{
  // Reading IR Sensors
  int ir_1_state = digitalRead(ir_1);
  int ir_2_state = digitalRead(ir_2);
  if ((ir_1_state == LOW) && (ir_2_state == HIGH))
  {
    entry(); // calling entry function
  }
  else if ((ir_1_state == HIGH) && (ir_2_state == LOW))
  {
    exit_(); // calling exit function
  }
  else
  {
    // Printing msg if no person found
    delay(500);
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Waiting for");
    lcd.setCursor(5, 1);
    lcd.print("Person");
  }
}


void entry()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Wait for Temp Check ");
  delay(300);
  temperature(); // calling temperature checking function
  second_check(); // checking for second IR input
}


void exit_()
{
  gate_open();
  delay(500);
  first_check(); // checking for First IR input
}


void temperature()
{
  int temp_obj = mlx.readObjectTempF(); // Reading temperature
  lcd.setCursor(0, 1);
  lcd.print("Temp = " );
  lcd.setCursor(8, 1);
  lcd.print(temp_obj);
  delay(300);
  if (temp_obj <= 99) // checking temperature range
  {
    gate_open(); // calling gate opening function
  }
  else
  {
    // if temperature is high than normal temperature then buzzer will ring
    digitalWrite(buzz, HIGH);
    delay(2000);
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("High Temperature");
    delay(1000);
    digitalWrite(buzz, LOW);
    temperature(); // calling temperature function in recrusion
  }
  delay(1000);
}


void first_check()
{
  int ir_1_state = digitalRead(ir_1); // reading input from ir sensor
  if (ir_1_state == LOW)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("* Thankyou *");
    gate_close(); // calling closing gate function
  }
  else
  {
    first_check(); // calling function in recrusion
  }
}


void second_check()
{
  int ir_2_state = digitalRead(ir_2); // reading input from ir sensor
  if (ir_2_state == LOW)
  {
    gate_close(); // calling closing gate function
    delay(500);
  }
  else
  {
    second_check(); // calling function in recrusion
  }
}


void gate_open()
{
  // delay based gate opening command
  digitalWrite(motorPin_1, HIGH);
  digitalWrite(motorPin_2, LOW);
  Serial.print("MOTORS ON");
  delay(60);
  motor_off(); // calling motor swithing off function
}


void gate_close()
{
  // delay based gate closing command
  delay(700);
  digitalWrite(motorPin_1, LOW);
  digitalWrite(motorPin_2, HIGH);
  Serial.print("MOTORS on");
  delay(60);
  motor_off(); // calling motor swithing off function
}

void motor_off()
{
  // turning motor off command
  digitalWrite(motorPin_1, LOW);
  digitalWrite(motorPin_2, LOW);
  Serial.print("MOTORS OFF");
}
