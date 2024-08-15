const router = require("express").Router();
const { authUser } = require("../middlewares/auth.middleware");
const asyncWrap = require("../utils/asyncWrap");
const {
  getSensor,
  getDataSensor,
  insertDataSensor,
  getSensorByRoom,
  getAllSensorData
} = require("../app/controllers/sensor.controller");

router.get("/", authUser, asyncWrap(getSensor));
router.get("/:roomId", authUser, asyncWrap(getSensorByRoom));
router.get("/all-data/:roomId", authUser, asyncWrap(getAllSensorData));

router.post("/get-data", authUser, asyncWrap(getDataSensor));
// router.get("/history/:roomId", authUser, asyncWrap(getSensorHistory));
router.post("/push-data", authUser, asyncWrap(insertDataSensor));


module.exports = router;
