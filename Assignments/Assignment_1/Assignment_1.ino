//uncomment this to enable tests
#define tests

const int MAX_VALUE = 10; //maximum value for a
const int MIN_VALUE = 0; //minimum value for a

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  Serial.println("Math equation to compute is {4 * A - 20} = B");

  int A = -100; //start with a invalid value


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

    //cant be string array, C++ will NOT let it work
    const char* words[10] = {"zero", " one", " two", " three", " four", " five", " six", " seven", " eight", " nine"};

    //return words[9];

    if (absNum < MAX_VALUE && absNum >= MIN_VALUE)
    {
        //the space being here is NECCESARY or stuff breaks because C++ is stupid
        String message = getSignedness(num) + words[absNum];
        return message;
    }
    return String(num);
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

    return "";
}

void loop() {
  // do nothing here
}
