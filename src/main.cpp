#include <Arduino.h>
#include <Keypad.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

char keys[4][4] = {{'1', '2', '3', '+'},
                   {'4', '5', '6', '-'},
                   {'7', '8', '9', '*'},
                   {'.', '0', '=', '/'}};
byte rowPins[4] = {5, 4, 3, 2};
byte colPins[4] = {A0, A1, A2, A3};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

String input = "";
bool newCalculation = false;
bool hasDecimal = false;  // To avoid input errors
bool hasOperator = false; //
const int MAX_INPUT = 15; //

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
}

bool isOperator(char c) { return c == '+' || c == '-' || c == '*' || c == '/'; }

double calculate(String str) {
  if (str.length() == 0)
    return NAN;

  double result = 0;
  double prevNum = 0;
  char nextOp = '+';
  String currentNumStr = "";

  for (size_t i = 0; i <= str.length(); i++) {
    char c;
    if (i < str.length()) {
      c = str.charAt(i);
    } else {
      c = '#'; // End of line
    }

    if (isDigit(c) || c == '.') {
      currentNumStr += c;
    } else if (isOperator(c) || c == '#') {
      double currentNum = currentNumStr.toFloat();
      currentNumStr = "";

      if (nextOp == '+') {
        result += prevNum;
        prevNum = currentNum;
      } else if (nextOp == '-') {
        result += prevNum;
        prevNum = -currentNum;
      } else if (nextOp == '*') {
        prevNum *= currentNum;
      } else if (nextOp == '/') {
        if (currentNum == 0)
          return NAN; // Divide by 0 error
        else
          prevNum /= currentNum;
      }
      nextOp = c;
    }
  }
  return result + prevNum;
}

void loop() {
  if ((millis() / 500) % 2 == 0) {
    lcd.cursor();
  } else {
    lcd.noCursor();
  }

  char c = keypad.getKey();
  if (c) {
    if (newCalculation) {
      input = "";
      newCalculation = false;
      hasDecimal = false;
      hasOperator = false;
      lcd.clear();
    }

    if (c == '=') {
      lcd.setCursor(0, 1);
      lcd.print('=');
      lcd.print(calculate(input), 3);
      newCalculation = true;
    } else if (input.length() >= MAX_INPUT) {
      // pass
    } else if (isOperator(c)) {
      if (hasOperator || input.length() == 0) {
        // pass
      } else {
        input += c;
        hasOperator = true;
        hasDecimal = false;
        lcd.print(c);
      }
    } else if (c == '.') {
      if (hasDecimal) {
        // pass
      } else {
        input += c;
        hasDecimal = true;
        hasOperator = false;
        lcd.print(c);
      }
    } else {
      input += c;
      hasOperator = false;
      lcd.print(c);
    }
  }
}