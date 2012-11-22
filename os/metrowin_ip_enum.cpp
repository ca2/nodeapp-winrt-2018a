#include "framework.h"


namespace metrowin
{


   ip_enum::ip_enum(::ca::application * papp) :
      ca(papp),
      ::ca4::ip_enum(papp)
   {

   }


   ip_enum::~ip_enum()
   {

   }



   ::count ip_enum::enumerate(stringa & stra)
   {

      int c = 0;

      ::Windows::Networking::Connectivity::ConnectionProfile ^ connectionProfile = ::Windows::Networking::Connectivity::NetworkInformation::GetInternetConnectionProfile();

      if(connectionProfile == nullptr)
         return 0;

      if(connectionProfile == nullptr || connectionProfile->NetworkAdapter == nullptr || connectionProfile->NetworkAdapter == nullptr)
         return 0;

      ::Windows::Foundation::Collections::IVectorView < ::Windows::Networking::HostName ^ > ^ names = ::Windows::Networking::Connectivity::NetworkInformation::GetHostNames();


      for(int i = 0; i < names->Size; i++)
      {

         ::Windows::Networking::HostName ^ name = names->GetAt(i);

         if(name == nullptr)
            continue;

         if(name->IPInformation == nullptr)
            continue;

         if(name->IPInformation->NetworkAdapter == nullptr)
            continue;

         if(name->IPInformation->NetworkAdapter->NetworkAdapterId == connectionProfile->NetworkAdapter->NetworkAdapterId)
         {

            stra.add(name->DisplayName->Begin());

            c++;

         }

      }

      return c;

   }


} // namespace metrowin



