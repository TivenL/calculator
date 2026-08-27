A calculator that's powered by an Arduino Uno.

Hardware used:
- Arduino Uno R3
- 1602 LCD
- 4x4 matrix keypad

The general idea of the calculator is: a character is pressed on the keypad, the character is added to the 1st line of the LCD, when pressing enter, the Arduino calculates the expression, and the answer is displayed on the 2nd line.

Input is validated during key-presses to prevent invalid inputs such as a number with multiple decimals or multiple operators in a row. Calculation follows order of operation and cases such as division by 0 are accounted for. Final answer is rounded to 3 decimal places.
