/*
 * ZeroTier One - Global Peer to Peer Ethernet
 * Copyright (C) 2011-2014  ZeroTier Networks LLC
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --
 *
 * ZeroTier may be used and distributed under the terms of the GPLv3, which
 * are available at: http://www.gnu.org/licenses/gpl-3.0.html
 *
 * If you would like to embed ZeroTier into a commercial application or
 * redistribute it in a modified binary form, please contact ZeroTier Networks
 * LLC. Start here: http://www.zerotier.com/
 */

#ifndef ZT_WINDOWSETHERNETTAPFACTORY_HPP
#define ZT_WINDOWSETHERNETTAPFACTORY_HPP

#include <vector>
#include <string>

#include "../node/EthernetTapFactory.hpp"
#include "../node/Mutex.hpp"

namespace ZeroTier {

class WindowsEthernetTapFactory : public EthernetTapFactory
{
public:
	class Env
	{
	public:
		Env();
		BOOL is64Bit; // true if WIN64 or WoW64 (32-bit binary on 64-bit architecture)
		const char *devcon; // name of devcon binary in pathToHelpers to use
		const char *tapDriver; // relative path to driver under pathToHelpers to use
	};

	/**
	 * Constants related to Windows environment, computed on program start
	 */
	static const Env WINENV;

	/**
	 * @param pathToHelpers Path to devcon32.exe, devcon64.exe, and other required helper binaries (ZeroTier running directory)
	 */
	WindowsEthernetTapFactory(const char *pathToHelpers);
	virtual ~WindowsEthernetTapFactory();

	virtual EthernetTap *open(
		const MAC &mac,
		unsigned int mtu,
		unsigned int metric,
		uint64_t nwid,
		const char *desiredDevice,
		const char *friendlyName,
		void (*handler)(void *,const MAC &,const MAC &,unsigned int,const Buffer<4096> &),
		void *arg);
	virtual void close(EthernetTap *tap,bool destroyPersistentDevices);

	/**
	 * Uninstalls all persistent tap devices in the system belonging to ZeroTier
	 *
	 * This is for uninstallation. Do not call this while tap devices are active.
	 */
	static void destroyAllPersistentTapDevices(const char *pathToHelpers);

private:
	static void _deletePersistentTapDevice(const char *pathToHelpers,const char *instanceId);

	std::string _pathToHelpers;
	std::vector<EthernetTap *> _devices;
	Mutex _devices_m;
};

} // namespace ZeroTier

#endif
