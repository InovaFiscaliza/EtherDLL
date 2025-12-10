#pragma once

#pragma pack(push, 8) // get a definite structure alignment

namespace RohdeSchwarz
{
namespace ViCom
{

/**
ViCom data processors are listeners which can be registered at an ViCom interface
to retrieve different kind of data.
*/
class CViComDataProcessor
{
public:
   virtual ~CViComDataProcessor() {};

   /**
   Called when the data processor is registered to a scanner.

   @param[in] dwScannerDataId
      internal address of the scanner module
   */
   virtual void RegisterScannerId( WORD dwScannerDataId ) = 0;
   
   /**
   Called when the data processor is unregistered from a scanner.

   @param[in] dwScannerDataId
      internal address of the scanner module
   */
   virtual void RemoveScannerId( WORD dwScannerDataId ) = 0;
};


/**
This interface can be used to register at a data generating ViCom interface to
collect the raw data originating from the underlying device.

@note
   Only internal ViCom components will provide adequate implementations of this
   this interface, since they need to now the internal data format used in the
   scanners.

   You can still extend this class and handle the binary block that is given
   in the OnScannerDataMeasured() method as data chunk.
*/
class CViComRawDataProcessor : public CViComDataProcessor
{
public:
   typedef unsigned char* RawScannerData;

   // Virtual destructor must always be declared in interface classes
   virtual ~CViComRawDataProcessor() {}

   /**
   Every time raw data is available, it is reported to the raw data processor
   using this function. The raw data is given as byte array.

   @note
      This is called during the reception of the internal data structure, so
      this MUST process data very fast or delay processing into another thread
      to make sure the calling thread is not blocked for too long. This implies
      that this method may be called from different threads on the same object.
      Make sure to put synchronization mechanisms into the implementation to
      avoid race-conditions. The callback is not allowed to throw exceptions!

   @param [in] pRawData
         the internal data structure that carries the raw data.

   @param[in] dwLength
         the length in bytes of the measured data block
   */
   virtual void OnScannerDataMeasured( 
      const RawScannerData pRawData, 
      DWORD dwLength ) = 0;
};


/**
This interface can be used to register at a data generating ViCom interface to
collect the result data ViCom generates from the raw device data.
*/
template <typename RESULT_TYPE>
class CViComResultDataProcessor: public CViComDataProcessor
{
public:
   typedef typename RESULT_TYPE* ScannerData;

   // Virtual destructor must always be declared in interface classes
   virtual ~CViComResultDataProcessor() {}


   /**
   Every time data is available, it is reported to the data listener
   using this function.

   @note
      This is called during the reception of the internal data structure, so
      this MUST process data very fast or delay processing into another thread
      to make sure the calling thread is not blocked for too long. This implies
      that this method may be called from different threads on the same object.
      Make sure to put synchronization mechanisms into the implementation to
      avoid race-conditions. The callback is not allowed to throw exceptions!

   @param [in] Data
         the interface specific data structure that carries the result data.
   */
   virtual void OnScannerDataMeasured( const ScannerData pData ) = 0;
};

} // namespace ViCom
} // namespace RohdeSchwarz

#pragma pack(pop) // reset your structure alignment
