#include <string.h>
#include <jansson.h>
#include "orcania.h"

/**
 * 
 * Orcania library
 * 
 * Different functions for different purposes but that can be shared between
 * all Angharad modules
 * 
 */

/**
 * char * str_replace(const char * source, char * old, char * new)
 * replace all occurences of old by new in the string source
 * return a char * with the new value
 * return NULL on error
 * returned value must be free'd after use
 */
char * str_replace(const char * source, const char * old, const char * new) {
  char * to_return, * ptr, * pre, * next;
  size_t len, pre_len;
  
  if (source == NULL || old == NULL || new == NULL) {
    return NULL;
  }
  
  ptr = strstr(source, old);
  if (ptr == NULL) {
    return strdup(source);
  } else {
    pre_len = ptr-source;
    pre = malloc((pre_len+1)*sizeof(char));
    if (pre == NULL) {
      return NULL;
    }
    memcpy(pre, source, pre_len);
    pre[pre_len] = '\0';
    
    next = str_replace(source+strlen(pre)+strlen(old), old, new);
    if (next == NULL) {
      free(pre);
      return NULL;
    }
    len = ((ptr-source)+strlen(new)+strlen(next));
    to_return = malloc((len+1)*sizeof(char));
    if (to_return == NULL) {
      free(pre);
      free(next);
      return NULL;
    }
    if (snprintf(to_return, (len+1), "%s%s%s", pre, new, next) < 0) {
      free(pre);
      free(next);
      free(to_return);
      return NULL;
    }
    free(pre);
    free(next);
    return to_return;
  }
}

/**
 * char * msprintf(const char * message, ...)
 * Implementation of sprintf that return a malloc'd char * with the string construction
 * because life is too short to use 3 lines instead of 1
 * but don't forget to free the returned value after use!
 */
char * msprintf(const char * message, ...) {
  va_list argp, argp_cpy;
  size_t out_len = 0;
  char * out = NULL;
  va_start(argp, message);
  va_copy(argp_cpy, argp); // We make a copy because in some architectures, vsnprintf can modify argp
  out_len = vsnprintf(NULL, 0, message, argp);
  out = malloc(out_len+sizeof(char));
  if (out == NULL) {
    return NULL;
  }
  vsnprintf(out, (out_len+sizeof(char)), message, argp_cpy);
  va_end(argp);
  va_end(argp_cpy);
  return out;
}

/**
 * nstrdup
 * a modified strdup function that don't crash when source is NULL, instead return NULL
 * Returned value must be free'd after use
 */
char * nstrdup(const char * source) {
  return (source==NULL?NULL:strdup(source));
}

/**
 * json_t * json_search(json_t * haystack, json_t * needle)
 * jansson library addon
 * This function could be removed if y pull request is accepted in jansson official repository:
 * https://github.com/akheron/jansson/pull/265
 * Look for an occurence of needle within haystack
 * If needle is present in haystack, return the reference to the json_t * that is equal to needle
 * If needle is not found, return NULL
 */
json_t * json_search(json_t * haystack, json_t * needle) {
    json_t * value1, * value2;
    size_t index;
    const char * key;
    
    if (!haystack || !needle)
        return NULL;
    
    if (haystack == needle)
        return haystack;
    
    // If both haystack and needle are the same type, test them
    if (json_typeof(haystack) == json_typeof(needle))
        if (json_equal(haystack, needle))
            return haystack;
    
    // If they are not equals, test json_search in haystack elements recursively if it's an array or an object
    if (json_is_array(haystack)) {
        json_array_foreach(haystack, index, value1) {
            if (json_equal(value1, needle)) {
                return value1;
            } else {
                value2 = json_search(value1, needle);
                if (value2 != NULL) {
                    return value2;
                }
            }
        }
    } else if (json_is_object(haystack)) {
        json_object_foreach(haystack, key, value1) {
            if (json_equal(value1, needle)) {
                return value1;
            } else {
                value2 = json_search(value1, needle);
                if (value2 != NULL) {
                    return value2;
                }
            }
        }
    }
    return NULL;
}
