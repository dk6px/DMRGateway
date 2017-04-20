/*
 *   Copyright (C) 2015,2016,2017 by Jonathan Naylor G4KLX
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "Conf.h"
#include "Log.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>

const int BUFFER_SIZE = 500;

enum SECTION {
  SECTION_NONE,
  SECTION_GENERAL,
  SECTION_LOG,
	SECTION_MMDVM,
  SECTION_DMR_NETWORK,
	SECTION_XLX_NETWORK
};

CConf::CConf(const std::string& file) :
m_file(file),
m_daemon(false),
m_xlxSlot(2U),
m_timeout(10U),
m_logDisplayLevel(0U),
m_logFileLevel(0U),
m_logFilePath(),
m_logFileRoot(),
m_mmdvmAddress(),
m_mmdvmPort(0U),
m_mmdvmLocal(0U),
m_mmdvmDebug(false),
m_xlxNetworkAddress(),
m_xlxNetworkPort(0U),
m_xlxNetworkLocal(0U),
m_xlxNetworkPassword(),
m_xlxNetworkOptions(),
m_xlxNetworkSlot(1U),
m_xlxNetworkTG(9U),
m_xlxNetworkDebug(false),
m_dmrNetworkAddress(),
m_dmrNetworkPort(0U),
m_dmrNetworkLocal(0U),
m_dmrNetworkPassword(),
m_dmrNetworkDebug(false)
{
}

CConf::~CConf()
{
}

bool CConf::read()
{
  FILE* fp = ::fopen(m_file.c_str(), "rt");
  if (fp == NULL) {
    ::fprintf(stderr, "Couldn't open the .ini file - %s\n", m_file.c_str());
    return false;
  }

  SECTION section = SECTION_NONE;

  char buffer[BUFFER_SIZE];
  while (::fgets(buffer, BUFFER_SIZE, fp) != NULL) {
    if (buffer[0U] == '#')
      continue;

    if (buffer[0U] == '[') {
      if (::strncmp(buffer, "[General]", 9U) == 0)
        section = SECTION_GENERAL;
      else if (::strncmp(buffer, "[Log]", 5U) == 0)
        section = SECTION_LOG;
      else if (::strncmp(buffer, "[MMDVM]", 13U) == 0)
        section = SECTION_MMDVM;
			else if (::strncmp(buffer, "[XLX Network]", 13U) == 0)
				section = SECTION_XLX_NETWORK;
			else if (::strncmp(buffer, "[DMR Network]", 13U) == 0)
				section = SECTION_DMR_NETWORK;
			else
        section = SECTION_NONE;

      continue;
    }

    char* key = ::strtok(buffer, " \t=\r\n");
    if (key == NULL)
      continue;

    char* value = ::strtok(NULL, "\r\n");
    if (value == NULL)
      continue;

		if (section == SECTION_GENERAL) {
			if (::strcmp(key, "Daemon") == 0)
				m_daemon = ::atoi(value) == 1;
			else if (::strcmp(key, "XLSSlot") == 0)
				m_xlxSlot = (unsigned int)::atoi(value);
			else if (::strcmp(key, "Timeout") == 0)
				m_timeout = (unsigned int)::atoi(value);
		} else if (section == SECTION_LOG) {
      if (::strcmp(key, "FilePath") == 0)
        m_logFilePath = value;
      else if (::strcmp(key, "FileRoot") == 0)
        m_logFileRoot = value;
      else if (::strcmp(key, "FileLevel") == 0)
        m_logFileLevel = (unsigned int)::atoi(value);
      else if (::strcmp(key, "DisplayLevel") == 0)
        m_logDisplayLevel = (unsigned int)::atoi(value);
    } else if (section == SECTION_MMDVM) {
			if (::strcmp(key, "Address") == 0)
				m_mmdvmAddress = value;
			else if (::strcmp(key, "Port") == 0)
				m_mmdvmPort = (unsigned int)::atoi(value);
			else if (::strcmp(key, "Local") == 0)
				m_mmdvmLocal = (unsigned int)::atoi(value);
			else if (::strcmp(key, "Debug") == 0)
				m_mmdvmDebug = ::atoi(value) == 1;
		} else if (section == SECTION_XLX_NETWORK) {
			if (::strcmp(key, "Address") == 0)
				m_xlxNetworkAddress = value;
			else if (::strcmp(key, "Port") == 0)
				m_xlxNetworkPort = (unsigned int)::atoi(value);
			else if (::strcmp(key, "Local") == 0)
				m_xlxNetworkLocal = (unsigned int)::atoi(value);
			else if (::strcmp(key, "Password") == 0)
				m_xlxNetworkPassword = value;
			else if (::strcmp(key, "Options") == 0)
				m_xlxNetworkOptions = value;
			else if (::strcmp(key, "Slot") == 0)
				m_xlxNetworkSlot = (unsigned int)::atoi(value);
			else if (::strcmp(key, "TG") == 0)
				m_xlxNetworkTG = (unsigned int)::atoi(value);
			else if (::strcmp(key, "Debug") == 0)
				m_xlxNetworkDebug = ::atoi(value) == 1;
		} else if (section == SECTION_DMR_NETWORK) {
      if (::strcmp(key, "Address") == 0)
        m_dmrNetworkAddress = value;
      else if (::strcmp(key, "Port") == 0)
        m_dmrNetworkPort = (unsigned int)::atoi(value);
      else if (::strcmp(key, "Local") == 0)
        m_dmrNetworkLocal = (unsigned int)::atoi(value);
      else if (::strcmp(key, "Password") == 0)
        m_dmrNetworkPassword = value;
      else if (::strcmp(key, "Debug") == 0)
        m_dmrNetworkDebug = ::atoi(value) == 1;
    }
  }

  ::fclose(fp);

  return true;
}

bool CConf::getDaemon() const
{
	return m_daemon;
}

unsigned int CConf::getXLXSlot() const
{
	return m_xlxSlot;
}

unsigned int CConf::getTimeout() const
{
	return m_timeout;
}

unsigned int CConf::getLogDisplayLevel() const
{
	return m_logDisplayLevel;
}

unsigned int CConf::getLogFileLevel() const
{
	return m_logFileLevel;
}

std::string CConf::getLogFilePath() const
{
	return m_logFilePath;
}

std::string CConf::getLogFileRoot() const
{
	return m_logFileRoot;
}

std::string CConf::getMMDVMAddress() const
{
	return m_mmdvmAddress;
}

unsigned int CConf::getMMDVMPort() const
{
	return m_mmdvmPort;
}

unsigned int CConf::getMMDVMLocal() const
{
	return m_mmdvmLocal;
}

bool CConf::getMMDVMDebug() const
{
	return m_mmdvmDebug;
}

std::string CConf::getXLXNetworkAddress() const
{
	return m_xlxNetworkAddress;
}

unsigned int CConf::getXLXNetworkPort() const
{
	return m_xlxNetworkPort;
}

unsigned int CConf::getXLXNetworkLocal() const
{
	return m_xlxNetworkLocal;
}

std::string CConf::getXLXNetworkPassword() const
{
	return m_xlxNetworkPassword;
}

std::string CConf::getXLXNetworkOptions() const
{
	return m_xlxNetworkOptions;
}

unsigned int CConf::getXLXNetworkSlot() const
{
	return m_xlxNetworkSlot;
}

unsigned int CConf::getXLXNetworkTG() const
{
	return m_xlxNetworkTG;
}

bool CConf::getXLXNetworkDebug() const
{
	return m_xlxNetworkDebug;
}

std::string CConf::getDMRNetworkAddress() const
{
	return m_dmrNetworkAddress;
}

unsigned int CConf::getDMRNetworkPort() const
{
	return m_dmrNetworkPort;
}

unsigned int CConf::getDMRNetworkLocal() const
{
	return m_dmrNetworkLocal;
}

std::string CConf::getDMRNetworkPassword() const
{
	return m_dmrNetworkPassword;
}

bool CConf::getDMRNetworkDebug() const
{
	return m_dmrNetworkDebug;
}
