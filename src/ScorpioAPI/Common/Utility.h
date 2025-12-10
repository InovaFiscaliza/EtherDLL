/**************************************************************************
*                           CONFIDENTIAL                                  *
* Unauthorized access to, copying, use of or disclosure of this software, *
* or any of its features, is strictly prohibited. Your access to or       *
* possession of a copy of this software is persuant to a limited license; *
* ownership of the software and any associated media remains with TCI.    *
*                                                                         *
* Copyright 2006-2021 TCI International, Inc. All rights reserved		  *
**************************************************************************/

#pragma once

#include <comdef.h>
#include <iomanip>
#include <memory>
#include <sstream>

#include "EquipCtrlMsg.h"
#include "ErrorCodes.h"

#pragma pointers_to_members(full_generality, virtual_inheritance)

// Fatal Errors
#define THROW_LOGIC_ERROR() do { ASSERT(FALSE); throw std::logic_error(static_cast<std::ostringstream&>(std::ostringstream() << std::flush << __FILE__ << ":" << __LINE__).str()); } while(false)
#define TEST_LOGIC_ERROR(expr) do { if(!(expr)) THROW_LOGIC_ERROR(); } while(false)

namespace Utility
{
	enum EBoardType { C_UNKNOWN_BOARD, C_GE_BOARD, C_CURTISSWRIGHT_BOARD, C_GMS_BOARD };  // Board Type supported.

	// Synchronous DeviceIoControl on device openned for overlapped operation
	_Success_(return) bool DeviceIoControlSync(HANDLE device, DWORD ioControlCode, _In_opt_bytecount_(inBufferSize) LPVOID inBuffer, DWORD inBufferSize,
		_Out_opt_bytecap_(outBufferSize) LPVOID outBuffer, DWORD outBufferSize, _Out_ DWORD& bytesReturned);

	// Disable Windows Plug-and-Play COM port.
	void DisableComPlugNPlay(_In_ unsigned char compassPort);

	std::string exec(const char* cmd);
	EBoardType GetBoardType(void);

	// Extended error message for CFileException
	void GetErrorMessage(_In_ const CFileException* ex, _Out_ CString& errorMessage);

	// Define our own 32-bit GetTickCount so that #pragma warning(suppress: 28159)
    // will only be done at 1 single spot.
    //@note: This method has the same limit of the count rolling over every 49
    //       days. It is expected that the caller has taken into account this
    //       limitation in its code.
#pragma warning(suppress: 28159)
	inline DWORD GetXpTickCount(void) { return(GetTickCount()); }

    // Thread helpers
	void SetThreadName(_In_ LPCSTR name);

	template<typename T, typename U = void> struct SThreadWrapperArg
	{
		SThreadWrapperArg(T* instance, void (T::*Thread)(U), U threadArg, void (T::*OnFail)(DWORD, LPCTSTR), const CStringA& threadName = typeid(T).name())
			: Thread(Thread), threadArg(threadArg), OnFail(OnFail), threadName(threadName), instance(instance) {}

		static UINT AFX_CDECL ThreadWrapper(void* arg)
		{
			SThreadWrapperArg* wrapperArg = static_cast<SThreadWrapperArg*>(arg);
			SetThreadName(wrapperArg->threadName);
			DWORD error = ERROR_SUCCESS;
			CString description;

			// Run the thread function
			try
			{
				((wrapperArg->instance)->*(wrapperArg->Thread))(wrapperArg->threadArg);
				delete wrapperArg;
				return 0;
			}
			// Catch all the possible fatal exceptions in as much detail as possible
			catch(ErrorCodes::EErrorCode errorCode)
			{
				description.Format(_T("Internal error code %ld"), errorCode);
				error = ERROR_PROCESS_ABORTED;
			}
			catch(CException* ex)
			{
				static const int MAX_ERROR = 1024;
				LPTSTR buffer = description.GetBuffer(MAX_ERROR);
				CFileException* fileEx = dynamic_cast<CFileException*>(ex);

				if(fileEx != nullptr && fileEx->m_cause == CFileException::genericException)
				{
					FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
						nullptr, fileEx->m_lOsError, 0, buffer, MAX_ERROR, nullptr);

					if(!fileEx->m_strFileName.IsEmpty())
					{
						CString message;
						message.Format(_T("An error occurred accessing %s: %s"), LPCTSTR(fileEx->m_strFileName), buffer);
						_tcsncpy_s(buffer, MAX_ERROR, message, _TRUNCATE);
					}

					error = fileEx->m_lOsError;
				}
				else
				{
					ex->GetErrorMessage(buffer, MAX_ERROR);
					error = ERROR_PROCESS_ABORTED;
				}

				description.ReleaseBuffer();
				ex->Delete();
			}
			catch(const std::exception& ex)
			{
				description = ex.what();
				error = ERROR_PROCESS_ABORTED;
			}
			catch(const _com_error& ex)
			{
				description = ex.ErrorMessage();
				error = ERROR_PROCESS_ABORTED;
			}
			catch(...)
			{	
				description = _T("An unexpected error occured");
				error = ERROR_PROCESS_ABORTED;
			}

			((wrapperArg->instance)->*(wrapperArg->OnFail))(error, description + _T(" in thread ") + CString(wrapperArg->threadName) +
				_T(" in ") + CA2T(typeid(T).name()));
			delete wrapperArg;

			return error;
		};

		void (T::*Thread)(U);
		void (T::*OnFail)(DWORD error, LPCTSTR description);
		CStringA threadName;
		T* instance;
		U threadArg;
	};
	
	template<typename T> struct SThreadWrapperArg<T, void>
	{
		SThreadWrapperArg(T* instance, void (T::*Thread)(void), void (T::*OnFail)(DWORD, LPCTSTR), const CStringA& threadName = typeid(T).name())
			: Thread(Thread), OnFail(OnFail), threadName(threadName), instance(instance) {}

		static UINT AFX_CDECL ThreadWrapper(void* arg)
		{
			SThreadWrapperArg* wrapperArg = static_cast<SThreadWrapperArg*>(arg);
			SetThreadName(wrapperArg->threadName);
			DWORD error = ERROR_SUCCESS;
			CString description;

			// Run the thread function
			try
			{
				((wrapperArg->instance)->*(wrapperArg->Thread))();
				delete wrapperArg;
				return 0;
			}
			// Catch all the possible fatal exceptions in as much detail as possible
			catch(ErrorCodes::EErrorCode errorCode)
			{
				description.Format(_T("Internal error code %u"), errorCode);
				error = ERROR_PROCESS_ABORTED;
			}
			catch(CException* ex)
			{
				static const int MAX_ERROR = 1024;
				LPTSTR buffer = description.GetBuffer(MAX_ERROR);
				CFileException* fileEx = dynamic_cast<CFileException*>(ex);

				if(fileEx != nullptr && fileEx->m_cause == CFileException::genericException)
				{
					FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
						nullptr, fileEx->m_lOsError, 0, buffer, MAX_ERROR, nullptr);

					if(!fileEx->m_strFileName.IsEmpty())
					{
						CString message;
						message.Format(_T("An error occurred accessing %s: %s"), LPCTSTR(fileEx->m_strFileName), buffer);
						_tcsncpy_s(buffer, MAX_ERROR, message, _TRUNCATE);
					}

					error = fileEx->m_lOsError;
				}
				else
				{
					ex->GetErrorMessage(buffer, MAX_ERROR);
					error = ERROR_PROCESS_ABORTED;
				}

				description.ReleaseBuffer();
				ex->Delete();
			}
			catch(const std::exception& ex)
			{
				description = ex.what();
				error = ERROR_PROCESS_ABORTED;
			}
			catch(const _com_error& ex)
			{
				description = ex.ErrorMessage();
				error = ERROR_PROCESS_ABORTED;
			}
			catch(...)
			{	
				description = _T("An unexpected error occured");
				error = ERROR_PROCESS_ABORTED;
			}

			((wrapperArg->instance)->*(wrapperArg->OnFail))(error, description + _T(" in thread ") + CString(wrapperArg->threadName) +
				_T(" in ") + CA2T(typeid(T).name()));
			delete wrapperArg;

			return error;
		};

		void (T::*Thread)(void);
		void (T::*OnFail)(DWORD error, LPCTSTR description);
		CStringA threadName;
		T* instance;
	};

	template<typename T> UINT AFX_CDECL ThreadWrapper(void* arg)
	{
		SThreadWrapperArg<T>* wrapperArg = static_cast<SThreadWrapperArg<T>*>(arg);
		SetThreadName(wrapperArg->threadName);
		DWORD error = ERROR_SUCCESS;
		CString description;

		// Run the thread function
		try
		{
			((wrapperArg->instance)->*(wrapperArg->Thread))();
			delete wrapperArg;
			return 0;
		}
		// Catch all the possible fatal exceptions in as much detail as possible
		catch(ErrorCodes::EErrorCode errorCode)
		{
			description.Format(_T("Internal error code %ld"), errorCode);
			error = ERROR_PROCESS_ABORTED;
		}
		catch(CException* ex)
		{
			static const int MAX_ERROR = 1024;
			LPTSTR buffer = description.GetBuffer(MAX_ERROR);
			CFileException* fileEx = dynamic_cast<CFileException*>(ex);

			if(fileEx != nullptr && fileEx->m_cause == CFileException::genericException)
			{
				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
					nullptr, fileEx->m_lOsError, 0, buffer, MAX_ERROR, nullptr);

				if(!fileEx->m_strFileName.IsEmpty())
				{
					CString message;
					message.Format(_T("An error occurred accessing %s: %s"), LPCTSTR(fileEx->m_strFileName), buffer);
					_tcsncpy_s(buffer, MAX_ERROR, message, _TRUNCATE);
				}

				error = fileEx->m_lOsError;
			}
			else
			{
				ex->GetErrorMessage(buffer, MAX_ERROR);
				error = ERROR_PROCESS_ABORTED;
			}

			description.ReleaseBuffer();
			ex->Delete();
		}
		catch(const std::exception& ex)
		{
			description = ex.what();
			error = ERROR_PROCESS_ABORTED;
		}
		catch(const _com_error& ex)
		{
			description = ex.ErrorMessage();
			error = ERROR_PROCESS_ABORTED;
		}
		catch(...)
		{	
			description = _T("An unexpected error occured");
			error = ERROR_PROCESS_ABORTED;
		}

		((wrapperArg->instance)->*(wrapperArg->OnFail))(error, description + _T(" in thread ") + CString(wrapperArg->threadName) +
			_T(" in ") + CA2T(typeid(T).name()));
		delete wrapperArg;

		return error;
	};
};

class CLocalizedString
:
	public CStringW
{
public:
	CLocalizedString(UINT id, WORD langId)
	:
		CStringW()
	{
#pragma warning(suppress : 6309) // Arg 1 CAN be nullptr
#pragma warning(suppress : 6387) // Arg 1 CAN be nullptr
		if(!LoadStringW(nullptr, id, langId))
		{
#pragma warning(suppress : 6309) // Arg 1 CAN be nullptr
#pragma warning(suppress : 6387) // Arg 1 CAN be nullptr
			if(!LoadStringW(nullptr, id, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL)))
			{
				SetString(_T("Bad resource"));
			}
		}

		return;
	}
};