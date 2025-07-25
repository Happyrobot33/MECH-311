void setup() {
  // Open serial port
  Serial.begin(9600);

  // get a value for A
  Serial.print("\nEnter a value for A: ");
  while (Serial.available() == 0);  //wait for data available
  int A = Serial.parseInt();
  Serial.println(A);

  // get a value for B
  Serial.print("\nEnter a value for B: ");
  while (Serial.available() == 0);  //wait for data available
  int B = Serial.parseInt();
  Serial.println(B);

  debugPrint(A, B);

  /////////////////////START HERE//////////////////////
  // swap the values stored in A and B
  // A and B already exist, have numbers stored in them,
  // and are ints

  Serial.println("Swapping values...");
  if (A < B)
  {
    int tempA = A; //as long as these aren't objects this is fine :)
    A = B;
    B = tempA;
  }

  ///////////////////// END HERE //////////////////////

  debugPrint(A, B);
}

void debugPrint(int a, int b)
{
  Serial.print("A is now ");
  Serial.print(a);
  Serial.print(" and B is now ");
  Serial.print(b);
  Serial.println(".");
}

void loop() {
  // put your main code here, to run repeatedly:
}
