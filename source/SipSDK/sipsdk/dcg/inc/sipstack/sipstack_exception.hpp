#ifndef IVSCBB_SS_SIPSTACK_EXCEPTION_HPP
#define IVSCBB_SS_SIPSTACK_EXCEPTION_HPP

#include "sipstack_import.hpp"

using namespace std;
namespace SipStack{

class SipStackException
{
public:
    SipStackException(void);
    SipStackException(string const & exceptMsg);
    SipStackException(string const & exceptId, string const & exceptMsg);
    virtual ~SipStackException(){}
    
    virtual const char* exceptId() const throw();
    virtual const char* what() const throw();

public: 
    SipStackException(const SipStackException &exObj);
    SipStackException& operator=(const SipStackException &exObj);

private:
    string  _exceptId;
    string  _exceptMsg;

};

}
 

#endif

