#
#  b43 debugging library
#
#  Copyright (C) 2008 Michael Buesch <mb@bu3sch.de>
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License version 3
#  as published by the Free Software Foundation.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

import sys
import os
import re


# SHM routing values
B43_SHM_UCODE		= 0
B43_SHM_SHARED		= 1
B43_SHM_REGS		= 2
B43_SHM_IHR		= 3
B43_SHM_RCMTA		= 4


class B43Exception(Exception):
	pass


B43_MMIO_PSMDEBUG = 0x154

class B43PsmDebug:
	"""Parse the contents of the PSM-debug register"""
	def __init__(self, reg_content):
		self.pc = reg_content & 0xFFF
		return

	def getPc(self):
		"""Get the microcode program counter"""
		return self.pc


class B43:
	def __init__(self, phy):
		debugfs_path = self.__debugfs_find()

		# Construct the debugfs b43 path to the device
		b43_path = debugfs_path + "/b43/"
		if phy:
			b43_path += phy
		else:
			# Get the PHY.
			phys = os.listdir(b43_path)
			if not phys:
				print "Could not find any b43 device"
				raise B43Exception
			if len(phys) != 1:
				print "Found multiple b43 devices."
				print "You must call this tool with a phyX parameter to specify a device"
				raise B43Exception
			phy = phys[0]
			b43_path += phy;

		# Open the debugfs files
		try:
			self.f_mmio16read = file(b43_path + "/mmio16read", "r+")
			self.f_mmio16write = file(b43_path + "/mmio16write", "w")
			self.f_mmio32read = file(b43_path + "/mmio32read", "r+")
			self.f_mmio32write = file(b43_path + "/mmio32write", "w")
			self.f_shm16read = file(b43_path + "/shm16read", "r+")
			self.f_shm16write = file(b43_path + "/shm16write", "w")
			self.f_shm32read = file(b43_path + "/shm32read", "r+")
			self.f_shm32write = file(b43_path + "/shm32write", "w")
		except IOError, e:
			print "Could not open debugfs file %s: %s" % (e.filename, e.strerror)
			raise B43Exception
		return

	# Get the debugfs mountpoint.
	def __debugfs_find(self):
		mtab = file("/etc/mtab").read().splitlines()
		regexp = re.compile(r"^[\w\-_]+\s+([\w/\-_]+)\s+debugfs")
		path = None
		for line in mtab:
			m = regexp.match(line)
			if m:
				path = m.group(1)
				break
		if not path:
			print "Could not find debugfs in /etc/mtab"
			raise B43Exception
		return path

	def read16(self, reg):
		"""Do a 16bit MMIO read"""
		try:
			self.f_mmio16read.seek(0)
			self.f_mmio16read.write("0x%X" % reg)
			self.f_mmio16read.seek(0)
			val = self.f_mmio16read.read()
		except IOError, e:
			print "Coult not access debugfs file %s: %s" % (e.filename, e.strerror)
			raise B43Exception
		return int(val, 16)

	def read32(self, reg):
		"""Do a 32bit MMIO read"""
		try:
			self.f_mmio32read.seek(0)
			self.f_mmio32read.write("0x%X" % reg)
			self.f_mmio32read.seek(0)
			val = self.f_mmio32read.read()
		except IOError, e:
			print "Coult not access debugfs file %s: %s" % (e.filename, e.strerror)
			raise B43Exception
		return int(val, 16)

	def write16(self, reg, value):
		"""Do a 16bit MMIO write"""
		try:
			self.f_mmio16write.seek(0)
			self.f_mmio16write.write("0x%X = 0x%X" % (reg, value))
		except IOError, e:
			print "Coult not access debugfs file %s: %s" % (e.filename, e.strerror)
			raise B43Exception
		return

	def write32(self, reg, value):
		"""Do a 32bit MMIO write"""
		try:
			self.f_mmio32write.seek(0)
			self.f_mmio32write.write("0x%X = 0x%X" % (reg, value))
		except IOError, e:
			print "Coult not access debugfs file %s: %s" % (e.filename, e.strerror)
			raise B43Exception
		return

	def shmRead16(self, routing, offset):
		"""Do a 16bit SHM read"""
		try:
			self.f_shm16read.seek(0)
			self.f_shm16read.write("0x%X 0x%X" % (routing, offset))
			self.f_shm16read.seek(0)
			val = self.f_shm16read.read()
		except IOError, e:
			print "Coult not access debugfs file %s: %s" % (e.filename, e.strerror)
			raise B43Exception
		return int(val, 16)

	def shmMaskSet16(self, routing, offset, mask, set):
		"""Do a 16bit SHM mask-and-set operation"""
		try:
			self.f_shm16write.seek(0)
			self.f_shm16write.write("0x%X 0x%X 0x%X 0x%X" % (routing, offset, mask, set))
		except IOError, e:
			print "Coult not access debugfs file %s: %s" % (e.filename, e.strerror)
			raise B43Exception
		return

	def shmWrite16(self, routing, offset, value):
		"""Do a 16bit SHM write"""
		self.shmMaskSet16(routing, offset, 0, value)
		return

	def shmRead32(self, routing, offset):
		"""Do a 32bit SHM read"""
		try:
			self.f_shm32read.seek(0)
			self.f_shm32read.write("0x%X 0x%X" % (routing, offset))
			self.f_shm32read.seek(0)
			val = self.f_shm32read.read()
		except IOError, e:
			print "Coult not access debugfs file %s: %s" % (e.filename, e.strerror)
			raise B43Exception
		return int(val, 16)

	def shmMaskSet32(self, routing, offset, mask, set):
		"""Do a 32bit SHM mask-and-set operation"""
		try:
			self.f_shm32write.seek(0)
			self.f_shm32write.write("0x%X 0x%X 0x%X 0x%X" % (routing, offset, mask, set))
		except IOError, e:
			print "Coult not access debugfs file %s: %s" % (e.filename, e.strerror)
			raise B43Exception
		return

	def shmWrite32(self, routing, offset, value):
		"""Do a 32bit SHM write"""
		self.shmMaskSet32(routing, offset, 0, value)
		return

	def ucodeRegsRead(self):
		"""Returns an array of 64 ints. One for each ucode register."""
		ret = []
		for i in range(0, 64):
			val = self.shmRead16(B43_SHM_REGS, i)
			ret.append(val)
		return ret

	def shmSharedRead(self):
		"""Returns an array of ints containing the bytewise SHM contents."""
		ret = []
		for i in range(0, 4096, 4):
			val = self.shmRead32(B43_SHM_SHARED, i)
			ret.append(val & 0xFF)
			ret.append((val >> 8) & 0xFF)
			ret.append((val >> 16) & 0xFF)
			ret.append((val >> 24) & 0xFF)
		return ret

	def getPsmDebug(self):
		"""Read the PSM-debug register and return an instance of B43PsmDebug."""
		val = self.read32(B43_MMIO_PSMDEBUG)
		return B43PsmDebug(val)
