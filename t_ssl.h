#ifndef __T_SSL_H
#define __T_SSL_H
#include <openssl/ssl.h>  
#include <openssl/crypto.h>  
#include <openssl/err.h>  
#include <openssl/bio.h>  
#include <openssl/pkcs12.h>  
#include <stdbool.h>  

bool ssl_init(int fd);
void ssl_close();
#endif
