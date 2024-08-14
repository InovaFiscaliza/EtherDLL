#include "stdafx.h"
#include "ScorpioAPIDll.h"

std::string ERetCodeToString(ERetCode code)
{
    switch (code)
    {
        case API_SUCCESS: return "API success";
        case API_NO_INSTANCE: return "No API instance";
        case CMD_SENT_ERROR: return "Command sent error";
        case INVALID_REQUEST_ID: return "Invalid request ID";
        case INVALID_SERVER_ID: return "Invalid server ID";
        case UNABLE_TO_RESOLVE_HOST: return "Unable to resolve host";
        case PARM_MEMORY_UNINITIALIZED: return "Memory parameter uninitialized";
        case NO_VALID_API_LICENSE: return "No valid API license";
        case API_CONNECTONS_EXCEEDED: return "Maximal number of API connections exceeded";
        case MEMORY_ALLOC_ERROR: return "Memory allocation error";
        default: return "Unknown error code";
    }
}