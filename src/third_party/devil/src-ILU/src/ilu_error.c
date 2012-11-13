//-----------------------------------------------------------------------------
//
// ImageLib Utility Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 03/03/2009
//
// Filename: src-ILU/src/ilu_error.c
//
// Description: Error functions
//
//-----------------------------------------------------------------------------


#include "ilu_internal.h"
#include "ilu_error/ilu_err-english.h"


ILconst_string *iluErrors;
ILconst_string *iluLibErrors;
ILconst_string *iluMiscErrors;
#define ILU_NUM_LANGUAGES 1

ILconst_string *iluErrorStrings[ILU_NUM_LANGUAGES] = {
	iluErrorStringsEnglish,
};

ILconst_string *iluLibErrorStrings[ILU_NUM_LANGUAGES] = {
	iluLibErrorStringsEnglish,
};

ILconst_string *iluMiscErrorStrings[ILU_NUM_LANGUAGES] = {
	iluMiscErrorStringsEnglish,
};


ILconst_string ILAPIENTRY iluErrorString(ILenum Error)
{
	// Now we are dealing with Unicode strings.
	if (Error == IL_NO_ERROR) {
		return iluMiscErrors[0];
	}
	if (Error == IL_UNKNOWN_ERROR) {
		return iluMiscErrors[1];
	}
	if (Error >= IL_INVALID_ENUM && Error <= IL_FILE_READ_ERROR) {
		return (ILstring)iluErrors[Error - IL_INVALID_ENUM];
	}
	if (Error >= IL_LIB_GIF_ERROR && Error <= IL_LIB_EXR_ERROR) {
		return (ILstring)iluLibErrors[Error - IL_LIB_GIF_ERROR];
	}

	return iluMiscErrors[0];
}


ILboolean ILAPIENTRY iluSetLanguage(ILenum Language)
{
	switch (Language)
	{
		case ILU_ENGLISH:
			iluErrors = iluErrorStrings[ILU_ENGLISH];
			iluLibErrors = iluLibErrorStrings[ILU_ENGLISH];
			iluMiscErrors = iluMiscErrorStrings[ILU_ENGLISH];
			break;

		default:
			ilSetError(IL_INVALID_ENUM);
			return IL_FALSE;
	}

	return IL_TRUE;
}

 	  	 
