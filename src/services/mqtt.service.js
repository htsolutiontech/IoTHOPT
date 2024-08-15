const mqtt = require("mqtt");
const smart_home_hat = "smart_home_humidity_and_temperature";
const { insertDataSensorDb } = require("../db/sensor.db");
const report =require('./report.service')
const host_mqtt = "mqttvht.innoway.vn";
const port_mqtt = "8916";
const clientId = `317c0dc6-a039-4b7a-a2ea-fb88b0d43300`;
const connectUrl = `mqtt://${host_mqtt}:${port_mqtt}`;

// thực hiện tạo connect tới mqtt broker
var mqttClient = mqtt.connect(connectUrl, {
  clientId,
  clean: true,
  connectTimeout: 4000,
  reconnectPeriod: 1000,
});

mqttClient.once("connect", function () {
  console.log("Connect to mqtt successfully");
  mqttClient.subscribe(smart_home_hat);

  mqttClient.on("message", async (topic, msg) => {
    const rawMessage = msg.toString();

    try {
      // Try parsing the message as JSON
      const message = JSON.parse(rawMessage);
      const { humidityAir, temperature, CO, pm25, so2, no2, aqi_so2, aqi_CO, aqi_pm25, aqi_no2, location } = message;
// console.log(temperature)
if(temperature>40){
await report();
}
      await insertDataSensorDb({
        humidityAir,
        temperature,
        CO,
        pm25,
        so2,
        no2,
        aqi_so2,
        aqi_CO,
        aqi_pm25,
        aqi_no2,
        location,
      });
    } catch (error) {
      console.error("Error parsing JSON:", error);
      console.error("Raw message:", rawMessage); // Log the message causing the issue
    }
  });
});

mqttClient.on("error", function (error) {
  console.log("Unable to connect: " + error);
});

module.exports = mqttClient;
