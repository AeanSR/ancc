/*
    ancc: another noobish C compiler
    Aean, 2014.6.28
        Website: http://aean.net/  Mail: v@aean.net

    ancc project is licensed under the terms of the MIT License.
    Copyright (C) 2014 Aean
*/

#include "ancc.h"

int main(int argc, char** argv) {
    int i;
    for(i = 0; i < NAL; i++)
        keylist[i].name = strpool(keylist[i].name);
    printf(
        "ancc: another noobish C compiler.\n"
        "    Aean, 2014 <v@aean.net>\n"
        "ancc project is licensed under the terms of the MIT License. You should have    "
        "recieved a copy of the license. If not, please visit <http://mit-license.org/>. "
        "\n\n"
    );
    if (argc < 2) {
        char* p = argv[0];
        while(*p)p++;
        while(p > argv[0] && *p!='\\')p--;
        if(*p=='\\')p++;

        printf(
            "ancc CLI Format:\n"
            "%s <sourcename>\n",
            p);
        return 0;
    }
    push_file(argv[1]);
    preprocess();
    read_source();
    lr1();
    printf("%d errors, %d warnings\n", error_occured, warning_occured);
    return 0;
}
