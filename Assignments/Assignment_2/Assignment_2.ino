//psuedocode
/*

Ask for how many numbers they want to enter
Save that into a integer variable
Make a new array the size of that integer variable
iterate and take in that many numbers into that new array
Calculate the average, min, and max of that array
Print out the results

*/

//uncomment this to enable tests
//#define tests

int size = 0;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  #ifdef tests
  //run tests
  TakeInInput(5, new int[5]{1, 2, 3, 4, 5});
  TakeInInput(3, new int[3]{-10, 0, 10});
  TakeInInput(4, new int[4]{100, 200, 300, 400});
  TakeInInput(6, new int[6]{5, 3, 8, 1, 4, 7});
  TakeInInput(1, new int[1]{42});
  #endif

  #ifndef tests
  //ask for how many numbers they want
  Serial.println("How many numbers do you want to enter?");
  while (Serial.available() == 0) {
    // wait for user input
  }
  int howMany = Serial.parseInt(); // Read the integer input
  Serial.print("You will enter ");
  Serial.print(howMany);
  Serial.println(" numbers.");

  TakeInInput(howMany, new int[howMany]);
  #endif
}

void TakeInInput(int sizeInput, int values[])
{
  #ifdef tests
  Serial.print("Size: ");
  Serial.println(sizeInput);
  #endif

  #ifndef tests
  //read in values
  Serial.print("Please enter ");
  Serial.print(sizeInput);
  Serial.println(" integers:");
  for(int i = 0; i < sizeInput; ++i){
    while (Serial.available() == 0) {
      // wait for user input
    }
    values[i] = Serial.parseInt(); // Read the integer input
    Serial.print("You entered: ");
    Serial.println(values[i]);
  }
  #endif

  #ifdef tests
  Serial.print("Values: ");
  for(int i = 0; i < sizeInput; ++i){
    Serial.print(values[i]);
    Serial.print(" ");
  }
  #endif

  //calc stats
  CalcStats(sizeInput, values);
}

void CalcStats(int sizeInput, int values[])
{
  float average = 0;
  int min = values[0];
  int max = values[0];

  for(int i = 0; i < sizeInput; ++i){
    average += values[i];
    if(values[i] < min){
      min = values[i];
    }
    if(values[i] > max){
      max = values[i];
    }
  }

  average /= sizeInput;

  Serial.print("Average: ");
  Serial.println(average);
  Serial.print("Min: ");
  Serial.println(min);
  Serial.print("Max: ");
  Serial.println(max);
}

void loop() {
  // do nothing here
}
