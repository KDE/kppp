//---------------------------------------------------------------------------
//
//             kPPP: A pppd front end for the KDE project
//
//---------------------------------------------------------------------------
//
// (c) 1997-1998 Bernd Johannes Wuebben <wuebben@kde.org>
// (c) 1997-1999 Mario Weilguni <mweilguni@kde.org>
// (c) 1998-1999 Harri Porten <porten@kde.org>
//
// derived from Jay Painters "ezppp"
//
//---------------------------------------------------------------------------
//
//  $Id$
//
//---------------------------------------------------------------------------
//
//  This program is free software; you can redistribute it and-or
//  modify it under the terms of the GNU Library General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Library General Public License for more details.
//
//  You should have received a copy of the GNU Library General Public
//  License along with this program; if not, write to the Free
//  Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
//---------------------------------------------------------------------------

#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
#include <stdio.h>
#include <limits.h>

#include "utils.h"
#include "kpppconfig.h"

// in case of error, we simply return "0" and hope it's ok :-)
int pppInterfaceNumber() {
  char ifname[PATH_MAX];

  for(int i = 0; i < 8; i++) {
    snprintf(ifname, sizeof(ifname), "%s/ppp%d.pid", PPP_PID_DIR, i);
    if(access(ifname, F_OK) == -1)
      return i;
  }

  // panic
  fprintf(stderr, "pppInterfaceNumber: cannot detect ppp interface number!\n");
  return 0;
}

