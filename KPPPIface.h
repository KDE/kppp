#ifndef KPPPIFACE_H
#define KPPPIFACE_H

#include <dcopobject.h>

/* IMPORTANT: when using the aboutToDisconnect signal bear in mind that

    - if pppd dies, aboutToDisconnect will never be emitted because the
      connection is already dead by the time kppp knows about it.
      disconnected() will be emitted regardless of the cause of
      disconnection.

    - during a normal disconnection aboutToDisconnect will be emitted 
      shortly before disconnection, but in systems under heavy load there's 
      no warranty that the signal will be delivered to the applications 
      before the disconnection is performed. DCOP works this way, 
      sorry.

*/

class KpppIface : virtual public DCOPObject
{
  K_DCOP

  k_dcop:
    virtual void beginConnect() = 0;
    virtual void disconnect() = 0;
    virtual bool isConnected() const = 0;
    
  k_dcop_signals:
    
    void aboutToConnect();
    void connected();
    void aboutToDisconnect();	// see the note above
    void disconnected();

};

#endif
