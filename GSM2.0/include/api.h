#ifndef __API_H__
#define __API_H__

#ifdef __cplusplus
#define Z_API extern "C"
#else
#define Z_API extern
#endif

#ifdef API_WIN_EXPORTS
#define SGE_API_EXPORT __declspec(dllexport)
#else
#define SGE_API_EXPORT __declspec(dllimport)
#endif
#define GTP_TYPE_STRING     1
#define GTP_TYPE_ENCRYPT    4

#define IS_CONNECTED        1
#define IS_DISCONNECTED     2

typedef struct api api_t;
typedef struct {
    char ip[32];
    int port;
    int state;
} api_connection_state_t;

typedef struct {
    /**
     * api_recv_msg_, 接收处理回调函数
     *
     * @param msg 输出参数，前置返回的消息报文
     * @param type 输出参数，消息报文的类型
     * @param con_len 输出参数，消息报文的长度
     *
     * @return 
     *
     */
    void (* api_recv_msg_)(const char * msg, int type, unsigned short con_len, void * ctx);

    /**
     * api_cb_connected, 网络连接成功时的回调函数
     *
     * @param api 输入参数
     *
     * @return  0: 成功
     *         -1: 失败
     */
    int (* api_cb_connected_)(api_connection_state_t * cs, void * ctx);

    /**
     * api_cb_disconnect, 网络连接断开时的回调函数
     *
     * @param api 输入参数
     *
     * @return  0: 成功
     *         -1: 失败
     */
    int (* api_cb_disconnected_)(api_connection_state_t * cs, void * ctx);
} api_cb_t;

/**
 * api_init, api初始化
 *
 * @param ip 输入参数，前置ip地址
 * @param port 输入参数，前置端口
 * @param recv_msg 输入参数，接受处理函数
 * @param safe_path 输入参数，证书存放路径
 * @param ctx 输入参数，接收处理函数需要使用的参数
 *
 * @return api_t*: 成功
 *           NULL: 失败
 */
Z_API SGE_API_EXPORT api_t * api_init(const char * ip, int port, api_cb_t * cb, void * ctx);

/**
 * api_wait, 等待api线程的结束
 *
 * @param void
 *
 * @return void
 */
Z_API SGE_API_EXPORT void api_wait(api_t * api);

/**
 * api_free, api结束后释放其所分配的资源
 *
 * @param api 输入参数，api,调用api_init的成功返回值。（以下函数的api_t*类型的参数同此解释。）
 *
 * @return void
 */
Z_API SGE_API_EXPORT void api_free(api_t * api);

/**
 * api_send_msg, 向前置发送请求消息
 *
 * @param api       输入参数
 * @param gtp_msg   输入参数，应用消息
 * @param con_len   输入参数，应用消息长度
 *
 * @return  0: 成功
 *         -1: 参数错误
 *         -2: 内存分配失败
 *         -4：尚未与前置连接
 */
Z_API SGE_API_EXPORT int api_send_msg(api_t * api, void * gtp_msg, unsigned short con_len);

/**
 * api_send_msg_no_enc, 向前置发送请求消息
 *
 * @param api       输入参数
 * @param gtp_msg   输入参数，应用消息
 * @param con_len   输入参数，应用消息长度
 *
 * @return  0: 成功
 *         -1: 参数错误
 *         -2: 内存分配失败
 *         -4：尚未与前置连接
 */
Z_API SGE_API_EXPORT int api_send_msg_no_enc(api_t * api, const char * gtp_msg, unsigned short con_len);

/**
 * api_get_version, 获取api的版本号
 *
 * @param api 输入参数
 *
 * @return char*: 成功
 *          NULL: 失败
 */
Z_API SGE_API_EXPORT char * api_get_version(api_t * api);

/**
 * api_thread_begin，需要写日志时在发送请求前调用
 *
 * @param tname 输入参数，日志文件名
 *
 * @retrun  0: 成功
 *         -1: 失败
 */
Z_API SGE_API_EXPORT int api_thread_begin(const char * tname);

/**
 * api_thread_end,发送结束后调用
 *
 * @param void
 *
 * @return void
 */
Z_API SGE_API_EXPORT void api_thread_end(void);

/**
 * api_disconnect,闪断
 *
 * @param api 输入参数
 *
 * @return  0: 成功
 *         -1：参数错误
 */
Z_API SGE_API_EXPORT int api_disconnect(api_t * api);

#endif  /* __API_H__ */

