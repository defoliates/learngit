#include "stdafx.h"

#ifndef _AC_BH_H
#define _AC_BH_H
#define PATTERN_LEN 1024	//ģʽ����󳤶�1KB
#define TEXT_LEN 512000 	//�ı���󳤶�50KB
//#define MAX_ITEMS 20
#define MAX_ITEMS 10000 //�õ�ƥ���������������ٵõ�ƥ���򲻼�¼���


/// ģʽ���ڵ�ṹ
typedef struct _pattern_tree_node
{
	int label;         // ��ʶ, -2 ���ڵ�, -1 �м�ڵ�, n ��n���ִ�β�ڵ�
	int depth;           // �ڵ����

	unsigned char ch;       // �ڵ��Ӧ���ַ�
	int GSshift;        // ���ִ�λ��
	int BCshift;
	unsigned char one_child;     // ���е�һ���������ַ�
	struct _pattern_tree_node *childs[256]; // 256���ַ��Ķ�Ӧ�ڵ�ָ��
	int nchild;                             // �ӽڵ����
	struct _pattern_tree_node *parent;      // ���ڵ�
} pattern_tree_node;

// �ؼ������ݽṹ
typedef struct _pattern_data
{
	unsigned char data[PATTERN_LEN]; // �ؼ����ִ�
	int len;                         // �ؼ����ִ���
} pattern_data;

// ģʽ���ṹ
typedef struct _pattern_tree
{
	pattern_tree_node *root;  // �����ڵ�
	int max_depth;     // ����ִ����
	int min_pattern_size;          // ��̵��ִ�����
	int BCshift[256];             // 256���ַ��л��ַ���shift
	int BCBshift[256];              // 256���ַ�ǰ�û��ַ�ת�Ʊ�
	pattern_data *pattern_list;  // ָ��ڵ������һ���ִ���ָ��
	int pattern_count;             // �������ִ�����

} pattern_tree;

// ƥ����Ϣ�ṹ��
typedef struct _matched_info
{
	int pattern_i;               // �ؼ����ڹؼ��������е�index
	unsigned long offset;        // �ڴ�ƥ���ı�text�е�ƫ��ֵ
} matched_info_t;
// ���� ģʽ��
int ACtree_build(pattern_tree *ptree,
	pattern_data *patterns,
	int npattern);
// ��ӡ ��ǰ�ڵ㼰�����к�׺�ڵ�
void _print_tree(pattern_tree_node *root);
// ��ӡ ����ģʽ��
void ACtree_print_tree(pattern_tree *ptree);
// ��ӡ �������
int match_resualt_printf_ex(unsigned char *text,
	pattern_data *patterns,
	int npattern,
	matched_info_t matched_items[],
	int nmatched);
// �ͷ� ģʽ���ռ�
void _clean_tree(pattern_tree_node *root);
// ���� ģʽ����BCshift
int ACtree_compute_BCshifts(pattern_tree *ptree);
// ���� ac_bmģʽ��
pattern_tree *acbm_init(pattern_data *patterns, int npattern);
// �ͷ� ac_bmģʽ���ռ�
void acbm_clean(pattern_tree *ptree);
// ac_bm�����㷨�Ľ���
int acbm_search(pattern_tree *ptree,
	unsigned char *text,
	int text_len,
	matched_info_t matched_items[],
	int nmax_index);
// ���㲢����ACTreeģʽ����BCshift��GSshift
int ACtree_compute_shifts(pattern_tree *ptree);
// ���� ģʽ����BCBshift
int ACtree_compute_BCBshifts(pattern_tree *ptree);
// ���㲢����ACTreeģʽ����GSshift
int ACtree_compute_GSshifts(pattern_tree *ptree);
// ��ʼ��ACTree��GSshift
int ACtree_init_GSshifts(pattern_tree *ptree);
// ��ʼ����ǰ�ڵ��GSshift
int _init_GSshifts(pattern_tree_node *root, int shift);
// ����ACTree�йؼ���pat1��GSshift
int set_GSshift(pattern_tree *ptree,
	unsigned char *pat,
	int depth,
	int shift);
// ����ACTree�йؼ���pat1��pat2�������仯��GSshift
int compute_GSshift(pattern_tree *ptree,
	unsigned char *pat1,
	int pat1_len,
	unsigned char *pat2,
	int pat2_len);
//���õĺ���
int last_search(char *text,char *pattern);
#endif