#ifndef __GENERIC_H__
#define __GENERIC_H__

#include "mainwindowimpl.h"

/*! Checks if the string is a valid output string
    \param str The string we wish to check
    \result 1 if the string is fully valid, 0 if not */
char strIsValidOutputStr(QString str);

/*! Checks if the string contains anything else than a number
    \param str The string we wish to check
    \result 1 if the string is fully numeric, 0 if not */
char strIsNumeric(QString str);

#endif // __GENERIC_H__
