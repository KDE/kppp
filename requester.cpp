/*
 *              kPPP: A pppd Front End for the KDE project
 *
 * $Id$
 *
 *              Copyright (C) 1997,98 Bernd Johannes Wuebben,
 *		                      Mario Weilguni,
 *                                    Harri Porten
 *
 *
 * This file was contributed by Harri Porten <porten@tu-harburg.de>
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifdef __osf__
#define _XOPEN_SOURCE_EXTENDED 1
#endif


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

#ifdef __osf__
#undef accept
extern "C" unsigned int alarm(unsigned int);
#endif

#ifdef _XPG4_2
extern "C" {
  ssize_t sendmsg(int, const struct msghdr *, int);
  ssize_t recvmsg(int, struct msghdr *, int);
}
#endif

#include <kdebug.h>
#include <qfile.h>

#include "auth.h"
#include "pppdata.h"
#include "opener.h"
#include "requester.h"
#include "devices.h"

Requester *Requester::rq = 0L;

Requester::Requester(int s) : socket(s) {
  assert(rq==0L);
  rq = this;
  lastStatus = -1;
}

Requester::~Requester() {
}

//
// Receive file name and file descriptors from envoy
//
int Requester::recvFD() {
  struct { struct cmsghdr cmsg; int fd; } control;
  struct msghdr	msg;
  struct ResponseHeader response;

  struct iovec iov;
  int flags = 0, fd, len;
  size_t cmsglen;

  msg.msg_name = 0L;
  msg.msg_namelen = 0;
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;

  iov.iov_base = IOV_BASE_CAST &response;
  iov.iov_len = sizeof(struct ResponseHeader);
#ifdef CMSG_LEN
  cmsglen = CMSG_LEN(sizeof(int));
#else
  cmsglen = sizeof(struct cmsghdr) + sizeof(int);
#endif
  control.cmsg.cmsg_len = cmsglen;
  control.cmsg.cmsg_level = SOL_SOCKET;
  control.cmsg.cmsg_type = MY_SCM_RIGHTS;

  msg.msg_control = (char *) &control;
  msg.msg_controllen = control.cmsg.cmsg_len;

  fd = -1;

  // set alarm in case recvmsg() hangs
  signal(SIGALRM, recv_timeout);
  alarm(2);

  len = recvmsg(socket, &msg, flags);

  alarm(0);
  signal(SIGALRM, SIG_DFL);

  if(len <= 0) {
    kError(5002) << "recvmsg failed " << strerror(errno) << endl;
    return -1;
  } else if (msg.msg_controllen < cmsglen) {
    kError(5002) << "recvmsg: truncated message " << strerror(errno) << endl;
    exit(1);
  } else {
#ifdef CMSG_DATA
    fd = *((int *)CMSG_DATA(&control.cmsg));
#else
    fd = *((int *) control.cmsg.cmsg_data);
#endif
    kDebug(5002) << "response.status: " << response.status;
    assert(response.status <= 0);
    if(response.status < 0)
      return response.status;
  }

  return fd;
}

bool Requester::recvResponse() {

  struct msghdr	msg;
  struct iovec iov;
  struct ResponseHeader response;
  int flags = 0, len;

  msg.msg_name = 0L;
  msg.msg_namelen = 0;
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  msg.msg_control = 0L;
  msg.msg_controllen = 0;

  iov.iov_base = IOV_BASE_CAST &response;
  iov.iov_len = sizeof(struct ResponseHeader);
  kDebug(5002) << "recvResponse(): waiting for message";
  len = recvmsg(socket, &msg, flags);
  kDebug(5002) << "recvResponse(): received message";
  if (len <= 0) {
    if (errno == EINTR)
      kDebug(5002) << "Interrupted system call. Continuing.";
    else
      perror("recvmsg failed");
  } else {
    kDebug(5002) << "response.status: " << response.status;
  }

  lastStatus = response.status;
  return (response.status == 0);
}

int Requester::openModem(const QString & dev) {

  struct OpenModemRequest req;
  req.header.type = Opener::OpenDevice;
  if((req.deviceNum = indexDevice(dev)) < 0)
    return -1;

  sendRequest((struct RequestHeader *) &req, sizeof(req));
  return recvFD();
}


int Requester::openLockfile(const QString &dev, int flags) {

  struct OpenLockRequest req;

  req.header.type = Opener::OpenLock;
  if((req.deviceNum = indexDevice(dev)) < 0)
    return -1;
  req.flags = flags;

  sendRequest((struct RequestHeader *) &req, sizeof(req));
  return recvFD();
}


bool Requester::removeLockfile() {

  struct RemoveLockRequest req;

  req.header.type = Opener::RemoveLock;

  sendRequest((struct RequestHeader *) &req, sizeof(req));
  return recvResponse();
}


int Requester::openResolv(int flags) {

  struct OpenResolvRequest req;

  req.header.type = Opener::OpenResolv;
  req.flags = flags;
  sendRequest((struct RequestHeader *) &req, sizeof(req));
  return recvFD();
}


int Requester::openSysLog() {

  struct OpenLogRequest req;

  req.header.type = Opener::OpenSysLog;
  sendRequest((struct RequestHeader *) &req, sizeof(req));
  return recvFD();
}


bool Requester::setSecret(int method, const QString &name, const QString &password) {
#ifdef __GNUC__
#warning check if QString != 0 means using isNull or isEmpty
#endif
  assert(!name.isNull());
  assert(!password.isNull());

  if(method == AUTH_PAPCHAP)
    return setSecret(AUTH_PAP, name, password) &&
	setSecret(AUTH_CHAP, name, password);

  struct SetSecretRequest req;
  req.header.type = Opener::SetSecret;
  switch(method) {
  case AUTH_PAP:
    req.method = Opener::PAP;
    break;
  case AUTH_CHAP:
    req.method = Opener::CHAP;
    break;
  default:
    return false;
  }
  strncpy(req.username, QFile::encodeName(name), Opener::MaxStrLen);
  req.username[Opener::MaxStrLen] = '\0';
  strncpy(req.password, QFile::encodeName(password), Opener::MaxStrLen);
  req.password[Opener::MaxStrLen] = '\0';
  sendRequest((struct RequestHeader *) &req, sizeof(req));
  return recvResponse();
}

bool Requester::removeSecret(int authMethod) {
  struct RemoveSecretRequest req;
  req.header.type = Opener::RemoveSecret;
  if(authMethod == AUTH_PAP)
    req.method = Opener::PAP;
  else
    if(authMethod == AUTH_CHAP)
      req.method = Opener::CHAP;
    else
      return false;

  sendRequest((struct RequestHeader *) &req, sizeof(req));
  return recvResponse();
}

bool Requester::setHostname(const QString &name) {
  if (name.isEmpty())
    return false;
  struct SetHostnameRequest req;
  req.header.type = Opener::SetHostname;
  strncpy(req.name, QFile::encodeName(name), Opener::MaxStrLen);
  req.name[Opener::MaxStrLen] = '\0';
  sendRequest((struct RequestHeader *) &req, sizeof(req));
  return recvResponse();
}


bool Requester::execPPPDaemon(const QString &arguments) {
  struct ExecDaemonRequest req;
  req.header.type = Opener::ExecPPPDaemon;
  strncpy(req.arguments, QFile::encodeName(arguments), MAX_CMDLEN);
  req.arguments[MAX_CMDLEN] = '\0';
  sendRequest((struct RequestHeader *) &req, sizeof(req));
  if(recvResponse()==0) {
    gpppdata.setpppdRunning(true);
    return true;
  } else
    return false;
}


bool Requester::killPPPDaemon() {
  struct KillDaemonRequest req;
  gpppdata.setpppdRunning(false);
  req.header.type = Opener::KillPPPDaemon;
  sendRequest((struct RequestHeader *) &req, sizeof(req));
  return recvResponse();
}

int Requester::pppdExitStatus()
{
  struct PPPDExitStatusRequest req;
  req.header.type = Opener::PPPDExitStatus;
  sendRequest((struct RequestHeader *) &req, sizeof(req));
  return recvResponse();
}

bool Requester::stop() {

  struct StopRequest req;
  req.header.type = Opener::Stop;
  sendRequest((struct RequestHeader *) &req, sizeof(req));

  //  return recvResponse();
  return true;
}


bool Requester::sendRequest(struct RequestHeader *request, int len) {

  request->len = len - sizeof(struct RequestHeader);

  struct msghdr	msg;
  struct iovec iov;

  iov.iov_base = IOV_BASE_CAST request;
  iov.iov_len = len;

  msg.msg_name = 0L;
  msg.msg_namelen = 0;
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  msg.msg_control = 0L;
  msg.msg_controllen = 0;
  kDebug(5002) << "sendRequest: trying to send msg type " << request->type;
  sendmsg(socket, &msg, 0);
  kDebug(5002) << "sendRequest: sent message";

  return true;
}


int Requester::indexDevice(const QString &dev) {

  int index = -1;

  for(int i = 0; devices[i]; i++)
    if (dev == devices[i])
      index = i;
  return index;
}


void recv_timeout(int) {
  kDebug(5002) << "timeout()";
}
