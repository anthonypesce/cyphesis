// Cyphesis Online RPG Server and AI Engine
// Copyright (C) 2000-2003 Alistair Riddoch
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

// $Id$

#error This file has been removed from the build.
#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include "CommLocalClient.h"

#include "TrustedConnection.h"
#include "CommServer.h"

/// \brief Constructor for local client socket object.
///
/// @param svr Reference to the object that manages all socket communication.
/// @param fd Socket file descriptor
CommLocalClient::CommLocalClient(CommServer & svr, int fd,
                                 const std::string & address,
                                 const std::string & id) :
   CommClient(svr, fd, *new TrustedConnection(*this, svr.m_server,
                                              "local", id))
{
}


CommLocalClient::~CommLocalClient()
{
}
