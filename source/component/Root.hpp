#pragma once

#include "IRoot.hpp"



namespace base {



class Root
   : public IRoot
{
public:
   Root( );
   ~Root( ) override;

protected:
   virtual void exit( ) const final;

private:
   void process_event( const SysEvent& ) override;
};



} // namespace base
