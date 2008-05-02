void cleanup();

void read_header();

void gen_field();

void fprint_cellspacing();
void edit_cellspacing();
void initialize_cellspacing_state();
void initialize_cellspacing();
int  calculate_cellspacing();

void fprint_fieldradar();
void initialize_fieldradar();
void initialize_fieldradar_state();
int  calculate_fieldradar();
void edit_fieldradar();

void fprint_insitudata();
void initialize_insitudata();
void edit_insitudata();

void fprint_navigation();
void initialize_navigation();
void edit_navigation();

void fprint_parameter();
void initialize_parameter();
void initialize_parameter_state();
void edit_parameter();
int  calculate_parameter();

void fprint_radar();
void initialize_radar();
void initialize_radar_state();
void edit_radar();
int calculate_radar();

void fprint_volume();
void initialize_volume();
int  calculate_volume();
void edit_volume();

void fprint_waveform();
void initialize_waveform();
void initialize_waveform_state();
int  calculate_waveform();
void edit_waveform();

void initialize_engin_state();
int  calculate_engin();
void initialize_engin();

void set_hpa();

void set_rcvr();

void set_antenna();

void set_pscheme();

void set_attenuator();

void verify();

int  big_little(void);

void unpk_short(short *,int,int);
void unpk_long(long *,int,int);
void unpk_float(float *,int,int);
void unpack_hd(void);







