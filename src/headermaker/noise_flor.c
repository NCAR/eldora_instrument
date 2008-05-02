float receiver_noise_temp, waveguide_loss, bandwidth, system_noise_temp;
#define BOLTZMAN        1.380658e-23   /* Boltzman's constant */
/* The receiver noise floor (dBm) can be calculated from the antenna noise
   temperature and the receiver noise figure */

if(fradar_st.r_noise_floor != 0)
  number_to_calculate--;
else if(fradar_st.r_noise_figure > 0 && fradar_st.loss_in > 0 &&
	fradar_st.ant_noise_temp > 0 && engin_st.chipdur > 0)
  {
      receiver_noise_temp = 
	290.0 * (pow(10.0,(double)(fradar[FORE].r_noise_figure/10.0)) - 1.0);
      waveguide_loss = pow(10.0,(double)(fradar[FORE].loss_in/10.0));
      system_noise_temp = fradar[FORE].ant_noise_temp +
	290.0 * (waveguide_loss - 1) +
	  waveguide_loss * receiver_noise_temp;
      bandwidth = 1.0e6/engin_win.chipdur;
      fradar[FORE].r_noise_floor = 30 + 10 *
	log10((double)(BOLTZMAN * system_noise_temp * bandwidth));

      receiver_noise_temp = 
	290.0 * (pow(10.0,(double)(fradar[AFT].r_noise_figure/10.0)) - 1.0);
      waveguide_loss = pow(10.0,(double)(fradar[AFT].loss_in/10.0));
      system_noise_temp = fradar[AFT].ant_noise_temp +
	290.0 * (waveguide_loss - 1) +
	  waveguide_loss * receiver_noise_temp;
      bandwidth = 1.0e6/engin_win.chipdur;
      fradar[AFT].r_noise_floor = 30 + 10 *
	log10((double)(BOLTZMAN * system_noise_temp * bandwidth));

      fradar_st.r_noise_floor = 3;
      number_to_calculate--;
  }
