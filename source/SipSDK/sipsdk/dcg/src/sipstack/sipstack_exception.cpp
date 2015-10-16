#include "sipstack_import.hpp"
#include "sipstack_exception.hpp"


namespace SipStack{


SipStackException::SipStackException():_exceptId(""), _exceptMsg("")
{
    
} 


SipStackException::SipStackException(string const & exceptMsg):_exceptId(""), _exceptMsg(exceptMsg.c_str())
{
    // cout<<endl<<"SipStackException: exceptId: null, msg:" << _exceptMsg;
} 


SipStackException::SipStackException(string const & exceptId, string const & msg):_exceptId(exceptId.c_str()), _exceptMsg(msg.c_str())
{
    // cout<<endl<<"SipStackException: exceptId: " << exceptId <<", msg:" << _exceptMsg;
}
 

const char* SipStackException::exceptId() const throw()
{
    return _exceptId.c_str();
}


const char* SipStackException::what() const throw()
{
    return _exceptMsg.c_str();
}


SipStackException::SipStackException(const SipStackException &exObj)
{
    *this = exObj;
}


SipStackException& SipStackException::operator=(const SipStackException &exObj)
{        
    if (this == &exObj)
    {
        return *this;
    }

    _exceptId  = exObj._exceptId;
    _exceptMsg = exObj._exceptMsg;

    return *this;
}


}

