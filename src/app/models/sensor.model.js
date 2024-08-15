const mongoose = require("mongoose");
const Schema = mongoose.Schema;

const sensorSchema = new mongoose.Schema(
  {
    humidityAir: {
      type: Number,
    },
    temperature: {
      type: Number,
    },
    CO: {
      type: Number,
    },
    aqi_CO: {
      type: Number,
    },
    so2: {
      type: Number,
    },
    aqi_so2: {
      type: Number,
    },
    no2: {
      type: Number,
    },
    aqi_no2: {
      type: Number,
    },
    pm25: {
      type: Number,
    },
    aqi_pm25: {
      type: Number,
    },
    locationId: {
      type: String,
      ref: 'Room'
    },
    createdDate: {
      type: Date,
      default: () => new Date(), // Thời gian hiện tại theo UTC
    },
    modifiedDate: {
      type: Date,
      default: () => new Date(), // Thời gian hiện tại theo UTC
    },
  },
  { timestamps: true }
);

sensorSchema.pre('save', function(next) {
  const offset = 7 * 60 * 60 * 1000; // 7 giờ = 25,200,000 mili giây
  
  // Cập nhật thời gian chính xác là 16h53 chiều theo giờ Việt Nam
  const exactTime = new Date();
  // exactTime.setUTCHours(9, 53, 0, 0); // 16h53 chiều giờ Việt Nam là 9h53 UTC

  // Cập nhật createdDate nếu chưa có
  if (!this.createdDate) {
    this.createdDate = new Date(exactTime.getTime() + offset);
  }
  this.createdDate = new Date(exactTime.getTime() + offset);
  // Cập nhật modifiedDate với thời gian hiện tại là 16h53 chiều
  this.modifiedDate = new Date(exactTime.getTime() + offset);

  next();
});

module.exports = mongoose.model("Sensor", sensorSchema);
