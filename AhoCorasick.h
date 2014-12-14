#ifndef AHOCORASICK_H
#define AHOCORASICK_H

extern "C" {
#include "aho-corasick.h"
}

class AhoCorasick 
{
    public:
        AhoCorasick() {aho_corasick_init(&root);}
        virtual ~AhoCorasick() {aho_corasick_destroy(&root);}

        int add(const char *aStr, int len) {
            return aho_corasick_addstring(&root, (unsigned char *)aStr, len);
        }
        int add(const unsigned char *aStr, int len) {
            return aho_corasick_addstring(&root, (unsigned char *)aStr, len);
        }
        int compile(void) {
            return aho_corasick_maketree(&root);
        }
        int search(const unsigned char *aStr, int len, char *match) {
            int ret;
            aho_corasick_state_t *state = root.zerostate, *os;
            size_t s, e;

            ret = ahocorasick_KeywordTree_search_helper(state, (unsigned char *)aStr, len, 0, &s, &e, &os);
            if (ret) {
                strncpy(match, (const char *) &aStr[s], e-s);
            }
            return ret;
        }
        int search(const char *aStr, int len, char *match) {
            return search((const unsigned char *) aStr, len, match);
        }

    private:
        aho_corasick_t root;
};

#endif

