int  read_header(char* filename);

void gen_field();

void make_new();

void fprint_cellspacing(struct cell_spacing_d *, FILE *);
void initialize_cellspacing(struct cell_spacing_d *);
void initialize_cellspacing_state();
int  calculate_cellspacing();
void edit_cellspacing(struct cell_spacing_d *);

void fprint_fieldradar(struct field_radar_i *, FILE *);
void initialize_fieldradar(struct field_radar_i *);
void initialize_fieldradar_state();
int  calculate_fieldradar();
void edit_fieldradar(struct field_radar_i *);

void fprint_insitudata(struct insitu_descript *, FILE *);
void initialize_insitudata(struct insitu_descript *);

void fprint_navigation(struct nav_descript *, FILE *);
void initialize_navigation(struct nav_descript *);
void edit_navigation(struct nav_descript *);

void fprint_parameter(struct parameter_d *, FILE *);
void initialize_parameter(struct parameter_d *);
void initialize_parameter_state();
int  calculate_parameter();
void edit_parameter(struct parameter_d *);

void fprint_radar(struct radar_d *, FILE *);
void initialize_radar(struct radar_d *, int);
void initialize_radar_state();
int  calculate_radar();
void edit_radar(struct radar_d *);

void fprint_volume(struct volume_d *, FILE *);
void initialize_volume(struct volume_d *);
int  calculate_volume();
void edit_volume(struct volume_d *);

void fprint_waveform(struct waveform_d *, FILE *);
void initialize_waveform(struct waveform_d *);
void initialize_waveform_state();
int  calculate_waveform();
void edit_waveform(struct waveform_d *);

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

void cleanup(char*, int);

void unpk_short(short *,int,int);
void unpk_long(long *,int,int);
void unpk_float(float *,int,int);
void unpack_hd(void);







