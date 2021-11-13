/**
 * Copyright (c) 2021 Mir Drualga
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <D2Unicode.h>

#include <ctype.h>

BOOL __fastcall Unicode::isWordEnd(const Unicode* str, size_t index) {
  if (index == 0) {
    return FALSE;
  }

  return ::isalnum(str[index].ch)
      && !::isalnum(str[index + 1].ch);
}

Unicode* __fastcall Unicode::strcat(Unicode* dest, const Unicode* src) {
  size_t i = 0;
  while (dest[i].ch != L'\0') {
    ++i;
  }

  Unicode::strcpy(&dest[i], src);

  return dest;
}

int __fastcall Unicode::strcmp(const Unicode* str1, const Unicode* str2) {
  /*
   * This loop does not run beyond the end of either string. If the
   * end of only one string is reached, then a comparison between '\0'
   * to a different character is made, and a return is guaranteed to
   * happen.
   */
  for (size_t i = 0; (str1[i].ch != L'\0') || (str2[i].ch != L'\0'); ++i) {
    if (str1[i].ch < str2[i].ch) {
      return -1;
    } else if (str1[i].ch > str2[i].ch) {
      return 1;
    }
  }

  return 0;
}

Unicode* __fastcall Unicode::strcpy(Unicode* dest, const Unicode* src) {
  size_t i = 0;
  do {
    dest[i].ch = src[i].ch;
  } while (src[i++].ch != L'\0');

  return dest;
}

int __fastcall Unicode::stricmp(const Unicode* str1, const Unicode* str2) {
  /*
   * This loop does not run beyond the end of either string. If the
   * end of only one string is reached, then a comparison between '\0'
   * to a different character is made, and a return is guaranteed to
   * happen.
   */
  for (size_t i = 0; (str1[i].ch != L'\0') || (str2[i].ch != L'\0'); ++i) {
    Unicode ch1_upper = str1[i].toUpper();
    Unicode ch2_upper = str2[i].toUpper();

    if (ch1_upper.ch < ch2_upper.ch) {
      return -1;
    } else if (ch1_upper.ch > ch2_upper.ch) {
      return 1;
    }
  }

  return 0;
}

int __fastcall Unicode::strlen(const Unicode* str) {
  if (str == NULL) {
    return 0;
  }

  int i = 0;
  while ((str++)->ch != L'\0') {
    ++i;
  }

  return i;
}

Unicode* __fastcall Unicode::strstr(
    const Unicode* str,
    const Unicode* substr) {
  if (str == NULL || substr == NULL) {
    return NULL;
  }

  size_t i_str;
  for (i_str = 0; ; ++i_str) {
    for (; str[i_str].ch != substr[0].ch; ++i_str) {
      if (str[i_str].ch == L'\0') {
        return NULL;
      }
    }

    if (str[i_str].ch == '\0') {
      return NULL;
    }

    size_t i_substr;
    for (i_substr = 0;
        str[i_str + i_substr].ch == substr[i_substr].ch;
        ++i_substr) {
      if (str[i_str + i_substr].ch == L'\0') {
        break;
      }

      if (substr[i_substr].ch == L'\0') {
        return (Unicode*)&str[i_str];
      }
    }

    if (substr[i_substr].ch == L'\0') {
      return (Unicode*)&str[i_str];
    }
  }

  return (Unicode*)&str[i_str];
}
