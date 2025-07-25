void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // prompt the user to input an integer
  Serial.print("\nEnter a value: ");
  // wait for data
  while (Serial.available() == 0);
  // store data in a variable
  int Val = Serial.parseInt();
  // print out the response
  Serial.println(Val);
}

void loop() {
  // do nothing here
}
