/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include "stdint.i"

%insert(runtime) %{
    #include <cstdint>
    typedef uint16_t* (SWIGSTDCALL* LibPKMN_CSharpPKStringHelperCallback)(const uint16_t*);
    static LibPKMN_CSharpPKStringHelperCallback LibPKMN_csharp_pkstring_callback = NULL;
%}

%pragma(csharp) imclasscode=%{
    protected class LibPKMNPKStringHelper {

        public delegate string LibPKMNPKStringDelegate(IntPtr message);
        static LibPKMNPKStringDelegate pkstringDelegate = new LibPKMNPKStringDelegate(CreatePKString);

        [DllImport("$dllimport", EntryPoint="LibPKMNRegisterPKStringCallback_$module")]
        public static extern void LibPKMNRegisterPKStringCallback_$module(LibPKMNPKStringDelegate pkstringDelegate);

        static string CreatePKString([MarshalAs(UnmanagedType.LPWStr)]IntPtr cString) {
            return System.Runtime.InteropServices.Marshal.PtrToStringUni(cString);
        }

        static LibPKMNPKStringHelper() {
            LibPKMNRegisterPKStringCallback_$module(pkstringDelegate);
        }
    }

    static protected LibPKMNPKStringHelper libpkmnPKStringHelper = new LibPKMNPKStringHelper();
%}

%insert(runtime) %{
    #ifdef __cplusplus
    extern "C"
    {
    #endif
    SWIGEXPORT void SWIGSTDCALL LibPKMNRegisterPKStringCallback_$module(LibPKMN_CSharpPKStringHelperCallback callback) {
        LibPKMN_csharp_pkstring_callback = callback;
    }
    #ifdef __cplusplus
    }
    #endif
%}

namespace pkmn {

%naturalvar pkstring;

class pkstring;

%typemap(ctype, out="void *") pkstring "uint16_t*"
%typemap(imtype, inattributes="[MarshalAs(UnmanagedType.LPWStr)]") pkstring "string"
%typemap(cstype) pkstring "string"
%typemap(csdirectorin) pkstring "$iminput"
%typemap(csdirectorout) pkstring "$cscall"

%typemap(in, canthrow=1) pkstring
%{
    if(!$input)
    {
        SWIG_CSharpSetPendingExceptionArgument(SWIG_CSharpArgumentNullException, "null pkstring", 0);
        return $null;
    }
    $1.set($input);
%}
%typemap(out) pkstring %{ $result = LibPKMN_csharp_pkstring_callback($1); %}

%typemap(directorout, canthrow=1) pkstring
%{
    if(!$input)
    {
        SWIG_CSharpSetPendingExceptionArgument(SWIG_CSharpArgumentNullException, "null pkstring", 0);
        return $null;
    }   
   $result.set($input);
%}

%typemap(directorin) pkstring %{ $input = LibPKMN_csharp_pkstring_callback($1); %}

%typemap(csin) pkstring "$csinput"
%typemap(csout, excode=SWIGEXCODE) pkstring
{
    string ret = $imcall;$excode
    return ret;
}

%typemap(throws, canthrow=1) pkstring
%{
    std::string message($1.begin(), $1.end());
    SWIG_CSharpSetPendingException(SWIG_CSharpApplicationException, message.c_str());
    return $null;
%}

// const pkstring &
%typemap(ctype, out="void *") const pkstring & "uint16_t*"
%typemap(imtype, inattributes="[MarshalAs(UnmanagedType.LPWStr)]") const pkstring & "string"
%typemap(cstype) const pkstring & "string"
%typemap(csdirectorin) const pkstring & "$iminput"
%typemap(csdirectorout) const pkstring & "$cscall"

%typemap(in, canthrow=1) const pkstring &
%{
    if (!$input)
    {
        SWIG_CSharpSetPendingExceptionArgument(SWIG_CSharpArgumentNullException, "null pkstring", 0);
        return $null;
    }
    pkmn::pkstring $1_str($input);
    $1 = &$1_str;
%}

%typemap(out) const pkstring & %{ $result = LibPKMN_csharp_pkstring_callback(*$1); %}

%typemap(directorout, canthrow=1) const pkstring &
%{
    if(!$input)
    {
        SWIG_CSharpSetPendingExceptionArgument(SWIG_CSharpArgumentNullException, "null pkstring", 0);
        return $null;
    }   
   $result.set($input);
%}

%typemap(directorin) const pkstring & %{ $input = LibPKMN_csharp_pkstring_callback(*$1); %}

%typemap(csin) const pkstring & "$csinput"
%typemap(csout, excode=SWIGEXCODE) const pkstring &
{
    string ret = $imcall;$excode
    return ret;
}

%typemap(throws, canthrow=1) const pkstring &
%{
    std::string message($1->begin(), $1->end());
    SWIG_CSharpSetPendingException(SWIG_CSharpApplicationException, message.c_str());
    return $null;
%}

}
