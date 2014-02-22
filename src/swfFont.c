#include "swfFont.h"

static SEXP GetVarFromPkgEnv(const char *varName, const char *pkgName)
{
    /* See grDevices/src/devPS getFontDB() */
    SEXP pkgNS, pkgEnv, var;
    PROTECT(pkgNS = R_FindNamespace(ScalarString(mkChar(pkgName))));
    PROTECT(pkgEnv = Rf_findVar(install(".pkg.env"), pkgNS));
    if(TYPEOF(pkgEnv) == PROMSXP) {
        PROTECT(pkgEnv);
        pkgEnv = eval(pkgEnv, pkgNS);
        UNPROTECT(1);
    }  
    PROTECT(var = Rf_findVar(install(varName), pkgEnv));
    UNPROTECT(3);
    
    return var;
}

FT_Face swfGetFTFace(const pGEcontext gc)
{
    int gcfontface = gc->fontface;
    FontDesc *font;
    
    SEXP fontList;
    SEXP fontNames;
    SEXP extPtr;
    int i, listLen;
    
    /* Font list is sysfonts:::.pkg.env$.font.list,
       defined in sysfonts/R/font.R */    
    fontList = GetVarFromPkgEnv(".font.list", "sysfonts");
    
    /* Search the given family name */
    fontNames = GET_NAMES(fontList);
    listLen = Rf_length(fontList);
    for(i = 0; i < listLen; i++)
    {
        if(strcmp(gc->fontfamily, CHAR(STRING_ELT(fontNames, i))) == 0)
        {
            break;
        }
    }
    /* If not found, use "sans" */
    if(i == listLen) i = 0;
    if(gcfontface < 1 || gcfontface > 5) gcfontface = 1;
    
    extPtr = VECTOR_ELT(VECTOR_ELT(fontList, i), gcfontface - 1);
    font = (FontDesc *) R_ExternalPtrAddr(extPtr);
    
    return font->face;
}

