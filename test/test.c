#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>


#define MT6256  0x00000001
#define MT6258  0x00000002


#if (CHIPID == MT6256)
#define CHIP MT6256
//#error "MT6256"
#elif (CHIPID == MT6258)
#define CHIP MT6258
//#error "MT6258"
#else
#error "NO chip"
#endif


/**
 * Macro to set machine related register
 * value: value to set
 * processor: processor to use
 * op1: operation code 1
 * crn: register n
 * crm: register m
 * op2: operation code 2
 */
#define WRITE_SCCR(value, processor, op1, crn, crm, op2) \
asm volatile ("mcr " #processor "," #op1 ", %0," #crn "," #crm "," #op2 "" :: "r" (value))

/**
 * Macro to get machine related register
 * value: addr to store the value
 * processor: processor to use
 * op1: operation code 1
 * crn: register n
 * crm: register m
 * op2: operation code 2
 */
#define READ_SCCR(value, processor, op1, crn, crm, op2)	 \
asm volatile ("mrc " #processor "," #op1 ", %0," #crn "," #crm "," #op2 "" : "=r" (value))


/*! \def WRITE_PMCR(value)
    Macro to get/set PMCR register
    value is used to read/write value
 */
#define WRITE_PMCR(value)		WRITE_SCCR(value, p15, 0, c15, c12, 0)
#define READ_PMCR(value)	        READ_SCCR(value,  p15, 0, c15, c12, 0)

/*! \def WRITE_CCR(value)
    Macro to get/set CCR register
    value is used read/write value
 */
#define WRITE_CCR(value)                WRITE_SCCR(value, p15, 0, c15, c12, 1)
#define READ_CCR(value)                 READ_SCCR(value,  p15, 0, c15, c12, 1)

/**
 * uint32_t get_cycle_count(void)
 * @get cycle count of performance counter
 * @return execution result
 */
static inline uint32_t get_cycle_count(void)
{
        uint32_t value = 0;
        
        /** Read cycle count register */
        READ_CCR(value);

        return value;
}


/**
 * void init_perf_counter(int32_t)
 * @initilaize performance counter
 * @do_reset: flag to indicate do reset operation
 */
static inline void init_perf_counter(int32_t do_reset)
{
        /** in general enable all counters (including cycle counter) */
        uint32_t value = 0x1;

        /** peform reset */
        if (do_reset) {
                value |= 0x4;  // reset cycle counter to zero
        } 

        /** enable all counters and reset cycle counter to zero */
        WRITE_PMCR(value);
}

/**
 * void test_square_zigzag(void)
 * @square zigzag test program
 */
void test_square_zigzag(void)
{
#define TBL_ROW_NUM    (4)
#define TBL_MAX_SUM    (TBL_ROW_NUM - 1)
#define TBL_ELE_NUM    (TBL_ROW_NUM * TBL_ROW_NUM)

        int32_t input[TBL_ELE_NUM] = {  0,  1,  2,  3,
                                        4,  5,  6,  7,
                                        8,  9, 10, 11,
                                       12, 13, 14, 15, };
        int32_t output[TBL_ELE_NUM];

        int32_t *begin = &output[0];
        int32_t *end = &output[TBL_ELE_NUM - 1];
        int32_t x, offs, sum, i;

        for (sum = 0; sum < TBL_ROW_NUM; ++sum) {
                x = ((sum + 1) % 2) * sum;
                offs = 2 * (sum % 2) - 1;

                for (i = sum; i >= 0; --i) {
                        *begin++ = input[x * TBL_ROW_NUM + (sum-x)];
                        if (sum < TBL_MAX_SUM) {
                                *end-- = input[(TBL_MAX_SUM - x) * TBL_ROW_NUM + (TBL_MAX_SUM - (sum-x))];
                        }
                        x += offs;
                }
        }

        printf("output = { ");
        for (i = 0; i < TBL_ELE_NUM; ++i) {
                printf("%d, ", output[i]);
        }
        printf("}\n");

}

char* convert(char* s, int numRows) {
	char *out;
	int i, j, t, idx;
	int len = strlen(s);
    
	if (numRows == 1) {
		return s;
	}

        j = 0;
	out = malloc(len);

	for (i = 0; i < numRows; ++i) {
		idx = i;
		t = 0;
		while (idx < len) {
			*(out + j) = *(s + idx);
                        printf("%c", *(out + j));
			j += 1;
            
			if (i == 0 || i == numRows - 1) {
				idx += (2 * (numRows - 1));
			} else {
                                t += 1;
				idx += (t & 1) ? (2 * (numRows - 1 - i)) : (2 * i);
			}
		}
	}

        assert(j==len);

	return out;
}

/**
 * void test_zigzag(void)
 * @zigzag test program
 */
void test_zigzag(void)
{
	char s[] = "Apalindromeisaword,phrase,number,orothersequenceofunitsthatcanbereadthesamewayineitherdirection,withgeneralallowancesforadjustmentstopunctuationandworddividers.";

//      char s[] = "0123456789";
        char *output;
	int numRows = 2;

        printf("strlen(s)=%d\n", strlen(s));
        printf("sizeof(s)=%d\n", sizeof(s));

	output = convert(s, numRows);

        printf("strlen(output)=%d\n", strlen(output));
        printf("sizeof(output)=%d\n", sizeof(*output));

        printf("output = \"");
        printf("%s", output);
        printf("\"\n");
}

int trap(int* height, int heightSize) {
	int i, j, h ;
	int pre, curr, max, top;
	int water = 0;

	pre = 0;
	max = 0;

	for (i = 0; i < heightSize; ++i) {
		curr = height[i];

		if (curr > pre) {
			if (curr >= max) {
				top = max;
				max = curr;
			} else {
				top = curr;
			}

			for (h = pre; h < top; ++h) {
				j = i - 1;
				while (j >= 0 && height[j--] <= h) {
					water++;
				}
			}
		}

		pre = curr;
	}

	return water;
}

/**
 * void test_trapping_water(void)
 * @trapping water test program
 */
void test_trapping_water(void)
{
	int wall[] = { 0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1 };
	int wall_num = 12;
	int water;

	water = trap(wall, wall_num);

	printf("water = %d\n", water);
}

void test_switch_case(int c)
{
        int a = 0;

        switch (c) {
                case 0:
                        a = c + 1;
                        break;
                case 123:
                        a = c + 2;
                        break;
                case 2839:
                        a = c + 3;
                        break;
                case 9833:
                        a = c + 4;
                        break;
                case 4126538:
                        a = c + 5;
                        break;
                default:
                        a = 28;
                        break;
        }

        printf("a = %d\n", a);
}

void test_if_else(int c)
{
        int a = 0;

        if (c == 0) {
                a = c + 1;
        } else if (c == 123) {
                a = c + 2;
        } else if (c == 2839) {
                a = c + 3;
        } else if (c == 9833) {
                a = c + 4;
        } else if (c == 4126538) {
                a = c + 5;
        } else {
                a = 28;
        }

        printf("a = %d\n", a);
}

uint32_t func(uint32_t x)
{
        uint32_t n = x;
        n = ((n & 0xffff0000) >> 16) | ((n & 0x0000ffff) << 16);
        n = ((n & 0xff00ff00) >>  8) | ((n & 0x00ff00ff) <<  8);
        n = ((n & 0xf0f0f0f0) >>  4) | ((n & 0x0f0f0f0f) <<  4);
        n = ((n & 0xcccccccc) >>  2) | ((n & 0x33333333) <<  2);
        n = ((n & 0xaaaaaaaa) >>  1) | ((n & 0x55555555) <<  1);
        return n;
}

uint32_t func_for(uint32_t x)
{
        uint32_t i, s, m, l;
        uint32_t n = x;

        for (i = 0; i < 16; ++i) {
                s = 31 - i;
                m = (n & (1<<s))>>s;
                l = (n & (1<<i))>>i;

                n &= ~((1<<s)|(1<<i));
                n |= ((m<<i)|(l<<s));
        }
        return n;
}

uint16_t func_16(uint16_t x)
{
        uint16_t n = x;
        n = ((n & 0xff00) >>  8) | ((n & 0x00ff) <<  8);
        n = ((n & 0xf0f0) >>  4) | ((n & 0x0f0f) <<  4);
        n = ((n & 0xcccc) >>  2) | ((n & 0x3333) <<  2);
        n = ((n & 0xaaaa) >>  1) | ((n & 0x5555) <<  1);
        return n;
}

uint32_t half_add(uint32_t a, uint32_t b)
{
        if (b == 0) return a;
        uint32_t sum = a ^ b;             /* 相加但不進位 */
        uint32_t carry = (a & b) << 1;    /* 進位但不相加 */
        
        return half_add(sum, carry);
}

uint32_t mult(uint32_t x, uint32_t y)
{
	if (y == 0) return 0;

        uint32_t sum = (y & 0x1 && !(x & 0x80000000)) ? x : 0;

	return half_add(sum, mult(x<<1, y>>1));
}

signed int longer(char *a, char *b)
{
        return strlen(a) - strlen(b) > 0;
}


#define DEV_NUM            (1)
#define CHN_NUM            (4)

#define INVALID_DEV_IDX    (-1)
#define MIN_DEV_IDX        (0)
#define MAX_DEV_IDX        (DEV_NUM - 1)
#define INVALID_CHN_IDX    (-1)
#define MIN_CHN_IDX        (0)
#define MAX_CHN_IDX        (CHN_NUM - 1)


typedef enum {
        TYPE_BOOL,
        TYPE_CHAR,
        TYPE_INT8,
        TYPE_UINT8,
        TYPE_INT16,
        TYPE_UINT16,
        TYPE_INT32,
        TYPE_UINT32,
        TYPE_STR,
        TYPE_NUM,
} types;


typedef struct {
        int16_t  bayer;
        int16_t  fps;
} path_param;

typedef struct {
        int32_t  op_mode;
        int32_t  hdr_mode;
} dgen_param;

typedef struct {
        path_param  path;
        dgen_param  gen;
} dev_param;

typedef struct {
        uint8_t  mirror;
        uint8_t  flip;
} cgen_param;

typedef struct {
        int32_t  gop;
        int32_t  qp;
} enc_param;

typedef struct {
        cgen_param  gen;
        enc_param   enc;
} chn_param;


typedef struct {
        dev_param  dev[DEV_NUM];
        chn_param  chn[CHN_NUM];
} sample_conf;


sample_conf g_conf;


void get_value(void *dest, types type)
{
        char *val = strtok(NULL, " =");
	
//      printf("val = |%s|.\n", val);

        switch (type) {
        case TYPE_BOOL:
        //      *(bool *)dest = (bool)atoi(val);
                break;

        case TYPE_INT8:
                *(int8_t *)dest = (int8_t)atoi(val);
                break;

        case TYPE_UINT8:
                *(uint8_t *)dest = (uint8_t)atoi(val);
                break;

        case TYPE_INT16:
                *(int16_t *)dest = (int16_t)atoi(val);
                break;

        case TYPE_UINT16:
                *(uint16_t *)dest = (uint16_t)atoi(val);
                break;

        case TYPE_INT32:
                *(int32_t *)dest = (int32_t)atoi(val);
                break;

        case TYPE_UINT32:
                *(uint32_t *)dest = (uint32_t)atoi(val);
                break;

        case TYPE_STR:
        //      *(char *)dest = (char *)atoi(val);
                break;

        default:
                break;
        }

        return;
}

int parse_dev_path_param(char *tok, path_param *p)
{
        int hit = 1;

//      printf("tok = |%s|.\n", tok);

        if (!strcmp(tok, "bayer")) {
                get_value((void *)&p->bayer, TYPE_INT16);
        } else if (!strcmp(tok, "fps")) {
                get_value((void *)&p->fps, TYPE_INT16);
        } else {
                hit = 0;
        }

	return hit;
}

int parse_dev_gen_param(char *tok, dgen_param *p)
{
        int hit = 1;

//      printf("tok = |%s|.\n", tok);

        if (!strcmp(tok, "op_mode")) {
                get_value((void *)&p->op_mode, TYPE_INT32);
        } else if (!strcmp(tok, "hdr_mode")) {
                get_value((void *)&p->hdr_mode, TYPE_INT32);
        } else {
                hit = 0;
        }

	return hit;
}

int parse_chn_gen_param(char *tok, cgen_param *p)
{
        int hit = 1;

//      printf("tok = |%s|.\n", tok);

        if (!strcmp(tok, "mirror")) {
                get_value((void *)&p->mirror, TYPE_UINT8);
        } else if (!strcmp(tok, "flip")) {
                get_value((void *)&p->flip, TYPE_UINT8);
        } else {
                hit = 0;
        }

	return hit;
}

int parse_chn_enc_param(char *tok, enc_param *p)
{
        int hit = 1;

//      printf("tok = |%s|.\n", tok);

        if (!strcmp(tok, "gop")) {
                get_value((void *)&p->gop, TYPE_INT32);
        } else if (!strcmp(tok, "qp")) {
                get_value((void *)&p->qp, TYPE_INT32);
        } else {
                hit = 0;
        }

	return hit;
}

int parse_dev_param(char *tok)
{
        static int32_t idx = INVALID_DEV_IDX;

        int hit = 0;

//      printf("tok = |%s|.\n", tok);

        if (!strcmp(tok, "video_dev_idx")) {
                get_value((void *)&idx, TYPE_INT32);

                hit = 1;

                goto end;
        }

        /* Parae other device parameter for valid channel index */
        if (idx >= MIN_DEV_IDX && idx <= MAX_DEV_IDX) {
                dev_param *p = &g_conf.dev[idx];

                /* Parse path parameter */
                hit = parse_dev_path_param(tok, &p->path); if (hit) { goto end; }
        
                /* Parse general parameter */
                hit = parse_dev_gen_param(tok, &p->gen); if (hit) { goto end; }
        }

end:
        return hit;
}

int parse_chn_param(char *tok)
{
        static int32_t idx = INVALID_CHN_IDX;

        int hit = 0;

//      printf("tok = |%s|.\n", tok);

        if (!strcmp(tok, "video_chn_idx")) {
                get_value((void *)&idx, TYPE_INT32);

                hit = 1;

                goto end;
        }

        /* Parae other channel parameter for valid channel index */
        if (idx >= MIN_CHN_IDX && idx <= MAX_CHN_IDX) {
                chn_param *p = &g_conf.chn[idx];

                /* Parse general parameter */
                hit = parse_chn_gen_param(tok, &p->gen); if (hit) { goto end; }
        
                /* Parse encoder parameter */
                hit = parse_chn_enc_param(tok, &p->enc); if (hit) { goto end; }
        }

end:
        return hit;
}

int parse_param(char *str)
{
        int   hit = 0;
	char *tok = strtok(str, " =");

        while (tok != NULL) {
                hit = 0;

        //      printf("tok = |%s|.\n", tok);

                /* Stop parsing this line when comment sign found */
                if (!strncmp(tok, "#", strlen("#"))) {
                        hit = 1;
                        break;
                }

                /* Parse device parameter */
                hit = parse_dev_param(tok); if (hit) { goto next; }

                /* Parse channel parameter */
                hit = parse_chn_param(tok); if (hit) { goto next; }

                if (!hit) {
                        /* Bypass error for newline sign */
                        if (!strcmp(tok, "\n")) {
                                hit = 1;
                        } else {
                                /* Stop parsing when unknown parameter found */
                                printf("Unknown parameter: %s\n", tok);
                                break;
                        }
                }

next:
                /* Parse other parameters in same line */
                tok = strtok(NULL, " =");
        }

	return hit;
}

int parse_config_file(char *filename)
{
        int  ret = 0;
        char str[256];
	FILE *fp;

        /* Open config file for parsing */
	fp = fopen(filename, "r");
        if (fp == NULL) {
		printf("Error opening file.\n");
                return -1;
	}

        /* Parse config file line by line */
	while (fgets(str, sizeof(str), fp) != NULL) {
		ret = parse_param(str);

                /* Stop parsing when a unknown parameter found */
                if (!ret) {
                        printf("Parsing parameter file failed.\n");
                        break;
                }
	}

        fclose(fp);

        return ret;
}

int parse(int argc, char *argv[])
{
	int c;

//	printf("Init optind = %d\n", optind);

	while((c = getopt(argc, argv, "d:p:")) != -1) {
		switch(c) {
		case 'd':
                        /* Parse parameter from config file */
                        parse_config_file(optarg);
		//	printf("argv = %s, optind = %d\n", optarg, optind);
			break;

	        case 'p':
                        /* Parse parameter from command line */
                        parse_param(optarg);
		//	printf("argv = %s, optind = %d\n", optarg, optind);
			break;

	        case ':':
			printf("oops\n");
			break;

	        case '?':
		default:
			printf("wrong command\n");
			break;
		}
	}

	return 0;
}

void show_result(void)
{
        int i;

        for(i = 0; i < DEV_NUM; ++i) {
                dev_param *p = &g_conf.dev[i];

                printf("Video device %d:\n", i);
                printf("Path: bayer = %d, fps = %d.\n",
                       p->path.bayer, p->path.fps);
                printf("General: op_mode = %d, hdr_mode = %d.\n",
                       p->gen.op_mode, p->gen.hdr_mode);
                printf("\n");
        }

        for(i = 0; i < CHN_NUM; ++i) {
                chn_param *p = &g_conf.chn[i];

                printf("Video channel %d:\n", i);
                printf("General: mirror = %d, flip = %d.\n",
                       p->gen.mirror, p->gen.flip);
                printf("Encoder: gop = %d, qp = %d.\n",
                       p->enc.gop, p->enc.qp);
                printf("\n");
        }
}

#if 1
/**
 * void main(void)
 * main entry function
 */
int main(int argc, char *argv[])
{
//      uint32_t overhead = 0, t = 0;

        /** init performance counter */
//      init_perf_counter(1); 
        
        /** measure the counting overhead */
//      overhead = get_cycle_count();
//      overhead = get_cycle_count() - overhead;    

        parse(argc, argv);

        show_result();

        printf("Test Function Start ...\n");
        
//      t = get_cycle_count();
        
        /** do some stuff here ... */
//      test_program();
//      test_zigzag();
//	test_trapping_water();
//      test_switch_case(3);
//      test_if_else(3);
//	t = mult(4294967295,1);
//      t = longer("wifi", "teeeeee");


//      t = get_cycle_count() - t;
        
        printf("Test Function End.\n");

//      printf("the overhead to get cycle count is %d.\n", overhead);
//      printf("function took exactly %d cycles (including function call)\n", t - overhead);

        return 0;
}

#else
#include "omp.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
        int i;
        int childpid;

        #pragma omp for
	for (i = 0; i < 2; i++) {
		childpid = fork();
        	printf("-");
                printf("i: %d, process ID: %ld, parent ID: %ld, child ID: %ld\n",
                              i, (long)getpid(), (long)getppid(), (long)childpid);

	}
	wait(NULL); wait(NULL); wait(NULL);

	return 0;
}
#endif

