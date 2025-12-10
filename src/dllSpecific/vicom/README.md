# VICOM Specific DLL

This folder contains source code for the VICOM specific DLL interfacing.
Copy here C:\RuS\ViCom_15.60\inc folder
Edit .\inc\ViComLoader.h lines 185 e 186:
      m_pfCreateInterface  = reinterpret_cast<ftCreateInterface*> ( ::GetProcAddress( m_loaderManager.get(), "Create"  ) );
      m_pfDestroyInterface = reinterpret_cast<ftDestroyInterface*>( ::GetProcAddress( m_loaderManager.get(), "Destroy" ) );