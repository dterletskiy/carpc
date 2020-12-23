#pragma once



namespace application::components::onoff {

   struct Data
   {
   };

   enum class eID : std::uint8_t
   {
      Unloaded,
      Loading,
      Loaded
   };

   struct Types
   {
      using tID = eID;
      using tData = Data;
   };

} // namespace base::events
