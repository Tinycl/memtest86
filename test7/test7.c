#include <stdio.h>
#include <stdlib.h>
#include "asmhelper.h"

void test7_init(unsigned long long bytes)
{
    /*
    *  initial -> [start, half] 
    *  process  [start, end]
    *  start: 
    *  half: start + 2112
    *  end : half + 2122
    *
    */
	unsigned int len = 0;
	len = bytes / 4;    // dw
	len = len >> 4;     // uint of 64 byte chunks
	len = len >> 1;     // only init len/2, just copy first half onto the second half in the mov step
	unsigned int base_val = 1;
	unsigned int neg_val = 0;
	unsigned int buf[16];
    unsigned int count = 1;
	while (len > 0)
	{
		neg_val = ~base_val;
        // Set a block of 64 bytes   //   first block DWORDS are:
        buf[0] = base_val;             //   0x00000001
        buf[1] = base_val;             //   0x00000001
        buf[2] = base_val;             //   0x00000001
        buf[3] = base_val;             //   0x00000001
        buf[4] = neg_val;              //   0xfffffffe
        buf[5] = neg_val;              //   0xfffffffe
        buf[6] = base_val;             //   0x00000001
        buf[7] = base_val;             //   0x00000001
        buf[8] = base_val;             //   0x00000001
        buf[9] = base_val;             //   0x00000001
        buf[10] = neg_val;             //   0xfffffffe
        buf[11] = neg_val;             //   0xfffffffe
        buf[12] = base_val;            //   0x00000001
        buf[13] = base_val;            //   0x00000001
        buf[14] = neg_val;             //   0xfffffffe
        buf[15] = neg_val;             //   0xfffffffe

        len--;
        // Rotate the bit left, including an all-zero state.
        // It can't hurt to have a periodicity of 33 instead of
        // a power of two.
        if (base_val == 0) 
        {
            base_val = 1;
        }
        else if (base_val & 0x80000000) 
        {
            base_val = 0;
        }
        else 
        {
            base_val = base_val << 1;
        }

        printf("dd 0x%x;\n", buf[0]);
        printf("dd 0x%x;\n", buf[1]);
        printf("dd 0x%x;\n", buf[2]);
        printf("dd 0x%x;\n", buf[3]);
        printf("dd 0x%x;\n", buf[4]);
        printf("dd 0x%x;\n", buf[5]);
        printf("dd 0x%x;\n", buf[6]);
        printf("dd 0x%x;\n", buf[7]);
        printf("dd 0x%x;\n", buf[8]);
        printf("dd 0x%x;\n", buf[9]);
        printf("dd 0x%x;\n", buf[10]);
        printf("dd 0x%x;\n", buf[11]);
        printf("dd 0x%x;\n", buf[12]);
        printf("dd 0x%x;\n", buf[13]);
        printf("dd 0x%x;\n", buf[14]);
        printf("dd 0x%x;\n", buf[15]);

       /*
        if (count == 33)
        {
            break;
        }
        count++;    // a chunk has a 33 * 64 byte = 33 * 16 *dw = 2112 bytes 
        */

	}

}


void block_move(unsigned long* pbuf, unsigned long long len_dw)
{
    unsigned long half_len_dw = len_dw / 2;
    unsigned long* pmid = pbuf + half_len_dw;
    unsigned long* pend = pbuf + len_dw * 4;
    // Move first half to 2nd half:
    AsmMovsDW(pmid /*dst*/, pbuf /*src*/, half_len_dw);

    // Move the second half, less the last 8 dwords
    // to the first half plus an offset of 8 dwords.
    AsmMovsDW(pbuf + 32, pmid, half_len_dw - 8); // 32 byte, 8*4

    // Finally, move the last 8 dwords of the 2nd half
    // to the first 8 dwords of the first half.
    AsmMovsDW(pbuf, pend - 32, 8);

    /*
    *   pbuf    pmid   pend  [pbuf pmid] == [pmid pend]
    *   [pend-32 pend] [pmid pend-32] --  [pmid pend]
    *
    */
}

int main()
{

    test7_init(0x400000);
	//system("pause");
	return 0;
}