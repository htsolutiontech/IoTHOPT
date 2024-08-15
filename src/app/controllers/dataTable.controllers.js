// const Sensor = require('../models/sensor.model'); // Giả sử bạn có model sensor

// const getDataTable = async (req, res) => {
//   try {
//     const sensors = await Sensor.find().sort({ createdDate: -1 }); // Lấy dữ liệu sensor từ DB và sắp xếp theo thời gian mới nhất
//     const formattedData = sensors.map(sensor => ({
//       date: sensor.createdDate,
//       temperature: sensor.avgTemperature,
//       humidity: sensor.avgHumidityAir,
//     }));
//     res.json(formattedData);
//   } catch (error) {
//     res.status(500).json({ error: error.message });
//   }
// };

// module.exports = {
//   getDataTable,
// };
