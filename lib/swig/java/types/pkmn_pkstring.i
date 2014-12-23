/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

namespace pkmn {

%{
    #include <utility>
    #include <vector>
    #include "pkmn/types/pkstring.hpp"

    namespace pkmn
    {
        typedef std::pair<pkmn::pkstring, pkmn::pkstring> pkstring_pair_t;
        typedef std::vector<pkmn::pkstring>               pkstring_vector_t;
    }
%}

typedef std::pair<pkmn::pkstring, pkmn::pkstring> pkstring_pair_t;
typedef std::vector<pkmn::pkstring>               pkstring_vector_t;

%naturalvar pkstring;
class pkstring;

// pkstring
%typemap(jni) pkstring "jstring"
%typemap(jtype) pkstring "String"
%typemap(jstype) pkstring "String"
%typemap(javadirectorin) pkstring "$jniinput"
%typemap(javadirectorout) pkstring "$javacall"

%typemap(in) pkstring
%{if(!$input) {
    SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "null pkmn::pkstring");
    return $null;
  }
  const jchar *$1_pstr = jenv->GetStringChars($input, 0);
  std::wstring $1_wstr;

  if (!$1_pstr) return $null;
  jsize $1_len = jenv->GetStringLength($input);
  if ($1_len) {
    $1_wstr.reserve($1_len);
    for (jsize i = 0; i < $1_len; ++i) {
      $1_wstr.push_back((wchar_t)$1_pstr[i]);
    }
  }
  $1 = pkmn::pkstring($1_wstr);

  jenv->ReleaseStringChars($input, $1_pstr);
 %}

%typemap(directorout) pkstring
%{if(!$input) {
    SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "null pkmn::pkstring");
    return $null;
  }
  const jchar *$1_pstr = jenv->GetStringChars($input, 0);
  std::wstring $1_wstr;

  if (!$1_pstr) return $null;
  jsize $1_len = jenv->GetStringLength($input);
  if ($1_len) {
    $1_wstr.reserve($1_len);
    for (jsize i = 0; i < $1_len; ++i) {
      $1_wstr.push_back((wchar_t)$1_pstr[i]);
    }
  }
  $1 = pkmn::pkstring($1_wstr);

  jenv->ReleaseStringChars($input, $1_pstr);
 %}

%typemap(directorin,descriptor="Ljava/lang/String;") pkstring {
  jsize $1_len = $1.length();
  jchar *conv_buf = new jchar[$1_len];
  std::wstring $1_wstr = $1.std_wstring();

  for (jsize i = 0; i < $1_len; ++i) {
    conv_buf[i] = (jchar)$1_wstr[i];
  }
  $input = jenv->NewString(conv_buf, $1_len);

  delete [] conv_buf;
}

%typemap(out) pkstring
%{jsize $1_len = $1.length();
  jchar *conv_buf = new jchar[$1_len];
  std::wstring $1_wstr = $1.std_wstring();

  for (jsize i = 0; i < $1_len; ++i) {
    conv_buf[i] = (jchar)$1_wstr[i];
  }
  $result = jenv->NewString(conv_buf, $1_len);

  delete [] conv_buf; %}

%typemap(javain) pkstring "$javainput"

%typemap(javaout) pkstring {
    return $jnicall;
  }

// const pkstring &
%typemap(jni)  const pkstring & "jstring"
%typemap(jtype)  const pkstring & "String"
%typemap(jstype)  const pkstring & "String"
%typemap(javadirectorin)  const pkstring & "$jniinput"
%typemap(javadirectorout)  const pkstring & "$javacall"

%typemap(in) const pkstring &
%{if(!$input) {
    SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "null pkmn::pkstring");
    return $null;
  }
  const jchar *$1_pstr = jenv->GetStringChars($input, 0);
  std::wstring $1_wstr;

  if (!$1_pstr) return $null;
  jsize $1_len = jenv->GetStringLength($input);
  if ($1_len) {
    $1_wstr.reserve($1_len);
    for (jsize i = 0; i < $1_len; ++i) {
      $1_wstr.push_back((wchar_t)$1_pstr[i]);
    }
  }
  pkmn::pkstring $1_pkstr($1_wstr);
  $1 = new pkmn::pkstring($1_pkstr);

  jenv->ReleaseStringChars($input, $1_pstr);
 %}

%typemap(directorout,warning=SWIGWARN_TYPEMAP_THREAD_UNSAFE_MSG) const pkstring & 
%{if(!$input) {
    SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "null pkmn::pkstring");
    return $null;
  }
  const jchar *$1_pstr = jenv->GetStringChars($input, 0);
  static std::wstring $1_wstr;

  if (!$1_pstr) return $null;
  jsize $1_len = jenv->GetStringLength($input);
  /* possible thread/reentrant code problem */
  if ($1_len) {
    $1_wstr.reserve($1_len);
    for (jsize i = 0; i < $1_len; ++i) {
      $1_wstr.push_back((wchar_t)$1_pstr[i]);
    }
  }
  pkmn::pkstring $1_pkstr($1_wstr);
  $1 = new pkmn::pkstring($1_pkstr);

  jenv->ReleaseStringChars($input, $1_pstr); %}

%typemap(directorin,descriptor="Ljava/lang/String;") const pkstring & {
  jsize $1_len = $1.length();
  jchar *conv_buf = new jchar[$1_len];
  std::wstring $1_wstr = $1.std_wstring();

  for (jsize i = 0; i < $1_len; ++i) {
    conv_buf[i] = (jchar)$1_wstr[i];
  }
  $input = jenv->NewString(conv_buf, $1_len);

  delete [] conv_buf;
}

%typemap(out) const pkstring & 
%{jsize $1_len = $1->length();
  jchar *conv_buf = new jchar[$1_len];
  std::wstring $1_wstr = $1->std_wstring();

  for (jsize i = 0; i < $1_len; ++i) {
    conv_buf[i] = (jchar)$1_wstr[i];
  }
  $result = jenv->NewString(conv_buf, $1_len);

  delete [] conv_buf; %}

%typemap(javain)  const pkstring & "$javainput"

%typemap(javaout)  const pkstring & {
    return $jnicall;
  }


%typemap(throws) const pkstring &
%{ std::string message($1.begin(), $1.end());
   SWIG_JavaThrowException(jenv, SWIG_JavaRuntimeException, message.c_str());
   return $null; %}

}

