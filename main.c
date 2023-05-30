#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>

extern void* __start_codebook;
extern void* __stop_codebook;

struct log_entry {
    uint32_t msg;
    uint32_t arg0;
    uint32_t arg1;
    uint32_t arg2;
    uint32_t arg3;
} __attribute__ ((packed));

struct log_entry __log[64] __attribute__ ((section ("log"))) = {0};
static uint32_t log_idx = 0;

#define ULOG(num, arg1, arg2, arg3, arg4) do { \
    printf((const char *)(unsigned long)&__start_codebook + num, (arg1), (arg2), (arg3), (arg4)); \
} while(0)


void dump_log(void)
{
    int i;
    printf("dump_log: %d\n", log_idx);
    for (i = 0; i< log_idx; i++) {
        printf((const char *)(__log[i].msg + (unsigned long)&__start_codebook), __log[i].arg0, __log[i].arg1, __log[i].arg2, __log[i].arg3);
    }
}

void save_log(uint32_t msg, ...)
{
    va_list ap;
    uint32_t i;    
    va_start(ap, msg);
    __log[log_idx].msg = msg;
    __log[log_idx].arg0 = va_arg(ap, uint32_t);
    __log[log_idx].arg1 = va_arg(ap, uint32_t);
    __log[log_idx].arg2 = va_arg(ap, uint32_t);
    __log[log_idx].arg3 = va_arg(ap, uint32_t);
    log_idx++;
    va_end(ap);
    printf("\n");
}

#define LOG(fmt, ...) do { \
    const static char __buf[] __attribute__ ((section ("codebook"))) = fmt; \
    printf("%s:%d > LOG buf addr: %p %p\n", __FILE__, __LINE__, &__start_codebook, __buf); \
    save_log((unsigned long)__buf - (unsigned long)&__start_codebook,  ##__VA_ARGS__); \
} while(0)

int main(void)
{
    
    LOG("hello %d %d %d %d\n", 1, 2, 3, 4);    
    LOG("world\n");
    LOG("hello %d %d %d %d\n", 1, 2, 3, 5);
    
    dump_log();

}