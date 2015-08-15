// ͷ�ļ�
//#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "AC_BH.h"
#include <ctype.h>
#include <Windows.h>
#include <iostream>
#include <fstream>
using namespace std;
// �궨��
//#define CASE_SENSITIVE
//#define DEBUG_SEARCH
int ACtree_build(pattern_tree *ptree, pattern_data *patterns, int npattern)//�������Ϊָ���ͣ����÷���ֵ
{
	int i;
	pattern_tree_node *root = NULL, *parent = NULL;
	unsigned char ch;
	int max_pattern_len = 0, min_pattern_len = PATTERN_LEN;
	if (NULL == ptree || NULL == patterns || npattern < 0)
	{
		goto err;
	}

	root = (pattern_tree_node *)malloc(sizeof (pattern_tree_node));//Ϊ���ڵ����ռ�
	if (NULL == root)
	{
		goto err;
	}
	memset(root, 0, sizeof (pattern_tree_node));//ϵͳ���������rootָ��Ŀռ��ͷsizeof (pattern_tree_node)���ַ�Ϊȫ0
	root->label = -2;    // ������־
	root->depth = 0;     // �������
	// ��������ִ�ѭ��������ӽ�ACTree
	for (i = 0; i < npattern; i++)
	{
		int pat_len;
		int ch_i;
		pat_len = (patterns + i)->len;//��i��ģʽ���ĳ���pat_len
		if (pat_len == 0)//��ģʽ������Ϊ0ʱ��������ѭ��
		{
			continue;
		}
		else
		{
			if (pat_len > PATTERN_LEN)//��ģʽ�����ȴ���Ԥ��������ֵʱ����ȡģʽ����Ҳ��Ϊ���Ŵ�
			{
				pat_len = PATTERN_LEN;
			}
			if (pat_len > max_pattern_len)//��ȡ���ģʽ������
			{
				max_pattern_len = pat_len;
			}
			if (pat_len < min_pattern_len)//��ȡ���ģʽ������
			{
				min_pattern_len = pat_len;
			}
			parent = root;//��ʼ�����ڵ㣬ָ��ָ����ڵ�
			for (ch_i = 0; ch_i < pat_len; ch_i++)//�Ӹ��ڵ����������ģʽ��ֱ������Ҷ�ӽڵ�
			{
				ch = ((patterns + i)->data)[ch_i];
#ifndef CASE_SENSITIVE
				ch = tolower(ch);//#####################Ϊʲôѽʹ��tolower������תСд��
#endif
				// ��Ӧ���ַ�����ΪNULL
				if (NULL == parent->childs[ch])
				{
					break;
				}
				parent = parent->childs[ch];
			}
			if (ch_i < pat_len)
			{
				// �ڸ��ڵ�������½ڵ�
				for (; ch_i < pat_len; ch_i++)
				{
					pattern_tree_node *node = NULL;
					ch = ((patterns + i)->data)[ch_i];
#ifndef CASE_SENSITIVE
					ch = tolower(ch);
#endif
					node = (pattern_tree_node *)malloc(sizeof (pattern_tree_node));//Ϊ�½ڵ����ռ�
					if (node == NULL)
					{
						goto err;
					}
					memset(node, 0, sizeof(pattern_tree_node));//��ʼ���½ڵ�
					//����½ڵ�
					node->depth = ch_i + 1;
					node->ch = ch;
					node->label = -1;
					// ���½ڵ���ӵ����ڵ�Ķ�Ӧ�ַ�����ָ��
					parent->childs[ch] = node;//�൱��һ���򵥵�Hash����childs[ch]:��ch��ֵȷ�����ĸ�����
					// �����Ǵ�Сд�ķֱ�
#ifndef CASE_SENSITIVE
					if ((ch >= 'a') && (ch <= 'z'))
					{
						parent->childs[ch - 32] = node;
					}
#endif
					parent->nchild++;
					parent->one_child = ch;
					node->parent = parent;
					parent = node;
				}
			}
		}
		// lable ��¼�ִ������ڵڼ��������ִ�������ʾ�ǵڼ���ģʽ����β�ֽ�
		parent->label = i;
	}
	//ģʽ��������ϣ����ģʽ���ṹ
	ptree->pattern_list = patterns;
	ptree->pattern_count = npattern;
	ptree->root = root;
	ptree->max_depth = max_pattern_len;
	ptree->min_pattern_size = min_pattern_len;
	return 0;
err:
	// ������,�ͷ�����Ŀռ�
	if (ptree->root != NULL)
	{
		_clean_tree(ptree->root);
		free(ptree->root);
		ptree->root = NULL;
	}
	return -1;
}

/*--------------------------------------------------------------------------
* ������:
*    _print_tree
*
* ��������:
*    ��ӡ��ǰ�ڵ㼰�����к�׺�ڵ�
*
* ����˵��:
*    pattern_tree_node *root : ��ǰ�Ľڵ��ָ��
*
* ����ֵ����:  void
*    ��
----------------------------------------------------------------------------*/
void _print_tree(pattern_tree_node *root)
{
	int i;
	if (NULL == root)
		return;
	printf("ch:%2c   GSshift:%2d   label:%2d   depth:%2d   childs:", root->ch, root->GSshift, root->label, root->depth);
	for (i = 0; i < 256; i++)
	{
#ifndef CASE_SENSITIVE
		if ((i >= 'A') && (i <= 'Z'))
		{
			continue;
		}
#endif
		if (NULL != root->childs[i])
		{
			printf("%c ", (root->childs[i])->ch);
		}
	}
	printf("\n");
	// �ݹ��ӡ���ڵ�����к�׺�ڵ���Ϣ
	for (i = 0; i < 256; i++)
	{
		if (NULL != root->childs[i])
		{
			_print_tree(root->childs[i]);
		}
	}
	return;
}

/*--------------------------------------------------------------------------
* ������:
*    ACtree_print_tree
*
* ��������:
*    ��ӡ������ACTree�����нڵ��ַ���Ϣ
*
*  ����˵��:
*    pattern_tree *ptree    : ָ��ACTreeģʽ����ָ��
*
* ����ֵ����:  void
*    ��
----------------------------------------------------------------------------*/
//#if 0
void ACtree_print_tree(pattern_tree *ptree)
{
	printf("--- ACTree information---\n");
	if (NULL == ptree)
	{
		return;
	}
	if (NULL != ptree->root)
	{
		_print_tree(ptree->root);
	}
	return;
}
//#endif

/*--------------------------------------------------------------------------
* ������:
*    ACtree_compute_BCshifts
*
* ��������:
*    ����ptree��BCshift
*
*  ����˵��:
*    pattern_tree *ptree    : ָ��ACTreeģʽ����ָ��
*
* ����ֵ:  int
*    0 : ����ɹ�
----------------------------------------------------------------------------*/
int ACtree_compute_BCshifts(pattern_tree *ptree)//����ģʽ�����ҵ����е����ַ�������ģʽ���е�������ֲ���¼��BCshift[256]��
{
	int i, j = 0;
	for (i = 0; i < 256; i++) //��ʼ��BCshift
		ptree->BCshift[i] = ptree->min_pattern_size;
	for (i = ptree->min_pattern_size - 1; i > 0; i--)
	{
		for (j = 0; j < ptree->pattern_count; j++)
		{
			unsigned char ch;
			ch = (ptree->pattern_list + j)->data[i];

#ifndef CASE_SENSITIVE
			ch = tolower(ch);
#endif
			ptree->BCshift[ch] = i;
#ifndef CASE_SENSITIVE
			if ((ch > 'a') && (ch <'z'))
			{
				ptree->BCshift[ch - 32] = i;
			}
#endif
		}
	}
	return 0;
}

/*--------------------------------------------------------------------------
* ������:
*    ACtree_compute_BCBshifts
*
* ��������:
*    ����ptree��BCBshift
*
*  ����˵��:
*    pattern_tree *ptree    : ָ��ACTreeģʽ����ָ��
*
* ����ֵ:  int
*    0 : ����ɹ�
----------------------------------------------------------------------------*/
int ACtree_compute_BCBshifts(pattern_tree *ptree)
{
	int i, j = 0;

	for (i = 0; i < 256; i++)
		ptree->BCBshift[i] = ptree->min_pattern_size;

	for (i = ptree->min_pattern_size - 1; i >= 0; i--)
	{
		for (j = 0; j < ptree->pattern_count; j++)
		{
			unsigned char ch;

			ch = (ptree->pattern_list + j)->data[i];

#ifndef CASE_SENSITIVE
			ch = tolower(ch);
#endif

			ptree->BCBshift[ch] = i;

#ifndef CASE_SENSITIVE
			if ((ch > 'a') && (ch <'z'))
			{
				ptree->BCBshift[ch - 32] = i;
			}
#endif
		}
	}
	return 0;
}


/*--------------------------------------------------------------------------
* ������:
*    set_GSshift
*
* ��������:
*    ����ACTree�йؼ���pat1��GCshift
*
* ����˵��:
*    pattern_tree *ptree    : ָ��ACTreeģʽ����ָ��
*    unsigned char *pat     : �ؼ���pat1
*    int depth              : Ҫ�ı�GSshift�ַ������Ϊdepth
*    int shift              : ����еı仯
*
* ����ֵ:  int
*    0 : ����ɹ�
*   -1 : ����
----------------------------------------------------------------------------*/
int set_GSshift(pattern_tree *ptree, unsigned char *pat, int depth, int shift)
{
	int i;
	pattern_tree_node *node;
	if (NULL == ptree || NULL == ptree->root)
	{
		goto err;
	}
	node = ptree->root;
	for (i = 0; i < depth; i++)
	{
		node = node->childs[pat[i]];
		if (NULL == node)
		{
			goto err;
		}
	}

	// ȡСλ��
	node->GSshift = node->GSshift < shift ? node->GSshift : shift;
	return 0;
err:
	return -1;
}

/*--------------------------------------------------------------------------
* ������:
*    compute_GSshift
*
* ��������:
*    ����ACTree�йؼ���pat1��pat2�������仯��GSshift
*
* ����˵��:
*    pattern_tree *ptree    : ָ��ACTree��ָ��
*    unsigned char *pat1    : �ؼ���pat1
*    int pat1_len           : �ؼ���pat1���ִ�����
*    unsigned char *pat2    : �ؼ���pat2
*    int pat2_len           : �ؼ���pat2���ִ�����
*
* ����ֵ:  int
*    0 : ����ɹ� �� δ����
*   -1 : ����
----------------------------------------------------------------------------*/
int compute_GSshift(pattern_tree *ptree,
	unsigned char *pat1,
	int pat1_len,
	unsigned char *pat2,
	int pat2_len)
{
	unsigned char first_char;
	int i;
	int pat1_index, pat2_index, offset;
	if (NULL == pat1 || NULL == pat2 || pat1_len < 0 || pat2_len < 0)
	{
		goto err;
	}
	if (pat1_len == 0 || pat2_len == 0)
	{
		return 0;
	}
	first_char = pat1[0];

#ifndef CASE_SENSITIVE
	first_char = tolower(first_char);
#endif
	// ��pat2�ڶ����ַ���ʼ����, Ϊʲô���ǵ�һ���ַ����� ��Ϊ��һ���Ƚ�û�����塣��δ�����Ҫ������Ѱ��
	//ǰ׺���ַ������ظ����֣�Ϊʲôֻ���ǵڶ�λ������ڶ�λ��ֱͬ����ת������ڵ���λ���ֲ����Ҳ����ˣ�
	for (i = 1; i < pat2_len; i++)
	{
#ifndef CASE_SENSITIVE
		if (tolower(pat2[i]) != first_char)
#else 
		if (pat2[i] != first_char)
#endif
			break;
	}
	set_GSshift(ptree, pat1, 1, i);   //����ÿ��ģʽ����һ���ַ���GSshift
	//ACtree_print_tree (ptree) ;   //����
	i = 1;               //Ϊʲô�����¸�ֵ1����
	while (1)                       //i������ѭ������;����
	{
		// ��pat2��Ѱ����pat1���ַ���ͬ�ַ����׳���λ�ã�
#ifndef CASE_SENSITIVE
		while (i < pat2_len && tolower(pat2[i]) != first_char)
#else
		while (i < pat2_len && pat2[i] != first_char)          //�ҵ���ģʽ����һλ��ȵĵط�
#endif
			i++;

		// pat2ʣ���ַ���δ������pat1���ַ���ͬ�ַ�,��������
		if (i == pat2_len)
		{
			break;
		}

		// ��Ȼ���������ַ���ͬ�ַ�
		pat2_index = i;
		pat1_index = 0;
		offset = i;                         //�ƶ��ľ��룿��

		// ���ƫ�Ʒ���������̹ؼ��ֳ���,���迼��,��Ϊ����������ת����min_pattern_size
		if (offset > ptree->min_pattern_size)
		{
			break;
		}

		while (pat2_index < pat2_len && pat1_index < pat1_len) // �ظ�ѭ��ֱ�����ؼ������ַ��״β�ͬ
		{
#ifndef CASE_SENSITIVE
			if (tolower(pat1[pat1_index]) != tolower(pat2[pat2_index]))
#else
			if (pat1[pat1_index] != pat2[pat2_index])
#endif
				break;
			pat1_index++;               //�ǱȽ�λ���ַ������
			pat2_index++;
		}

		if (pat2_index == pat2_len) // �ؼ���pat1ǰ׺�ǹؼ���pat2��׺
		{
			int j;                    //j����;��
			for (j = pat1_index; j < pat1_len; j++)
			{
				set_GSshift(ptree, pat1, j + 1, offset);
				//ACtree_print_tree (ptree) ;   //����
			}
		}
		else // pat1��ǰ׺��pat2���Ӵ�,��pat2����
		{
			set_GSshift(ptree, pat1, pat1_index + 1, offset);          //�ַ����ڵ���Ⱥ���Ų�һλ
			//ACtree_print_tree (ptree) ;   //����
		}
		i++;
	}
	return 0;

err:
	return -1;
}

/*--------------------------------------------------------------------------
* ������:
*    ACtree_compute_GSshifts
*
* ��������:
*    ��������ACTree��GSshifts
*
* ����˵��:
*    pattern_tree *ptree    : ָ��ACTree��ָ��
*
* ����ֵ:  int
*    0 : ������
----------------------------------------------------------------------------*/
int ACtree_compute_GSshifts(pattern_tree *ptree)
{
	int pat_i = 0, pat_j = 0;

	for (pat_i = 0; pat_i < ptree->pattern_count; pat_i++)
	{
		for (pat_j = 0; pat_j < ptree->pattern_count; pat_j++)
		{
			unsigned char *ppat_i = (ptree->pattern_list + pat_i)->data;
			int patlen_i = (ptree->pattern_list + pat_i)->len;
			unsigned char *ppat_j = (ptree->pattern_list + pat_j)->data;
			int patlen_j = (ptree->pattern_list + pat_j)->len;
			//ֻ��Ҫ����ppat_i�Ϳ������ÿ���ַ���GSshift��ͨ��set_GSshift���㡣
			compute_GSshift(ptree, ppat_i, patlen_i, ppat_j, patlen_j);
			//   printf ("\n���� ACtree_compute_GSshifts---------------") ; // ����ʹ�� 
			//   printf ("\n----���� compute_GSshift-------------------\n") ; // ����ʹ��
			//   ACtree_print_tree(ptree) ;  // ����ʹ��
		}
	}
	return 0;
}

/*--------------------------------------------------------------------------
* ������:
*    _init_GSshifts
*
* ��������:
*    ��ʼ����ǰ�ڵ㼰�����к����ڵ�ĵ�GSshiftsΪ����shift
*
* ����˵��:
*    pattern_tree_node *root    : ָ��ǰ�ڵ��ָ��
*    int shift                  : ��ʼ��ֵ
*
*������ֵ:  int
*    0 : ������
----------------------------------------------------------------------------*/
int _init_GSshifts(pattern_tree_node *root, int shift)
{
	int i;
	if (root->label != -2)
		root->GSshift = shift;

	for (i = 0; i < 256; i++)
	{
#ifndef CASE_SENSITIVE
		if ((i >= 'A') && (i <= 'Z'))
		{
			continue;
		}
#endif
		if (NULL != root->childs[i])
		{
			_init_GSshifts(root->childs[i], shift);
		}
	}
	return 0;
}

/*--------------------------------------------------------------------------
* ������:
*    ACtree_init_GSshifts
*
*����������:
*    ��ʼ������ACtreeģʽ����GSshifts
*
*  ����˵��:
*    pattern_tree *ptree : ָ��ACTree��ָ��
*
* ����ֵ:  int
*    0 : ������
----------------------------------------------------------------------------*/
int ACtree_init_GSshifts(pattern_tree *ptree)
{
	_init_GSshifts(ptree->root, ptree->min_pattern_size);
	return 0;
}

/*--------------------------------------------------------------------------
* ������:
*    ACtree_compute_shifts
*
* ��������:
*    ���㲢����ACtree��BCshift��GSshift
*
* ����˵��:
*    pattern_tree *ptree    : ָ��ACTree��ָ��
*
* ����ֵ:  int
*    0 : ������
----------------------------------------------------------------------------*/
int ACtree_compute_shifts(pattern_tree *ptree)
{
	ACtree_compute_BCshifts(ptree);
	ACtree_compute_BCBshifts(ptree);
	return 0;
}

/*--------------------------------------------------------------------------
* ������:
*    acbm_init
*
* ��������:
*    ���� ac_bmģʽ��,ͬʱ���㲢����BCshift��GSshift
*
* ����˵��:
*    pattern_data *patterns    : ָ��ؼ����ִ������ָ��
*    int npattern              : �ؼ����ִ�����
*
* ����ֵ:  pattern_tree *
*    ptree : ����ɹ�,����ptree
*    NULL  : ����ʧ��,���ؿ�ָ��
----------------------------------------------------------------------------*/
pattern_tree *acbm_init(pattern_data *patterns, int npattern)
{
	//����ģʽ���ṹ������ռ�
	pattern_tree *ptree = NULL;
	ptree = (pattern_tree *)malloc(sizeof (pattern_tree));
	if (NULL == ptree)
	{
		goto err;
	}

	memset(ptree, 0, sizeof(pattern_tree));//ϵͳ����������ptreeָ��Ŀռ��ͷsizeof(pattern_tree)���ַ�Ϊȫ0
	ACtree_build(ptree, patterns, npattern);
	// printf ("\n���� acbm_init----------------------------") ;  // ����ʹ��
	// printf ("\n----���� ACtree_build---------------------\n") ; // ����ʹ��
	// ACtree_print_tree(ptree) ; // ����ʹ��
	ACtree_compute_shifts(ptree);
	// printf ("\n���� acbm_init----------------------------") ; // ����ʹ�� 
	// printf ("\n----���� ACtree_compute_shifts------------\n") ; // ����ʹ��
	// ACtree_print_tree(ptree) ;  // ����ʹ��
	return ptree;
err:
	return NULL;
}
/*--------------------------------------------------------------------------
* ������:
*    _clean_tree
*
* ��������:
*    �ͷŵ�ǰ�ڵ㼰�����к����ڵ�����Ĵ洢�ռ�
*
* ����˵��:
*    pattern_tree_node *root : ָ��ǰ�ڵ��ָ��
*
* ����ֵ:  void
*     ��
----------------------------------------------------------------------------*/
void _clean_tree(pattern_tree_node *root)
{
	int i;
	for (i = 0; i < 256; i++)
	{
#ifndef CASE_SENSITIVE
		if ((i >= 'A') && (i <= 'Z'))
		{
			continue;
		}
#endif
		if (NULL != root->childs[i])
		{
			_clean_tree(root->childs[i]);
			free(root->childs[i]);
			root->childs[i] = NULL;
		}
	}
	return;
}

/*--------------------------------------------------------------------------
* ������:
*    acbm_clean
*
*  ��������:
*    �ͷ�����ac_bmģʽ������Ŀռ�
*
* ����˵��:
*    pattern_tree *ptree   : ָ��ACTree��ָ��
*
* ����ֵ:  void
*     ��
----------------------------------------------------------------------------*/
void acbm_clean(pattern_tree *ptree)
{
	if (NULL == ptree)
	{
		return;
	}
	if (NULL != ptree->root)
	{
		_clean_tree(ptree->root);
		free(ptree->root);
		ptree->root = NULL;
	}
	free(ptree);
	return;
}

/*--------------------------------------------------------------------------
* ������:
*    acbm_search
*
* ��������:
*    �����㷨,����������ı�text�а����Ĺؼ��ֵĸ���,�����������Ĺؼ���
*    �ڹؼ�������patterns�е�indexλ���Լ��ڴ�ƥ���ı�text�е�ƫ��ֵ����
*    �������Ⱥ�˳�����α��浽����matched_items.
*
* ����˵��:
*    pattern_tree *ptree   : ָ��ACTree��ָ��
*    unsigned char *text   : ��ƥ����ı�
*    int text_len          : ��ƥ���ı��ĳ���
*    matched_info_t matched_items[] : �������������Ϣ������
*    int nmax_index        : �ɱ���ƥ������������
cur_index             : ����ƥ���ƶ�����
*   base_index            : ������ƥ���ƶ�����
* ����ֵ:  int
*    nmatched  : ƥ�䵽�Ĺؼ��ֵĸ���
nmatched+= acbm_search (ptree, text, text_len, matched_items, nmax_index) ;
----------------------------------------------------------------------------*/
int acbm_search(pattern_tree *ptree, unsigned char *text, int text_len, matched_info_t matched_items[], int nmax_index)
{
	int nmatched = 0;
	register int base_index = 0, cur_index = 0;//register����һ������ڼĴ����еı����Լӿ��ȡ�ٶȣ�ͨ����������һ������ʹ�õı���
	register int real_shift = 0, gs_shift = 0, bc_shift = 0, bcb_shift = 0;//bcb_shiftΪ��ǰƥ�俪ʼ��ǰһ���ַ��Ļ��ַ�ת����
	pattern_tree_node *node = NULL;

	if (text_len < ptree->min_pattern_size)//�����ı�����С�����ģʽ�����ȣ���ֱ�ӽ���
	{
		goto ret;
	}

	base_index = text_len - ptree->min_pattern_size;//��ʼ���ı�ƫ��ֵ������ǰ���ڵ��ı�ͷ�ľ���

	while (base_index >= 0)
	{
		cur_index = base_index;//��ʼ���ı���ǰ�ıȽ�λ
		node = ptree->root;//�Ƚ�ָ��ָ����ڵ�

#ifdef DEBUG_SEARCH
		printf("Checking pattern tree at %d...", base_index);
#endif

		while (NULL != node->childs[text[cur_index]]) //���õ�ƥ��֮��Ҫ�������ƥ�䣬��ΪҲ���и�����ģʽ����ƥ��
		{
			node = node->childs[text[cur_index]];

			// ƥ�䵽һ���ؼ���,���浽matched_items��
			if (node->label >= 0)
			{
#ifdef DEBUG_SEARCH
				printf("Matched(%d) ", node->label);
#endif
				matched_items[nmatched].pattern_i = node->label;
				matched_items[nmatched].offset = base_index;
#ifdef DEBUG_SEARCH
				printf("%s\n", text + matched_items[nmatched].offset);
#endif
				nmatched++;
				if (nmatched == nmax_index)
				{
					goto ret;
				}
			}
			cur_index++;
			if (cur_index >= text_len)
			{
				break;
			}
		}

		//����node->childs[text[cur_index]]==NULL�������������node->nchild>0˵��ʧ�䣬Ҫ�����ƶ���node->nchild=0��˵���Ѿ�����Ҷ�ӽڵ㣬ƥ��Ҳ�ǳɹ���
		if (node->nchild > 0)
		{
			// ���ַ�,��GSshift��BCshift�������ַ���λ��
			if (cur_index < text_len)
			{
				bc_shift = ptree->BCshift[text[cur_index]] + base_index - cur_index;
				//BCshift�ڲ�ƥ��ʱ�������ı����Ǹ���ƥ���ַ������ĺ����ǲ�ƥ����Ǹ��ַ�����������ڵ�ľ���,
				//cur_index-base_index�ı��в�ƥ��λ����ڵ����Ծ���
				//bc_shift�õ��ľ����ı��в�ƥ����Ǹ��ַ��ƶ�����������ַ���Ӧλ����Ҫ�ƶ��ľ���

				bcb_shift = ptree->BCBshift[text[base_index - 1]] + 1;

			}
			else
			{
				bc_shift = 1;
			}

			if (bc_shift <= 0)bc_shift = 1;                          //leo gai
			real_shift = bcb_shift > bc_shift ? bcb_shift : bc_shift; // ȡ������תleo gai
			//skip++;
			base_index -= real_shift;
#ifdef DEBUG_SEARCH
			printf("Failed, BCBSshift:%d, BCshift:%d Realshift%d", bcb_shift, bc_shift, real_shift);
#endif
		}
		else
		{
			// 1��ƥ��ɹ�������,������ǰ�洦��
			base_index--;
			//skip++;
#ifdef DEBUG_SEARCH
			printf("Matched, shift %d", 1);
#endif
		}
#ifdef DEBUG_SEARCH
		printf("\n");
#endif
	}

ret:
	return  nmatched;
}
/*--------------------------------------------------------------------------
* ������:
*    match_resualt_printf
*
* ��������:
*    �������ӡ�������Ĺؼ��ֽ��
*
* ����˵��:
*    unsigned char *text    : ����Ĵ��������ı�
*    pattern_data *patterns : �ؼ�������
*    int npattern           : �ؼ��ָ���
*    matched_info_t matched_items[] : �������������Ϣ������
*    int nmatched        : �������Ĺؼ��ָ���
*
* ����ֵ:  int
*    0 : ��ȫ����,������
----------------------------------------------------------------------------*/
int match_resualt_printf(unsigned char *text, pattern_data *patterns, int npattern, matched_info_t matched_items[], int nmatched)
{
	FILE * log;
	fopen_s(&log, "log.txt", "wb");
	//fwrite("�鵽�����ܴ��У�", 16, 1, log);
	char *temp = ",";
	int i = 0;
	printf("\n--- key worlds ---\n");
	for (i = 0; i < npattern; i++)
	{
		(patterns + i)->count = 0;
		if ((i + 1) % 6 == 0)
		{
			printf("\n");
		}
		printf("%2d %s\n", i + 1, (patterns + i)->data);
	}
	int a = 0;
	for (i = 0; i < nmatched; i++)
	{
		(patterns + matched_items[i].pattern_i)->count = 0;
		for (int j = 0; j < nmatched; j++)
		{
			if (matched_items[i].pattern_i == matched_items[j].pattern_i)
			{
				(patterns + matched_items[i].pattern_i)->count++;
			}
		}
	}
	for (i = 0; i < npattern; i++)
	{
		if ((patterns + i)->count != 0)
		{
			a = (patterns + matched_items[i].pattern_i)->len;
			char *str = new char[a + 1];
			for (int j = 0; j < a; j++)
			{
				str[j] = (patterns + i)->data[j];
			}
			fwrite(str, a, 1, log);
			//fwrite("\t", 1, 1, log);
			fwrite("\r\n", 2, 1, log);
			fprintf(log, "%d\n", (patterns + i)->count);
			fwrite("\r\n", 2, 1, log);
			delete str;
		}
	}
	fclose(log);
	asciiToUnicode("log.txt", "SecretReport.txt");
	return 0;
}
//���õĺ���
int asciiToUnicode(char* asciiTxt, char * unicodeTxt)
{
	FILE * unicodeFile;
	fopen_s(&unicodeFile, unicodeTxt, "wb");
	FILE * asciiFile;
	fopen_s(&asciiFile, asciiTxt, "rb");
	void * buffer;    // ������ָ��
	int size;    // Դ�ļ���С

	fseek(asciiFile, 0, SEEK_END);    // ��Դ�ļ���ָ���ƶ���ĩβ
	int asciiFileSize = ftell(asciiFile);    // ��ȡԴ�ļ���С,����Ҫ����unicode�ļ���ͷ2λ�ı�ʶ��

	buffer = malloc(asciiFileSize);    // ���뻺����
	fseek(asciiFile, 0, SEEK_SET);    // ��Դ�ļ���ָ���ƶ�����ͷ, ͨ�ϣ���2λ

	size_t readSize = fread(buffer, asciiFileSize, 1, asciiFile);    // ��ȡ

	int dstBufferSize;
	wchar_t *dstBuffer;

	dstBufferSize = MultiByteToWideChar(CP_ACP, 0, (LPCCH)buffer, asciiFileSize, NULL, NULL);
	dstBuffer = new wchar_t[dstBufferSize];
	ZeroMemory(dstBuffer, dstBufferSize * sizeof(wchar_t));

	int convertSize = MultiByteToWideChar(CP_ACP, 0, (LPCCH)buffer, asciiFileSize, dstBuffer, dstBufferSize);

	char head[2] = { 0xFF, 0xFE }; // windowsĬ�ϰ�С�˱���
	//{ 0xFE, 0xFF }; // ����˱���

	fwrite(head, 2, 1, unicodeFile);
	fwrite(dstBuffer, convertSize * 2, 1, unicodeFile);

	delete[] dstBuffer;

	fclose(unicodeFile);    // �ر�Դ�ļ�
	fclose(asciiFile);    // �ر�Ŀ���ļ�
	free(buffer);    // �ͷŻ�����
	return 1;
}

int unicodeToAscii(char * unicodeTxt, char* asciiTxt)
{
	FILE * unicodeFile;
	fopen_s(&unicodeFile, unicodeTxt, "rb");    // ֻ����ʽ��Դ�ļ�
	FILE * asciiFile;
	fopen_s(&asciiFile, asciiTxt, "wb");    // ֻд��ʽ��Ŀ���ļ�
	void * buffer;    // ������ָ��
	//int size;    // Դ�ļ���С

	fseek(unicodeFile, 0, SEEK_END);    // ��Դ�ļ���ָ���ƶ���ĩβ
	int unicodeFileSize = ftell(unicodeFile) - 2;    // ��ȡԴ�ļ���С,����Ҫ����unicode�ļ���ͷ2λ�ı�ʶ��

	buffer = malloc(unicodeFileSize);    // ���뻺����

	fseek(unicodeFile, 2, SEEK_SET);    // ��Դ�ļ���ָ���ƶ�����ͷ, ͨ�ϣ���2λ

	size_t readSize = fread(buffer, unicodeFileSize, 1, unicodeFile);    // ��ȡ


	int dstBufferSize;
	char *dstBuffer;

	// unicodeFileSize�����������ֵ�ǰ���ascii�ĳ���ͳ�Ƴ����ģ���WideCharToMultiByte�Ĳ���ʵ���Ǵ�����ֽڵ���Ŀ�����Գ���2,(unicodeFileSize�϶���ż��)
	dstBufferSize = WideCharToMultiByte(CP_ACP, 0, (LPCWCH)buffer, unicodeFileSize / 2, NULL, 0, NULL, NULL);
	dstBuffer = new char[dstBufferSize];
	ZeroMemory(dstBuffer, dstBufferSize);

	int convertSize = WideCharToMultiByte(CP_ACP, 0, (LPCWCH)buffer, unicodeFileSize / 2, dstBuffer, dstBufferSize, NULL, NULL);

	// ��ʵ�����dstBufferSize == unicodeFileSize / 2 == convertSize
	// ��ʵ������仰�Ǵ�ģ�dstBufferSize == convertSize == unicodeFileSize - Ӣ���ַ���Ŀ
	fwrite(dstBuffer, convertSize, 1, asciiFile);

	delete[] dstBuffer;

	fclose(unicodeFile);    // �ر�Դ�ļ�
	fclose(asciiFile);    // �ر�Ŀ���ļ�
	free(buffer);    // �ͷŻ�����
	return 1;
}
int GetTextEncode(char* strTxtPath)
{
	int nType = -1;//error
	//��Ҫ�жϵ��ļ�
	FILE *pFile = NULL;
	errno_t dError = fopen_s(&pFile, strTxtPath, "r");
	if (0 != dError)
	{
		fclose(pFile);
		return nType;
	}
	//����Ҫע������unsigned   char����Ȼ�Ļ���ȡ�������ݻ���Ϊ������޷���ȷ�ж�
	unsigned   char*   chFileFlag = new   unsigned   char[3];
	fread(chFileFlag, 1, 3, pFile);

	if (chFileFlag[0] == 0xEF && chFileFlag[1] == 0xBB && chFileFlag[2] == 0xBF)
		nType = 1;//UTF-8
	else if (chFileFlag[0] == 0xFF && chFileFlag[1] == 0xFE)
		nType = 2;//Unicode
	else if (chFileFlag[0] == 0xFE && chFileFlag[1] == 0xFF)
		nType = 3;//Unicode big endian text
	else
		nType = 4;//ASCII
	fclose(pFile);
	delete chFileFlag;
	return nType;
}


int last_search(char *text1, char *pattern1)
{
	if (GetTextEncode(text1) != 4)
	{
		unicodeToAscii(text1, "test.txt");
		//printf("TextEncode=%d", GetTextEncode(text1));
	}
	else
	{
		printf("TextEncode=%d", GetTextEncode(text1));
		rename(text1, "test.txt");
	}
	unicodeToAscii(pattern1, "patterns.txt");
	int i = 0;
	int j, n;
	int npattern; // �ؼ��ָ���,��ʱע��ı�
	int length;
	int text_len;// ��ƥ���ı��ִ�����,��̬�ı�
	int success = 0;
	int nmatched = 0;
	char temp;
	int xx = 1000;
	FILE *fp;

	pattern_tree *ptree;
	pattern_data *patterns;
	int nmax_index = MAX_ITEMS;
	struct _matched_info matched_items[MAX_ITEMS];
	unsigned char keyword[PATTERN_LEN]; //ģʽ������
	unsigned char text[TEXT_LEN]; //�����ı� 
	//***********************ȥ��������ǰ����****************************
	int matched = 0;
	register int base_index = 0, cur_index = 0;//register����һ������ڼĴ����еı����Լӿ��ȡ�ٶȣ�ͨ����������һ������ʹ�õı���
	register int real_shift = 0, gs_shift = 0, bc_shift = 0, bcb_shift = 0;//bcb_shiftΪ��ǰƥ�俪ʼ��ǰһ���ַ��Ļ��ַ�ת����
	pattern_tree_node *node = NULL;
	//***************************************************************

	for (j = 0; j<TEXT_LEN; j++)
	{
		text[j] = '\0';
	}
	n = 0;//n��¼ģʽ��������	 
	fp = fopen("patterns.txt", "r");
	if (!fp)
	{
		printf("Open file: pattern.txt error!\n");
		exit(0);
	}
	while (xx != 0)//��ȡģʽ������������pattern.txt�ļ�������
	{
		xx = fread(&temp, 1, 1, fp);
		if (temp == '\n') n++;
	}
	npattern = ++n;
	printf("ģʽ������Ϊ��%d\n", npattern);
	rewind(fp);

	patterns = (pattern_data *)malloc(sizeof(pattern_data)* npattern);//Ϊģʽ���������ռ�
	if (NULL == patterns)//����ռ����ʧ���򱨴���
	{
		printf("error\n");
		exit(1);
	}

	printf("����ģʽ��,�����ģʽ�����飺\n");//��ȡģʽ�������ģʽ������
	for (i = 0; i<n; i++)//��i��ģʽ��
	{
		for (j = 0; j<PATTERN_LEN; j++)
		{
			keyword[j] = '\0';
		}
		length = 0;//lengthΪ��ǰģʽ���ĳ���
		//while(((temp=fgetc(fp))!='\n')&&(temp!=EOF))
		while ((xx = fread(&temp, 1, 1, fp) != 0) && (temp != '\n'))
		{
			keyword[length] = temp;
			length++;
		}
		//���ģʽ������
		(patterns + i)->len = length;
		printf("%s\n", keyword);
		memcpy((patterns + i)->data, keyword, sizeof(unsigned char)*length);
		//strcpy((patterns + i)->data, keyword);
		//		printf("ģʽ��%d��%s  ���ȣ�%d\n",i,(patterns+i)->data,(patterns+i)->len);
	}
	fclose(fp);

	//�����Զ���	 
	ptree = acbm_init(patterns, npattern);
	ACtree_compute_BCshifts(ptree);


	printf("��������ı���\n");
	fp = fopen("test.txt", "r");
	if (!fp)
	{
		printf("Open file: Text.txt error!\n");
		exit(0);
	}

	xx = fread(&temp, 1, 1, fp);
	while (xx != 0)
	{
		i = 0;
		while ((i < TEXT_LEN) && (xx != 0))
		{
			text[i] = temp;
			xx = fread(&temp, 1, 1, fp);
			i++;
			//printf("i:%d\n",i);
		}
		if (xx == 0)
		{
			for (j = i; j<TEXT_LEN; j++)
			{
				text[j] = '\0';
			}
		}
		if ((text_len = strlen((char*)text))>TEXT_LEN)//��ô����ı�����
			text_len = TEXT_LEN;
		nmatched = acbm_search(ptree, text, text_len, matched_items, nmax_index);
		// ACtree_print_tree (ptree) ; // ����ʹ��
		success = match_resualt_printf(text, patterns, npattern, matched_items, nmatched);

		acbm_clean(ptree);
		printf("%d\n", nmatched);
		//system("pause");
	}
	fclose(fp);
	return nmatched;
}