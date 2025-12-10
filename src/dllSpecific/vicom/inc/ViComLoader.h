// class template CViComLoader
#pragma once
#include <memory>
#include "ViComBasicInterfaceData.h"
#include "ViComError.h"
#pragma pack(push, 8) // get a definite structure alignment



namespace RohdeSchwarz
{
namespace ViCom
{

/**
Helper class to load dynamic libraries. Used in CViComLoader.
*/
class LibLoad
{
   HINSTANCE const h;
   LibLoad( const LibLoad& );
   void operator=( const LibLoad& );
public:
   explicit LibLoad( const char* s )
      : h( ::LoadLibraryA( s ) )
   {
      if( !h )
      {
         VICOM_THROW_VICEC( INTERFACE_DLL_NOT_LOADED );
      }
   }
   ~LibLoad()
   {
      ::FreeLibrary( h );
   }
   HINSTANCE get() const
   {
      return h;
   }
};

/**
Generic interface specifying how a ViCom interface loader looks like.
*/
class CViComLoaderBase
{
public:
                   CViComLoaderBase    ()
   {}
   virtual        ~CViComLoaderBase    ()
   {}

   /**
   Loads the interface, e.g. triggers the device connection.

   As various threads have to be started and executed, the call of the load function must not be done
   during process startup and DLL initialization routines, as threads do not begin execution until DLL
   initialization is done for the process. Microsoft thread startup specification can be found at:

   @see http://msdn.microsoft.com/en-us/library/ms682453(VS.85).aspx

   After this function has returned successfully:
   - the DLLs used to run the specific ViCom interface will be loaded
   - a pointer to a ViCom interface object will be available when GetInterface() is called.

   @param[in] (optional) uri
   The network address where the device can be reached. For TSMW this can be an IPv4 address in
   dot decimal notation, e.g. 192.168.0.2.
   In case there's no address specified ViCom internal device detection mechanisms get active
   and connect the first found device.

   @param [out] rViComError
   In case of error, this is filled with the error code and an description string
   */
   virtual bool   Connect( CViComError& rViComError, const char* uri = NULL ) = 0;
   virtual void   Connect( const char* uri = NULL ) = 0;
   //!< @throw CViComError @see Connect( CViComError&, const char* )

   /**
   Check the state connection state.

   @param [out] rViComError
   In case of error, this is filled with the error code and an description string

   @return
   if the Loader has an active interface available that can be queried using GetInterface()
   or not. In other words, if a connection to the device has been established or not.
   **/
   virtual bool   Connected( CViComError& rViComError ) = 0;
   virtual bool   Connected() = 0;
   //!< @throw CViComError @see Connected( CViComError& )

   /**
   Release the interface created with this loader if one is available. Frees the
   allocated resources and unload the interface DLLs.

   @param [out] rViComError
   In case of error, this is filled with the error code and an description string
   */
   virtual bool   Disconnect( CViComError& rViComError ) = 0;
   virtual void   Disconnect( ) = 0;
   //!< @throw CViComError @see Disconnect( CViComError& )

private:
   CViComLoaderBase  (const CViComLoaderBase&);  //!< copy is not allowed
   void operator=    (const CViComLoaderBase&);  //!< copy is not allowed
};

/**
Templated base class that specifies the most important common method for all
different loaders: GetInterface().
*/
template <class APP_INTERFACE_CLASS>
class CViComLoader : public CViComLoaderBase
{
public:

   typedef APP_INTERFACE_CLASS interface_type;

private:

   /**
   Custom unique_ptr implementation that maintains ownership over a pointer and invokes
   a custom deleter when destroyed or when reset() is called.
    */
   class unique_ptr
   {
   public:

      typedef void*       _cdecl ftDestroyInterface( void* );

      unique_ptr()
         : m_pInterface( NULL )
         , m_pfDestroyInterface( NULL )
      {}
      unique_ptr( interface_type* pInterface, ftDestroyInterface* pDestroyInterface )
         : m_pInterface( pInterface )
         , m_pfDestroyInterface( pDestroyInterface )
      {}
      ~unique_ptr()
      {
         reset();
      }
      void reset()
      {
         if( m_pfDestroyInterface != NULL )
         {
            m_pfDestroyInterface( m_pInterface );
            m_pInterface = NULL;
            m_pfDestroyInterface = NULL;
         }
      }
      unique_ptr& operator=( unique_ptr&& rhs )
      {
         if( this != &rhs )
         {
            reset();
            m_pInterface = rhs.m_pInterface;
            m_pfDestroyInterface = rhs.m_pfDestroyInterface;

            rhs.m_pInterface = NULL;
            rhs.m_pfDestroyInterface = NULL;
         }

         return *this;
      };
      operator interface_type*() const
      {
         return m_pInterface;
      }

   private:
      unique_ptr(const unique_ptr&);   //!< copy is not allowed

      ftDestroyInterface*   m_pfDestroyInterface;
      interface_type*       m_pInterface;
   };

public:

   CViComLoader( Receiver::Type receiver )
      : m_loaderManager( "ViComLoaderManager.dll" )
      , m_receiverType( receiver )
   {
      m_pfCreateInterface  = reinterpret_cast<ftCreateInterface*> ( ::GetProcAddress( m_loaderManager.get(), "Create"  ) );
      m_pfDestroyInterface = reinterpret_cast<ftDestroyInterface*>( ::GetProcAddress( m_loaderManager.get(), "Destroy" ) );
   }

   //** Interaction **/
   /**
   To interact with the receiver an interface is used. This function returns the specific interface
   used to control a special application (e.g. WCDMA scanning) which is defined by the interface
   class APP_INTERFACE_CLASS. This specific interface has a function which provides a more common 
   interface (CViComBasicInterface) which is used to control rather basic function (device specific)
   than specific functions used by the selected application.

   The ViCom interface is available only after CViComLoader::Connect() has returned successfully,
   and before CViComLoader::Disconnect() has been called.

   @return
   In case of success, a pointer to a ViCom interface object of the type specified when the CViComLoader
   object was instantiated. NULL if operation failed. The parameter rViComError contains the error information.
   */
   virtual APP_INTERFACE_CLASS* GetInterface (CViComError& rViComError) { return m_pInterface; }
   virtual APP_INTERFACE_CLASS* GetInterface ()                         { return m_pInterface; }
   //!< @throw CViComError @see GetInterface( CViComError& )

   /**
   To interact with the receiver an interface is used. This function returns the specific interface
   used to control a special application (e.g. WCDMA scanning) which is defined by the interface
   class APP_INTERFACE_CLASS. This specific interface has a function which provides a more common 
   interface (CViComBasicInterface) which is used to control rather basic function (device specific)
   than specific functions used by the selected application.

   The ViCom interface is available only after CViComLoader::Connect() has returned successfully,
   and before CViComLoader::Disconnect() has been called.

   @return
   In case of success, a pointer to a ViCom interface object of the type specified when the CViComLoader
   object was instantiated. NULL if operation failed. The parameter rViComError contains the error information.
   */
   virtual Receiver::Type GetReceiverType()                           { return m_receiverType; }
   virtual Receiver::Type GetReceiverType( CViComError& rViComError ) { return m_receiverType; }
   //!< @throw CViComError @see GetReceiverType( CViComError& )

   /**
   Helper operator allowing easy interface access.
   */
   APP_INTERFACE_CLASS* operator-> () { return  GetInterface(); }
   APP_INTERFACE_CLASS& operator*  () { return *GetInterface(); }

public:  //* CViComLoaderBase implementation **************************************/

   virtual void   Connect( const char* uri = "" )
   {
      CViComError error;

      Connect( error, uri );

      if( error.GetErrorCode() != VICEC_NO_ERROR )
      {
         throw error;
      }
   }

   virtual bool   Connect( CViComError& rViComError, const char* uri = "" )
   {
      if( m_pInterface != NULL )
      {
         rViComError = VICOM_MAKE_VICEC( INTERFACE_ALREADY_LOADED );
         return false;
      }

      m_pInterface = 
         unique_ptr( (interface_type*) m_pfCreateInterface( APP_INTERFACE_CLASS::GetType(), m_receiverType, uri, &rViComError ),
                     m_pfDestroyInterface );

      if( rViComError.GetErrorCode() != VICEC_NO_ERROR )
      {
         return false;
      }

      return true;
   }
   virtual bool   Connected            ()
   {
      return m_pInterface != NULL;    
   }

   virtual bool   Connected            ( CViComError& rViComError )
   {
      try
      {
         rViComError = CViComError();
         return Connected();
      }
      catch( const CViComError& e )
      {
         rViComError = e;
      }
   
      return false;
   }

   virtual void Disconnect()
   {
      m_pInterface.reset();
   }

   virtual bool   Disconnect           ( CViComError& rViComError )
   {
      try
      {
         rViComError = CViComError();
         Disconnect();
         return true;
      }
      catch( const CViComError& e )
      {
         rViComError = e;
      }

      return false;
   }

private:

   Receiver::Type             m_receiverType;
   LibLoad                    m_loaderManager;
   typedef void*       _cdecl ftCreateInterface ( Technology::Type, Receiver::Type, const char*, CViComError* );
   typedef typename unique_ptr::ftDestroyInterface ftDestroyInterface;
   ftCreateInterface*         m_pfCreateInterface;
   ftDestroyInterface*        m_pfDestroyInterface;
   unique_ptr                 m_pInterface;
};

} // namespace ViCom
} // namespace RohdeSchwarz

#pragma pack(pop) // reset your structure alignment