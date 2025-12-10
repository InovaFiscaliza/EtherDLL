# VICOM Specific DLL

This folder contains source code for the VICOM specific DLL interfacing.<br>
Copy here C:\RuS\ViCom_15.60\inc folder<br>
Edit .\inc\ViComLoader.h lines 153, 185 e 186:<br>
      153: unique_ptr& operator=( unique_ptr&& rhs )
      185: m_pfCreateInterface  = reinterpret_cast<ftCreateInterface*> ( ::GetProcAddress( m_loaderManager.get(), "Create"  ) );<br>
      186: m_pfDestroyInterface = reinterpret_cast<ftDestroyInterface*>( ::GetProcAddress( m_loaderManager.get(), "Destroy" ) );<br>