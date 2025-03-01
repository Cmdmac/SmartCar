#include "QMI8658.h"

const char* TAG_QMI8658 = "QMI8658";
void QMI8658::setUp() {
    
//     typedef struct {
//     i2c_mode_t mode;     /*!< I2C mode */
//     int sda_io_num;      /*!< GPIO number for I2C sda signal */
//     int scl_io_num;      /*!< GPIO number for I2C scl signal */
//     bool sda_pullup_en;  /*!< Internal GPIO pull mode for I2C sda signal*/
//     bool scl_pullup_en;  /*!< Internal GPIO pull mode for I2C scl signal*/

//     union {
//         struct {
//             uint32_t clk_speed;      /*!< I2C clock frequency for master mode, (no higher than 1MHz for now) */
//         } master;                    /*!< I2C master config */
// #if SOC_I2C_SUPPORT_SLAVE
//         struct {
//             uint8_t addr_10bit_en;   /*!< I2C 10bit address mode enable for slave mode */
//             uint16_t slave_addr;     /*!< I2C address for slave mode */
//             uint32_t maximum_speed;  /*!< I2C expected clock speed from SCL. */
//         } slave;                     /*!< I2C slave config */
// #endif // SOC_I2C_SUPPORT_SLAVE
//     };
//     uint32_t clk_flags;              /*!< Bitwise of ``I2C_SCLK_SRC_FLAG_**FOR_DFS**`` for clk source choice*/
// } i2c_config_t;
/*
    i2c_config_t i2c_conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = QMI_I2C_SDA,
        .scl_io_num = QMI_I2C_SCL,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        // .master.clk_speed = QMI_I2C_FREQ_HZ
    };
    i2c_conf.master.clk_speed = QMI_I2C_FREQ_HZ;
    i2c_param_config(I2C_NUM_0, &i2c_conf);

    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, i2c_conf.mode, 0, 0, 0));
*/

    uint8_t id = 0; // 芯片的ID号

    readRegister(QMI8658_WHO_AM_I, &id ,1); // 读芯片的ID号
    while (id != 0x05)  // 判断读到的ID号是否是0x05
    {
        ESP_LOGI(TAG_QMI8658, "find id ");
        vTaskDelay(1000 / portTICK_PERIOD_MS);  // 延时1秒
        readRegister(QMI8658_WHO_AM_I, &id ,1); // 读取ID号
    }
    ESP_LOGI(TAG_QMI8658, "QMI8658 OK!");  // 打印信息

    writeRegister(QMI8658_RESET, 0xb0);  // 复位  
    vTaskDelay(10 / portTICK_PERIOD_MS);  // 延时10ms
    writeRegister(QMI8658_CTRL1, 0x40); // CTRL1 设置地址自动增加
    writeRegister(QMI8658_CTRL7, 0x03); // CTRL7 允许加速度和陀螺仪
    writeRegister(QMI8658_CTRL2, 0x95); // CTRL2 设置ACC 4g 250Hz
    writeRegister(QMI8658_CTRL3, 0xd5); // CTRL3 设置GRY 512dps 250Hz 
}

// 读取QMI8658寄存器的值
// esp_err_t QMI8658::readRegister(uint8_t reg_addr, uint8_t *data, size_t len)
// {
//     return i2c_master_write_read_device(I2C_NUM_0, QMI8658_SENSOR_ADDR,  &reg_addr, 1, data, len, 1000 / portTICK_PERIOD_MS);
// }
bool QMI8658::readRegister(uint8_t reg, uint8_t *value, int len) {
    _wire->beginTransmission(this->_addr);
    _wire->write(reg);
    uint8_t ret = _wire->endTransmission(false);
    if (ret != 0) {
        // DEBUG_PRINTELN("PCA9557 write fail code " + String(ret));
        return false;
    }

    int n = _wire->requestFrom(this->_addr, (uint8_t) len);
    if (n != len) {
        // DEBUG_PRINTELN("PCA9557 read fail");
        return false;
    }

    *value = _wire->read();

    return true;
}

// 给QMI8658的寄存器写值
// esp_err_t QMI8658::writeRegister(uint8_t reg_addr, uint8_t data)
// {
//     uint8_t write_buf[2] = {reg_addr, data};

//     return i2c_master_write_to_device(I2C_NUM_0, QMI8658_SENSOR_ADDR, write_buf, sizeof(write_buf), 1000 / portTICK_PERIOD_MS);
// }
bool QMI8658::writeRegister(uint8_t reg, uint8_t value) {
    _wire->beginTransmission(this->_addr);
    _wire->write(reg);
    _wire->write(value);
    uint8_t ret = _wire->endTransmission();
    if (ret != 0) {
        // DEBUG_PRINTELN("PCA9557 write fail code " + String(ret));
        return false;
    }

    return true;
}

// 读取加速度和陀螺仪寄存器值
void QMI8658::readAccAndGry(QMI8658Data *p)
{
    uint8_t status, data_ready=0;
    int16_t buf[6];

    readRegister(QMI8658_STATUS0, &status, 1); // 读状态寄存器 
    if (status & 0x03) // 判断加速度和陀螺仪数据是否可读
        data_ready = 1;
    if (data_ready == 1){  // 如果数据可读
        data_ready = 0;
        readRegister(QMI8658_AX_L, (uint8_t *)buf, 12); // 读加速度和陀螺仪值
        p->acc_x = buf[0];
        p->acc_y = buf[1];
        p->acc_z = buf[2];
        p->gyr_x = buf[3];
        p->gyr_y = buf[4];
        p->gyr_z = buf[5];
    }
}

// 获取XYZ轴的倾角值
void QMI8658::getAngleFromAcc(QMI8658Data *p)
{
    float temp;

    readAccAndGry(p); // 读取加速度和陀螺仪的寄存器值
    // 根据寄存器值 计算倾角值 并把弧度转换成角度
    temp = (float)p->acc_x / sqrt( ((float)p->acc_y * (float)p->acc_y + (float)p->acc_z * (float)p->acc_z) );
    p->AngleX = atan(temp)*57.29578f; // 180/π=57.29578
    temp = (float)p->acc_y / sqrt( ((float)p->acc_x * (float)p->acc_x + (float)p->acc_z * (float)p->acc_z) );
    p->AngleY = atan(temp)*57.29578f; // 180/π=57.29578
    temp = sqrt( ((float)p->acc_x * (float)p->acc_x + (float)p->acc_y * (float)p->acc_y) ) / (float)p->acc_z;
    p->AngleZ = atan(temp)*57.29578f; // 180/π=57.29578
}
