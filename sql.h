#ifndef __SQL_H
#define __SQL_H

void* sql_thread(void* sql_param);
void sql_init();
bool sql_save_data(unsigned char *data,date_time param_date,int length);
void sql_set_unfinish(unsigned char* date);
void head_pack(unsigned char* data,int* offset,unsigned char cmd_flag,unsigned char ack_flag,unsigned char encrypt_type);
void date_pack(unsigned char* data,date_time date,int* offset);
void AES_pack(unsigned char* data,int* offset);
void login_pack(unsigned char* data,int serial_num,int* offset);
void location_data_pack(unsigned char* data,location_t location,int* offset);
void end_pack(unsigned char* data,int offset);
void get_current_date(date_time *date);
void sql_fault_mark();
void sql_mark_send_failed(char* date);

bool check_RSA_key();

void aes_decrypt(unsigned char* encrypt_string,int* len,FILE* fd);
bool aes_ecb_file_decrypt(const char* input_file,const char* output_file);
void sha256_file(const char* file_path,unsigned char* hash);
int base64_decode(char *in_str, int in_len, char *out_str);



void aes_encrypt(unsigned char* input_string,int input_len,int *offset);

void RSA_pub_decrypt(const char* pubkey_path,unsigned char *str,char *p_de);

void heart_beat(int sock);

//void sql_resend_time_plus(sqlite3 *db,unsigned char* date);


#endif
