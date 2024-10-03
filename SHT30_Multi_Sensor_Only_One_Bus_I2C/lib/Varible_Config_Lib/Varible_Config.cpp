#include <Varible_Config.h>


SHT30 _sensor;

HSTS016L_Config_t sensorConfig;

uint32_t interval = 5000;
uint32_t lastMillis = 0;

float temperature1 = 0.0;
float humidity1 = 0.0;

float temperature2 = 0.0;
float humidity2 = 0.0;

float temperature3 = 0.0;
float humidity3 = 0.0;

double latitude = 0.0;
double longitude = 0.0;

float nowCurrent = 0.0;
float nowPower = 0.0;

String currentTime;

void selectChannel(uint8_t bus)
{
    Wire.beginTransmission(0x70); // TCA9548A address is 0x70
    Wire.write(1 << bus);
    Wire.endTransmission();
    // Serial.println(bus);
}

void SHT30_Setup()
{
    Wire.begin(I2C_SDA, I2C_SCL);
    SHT30_init(&_sensor, 0x44);
    SHT30_begin(&_sensor);
}

void HSTS016L_Setup()
{
    HSTS016L_Init(&sensorConfig, CURRENT_PIN, CALIB_PIN, DEFAULT_R, DEFAULT_Q);

    uint16_t initialCurrent = analogRead(CURRENT_PIN);
    uint16_t initialCalib = analogRead(CALIB_PIN);

    HSTS016L_InitKalmanState(&sensorConfig, initialCurrent, initialCalib);
}

void read_And_Check_Current()
{
    nowCurrent = HSTS016L_ReadDCCurrent(&sensorConfig, NUM_SAMPLES, SAMPLE_INTERVAL, ANALOG_OFFSET);

    time_Flag = (nowCurrent >= OPR_CURRENT_THRESHOLD);

}

void read_Temp_And_Humi()
{
    selectChannel(2);
    temperature1 = SHT30_readTemperature(&_sensor);
    humidity1 = SHT30_readHumidity(&_sensor);

    selectChannel(3);
    temperature2 = SHT30_readTemperature(&_sensor);
    humidity2 = SHT30_readHumidity(&_sensor);

    selectChannel(4);
    temperature3 = SHT30_readTemperature(&_sensor);
    humidity3 = SHT30_readHumidity(&_sensor);

    read_Machine_Power();
}

void read_Lat_And_Long()
{
    if (checkNewData())
    {
        latitude = _gps.location.lat();
        longitude = _gps.location.lng();
    }
}

void read_Real_Time()
{
    currentTime = get_Current_Time();
}

void read_Machine_Power() {
    nowPower = OPR_VOLTAGE * nowCurrent;

}

void print_Data_Var()
{

    displayActiveTime();
    
    Serial.print("Temperature1: ");
    Serial.print(temperature1);
    Serial.print(" °C, Humidity1: ");
    Serial.print(humidity1);
    Serial.print(" %, ");
    Serial.print("Temperature2: ");
    Serial.print(temperature2);
    Serial.print(" °C, Humidity2: ");
    Serial.print(humidity2);
    Serial.print(" %, ");
    Serial.print("Temperature3: ");
    Serial.print(temperature3);
    Serial.print(" °C, Humidity3: ");
    Serial.print(humidity3);
    Serial.println(" %, ");

    Serial.print(F("Latitude: "));
    Serial.print(latitude, 6);
    Serial.print(F(", Longitude: "));
    Serial.print(longitude, 6);
    Serial.print(F(", Current: "));
    Serial.print(nowCurrent);
    Serial.print(" A");
    Serial.print(F(", Power: "));
    Serial.print(nowPower);
    Serial.print(" W");
    Serial.print(", Operation Flag: ");
    Serial.println(time_Flag);

    Serial.print("Operation Time: " + OPR_Time);
    Serial.print(", Total Number of Light On/Off Events: ");
    Serial.print(on_Off_Light_Count);
    Serial.println(", Current Time: " + currentTime);
    
}

void update_Data_To_Server()
{
    upload_Data(temperature1, humidity1, temperature2, humidity2, temperature3, humidity3, 
                nowCurrent, nowPower, latitude, longitude, on_Off_Light_Count, time_Flag, OPR_Time, currentTime);

    Serial.println("==================================================================================================================================");

}