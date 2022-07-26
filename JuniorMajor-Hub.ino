#define DEBUG_LOGS 0 // 1 for logs, 0 for no logs

struct __attribute__((packed)) ColumnState
{
    union
    {
        struct
        {
            uint16_t encoder1 : 2;
            uint16_t encoder2 : 2;
            uint16_t encoder3 : 2;
            uint16_t joystick_x : 2;
            uint16_t joystick_y : 2;
            uint16_t button : 1;
            uint16_t padding : 5;
        };
        uint8_t data[2];
    };
};

struct __attribute__((packed)) ReceiveState
{
    union
    {
        struct
        {
            union
            {
                struct
                {
                    ColumnState column;
                };
                uint8_t payload[sizeof(ColumnState)];
            };
            uint8_t crc8;
        };
        uint8_t data[sizeof(ColumnState) + sizeof(uint8_t)];
    };
};

struct __attribute__((packed)) TransmitState
{
    union
    {
        struct
        {
            union
            {
                struct
                {
                    uint8_t header[2];
                    ColumnState column[4];
                };
                uint8_t payload[4 * sizeof(ColumnState) + 2 * sizeof(uint8_t)];
            };
            uint8_t crc8;
        };
        uint8_t data[4 * sizeof(ColumnState) + 3 * sizeof(uint8_t)];
    };
};

void setup()
{
    // Debug USB
    Serial.begin(115200);
    // RS485 output
    Serial1.begin(115200);
    // RS485 input1
    Serial2.begin(115200);
    // input 2
    Serial3.begin(115200);
    // input 3
    Serial4.begin(115200);
    // input 4
    Serial5.begin(115200);
}

void loop()
{
    ReceiveState receive;
    TransmitState transmit;

    uint32_t millisLastTransmit = millis();

    while (true)
    {
        if (millis() - millisLastTransmit >= 20)
        {
            millisLastTransmit = millis();

            bool doWrite = false;
            memset(&receive, 0, sizeof(receive));
            memset(&transmit, 0, sizeof(transmit));
            // Column1
            if (Serial2.available() >= 3)
            {
                Serial2.readBytes(receive.data, 3);
                if (crc8(receive.payload, sizeof(receive.payload)) == receive.crc8)
                {
                    transmit.column[0] = receive.column;
                    doWrite = true;
                }
                memset(&receive, 0, sizeof(receive));
            }
            else
            {
                Serial2.flush();
            }
            // Column2
            if (Serial3.available() >= 3)
            {
                Serial3.readBytes(receive.data, 3);
                if (crc8(receive.payload, sizeof(receive.payload)) == receive.crc8)
                {
                    transmit.column[1] = receive.column;
                    doWrite = true;
                }
                memset(&receive, 0, sizeof(receive));
            }
            else
            {
                Serial3.flush();
            }
            // Column3
            if (Serial4.available() >= 3)
            {
                Serial4.readBytes(receive.data, 3);
                if (crc8(receive.payload, sizeof(receive.payload)) == receive.crc8)
                {
                    transmit.column[2] = receive.column;
                    doWrite = true;
                }
                memset(&receive, 0, sizeof(receive));
            }
            else
            {
                Serial4.flush();
            }
            // Column4
            if (Serial5.available() >= 3)
            {
                Serial5.readBytes(receive.data, 3);
                if (crc8(receive.payload, sizeof(receive.payload)) == receive.crc8)
                {
                    transmit.column[3] = receive.column;
                    doWrite = true;
                }
                memset(&receive, 0, sizeof(receive));
            }
            else
            {
                Serial5.flush();
            }
            if (doWrite)
            {
                transmit.header[0] = 0xC0;
                transmit.header[1] = 0xDB;
                transmit.crc8 = crc8(transmit.payload, sizeof(transmit.payload));

                // Debug USB
                if (DEBUG_LOGS)
                {
                    // TEST
                    uint8_t header1 = transmit.header[0];
                    uint8_t header2 = transmit.header[1];
                    uint8_t crc8_calc = crc8(transmit.payload, sizeof(transmit.payload));
                    uint8_t crc8 = transmit.crc8;
                    // Column 1
                    uint8_t column1_encoder1 = transmit.column[0].encoder1;
                    uint8_t column1_encoder2 = transmit.column[0].encoder2;
                    uint8_t column1_encoder3 = transmit.column[0].encoder3;
                    uint8_t column1_joystick_x = transmit.column[0].joystick_x;
                    uint8_t column1_joystick_y = transmit.column[0].joystick_y;
                    uint8_t column1_button = transmit.column[0].button;
                    // Column 2
                    uint8_t column2_encoder1 = transmit.column[1].encoder1;
                    uint8_t column2_encoder2 = transmit.column[1].encoder2;
                    uint8_t column2_encoder3 = transmit.column[1].encoder3;
                    uint8_t column2_joystick_x = transmit.column[1].joystick_x;
                    uint8_t column2_joystick_y = transmit.column[1].joystick_y;
                    uint8_t column2_button = transmit.column[1].button;
                    // Column 3
                    uint8_t column3_encoder1 = transmit.column[2].encoder1;
                    uint8_t column3_encoder2 = transmit.column[2].encoder2;
                    uint8_t column3_encoder3 = transmit.column[2].encoder3;
                    uint8_t column3_joystick_x = transmit.column[2].joystick_x;
                    uint8_t column3_joystick_y = transmit.column[2].joystick_y;
                    uint8_t column3_button = transmit.column[2].button;
                    // Column 4
                    uint8_t column4_encoder1 = transmit.column[3].encoder1;
                    uint8_t column4_encoder2 = transmit.column[3].encoder2;
                    uint8_t column4_encoder3 = transmit.column[3].encoder3;
                    uint8_t column4_joystick_x = transmit.column[3].joystick_x;
                    uint8_t column4_joystick_y = transmit.column[3].joystick_y;
                    uint8_t column4_button = transmit.column[3].button;

                    Serial.print("header1: ");
                    Serial.println(header1);
                    Serial.print("header2: ");
                    Serial.println(header2);
                    Serial.print("crc8_calc: ");
                    Serial.println(crc8_calc);
                    Serial.print("crc8: ");
                    Serial.println(crc8);

                    Serial.print("column1_encoder1: ");
                    Serial.println(column1_encoder1);
                    Serial.print("column1_encoder2: ");
                    Serial.println(column1_encoder2);
                    Serial.print("column1_encoder3: ");
                    Serial.println(column1_encoder3);
                    Serial.print("column1_joystick_x: ");
                    Serial.println(column1_joystick_x);
                    Serial.print("column1_joystick_y: ");
                    Serial.println(column1_joystick_y);
                    Serial.print("column1_button: ");
                    Serial.println(column1_button);

                    Serial.print("column2_encoder1: ");
                    Serial.println(column2_encoder1);
                    Serial.print("column2_encoder2: ");
                    Serial.println(column2_encoder2);
                    Serial.print("column2_encoder3: ");
                    Serial.println(column2_encoder3);
                    Serial.print("column2_joystick_x: ");
                    Serial.println(column2_joystick_x);
                    Serial.print("column2_joystick_y: ");
                    Serial.println(column2_joystick_y);
                    Serial.print("column2_button: ");
                    Serial.println(column2_button);

                    Serial.print("column3_encoder1: ");
                    Serial.println(column3_encoder1);
                    Serial.print("column3_encoder2: ");
                    Serial.println(column3_encoder2);
                    Serial.print("column3_encoder3: ");
                    Serial.println(column3_encoder3);
                    Serial.print("column3_joystick_x: ");
                    Serial.println(column3_joystick_x);
                    Serial.print("column3_joystick_y: ");
                    Serial.println(column3_joystick_y);
                    Serial.print("column3_button: ");
                    Serial.println(column3_button);

                    Serial.print("column4_encoder1: ");
                    Serial.println(column4_encoder1);
                    Serial.print("column4_encoder2: ");
                    Serial.println(column4_encoder2);
                    Serial.print("column4_encoder3: ");
                    Serial.println(column4_encoder3);
                    Serial.print("column4_joystick_x: ");
                    Serial.println(column4_joystick_x);
                    Serial.print("column4_joystick_y: ");
                    Serial.println(column4_joystick_y);
                    Serial.print("column4_button: ");
                    Serial.println(column4_button);
                }
                // Only print HEX over USB if logs are disabled
                if (!DEBUG_LOGS)
                {
                    Serial.write(transmit.data, sizeof(transmit));
                }
                //  RS485
                Serial1.write(transmit.data, sizeof(transmit));
            }
        }
    }
}

uint8_t crc8(const uint8_t *data, uint8_t len)
{
    uint8_t crc = 0x00;
    while (len--)
    {
        uint8_t extract = *data++;
        for (uint8_t tempI = 8; tempI; tempI--)
        {
            uint8_t sum = (crc ^ extract) & 0x01;
            crc >>= 1;
            if (sum)
            {
                crc ^= 0x8C;
            }
            extract >>= 1;
        }
    }
    return crc;
}
