#include "atmel_start.h"
#include "atmel_start_pins.h"




#define HEART 1


#if HEART
#define BH1792GLC_MEAS_CONTROL1       (0x41)
#define BH1792GLC_MEAS_CONTROL2       (0x42)
#define BH1792GLC_MEAS_CONTROL3       (0x43)
#define BH1792GLC_MEAS_CONTROL4       (0x44) //44/45
#define BH1792GLC_MEAS_CONTROL5       (0x46)
#define BH1792GLC_MEAS_START          (0x47)
#define BH1792GLC_DATAOUT_LEDOFF      (0x54)
#define BH1792GLC_SW_RESET		      (0x40)
#define BH1792GLC_MEAS_CONTROL1_RDY                     (1 << 7)
#define BH1792GLC_MEAS_CONTROL1_SEL_ADC_GREEN           (0 << 4)	// 0 for GREEN, 1 for IR
#define BH1792GLC_MEAS_CONTROL1_SEL_ADC_IR				(1 << 4)	// 0 for GREEN, 1 for IR
//#define BH1792GLC_MEAS_CONTROL1_LED_LIGHTING_FREQ_128HZ (0 << 2)    // BH1792GLC not exist
//#define BH1792GLC_MEAS_CONTROL1_RCYCLE_32HZ             (2 << 0)    // BH1792GLC not exist
#define BH1792GLC_MEAS_CONTROL1_SINGLE_MEASURE_MODE     (7 << 0) //7
#define BH1792GLC_MEAS_CONTROL2_LED_EN1_00              (0 << 6)
//#define BH1792GLC_MEAS_CONTROL2_LED_ON_TIME_0_3MS       (0 << 5)    // BH1792GLC not exist
#define BH1792GLC_MEAS_CONTROL2_LED_CURRENT_10MA        (63 << 0)
#define BH1792GLC_MEAS_CONTROL3_LED_EN2_0               (0 << 7)
#define BH1792GLC_MEAS_CONTROL3_LED_CURRENT_10MA        (63 << 0)
#define BH1792GLC_MEAS_START_MEAS_ST                    (1 << 0)
#define BH1792GLC_MEAS_CONTROL1_VAL_GREEN   (BH1792GLC_MEAS_CONTROL1_RDY | BH1792GLC_MEAS_CONTROL1_SEL_ADC_GREEN | BH1792GLC_MEAS_CONTROL1_SINGLE_MEASURE_MODE)
#define BH1792GLC_MEAS_CONTROL1_VAL_IR   (BH1792GLC_MEAS_CONTROL1_RDY | BH1792GLC_MEAS_CONTROL1_SEL_ADC_IR | BH1792GLC_MEAS_CONTROL1_SINGLE_MEASURE_MODE)
#define BH1792GLC_MEAS_CONTROL2_VAL   (BH1792GLC_MEAS_CONTROL2_LED_EN1_00 | BH1792GLC_MEAS_CONTROL2_LED_CURRENT_10MA)
#define BH1792GLC_MEAS_CONTROL3_VAL   (BH1792GLC_MEAS_CONTROL3_LED_EN2_0 | BH1792GLC_MEAS_CONTROL2_LED_CURRENT_10MA)
#define BH1792GLC_MEAS_CONTROL4_VAL		10
#define BH1792GLC_MEAS_CONTROL4_2_VAL  	00
#define BH1792GLC_MEAS_CONTROL5_VAL     0x03
#define BH1792GLC_MEAS_START_VAL      (BH1792GLC_MEAS_START_MEAS_ST)
#define BH1792GLC_SW_RESET_VAL		      (0x80)
#endif


bool sos_flag = false;


int main(void)
{
	atmel_start_init();

	usbdc_attach();
	delay_ms(100);
	usb_serial_begin();
	delay_ms(100);

	
	i2c_m_sync_enable(&I2C_0);
	
	struct io_descriptor *I2C_sensors;
	i2c_m_sync_get_io_descriptor(&I2C_0, &I2C_sensors);


	while(1){
		

		#if HEART

		i2c_m_sync_set_slaveaddr(&I2C_0, 0x5B, I2C_M_SEVEN);    //0x5B

		
		char buff_green[] = {BH1792GLC_MEAS_CONTROL1,BH1792GLC_MEAS_CONTROL1_VAL_GREEN,BH1792GLC_MEAS_CONTROL2_VAL,BH1792GLC_MEAS_CONTROL3_VAL,
		BH1792GLC_MEAS_CONTROL4_VAL,BH1792GLC_MEAS_CONTROL4_2_VAL,BH1792GLC_MEAS_CONTROL5_VAL};
		io_write(I2C_sensors,buff_green,sizeof(buff_green));
		
		char start[] = {BH1792GLC_MEAS_START,BH1792GLC_MEAS_START_VAL};

		uint16_t pulse_data[5];
		
		io_write(I2C_sensors, start, 2);
		delay_ms(31);


		for(uint16_t i = 0; i<640; i++){
			

			i2c_m_sync_cmd_read(&I2C_0, 0x50, pulse_data, 9);
			
			
			serial_write("LEDON: ");
			char buffer_led[6];
			sprintf(buffer_led, "%d\n",pulse_data[3]);
			serial_write(buffer_led);
	
			
			io_write(I2C_sensors, start, 2);
			
			delay_ms(31);
		}

		char buff_reset[] = {BH1792GLC_SW_RESET, BH1792GLC_SW_RESET_VAL};
		io_write(I2C_sensors, buff_reset, 2);

		

		io_write(I2C_sensors, start, 2);
		delay_ms(31);
		

		
		#endif

	}
	
}


