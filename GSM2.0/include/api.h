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
     * api_recv_msg_, ���մ���ص�����
     *
     * @param msg ���������ǰ�÷��ص���Ϣ����
     * @param type �����������Ϣ���ĵ�����
     * @param con_len �����������Ϣ���ĵĳ���
     *
     * @return 
     *
     */
    void (* api_recv_msg_)(const char * msg, int type, unsigned short con_len, void * ctx);

    /**
     * api_cb_connected, �������ӳɹ�ʱ�Ļص�����
     *
     * @param api �������
     *
     * @return  0: �ɹ�
     *         -1: ʧ��
     */
    int (* api_cb_connected_)(api_connection_state_t * cs, void * ctx);

    /**
     * api_cb_disconnect, �������ӶϿ�ʱ�Ļص�����
     *
     * @param api �������
     *
     * @return  0: �ɹ�
     *         -1: ʧ��
     */
    int (* api_cb_disconnected_)(api_connection_state_t * cs, void * ctx);
} api_cb_t;

/**
 * api_init, api��ʼ��
 *
 * @param ip ���������ǰ��ip��ַ
 * @param port ���������ǰ�ö˿�
 * @param recv_msg ������������ܴ�����
 * @param safe_path ���������֤����·��
 * @param ctx ������������մ�������Ҫʹ�õĲ���
 *
 * @return api_t*: �ɹ�
 *           NULL: ʧ��
 */
Z_API SGE_API_EXPORT api_t * api_init(const char * ip, int port, api_cb_t * cb, void * ctx);

/**
 * api_wait, �ȴ�api�̵߳Ľ���
 *
 * @param void
 *
 * @return void
 */
Z_API SGE_API_EXPORT void api_wait(api_t * api);

/**
 * api_free, api�������ͷ������������Դ
 *
 * @param api ���������api,����api_init�ĳɹ�����ֵ�������º�����api_t*���͵Ĳ���ͬ�˽��͡���
 *
 * @return void
 */
Z_API SGE_API_EXPORT void api_free(api_t * api);

/**
 * api_send_msg, ��ǰ�÷���������Ϣ
 *
 * @param api       �������
 * @param gtp_msg   ���������Ӧ����Ϣ
 * @param con_len   ���������Ӧ����Ϣ����
 *
 * @return  0: �ɹ�
 *         -1: ��������
 *         -2: �ڴ����ʧ��
 *         -4����δ��ǰ������
 */
Z_API SGE_API_EXPORT int api_send_msg(api_t * api, void * gtp_msg, unsigned short con_len);

/**
 * api_send_msg_no_enc, ��ǰ�÷���������Ϣ
 *
 * @param api       �������
 * @param gtp_msg   ���������Ӧ����Ϣ
 * @param con_len   ���������Ӧ����Ϣ����
 *
 * @return  0: �ɹ�
 *         -1: ��������
 *         -2: �ڴ����ʧ��
 *         -4����δ��ǰ������
 */
Z_API SGE_API_EXPORT int api_send_msg_no_enc(api_t * api, const char * gtp_msg, unsigned short con_len);

/**
 * api_get_version, ��ȡapi�İ汾��
 *
 * @param api �������
 *
 * @return char*: �ɹ�
 *          NULL: ʧ��
 */
Z_API SGE_API_EXPORT char * api_get_version(api_t * api);

/**
 * api_thread_begin����Ҫд��־ʱ�ڷ�������ǰ����
 *
 * @param tname �����������־�ļ���
 *
 * @retrun  0: �ɹ�
 *         -1: ʧ��
 */
Z_API SGE_API_EXPORT int api_thread_begin(const char * tname);

/**
 * api_thread_end,���ͽ��������
 *
 * @param void
 *
 * @return void
 */
Z_API SGE_API_EXPORT void api_thread_end(void);

/**
 * api_disconnect,����
 *
 * @param api �������
 *
 * @return  0: �ɹ�
 *         -1����������
 */
Z_API SGE_API_EXPORT int api_disconnect(api_t * api);

#endif  /* __API_H__ */

