#ifndef UTIL_H
#define UTIL_H

#define SERIAL_DEBUG_ENABLED
#ifdef SERIAL_DEBUG_ENABLED 
  #define DebugPrint(...)  \
        Serial.print(millis());     \
        Serial.print(": ");    \
        Serial.print(__PRETTY_FUNCTION__); \
        Serial.print(' ');      \
        Serial.print(__LINE__);     \
        Serial.print(' ');      \
        Serial.print(__VA_ARGS__)
  #define DebugPrintln(...)  \
        Serial.print(millis());     \
        Serial.print(": ");    \
        Serial.print(__PRETTY_FUNCTION__); \
        Serial.print(' ');      \
        Serial.print(__LINE__);     \
        Serial.print(' ');      \
        Serial.println(__VA_ARGS__)
#else
  #define DebugPrint(...)
  #define DebugPrintln(...)  
#endif

void print_array(int a[], int len) {
    Serial.print("[");
    for (int i = 0; i < len; i++) {
        Serial.print(a[i]);
        if (i != len - 1) {
            Serial.print(", ");
        }
    }
    Serial.println("]");
}



#endif