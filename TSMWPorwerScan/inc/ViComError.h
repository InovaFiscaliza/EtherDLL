// class CViComError

// Explanation:
// Error value which contains an unique error code in the context of ViCom as well as an error text.
#pragma once

#include "ViComBasicErrors.h"

#pragma pack(push, 8) // get a definite structure alignment for CViComError

namespace RohdeSchwarz
{
namespace ViCom
{

/**
The following functions can be used to get error information from an object of class CViComError.
A full list of general ViCom errors can be found in the header file ViComBasicErrors.h, and a list of specific
errors are in ViCom***Errors.h.
*/
class CViComError
{
public:
   static const size_t si_textArraySize = 240;
public:
   //! construction
   CViComError();

   /**
   * Convenience constructor to directly create an error object with all information possible.
   * @param dwErrorCode
   *     [in] error specific code (can be used to look-up error string in error definition header).
   * @param pszErrorString
   *     [in] descriptive string. Should be the same as defined in the error header for the error code.
   */
   CViComError(DWORD dwErrorCode, const char *pszErrorString);

   /**
   * Convenience constructor to directly create an error object with all information possible.
   * @param dwErrorCode
   *     [in] error specific code (can be used to look-up error string in error definition header).
   * @param pszErrorString
   *     [in] descriptive string. Should be the same as defined in the error header for the error code.
   * @param pszUserString
   *     [in] descriptive string added by the user, to provide more information
   */
   CViComError(DWORD dwErrorCode, const char *pszErrorString, const char *pszUserString);

public:  // methods
   //** Errorcode **/
   /**
   This function returns an error code which is unique for the ViCom interface. Error code definitions are declared in
   ViComBasicErrors.h and the error code file for the specific application (e.g. ViComWcdmaErrors.h)

   @return an unique error code. If no error occurred the function returns 0.
   */
   DWORD                GetErrorCode() const;

   //** Error Message **/
   /**
   This function returns the specific R&S error message. You can use this message during development or you may display your
   own message for certain error codes.

   @return a pointer to a string containing an R&S error message, or a standard error message for a specific ViCom interface
   (for example, a 3GPP error message).
   */
   const char           *GetErrorString() const;

private: // members
   DWORD                m_dwErrorCode;
   char                 m_acErrorString[si_textArraySize];
};

// Full inline implementation for convenient integration in your application software.

inline CViComError::CViComError() : m_dwErrorCode(VICEC_NO_ERROR)
{
   strcpy_s(m_acErrorString, "");
}

inline CViComError::CViComError(DWORD dwErrorCode, const char *pszErrorString) : m_dwErrorCode(dwErrorCode)
{
   strcpy_s (m_acErrorString, si_textArraySize, pszErrorString);
}

inline CViComError::CViComError(DWORD dwErrorCode, const char *pszErrorString, const char *pszUserString) : m_dwErrorCode(dwErrorCode)
{
      strcpy_s (m_acErrorString, si_textArraySize, pszErrorString);
      strcat_s (m_acErrorString, si_textArraySize, " - ");
      strcat_s (m_acErrorString, si_textArraySize, pszUserString);
}

inline DWORD CViComError::GetErrorCode() const
{
   return m_dwErrorCode;
}

inline const char *CViComError::GetErrorString() const
{
   return m_acErrorString;
}

namespace ViComInternal__
{
   inline void throw_vicec (const CViComError& e) {throw e;}
}
#define VICOM_MAKE_VICEC(name)  ((CViComError(VICEC_##name, VICEC_##name##_TEXT)))
#define VICOM_THROW_VICEC(name) ((ViComInternal__::throw_vicec(VICOM_MAKE_VICEC(name))))

} // namespace ViCom
} // namespace RohdeSchwarz

#pragma pack(pop) // reset your structure alignment
