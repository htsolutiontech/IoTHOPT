require('dotenv').config();


const devConfig = {
  app: {
    port:process.env.DEV_PORT || 4000,
    hostname: process.env.DEV_HOST_NAME || "localhost",
  },
  emailCfg: {
    emailUser: process.env.EMAIL_USER || 'trannamphuong0406@gmail.com',
    emailPassword: process.env.EMAIL_PASSWORD || 'vppw gvhe macc nszr',
  },
  db: {
    port : process.env.DEV_DB_PORT || 27018,
    hostname: process.env.DEV_DB_HOST_NAME || "localhost",
    username: process.env.DEV_DB_USER_NAME || "root",
    password: process.env.DEV_DB_PASSWORD || "123456",
    name: process.env.DEV_DB_NAME || "smarthome22"
  }
};

const proConfig = {
  app: {
    port :process.env.PRO_PORT || 4000,
    hostname: process.env.PRO_HOST_NAME,

  },
  db: {
    port : process.env.PRO_DB_PORT || 27018,
    hostname: process.env.PRO_DB_HOST_NAME || "localhost",
    username: process.env.PRO_DB_USER_NAME || "root",
    password: process.env.PRO_DB_PASSWORD || "123456",
    name: process.env.PRO_DB_NAME || "smarthome"
  }
};

const config = {
  dev: devConfig,
  pro: proConfig
}
const env = process.env.NODE_ENV ||"dev";

module.exports = config[env];
