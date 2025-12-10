# VICOM Specific DLL

This folder contains source code for the VICOM specific DLL interfacing.<br>
Copy here C:\RuS\ViCom_15.60\inc folder<br>
Edit .\inc\ViComLoader.h lines 185 e 186:<br>
      m_pfCreateInterface  = reinterpret_cast<ftCreateInterface*> ( ::GetProcAddress( m_loaderManager.get(), "Create"  ) );<br>
      m_pfDestroyInterface = reinterpret_cast<ftDestroyInterface*>( ::GetProcAddress( m_loaderManager.get(), "Destroy" ) );<br>