/*
 * crypto.h
 *
 * Created: 29/04/2019 14:50:26
 *  Author: sbrown
 */ 


#ifndef CRYPTO_H_
#define CRYPTO_H_

void crypto_init(unsigned char key[]);
unsigned char crypto_encrypt(unsigned char plainText[], unsigned char cipherText[]);
unsigned char crypto_decrypt(unsigned char cipherText[], unsigned char plainText[]);

#endif /* CRYPTO_H_ */