#ifndef _REQUESTER_H_
#define _REQUESTER_H_

class Requester {

public:
  Requester(int);
  ~Requester();

  int  openModem(const QString & dev);
  int  openLockfile(const QString & dev, int flags = 0);
  bool removeLockfile();
  int  openResolv(int flags);
  int  openSysLog();
  bool setSecret(int method, const QString & name, const QString & password);
  bool removeSecret(int authMethode);
  bool setHostname(const QString & name);
  bool execPPPDaemon(const QString & arguments);
  bool killPPPDaemon();
  int  pppdExitStatus();
  bool stop();

public:
  static Requester *rq;
  int lastStatus;

private:
  bool sendRequest(struct RequestHeader *request, int len);
  bool recvResponse();
  int  recvFD();
  int  indexDevice(const QString & dev);
  int  socket;
  bool expect_alarm;
};

void recv_timeout(int);

#endif
