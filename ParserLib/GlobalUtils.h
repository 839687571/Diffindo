#ifndef _GLOBAL_UTILS_H_
#define _GLOBAL_UTILS_H_

extern std::ofstream g_logoutFile;//�����־�ļ�����

/*
����: endian_swap
���ã��ڴ��ֽڷ�ת
������
	buf: ���������ݻ���ָ�룻
	len: ��ת���ݵĳ��ȣ�
����ֵ����
*/
void endian_swap(BYTE *buf, int len);

/*
����: read_len_swap
���ã���Դ��ַ��ȡ���ݵ�Ŀ���ַ������ת�ֽ���
������
	src: Դ���ݵ�ַ��
	dst: Ŀ�����ݵ�ַ
	len: ��ת���ݵĳ��ȣ�
����ֵ����
*/
void read_len_swap(BYTE *dst, BYTE *src, int len);

#endif
