/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

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

%include "stdint.i"
%include "exception.i"
%include "CamelCase.i"
%import "libpkmn_cs_swig.i"

%{
    #include "pkmn/database/lists.hpp"
    #include "pkmn/database/queries.hpp"
%}

%include "pkmn/database/lists.hpp"
%include "pkmn/database/queries.hpp"
