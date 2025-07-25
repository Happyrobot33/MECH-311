//uncomment this to enable tests
#define tests

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

#ifdef tests
    Serial.println("----TESTS ENABLED----");
#endif

  Serial.println("Math equation to compute is {4 * A - 20} = B");

  int A = -100; //start with a invalid value

  const int MAX_VALUE = 10; //maximum value for a
  const int MIN_VALUE = 0; //minimum value for a

  while (A < MIN_VALUE || A > MAX_VALUE)
  {
      Serial.print("\nEnter a value for A between " + String(MIN_VALUE) + "<-->" + String(MAX_VALUE) + "(inclusive): ");
      while (Serial.available() == 0);
      A = Serial.parseInt();
  }

#ifdef tests
//insert funny for loop
for (int i = MIN_VALUE; i <= MAX_VALUE; i++)
{
    A = i;
    Serial.println("Testing A = " + numberToStr(A) + "...");
#endif
  //we have a valid value for A past here that is bound checked
  Serial.println("Value " + numberToStr(A) + " is valid for A.");

  int B = 4 * A - 20;

  Serial.println("When A equals " + numberToStr(A) + ", then B equals " + numberToStr(B) + ".");

  #ifdef tests
}
  #endif
}

String numberToStr(int num)
{
    //if it is a single digit, represent it as a word
    //if it is more than one digit, then represent it as a numeral instead

    //absolute the value
    int absNum = abs(num);

    switch(absNum) //we do this to avoid duplicate cases
    {
        //breaks arent needed due to returns
        case 0:
            return "zero";
        case 1:
            return getSignedness(num) + " one";
        case 2:
            return getSignedness(num) + " two";
        case 3:
            return getSignedness(num) + " three";
        case 4:
            return getSignedness(num) + " four";
        case 5:
            return getSignedness(num) + " five";
        case 6:
            return getSignedness(num) + " six";
        case 7:
            return getSignedness(num) + " seven";
        case 8:
            return getSignedness(num) + " eight";
        case 9:
            return getSignedness(num) + " nine";
        default: //doesnt match, in this case handling larger than 9
            return String(num);
    }
}

String getSignedness(int num)
{
    if (num > 0)
    {
        return "positive";
    }
    else if (num < 0)
    {
        return "negative";
    }
}

void loop() {
  // do nothing here
}
