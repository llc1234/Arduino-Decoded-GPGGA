#include <SoftwareSerial.h>

SoftwareSerial ss(4, 3);  // RX, TX

String gpsData = "";

/*
<1: Excellent
1-2: Good
2-5: Moderate
5-10: Fair
>10: Poor
*/

void setup() {
  Serial.begin(9600);
  ss.begin(9600);
}

void loop() {
  while (ss.available() > 0) {
    char c = ss.read();
    gpsData += c;
    
    if (c == '\n') {
      if (gpsData.startsWith("$GPGGA")) {
        decodeGPGGA(gpsData);
      }
      gpsData = "";
    }
  }
}

void decodeGPGGA(String sentence) {
  int checksumIndex = sentence.indexOf('*');
  String usefulData = sentence.substring(1, checksumIndex);
  
  int index = 0;
  String parts[15];
  for (int i = 0; i < 15; i++) {
    index = usefulData.indexOf(',');
    if (index == -1) {
      parts[i] = usefulData;
      break;
    }
    parts[i] = usefulData.substring(0, index);
    usefulData = usefulData.substring(index + 1);
  }

  
  double latitude = convertToDecimalDegrees(parts[2], parts[3]);
  double longitude = convertToDecimalDegrees(parts[4], parts[5]);


  String satellites_ = parts[7];
  if (satellites_.length() > 1 && satellites_[0] == '0') {
    satellites_ = satellites_.substring(1);
  }

  
  // Serial.println("Decoded GPGGA Sentence:");
  // Serial.print("UTC Time: "); Serial.println(parts[1]);
  Serial.print("Latitude: "); Serial.print(latitude, 6);
  Serial.print(", Longitude: "); Serial.print(longitude, 6);
  // Serial.print("Fix Quality: "); Serial.println(parts[6]);
  Serial.print(", Satellites: "); Serial.print(satellites_);
  Serial.print(", Precision: "); Serial.print(parts[8]);
  Serial.print(", Altitude: "); Serial.print(parts[9]); Serial.print(" "); Serial.println(parts[10]);
  // Serial.print("Height of Geoid: "); Serial.print(parts[11]); Serial.print(" "); Serial.println(parts[12]);
  // Serial.print("Time since last DGPS update: "); Serial.println(parts[13]);
  // Serial.print("DGPS reference station id: "); Serial.println(parts[14]);
  // Serial.println();
}

double convertToDecimalDegrees(String nmeaCoord, String direction) {
  double coord = nmeaCoord.toFloat();
  int degrees = int(coord / 100);
  double minutes = coord - (degrees * 100);
  double decimalDegrees = degrees + (minutes / 60);
  
  // Apply the direction (N/S/E/W)
  if (direction == "S" || direction == "W") {
    decimalDegrees *= -1;
  }
  
  return decimalDegrees;
}
