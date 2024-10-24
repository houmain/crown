/*
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>
*/

#include <wajic.h>
#include <wajic_file.h>
#include <stdio.h>
#include <stdlib.h>

// This function is called at startup
WA_EXPORT(WajicMain) void WajicMain(int i)
{
	unsigned int size;
	unsigned char* file = WaFileMallocRead("TXT", &size);
	printf("File size: %u - content: [%s]...\n", size, file);
	free(file);

	char data[8];
	unsigned int read = WaFileRead("TXT", data, 0, 8);
	printf("Data: [%.*s] (read bytes: %d)\n", read, data, read);

	printf("File size: %u\n", WaFileGetSize("TXT"));

	char buf[128 + 1];
	FILE* f = fopen("TXT", "rb");
	int len = fread(buf, 1, 128, f);
	fclose(f);
	buf[128] = '\0';
	printf("fread len: %d - content: [%s]...\n", len, buf);
}
