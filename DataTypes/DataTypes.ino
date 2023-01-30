#include <Arduino.h>

// Pins can be assigned like this
int PIN_A = D7 // GPIO13
int PIN_A = 13 // D7

// This can also be used. Takes less RAM.
#define PIN_A 13

void setup() {
  // initialize serial communication at a baud rate of 9600
  Serial.begin(9600);

  // declare and initialize variables of different data types
  // int (integer) data type
  int myInt = -5;
  Serial.print("Integer value: ");
  Serial.println(myInt);

  // float (floating-point number) data type
  float myFloat = 3.14;
  Serial.print("Float value: ");
  Serial.println(myFloat);

  // double (double precision floating-point number) data type
  double myDouble = 3.14159265358979323846;
  Serial.print("Double value: ");
  Serial.println(myDouble);

  // boolean data type
  bool myBool = true;
  Serial.print("Boolean value: ");
  Serial.println(myBool);

  // char (character) data type
  char myChar = 'A';
  Serial.print("Character value: ");
  Serial.println(myChar);

  // string data type
  String myString = "Hello, world!";
  Serial.print("String value: ");
  Serial.println(myString);

  // byte data type
  byte myByte = 0xFF;
  Serial.print("Byte value: ");
  Serial.println(myByte);

  // long data type
  long myLong = -2000000;
  Serial.print("Long value: ");
  Serial.println(myLong);
  
  // unsigned int
  unsigned int myUint = 12345;
  Serial.print("Unsigned int value: ");
  Serial.println(myUint);

  // array
  int myArray[] = {1, 2, 3, 4, 5};
  Serial.print("Array: ");
  for (int i = 0; i < 5; i++) {
    Serial.print(myArray[i]);
    Serial.print(" ");
  }
  Serial.println();
  
  // short (short integer) data type
  short myShort = -32000;
  Serial.print("Short value: ");
  Serial.println(myShort);
  
  // unsigned char data type
  unsigned char myUchar = 250;
  Serial.print("Unsigned char value: ");
  Serial.println(myUchar);

  // signed long long data type
  signed long long mySLL = -124567891011;
  Serial.print("Signed long long value: ");
  Serial.println(mySLL);

  // unsigned long data type
  unsigned long myUL = 124567891011;
  Serial.print("Unsigned long value: ");
  Serial.println(myUL);

  // size_t data type
  size_t mySize = 124567891011;
  Serial.print("size_t value: ");
  Serial.println(mySize);

  // ptrdiff_t data type
  ptrdiff_t myPTRdiff = -124567891011;
  Serial.print("ptrdiff_t value: ");
  Serial.println(myPTRdiff);

  // wchar_t data type
  wchar_t myWchar = L'中';
  Serial.print("wchar_t value: ");
  Serial.println(myWchar);

  // nullptr_t data type
  nullptr_t myNull = nullptr;
  Serial.print("nullptr_t value: ");
  Serial.println(myNull);

  // complex data type
  std::complex<double> myComplex(3.0, 4.0);
  Serial.print("Complex number value: ");
  Serial.println(myComplex);
  
  // enum data type
  enum Color {RED, GREEN, BLUE};
  Color myColor = RED;
  Serial.print("Enumerated Color Value: ");
  Serial.println(myColor);

  // struct data type
  struct Point {
    int x;
    int y;
  };
  Point myPoint;
  myPoint.x = 10;
  myPoint.y = 20;
  Serial.print("Struct Point Value: x:");
  Serial.print(myPoint.x);
  Serial.print(" y:");
  Serial.println(myPoint.y);

  // union data type
  union myUnion{
    int intValue;
    float floatValue;
    char charValue;
  };
  myUnion myData;
  myData.intValue = 5;
  Serial.print("Union value: ");
  Serial.println(myData.intValue);
  
  // function pointer example
  int myFunction(int a) {
    // multiplies the input by 2
    return a*2;
  }
  int (*functionPointer)(int) = &myFunction; // declare the function pointer
  int result = functionPointer(5); // call the function through the pointer
  Serial.print("Function Pointer Result: ");
  Serial.println(result);

  // template example
  template<typename T>
  T max(T a, T b) {
    // returns the greater value
    return (a > b) ? a : b;
  }
  int a = 5, b = 7;
  Serial.print("Max value: ");
  Serial.println(max(a,b));

  // class example
  class MyClass {
    int myValue;
    public:
        MyClass(int value) {
            myValue = value;
        }
        // getter method
        int getValue() {
            return myValue;
        }
        // setter method
        void setValue(int value) {
            myValue = value;
        }
  };
  MyClass myObject(5);
  Serial.print("Class value: ");
  Serial.println(myObject.getValue());

}

// TODO: include updating missing/remaining variables/data types
void loop() {
  // update the values of the variables
  myInt++;
  myFloat += 0.1;
  myDouble += 0.1;
  myBool = !myBool;
  myChar++;
  myString += "!";
  myByte++;
  myLong += 100;
  myUint++;
  for (int i = 0; i < 5; i++) {
      myArray[i]++;
  }
  myShort++;
  myUchar++;
  mySLL += 100;
  myUL += 100;
  mySize += 100;
  myPTRdiff += 100;
  myWchar++;
  myColor = static_cast<Color>((myColor + 1) % 3);
  myPoint.x++;
  myPoint.y++;
  myData.intValue++;

  // print the new values of the variables
  Serial.print("Integer value: ");
  Serial.println(myInt);
  Serial.print("Float value: ");
  Serial.println(myFloat);
  Serial.print("Double value: ");
  Serial.println(myDouble);
  Serial.print("Boolean value: ");
  Serial.println(myBool);
  Serial.print("Character value: ");
  Serial.println(myChar);
  Serial.print("String value: ");
  Serial.println(myString);
  Serial.print("Byte value: ");
  Serial.println(myByte);
  Serial.print("Long value: ");
  Serial.println(myLong);
  Serial.print("Unsigned int value: ");
  Serial.println(myUint);
  Serial.print("Array: ");
  for (int i = 0; i < 5; i++) {
      Serial.print(myArray[i]);
      Serial.print(" ");
  }
  Serial.println();
  Serial.print("Short value: ");
  Serial.println(myShort);
  Serial.print("Unsigned char value: ");
  Serial.println(myUchar);
  Serial.print("Signed long long value: ");
  Serial.println(mySLL);
  Serial.print("Unsigned long value: ");
  Serial.println(myUL);
  Serial.print("size_t value: ");
  Serial.println(mySize);
  Serial.print("ptrdiff_t value: ");
  Serial.println(myPTRdiff);
  Serial.print("wchar_t value: ");
  Serial.println(myWchar);
  Serial.print("nullptr_t value: ");
  Serial.println(myNull);
  Serial.print("Complex number value: ");
  Serial.println(myComplex);
  Serial.print("Enumerated Color Value: ");
  Serial.println(myColor);
  Serial.print("Struct Point Value: x:");
  Serial.print(myPoint.x);
  Serial.print(" y:");
  Serial.println(myPoint.y);
  Serial.print("Union value: ");
  Serial.println(myData.intValue);
  delay(1000); // wait for 1 second before updating the values again
  
}

/*
In this version of the loop function, I update the values of all the variables declared in the setup function and then I printed the new values, so you can see how the values of the variables change with time.
It's important to note that in this example the delay of 1000ms is used to give you the chance to see the values changing, you can adjust the delay depending on your use case.
It's also important to keep in mind that the way you update the variables should match the data types and the purpose of your sketch.
It's important to keep in mind that some variables can't be incremented like this, for example strings,struct,enum or in some cases arrays, so you need to be careful when making updates to these variables.
You should also make sure that any updates to the variables stay within their expected range of values, otherwise, you might experience unexpected behavior in your sketch.
You may also want to consider using some conditional statements to control the updates of specific variables based on certain conditions.
For example, you could use an if statement to only update the value of a variable if a certain button is pressed.
You could also use a while loop to repeatedly update a variable until a certain condition is met.
It is important to test the sketch carefully in each state and with different inputs, to make sure it's working as expected, and if necessary, to make any adjustments to the sketch to improve its performance or functionality.
Also, Keep in mind that updating the variables constantly may impact the performance of your sketch and can use up more resources like processing power and memory.
*/