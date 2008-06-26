"""
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
"""

import sys
import os
import re
import md5
from tempfile import *


# SHM routing values
B43_SHM_UCODE		= 0
B43_SHM_SHARED		= 1
B43_SHM_REGS		= 2
B43_SHM_IHR		= 3
B43_SHM_RCMTA		= 4


class B43Exception(Exception):
	pass


B43_MMIO_MACCTL		= 0x120
B43_MMIO_PSMDEBUG	= 0x154

B43_MACCTL_PSM_MACEN	= 0x00000001
B43_MACCTL_PSM_RUN	= 0x00000002
B43_MACCTL_PSM_JMP0	= 0x00000004
B43_MACCTL_PSM_DEBUG	= 0x00002000


class B43PsmDebug:
	"""Parse the contents of the PSM-debug register"""
	def __init__(self, reg_content):
		self.raw = reg_content
		return

	def getRaw(self):
		"""Get the raw PSM-debug register value"""
		return self.raw

	def getPc(self):
		"""Get the microcode program counter"""
		return self.raw & 0xFFF


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

		self.b43_path = b43_path
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
			self.f_mmio16read.flush()
			self.f_mmio16read.seek(0)
			val = self.f_mmio16read.read()
		except IOError, e:
			print "Could not access debugfs file %s: %s" % (e.filename, e.strerror)
			raise B43Exception
		return int(val, 16)

	def read32(self, reg):
		"""Do a 32bit MMIO read"""
		try:
			self.f_mmio32read.seek(0)
			self.f_mmio32read.write("0x%X" % reg)
			self.f_mmio32read.flush()
			self.f_mmio32read.seek(0)
			val = self.f_mmio32read.read()
		except IOError, e:
			print "Could not access debugfs file %s: %s" % (e.filename, e.strerror)
			raise B43Exception
		return int(val, 16)

	def maskSet16(self, reg, mask, set):
		"""Do a 16bit MMIO mask-and-set operation"""
		try:
			mask &= 0xFFFF
			set &= 0xFFFF
			self.f_mmio16write.seek(0)
			self.f_mmio16write.write("0x%X 0x%X 0x%X" % (reg, mask, set))
			self.f_mmio16write.flush()
		except IOError, e:
			print "Could not access debugfs file %s: %s" % (e.filename, e.strerror)
			raise B43Exception
		return
	
	def write16(self, reg, value):
		"""Do a 16bit MMIO write"""
		self.maskSet16(reg, 0, value)
		return

	def maskSet32(self, reg, mask, set):
		"""Do a 32bit MMIO mask-and-set operation"""
		try:
			mask &= 0xFFFFFFFF
			set &= 0xFFFFFFFF
			self.f_mmio32write.seek(0)
			self.f_mmio32write.write("0x%X 0x%X 0x%X" % (reg, mask, set))
			self.f_mmio32write.flush()
		except IOError, e:
			print "Could not access debugfs file %s: %s" % (e.filename, e.strerror)
			raise B43Exception
		return

	def write32(self, reg, value):
		"""Do a 32bit MMIO write"""
		self.maskSet32(reg, 0, value)
		return

	def shmRead16(self, routing, offset):
		"""Do a 16bit SHM read"""
		try:
			self.f_shm16read.seek(0)
			self.f_shm16read.write("0x%X 0x%X" % (routing, offset))
			self.f_shm16read.flush()
			self.f_shm16read.seek(0)
			val = self.f_shm16read.read()
		except IOError, e:
			print "Could not access debugfs file %s: %s" % (e.filename, e.strerror)
			raise B43Exception
		return int(val, 16)

	def shmMaskSet16(self, routing, offset, mask, set):
		"""Do a 16bit SHM mask-and-set operation"""
		try:
			mask &= 0xFFFF
			set &= 0xFFFF
			self.f_shm16write.seek(0)
			self.f_shm16write.write("0x%X 0x%X 0x%X 0x%X" % (routing, offset, mask, set))
			self.f_shm16write.flush()
		except IOError, e:
			print "Could not access debugfs file %s: %s" % (e.filename, e.strerror)
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
			self.f_shm32read.flush()
			self.f_shm32read.seek(0)
			val = self.f_shm32read.read()
		except IOError, e:
			print "Could not access debugfs file %s: %s" % (e.filename, e.strerror)
			raise B43Exception
		return int(val, 16)

	def shmMaskSet32(self, routing, offset, mask, set):
		"""Do a 32bit SHM mask-and-set operation"""
		try:
			mask &= 0xFFFFFFFF
			set &= 0xFFFFFFFF
			self.f_shm32write.seek(0)
			self.f_shm32write.write("0x%X 0x%X 0x%X 0x%X" % (routing, offset, mask, set))
			self.f_shm32write.flush()
		except IOError, e:
			print "Could not access debugfs file %s: %s" % (e.filename, e.strerror)
			raise B43Exception
		return

	def shmWrite32(self, routing, offset, value):
		"""Do a 32bit SHM write"""
		self.shmMaskSet32(routing, offset, 0, value)
		return

	def getGprs(self):
		"""Returns an array of 64 ints. One for each General Purpose register."""
		ret = []
		for i in range(0, 64):
			val = self.shmRead16(B43_SHM_REGS, i)
			ret.append(val)
		return ret

	def getLinkRegs(self):
		"""Returns an array of 4 ints. One for each Link Register."""
		ret = []
		for i in range(0, 4):
			val = self.read16(0x4D0 + (i * 2))
			ret.append(val)
		return ret

	def getOffsetRegs(self):
		"""Returns an array of 7 ints. One for each Offset Register."""
		ret = []
		for i in range(0, 7):
			val = self.read16(0x4C0 + (i * 2))
			ret.append(val)
		return ret

	def shmSharedRead(self):
		"""Returns a string containing the SHM contents."""
		ret = ""
		for i in range(0, 4096, 4):
			val = self.shmRead32(B43_SHM_SHARED, i)
			ret += "%c%c%c%c" %	(val & 0xFF,
						 (val >> 8) & 0xFF,
						 (val >> 16) & 0xFF,
						 (val >> 24) & 0xFF)
		return ret

	def getPsmDebug(self):
		"""Read the PSM-debug register and return an instance of B43PsmDebug."""
		val = self.read32(B43_MMIO_PSMDEBUG)
		return B43PsmDebug(val)

	def getPsmConditions(self):
		"""This returns the contents of the programmable-PSM-conditions register."""
		return self.read16(0x4D8)

	def ucodeStop(self):
		"""Unconditionally stop the microcode PSM. """
		self.maskSet32(B43_MMIO_MACCTL, ~B43_MACCTL_PSM_RUN, 0)
		return

	def ucodeStart(self):
		"""Unconditionally start the microcode PSM. This will restart the
		microcode on the current PC. It will not jump to 0. Warning: This will
		unconditionally restart the PSM and ignore any driver-state!"""
		self.maskSet32(B43_MMIO_MACCTL, ~0, B43_MACCTL_PSM_RUN)
		return

class Disassembler:
	"""Disassembler for b43 firmware."""
	def __init__(self, binaryText, b43DasmOpts):
		input = NamedTemporaryFile()
		output = NamedTemporaryFile()

		input.write(binaryText)
		input.flush()
		#FIXME check b43-dasm errors
		os.system("b43-dasm %s %s %s" % (input.name, output.name, b43DasmOpts))

		self.asmText = output.read()

	def getAsm(self):
		"""Returns the assembly code."""
		return self.asmText

class Assembler:
	"""Assembler for b43 firmware."""
	def __init__(self, assemblyText, b43AsmOpts):
		input = NamedTemporaryFile()
		output = NamedTemporaryFile()

		input.write(assemblyText)
		input.flush()
		#FIXME check b43-asm errors
		os.system("b43-asm %s %s %s" % (input.name, output.name, b43AsmOpts))

		self.binaryText = output.read()

	def getBinary(self):
		"""Returns the binary code."""
		return self.binaryText

class TextPatcher:
	"""A textfile patcher that does not include any target context.
	This can be used to patch b43 firmware files."""

	class TextLine:
		def __init__(self, index, line):
			self.index = index
			self.line = line
			self.deleted = False

	def __init__(self, text, expected_md5sum):
		sum = md5.md5(text).hexdigest()
		if sum != expected_md5sum:
			print "Patcher: The text does not match the expected MD5 sum"
			print "Expected:   " + expected_md5sum
			print "Calculated: " + sum
			raise B43Exception
		text = text.splitlines()
		self.lines = []
		i = 0
		for line in text:
			self.lines.append(TextPatcher.TextLine(i, line))
			i += 1
		# Add an after-last dummy. Needed for the add-before logic
		lastDummy = TextPatcher.TextLine(i, "")
		lastDummy.deleted = True
		self.lines.append(lastDummy)

	def getText(self):
		"""This returns the current text."""
		textLines = []
		for l in self.lines:
			if not l.deleted:
				textLines.append(l.line)
		return "\n".join(textLines)

	def delLine(self, linenumber):
		"""Delete a line of text. The linenumber corresponds to the
		original unmodified text."""
		for l in self.lines:
			if l.index == linenumber:
				l.deleted = True
				return
		print "Patcher deleteLine: Did not find the line!"
		raise B43Exception

	def addText(self, beforeLineNumber, text):
		"""Add a text before the specified linenumber. The linenumber
		corresponds to the original unmodified text."""
		text = text.splitlines()
		index = 0
		for l in self.lines:
			if l.index == beforeLineNumber:
				break
			index += 1
		if index >= len(self.lines):
			print "Patcher addText: Did not find the line!"
			raise B43Exception
		for l in text:
			self.lines.insert(index, TextPatcher.TextLine(-1, l))
			index += 1

