#ifndef ETHERDLL_CONFIG_H
#define ETHERDLL_CONFIG_H

// ----------------------------------------------------------------------
/*
	Conditional library linking for ScorpioAPI based on build configuration
*/
#ifdef _X86_
	#pragma comment (lib, "ScorpioAPIDll.lib") //RELEASE/DEBUG 32Bits
#else
	#ifdef NDEBUG
		#pragma comment (lib, "ScorpioAPIDll.lib") //RELEASE 64Bits
	#else
		#pragma comment (lib, "ScorpioAPIDlld.lib") //DEBUG 64Bits
	#endif
#endif

#endif // ETHERDLL_CONFIG_H

// Include to solution specific libraries
extern nlohmann::json config;


#include <ExternalCodes.h>

#include <EtherDLLUtils.h>
#include <EtherDLLConstants.h>
#include <EtherDLLProcessApiResponse.h>
#include <EtherDLLConfig.h>

#include <EtherDLLAudio.h>

/*
#include "StdAfx.h"
#include <sstream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "ScorpioAPITypes.h"

#include <EtherDLLLog.h>
*/


// Defined in scorpioDLLRequest
void newDefaultConfigFile(const std::string& filename);
