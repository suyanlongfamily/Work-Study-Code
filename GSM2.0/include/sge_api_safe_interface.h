#ifndef SGE_API_SAFE_INTERFACE_H
#define SGE_API_SAFE_INTERFACE_H

#undef SGE_API
#ifdef __cplusplus
#define SGE_API extern "C"
#else
#define SGE_API extern
#endif


/* NOTE: All following API functions are not permitted to be used in multi threads. */

//Use it better 
#define API_SAFE_EASY_INIT(key_path, rootcert_path, svr_cert_path, cli_cert_path) \
        api_safe_init(key_path, rootcert_path, svr_cert_path, cli_cert_path, "123412")

/* 
 * key_path: file path of UserKey.key
 * rootcert_path: file path of RootCert.der
 * svr_cert_path: file path of server.der
 * cli_cert_path: file path of UserCert.der
 * passwd: certificate password, default 123412
 */
SGE_API void * api_safe_init(const char *key_path, const char *rootcert_path, const char *svr_cert_path, const char *cli_cert_path, const char *passwd);

SGE_API int api_safe_deinit(void * handle);

//cap must >= 2048
SGE_API long int api_safe_create_client_info(void * handle, char *output, size_t cap);

/*
 * input: this param is the safe info from server rsp
 * real_en_len: this param is the safe info len from server rsp
 */
SGE_API int api_safe_check_server_info(void * handle, const char *input, const size_t real_en_len);

//*cap >= len * 2
SGE_API int api_safe_encrypt(void * handle, const unsigned char *src, const size_t len, unsigned char *dst, size_t *cap);

//*cap >= len
SGE_API int api_safe_decrypt(void * handle, const unsigned char *src, const size_t len, unsigned char *dst, size_t *cap);

SGE_API const char *api_safe_get_version(void * handle);
#endif
