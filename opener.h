#ifndef _FILEOPENER_H_
#define _FILEOPENER_H_

#define DEVNULL "/dev/null"

// workaround for bug in glibc on RedHat 5.0 and Debian 2.1
#if (__GLIBC__ == 2 && __GLIBC_MINOR__ == 0 && defined(__linux__))
# define MY_SCM_RIGHTS 1
#else
# define MY_SCM_RIGHTS SCM_RIGHTS
#endif

#ifdef BSD
# define IOV_BASE_CAST (char *)
#else
# define IOV_BASE_CAST (void *)
#endif

class Opener {

public:
  Opener(int);
  ~Opener();

  enum { OpenDevice = 1,
         OpenLock, RemoveLock,
         OpenResolv,
         OpenSysLog,
         SetSecret, RemoveSecret,
         Stop };
  enum { PAP = 1, CHAP };
  enum { MaxPathLen = 30, MaxStrLen = 40 };

private:
  enum { Original=0x100, New=0x200, Old=0x400 } Version;
  void mainLoop();
  int sendFD(const char *ttypath, int ttyfd, struct ResponseHeader *response);
  int sendResponse(struct ResponseHeader *response);
  const char *deviceByIndex(int idx);
  bool createAuthFile(int authMethod, char *username, char *password);
  bool removeAuthFile(int authMethod);
  const char* authFile(int authMethod, int version = Original);

  int socket;
  char lockfile[MaxPathLen+1];
};


struct RequestHeader {
  int	type;
  int	len;
  //  int   id;     // TODO: Use a transmission id and check whether
                    //       response matches request 
};

struct ResponseHeader {
  int	status; /* 0 or errno */
  //  int   id; 
};

struct OpenModemRequest {
  struct RequestHeader header;
  int    deviceNum;
};

struct RemoveLockRequest {
  struct RequestHeader header;
};

struct OpenLockRequest {
  struct RequestHeader header;
  int    deviceNum;
  int    flags;
};

struct OpenResolvRequest {
  struct RequestHeader header;
  int    flags;
};

struct OpenLogRequest {
  struct RequestHeader header;
};

struct SetSecretRequest {
  struct RequestHeader header;
  int    authMethod;   // PAP or CHAP
  char   username[Opener::MaxStrLen+1];
  char   password[Opener::MaxStrLen+1];
};

struct RemoveSecretRequest {
  struct RequestHeader header;
  int    authMethod;   // PAP or CHAP
};

struct StopRequest {
  struct RequestHeader header;
};

union AllRequests {
  struct RequestHeader header;
  struct OpenModemRequest  modem;
  struct OpenLockRequest lock;
  struct RemoveLockRequest unlock;
  struct OpenResolvRequest resolv;
  struct SetSecretRequest secret;
  struct RemoveSecretRequest remove;
  struct OpenLogRequest log;
  struct StopRequest stop;
};

#endif
