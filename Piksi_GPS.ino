// Reading and parsing GPS data from PIKSI GPS Swift Navighation system //
// ---------------------------- Modified by ----------------------------//
// --------------------------- Kalpesh Patil ---------------------------//
// ---------------------------------------------------------------------//

// Reads position_Baseline NED data
// Reads velocity_Baseline NED data

const unsigned int MAX_INPUT = 200;
String line1, line2;
void setup() {
  Serial.begin(115200);
  Serial2.begin (115200);
}

void loop() {

  while (Serial2.available () > 0)
  {
    processIncomingByte (Serial2.read ());
  }
}

void processIncomingByte (const byte inByte)
{
  //Serial.println(inByte , HEX);

  static byte input_msg [MAX_INPUT];
  static unsigned int input_pos = 0;

  if (inByte == 0x55)
  {
    if (input_pos == 29)
    {
      for ( int i = 0 ; i < input_pos ; i++)
      {
        // Serial.print(input_msg [i], HEX);
        // Serial.print(" ");
      }
      // Serial.println();
      if ((input_msg[0] == 0x03) && (input_msg[1] == 0x02))
      {
        pos_msg_analyse (input_msg);
      }
      if ((input_msg[0] == 0x05) && (input_msg[1] == 0x02))
      {
        vel_msg_analyse (input_msg);
      }
    }
    input_pos = 0;
  }
  else
  {
    if (input_pos < (MAX_INPUT - 1))
    {
      input_msg [input_pos] = inByte;
      input_pos = input_pos + 1;
    }
  }

}

void pos_msg_analyse (byte byte_msg[29])
{
  double pos_ntime = 0;
  pos_ntime = bytesToInt(byte_msg[8], byte_msg[7], byte_msg[6], byte_msg[5]);
  // Serial.print(pos_ntime); Serial.print("  ");

  double pos_north = 0;
  pos_north = bytesToInt(byte_msg[12], byte_msg[11], byte_msg[10], byte_msg[9]);
//  Serial.print(pos_north); Serial.print("  ");

  double pos_east = 0;
  pos_east = bytesToInt(byte_msg[16], byte_msg[15], byte_msg[14], byte_msg[13]);
//  Serial.print(pos_east); Serial.print("  ");

  double pos_down = 0;
  pos_down = bytesToInt(byte_msg[20], byte_msg[19], byte_msg[18], byte_msg[17]);
//  Serial.print(pos_down); Serial.print("  ");

  double pos_hacc = 0;
  pos_hacc = Bytes2Intu16(byte_msg[22], byte_msg[21]);
//  Serial.print(pos_hacc); Serial.print("  ");

  double pos_vacc = 0;
  pos_vacc = Bytes2Intu16(byte_msg[24], byte_msg[23]);
//  Serial.print(pos_vacc); Serial.print("  ");

  double distance = 0;
  distance = sqrt(pos_north * pos_north + pos_east * pos_east);
//  Serial.print(distance); Serial.print("  ");

  line1 = String(pos_ntime) + "," + String(pos_north) + "," + String(pos_east) + "," + String(pos_down) + "," + String(pos_hacc) + "," + String(pos_vacc) + "," + String(distance) + "," ;
}

void vel_msg_analyse (byte byte_msg[29])
{
  double vel_ntime = 0;
  vel_ntime = bytesToInt(byte_msg[8], byte_msg[7], byte_msg[6], byte_msg[5]);
//  Serial.print(vel_ntime); Serial.print("  ");

  double vel_north = 0;
  vel_north = bytesToInt(byte_msg[12], byte_msg[11], byte_msg[10], byte_msg[9]);
//  Serial.print(vel_north); Serial.print("  ");

  double vel_east = 0;
  vel_east = bytesToInt(byte_msg[16], byte_msg[15], byte_msg[14], byte_msg[13]);
//  Serial.print(vel_east); Serial.print("  ");

  double vel_down = 0;
  vel_down = bytesToInt(byte_msg[20], byte_msg[19], byte_msg[18], byte_msg[17]);
//  Serial.print(vel_down); Serial.print("  ");

  double vel_hacc = 0;
  vel_hacc = Bytes2Intu16(byte_msg[22], byte_msg[21]);
//  Serial.print(vel_hacc); Serial.print("  ");

  double vel_vacc = 0;
  vel_vacc = Bytes2Intu16(byte_msg[24], byte_msg[23]);
//  Serial.print(vel_vacc); Serial.println("  ");

  line2 = line1 + String(vel_ntime) + "," + String(vel_north) + "," + String(vel_east) + "," + String(vel_down) + "," + String(vel_hacc) + "," + String(vel_vacc) + ",";
  Serial.println(line2);

}

int bytesToInt (int b4, int b3, int b2, int b1)
{
  int resultat = 0;
  resultat = (b4 << 24) | (b3 << 16) | (b2 << 8) | b1;
  return resultat;
}

int Bytes2Intu16 (int b2, int b1)
{
  int result = 0;
  result = (b2 << 8) | b1;
  return result;
}

