/* 动态字符串 */
#ifndef _SBUF_H
#define _SBUF_H
	
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
	
#define SBUF_EMPTY(buf) (buf->len?false:true)
#define SBUF_BUF data->size*0.2 //缓冲区增长基数
	
typedef struct
{
	char *buf;
	/* 缓冲区 */
	size_t size;
	/* 缓冲区大小 */
	size_t len;
	/* 当前字符串长度 */
}SBUF;
	
/* 初始化动态字符串 */
SBUF *sbuf_new(size_t size);
	
/* 释放内存 */
void sbuf_free(SBUF *data);
	
/* 追加内容 */
bool sbuf_append(SBUF *data,const char *fmt,...);
	
/* 追加指定大小内容 */
bool sbuf_nappend(SBUF *data,size_t n,const char *fmt,...);
	
/* 追加字符串 */
bool sbuf_append_str(SBUF *data,const char *s);
	
/* 追加字符 */
bool sbuf_append_chr(SBUF *data,char c);
	
/* 清空缓冲区 */
void sbuf_reset(SBUF *data);

/* 回退字符串 */
bool sbuf_back(SBUF *data,size_t len);

/* 清除指定区域字符串 */
bool sbuf_eraser(SBUF *data,size_t start,size_t end);

/* 复制一个动态字符串 */
bool sbuf_copy(SBUF *to,SBUF *from);

/* 指定位置插入一个字符串 */
bool sbuf_insert_str(SBUF *data,size_t pos,char *str);

/* 指定位置插入一个字符 */
bool sbuf_insert_chr(SBUF *data,size_t pos,char c);

/* 指定位置插入 */
bool sbuf_insert(SBUF *data,size_t pos,const char *fmt,...);

/* 指定位置插入指定数量字符 */
bool sbuf_ninsert(SBUF *data,size_t pos,size_t len,const char *fmt,...);

/* 返回字符串长度 */
size_t sbuf_length(SBUF *data);

/* 返回字符串 */
char *sbuf_string(SBUF *data);

#endif
