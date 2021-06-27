/*  The copyright in this software is being made available under the BSD License,
 *  included below. This software may be subject to other third party and
 *  contributor rights, including patent rights, and no such rights are granted
 *  under this license.
 *
 *  Copyright (c) Microsoft Corporation
 *
 *  All rights reserved.
 *
 *  BSD License
 *
 *  Redistribution and use in source and binary forms, with or without modification,
 *  are permitted provided that the following conditions are met:
 *
 *  Redistributions of source code must retain the above copyright notice, this list
 *  of conditions and the following disclaimer.
 *
 *  Redistributions in binary form must reproduce the above copyright notice, this
 *  list of conditions and the following disclaimer in the documentation and/or
 *  other materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ""AS IS""
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

int tolower (int c)
{
    if(c >= 'A' && c <= 'Z')
    {
        c -= ('A' - 'a');
    }
    return c;
}

int toupper (int c)
{
    if(c >= 'a' && c <= 'z')
    {
        c += ('A' - 'a');
    }
    return c;
}

int strncasecmp(const char *str1, const char *str2, size_t n)
{
    size_t i = 0;
    for(i = 0; i < n && str1[i] && str2[i]; i++)
    {
        char delta = tolower(str1[i]) - tolower(str2[i]);
        if (delta != 0)
        {
            return delta;
        }
    }
    return 0;
}

char *strcat(char *dst, const char *src)
{
    strncat(dst, src, strlen(src));
}

char *strncat(char *dst, const char *src, size_t siz)
{
    char *c = dst + strlen(dst);
    while(siz-- && (*c++ = *src++));
    *c = '\0';
	return dst;
}

#ifdef CUSTOM_RAND_GENERATE_BLOCK
#include <tee_internal_api.h>
int wolfRand(unsigned char* output, unsigned int sz)
{
    TEE_GenerateRandom((void *)output, (uint32_t)sz);

    return 0;
}
#endif

#ifdef XMALLOC_OVERRIDE
#include <tee_internal_api.h>
void *wolfMalloc(size_t n)
{
    void *addr = TEE_Malloc(n, TEE_MALLOC_FILL_ZERO);
    if (addr == NULL) {
        EMSG("Malloc failed, out of memory!");
        //TEE_Panic(TEE_ERROR_OUT_OF_MEMORY);
        return addr;
    } else {
        return addr;;
    }
}

void *wolfRealloc(void *p, size_t n)
{
    void *addr = TEE_Realloc(p, n);
    if (addr == NULL) {
        EMSG("Realloc failed, out of memory!");
        //TEE_Panic(TEE_ERROR_OUT_OF_MEMORY);
        return addr;
    } else {
        return addr;
    }
}

#endif