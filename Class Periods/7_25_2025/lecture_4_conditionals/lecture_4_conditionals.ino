void setup() {
  // Open serial port
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  // get a value for A
  Serial.print("\nEnter a value for A: ");
  while (Serial.available() == 0);  //wait for data available
  int A = Serial.parseInt();
  Serial.println(A);

  // This will contain the word positive, negative, or zero depending on the value of A
  String signOfA;

  /////////////////////START HERE//////////////////////
  // if A is positive, put the string "positive" in signOfA
  // if A is negative, put the string "negative" in signOfA
  // if A is zero, put the string "zero" in signOfA
  // note that signOfA already exists, so you don't need to declare it as a String
  if (A > 0) {
    signOfA = "positive";
  } else if (A < 0) {
    signOfA = "negative";
  } else {
    signOfA = "zero";
  }

  if (A > 100)
  {
    signOfA += " and greater than 100";
  }
  else if (A < 100)
  {
    signOfA += " and less than 100";
  }
  ///////////////////// END HERE //////////////////////

  Serial.print("A is ");
  Serial.print(signOfA);
  Serial.println(".");
}
