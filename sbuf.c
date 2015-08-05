#include "sbuf.h"

#ifdef _WIN32
int vasprintf(char **strp,const char *fmt,va_list ap);
#endif
	
SBUF *sbuf_new(size_t size)
{
	SBUF *data;
	
	data=malloc(sizeof(SBUF));
	if(!data) return NULL;
	
	if(size) data->size=size;
	else
		data->size=SBUF_BUF;
	
	data->len=0;
	data->buf=(char *)malloc(size);
	data->buf[0]='\0';
	
	return data;
}
	
void sbuf_free(SBUF *data)
{
	if(!data) return;
	if(data->buf) free(data->buf);
	free(data);
}
	
bool sbuf_append(SBUF *data,const char *fmt,...)
{
	static char empty='\0';
	va_list ap;
	int size;
	
	if(!data || !data->buf) return false;
	
	//获取字符串长度
	va_start(ap,fmt);
	size=vsnprintf(&empty,0,fmt,ap)+1;
	va_end(ap);
	if(size < 0) {size*=-1;};
	//如果当前缓冲区字符串长度加上要追加的字符串大于缓冲区则动态开辟存储空间
	if(data->size < data->len+size)
	{
		char *p;

		p=(char *)realloc(data->buf,data->size+data->len+size+SBUF_BUF);
	
		if(!p) return false;
	
		data->buf=p;
		//更新缓冲区大小
		data->size+=data->len+size+SBUF_BUF;
	}
	
	//开始追加
	va_start(ap,fmt);
	vsnprintf(data->buf+data->len,size,fmt,ap);
	va_end(ap);
	
	//更新缓冲区字符串长度
	data->len+=size-1;
	
	return true;
}
	
bool sbuf_nappend(SBUF *data,size_t n,const char *fmt,...)
{
	static char empty='\0';
	va_list ap;
	int size;
	
	if(!data || !data->buf) return false;
	
	//计算出需要追加的字符串长度
	va_start(ap,fmt);
	size=vsnprintf(&empty,0,fmt,ap);
	va_end(ap);
	if(size < 0) {size*=-1;};
	if(n > size) n=size;
	
	if(data->size < data->len+n+1)
	{
		char *p;

		p=(char *)realloc(data->buf,data->size+data->len+n+SBUF_BUF+1);
	
		if(!p) return false;
		data->buf=p;
	
		data->size+=data->len+n+SBUF_BUF+1;
	}
	
	va_start(ap,fmt);
	vsnprintf(data->buf+data->len,n+1,fmt,ap);
	va_end(ap);
	
	data->len+=n;
	
	return true;
}
	
bool sbuf_append_str(SBUF *data,const char *s)
{
	int size;
	
	if(!data || !data->buf || !s) return false;
	size=strlen(s)+1;
	
	if(data->size < data->len+size)
	{
		char *p;

		p=(char *)realloc(data->buf,data->size+data->len+size+SBUF_BUF);
	
		if(!p) return false;
		data->buf=p;
		data->size+=data->len+size+SBUF_BUF;
	}
	
	snprintf(data->buf+data->len,size,"%s",s);
	data->len+=size-1;
	
	return true;
}
	
bool sbuf_append_chr(SBUF *data,char c)
{
	if(!data || !data->buf) return false;
	if(data->size < data->len+2)
	{
		char *p;

		p=(char *)realloc(data->buf,data->size+data->len+SBUF_BUF+2);
		if(!p) return false;
		data->buf=p;
		data->size+=data->len+SBUF_BUF+2;
	}
	
	data->buf[data->len]=c;
	data->buf[data->len+1]='\0';
	++data->len;
	
	return true;
}
	
void sbuf_reset(SBUF *data)
{
	if(!data || !data->buf) return;
	
	data->len=0;
	data->buf[0]='\0';
}

bool sbuf_back(SBUF *data,size_t len)
{
	if(!data || !data->len) return false;

	data->len-=len;
	if(data->len <= 0)
		data->buf[0]='\0';
	else
		data->buf[data->len]='\0';

	return true;
}

bool sbuf_eraser(SBUF *data,size_t start,size_t end)
{
	if(!data || !data->len) return false;
	if(start < 0 || start >= data->len || start >= end || end <= 0 || end > data->len)
		return false;

	int len;

	len=data->len-end+1;
	memcpy(data->buf+start,data->buf+end,len);
	data->len-=end-start;

	return true;
}

bool sbuf_copy(SBUF *to,SBUF *from)
{
	if(!from || !from->buf) return false;

	if(!to)
		to=malloc(sizeof(SBUF));
	if(!to) return false;

	to->size=from->size;
	to->len=from->len;
	to->buf=malloc(sizeof(char)*from->size);
	if(!to->buf) return false;
	memcpy(to->buf,from->buf,from->len);
	
	return true;
}

bool sbuf_insert_str(SBUF *data,size_t pos,char *str)
{
	if(pos < 0 || pos > data->len) return false;
	if(!data || !data->buf) return false;

	int len=strlen(str);
	if(len <= 0) return false;

	sbuf_append_str(data,str);
	sbuf_eraser(data,data->len-len,data->len);
	memcpy(data->buf+pos+len,data->buf+pos,data->len-pos);
	memcpy(data->buf+pos,str,len);
	data->len+=len;

	return true;
}

bool sbuf_insert_chr(SBUF *data,size_t pos,char c)
{
	char buf[2];

	buf[0]=c;
	buf[1]='\0';
	return sbuf_insert_str(data,pos,buf);
}

bool sbuf_insert(SBUF *data,size_t pos,const char *fmt,...)
{
	if(pos < 0 || pos > data->len) return false;
	if(!data || !data->buf) return false;

	char *temp=NULL;
	va_list ap;
	size_t len;

	va_start(ap,fmt);
	len=vasprintf(&temp,fmt,ap);
	va_end(ap);
	sbuf_insert_str(data,pos,temp);
	free(temp);

	return true;
}

bool sbuf_ninsert(SBUF *data,size_t pos,size_t len,const char *fmt,...)
{
	if(pos < 0 || pos > data->len) return false;
	if(!data || !data->buf) return false;

	char *temp=NULL;
	va_list ap;
	size_t size;

	va_start(ap,fmt);
	size=vasprintf(&temp,fmt,ap);
	va_end(ap);

	if(size > len)
		temp[len]='\0';
	printf("in test %s\n",temp);
	sbuf_insert_str(data,pos,temp);
	free(temp);

	return true;
}

#ifdef _WIN32
int vasprintf(char **strp,const char *fmt,va_list ap)
{
        char *res=NULL;
        int len;
        static char empty='\0';

        len=vsnprintf(&empty,0,fmt,ap);
        if(len == -1)
        {
                *strp=NULL;
                return -1;
        }
        res=(char *)malloc(sizeof(char)*(len+1));
        if(res == NULL)
        {
                *strp=NULL;
                return -1;
        }

        if(vsnprintf(res,len+1,fmt,ap) == -1)
        {
                free(res);
                *strp=NULL;
                return -1;
        }

        *strp=res;
        return len;
}
#endif

