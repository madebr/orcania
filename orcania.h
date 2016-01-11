/**
 * 
 * Orcania library
 * 
 * Different functions for different purposes but that can be shared between
 * all Angharad modules
 * 
 */

#ifndef __ORCANIA_H__
#define __ORCANIA_H__

#define ORCANIA_VERSION 0.5
/**
 * char * str_replace(const char * source, char * old, char * new)
 * replace all occurences of old by new in the string source
 * return a char * with the new value
 * return NULL on error
 * returned value must be free'd after use
 */
char * str_replace(const char * source, const char * old, const char * new);

/**
 * nstrdup
 * a modified strdup function that don't crash when source is NULL, instead return NULL
 * Returned value must be free'd after use
 */
char * nstrdup(const char * source);

/**
 * char * msprintf(const char * message, ...)
 * Implementation of sprintf that return a malloc'd char * with the string construction
 * because life is too short to use 3 lines instead of 1
 * but don't forget to free the returned value after use!
 */
char * msprintf(const char * message, ...);

/**
 * json_t * json_search(json_t * haystack, json_t * needle)
 * jansson library addon
 * This function could be removed if y pull request is accepted in jansson official repository:
 * https://github.com/akheron/jansson/pull/265
 * Look for an occurence of needle within haystack
 * If needle is present in haystack, return the reference to the json_t * that is equal to needle
 * If needle is not found, return NULL
 */
json_t * json_search(json_t * haystack, json_t * needle);

#endif
