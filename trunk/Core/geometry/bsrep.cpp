/****************************************************************************

 This file is part of the GLC-lib library.
 Copyright (C) 2005-2008 Laurent Ribon (laumaya@users.sourceforge.net)
 http://glc-lib.sourceforge.net

 GLC-lib is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.

 GLC-lib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with GLC-lib; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

 *****************************************************************************/
//! \file bsrep.cpp implementation for the BSRep class.

#include "bsrep.h"
#include "../fileformatexception.h"
#include "../tracelog.h"

// The binary rep suffix
const std::string BSRep::m_Suffix("BSRep");

// The binary rep magic number
const QUuid BSRep::m_Uuid("{d6f97789-36a9-4c2e-b667-0e66c27f839f}");

// The binary rep version
const unsigned int BSRep::m_Version= 102;

// Mutex used by compression
QMutex BSRep::m_CompressionMutex;

// Default constructor
BSRep::BSRep(const std::string& fileName, bool useCompression)
: m_FileInfo()
, m_pFile(NULL)
, m_DataStream()
, m_UseCompression(useCompression)
, m_CompressionLevel(-1)
{
	setAbsoluteFileName(fileName);
	m_DataStream.setVersion(QDataStream::Qt_4_6);
	m_DataStream.setFloatingPointPrecision(QDataStream::SinglePrecision);
}

// Copy constructor
BSRep::BSRep(const BSRep& binaryRep)
: m_FileInfo(binaryRep.m_FileInfo)
, m_pFile(NULL)
, m_DataStream()
, m_UseCompression(binaryRep.m_UseCompression)
, m_CompressionLevel(binaryRep.m_CompressionLevel)
{
	m_DataStream.setVersion(QDataStream::Qt_4_6);
	m_DataStream.setFloatingPointPrecision(binaryRep.m_DataStream.floatingPointPrecision());
}

BSRep::~BSRep()
{
	delete m_pFile;
}

// Return true if the binary rep is up to date
bool BSRep::isUsable(const QDateTime& timeStamp)
{
	bool isUpToDate= false;
	if (open(QIODevice::ReadOnly))
	{
		if (headerIsOk())
		{
			isUpToDate= timeStampOk(timeStamp);
			isUpToDate= isUpToDate && close();
		}
	}
	else
	{
		std::string message(std::string("BSRep::loadRep Enable to open the file ") + m_FileInfo.fileName());
		FileFormatException fileFormatException(message, m_FileInfo.fileName(), FileFormatException::FileNotFound);
		close();
		throw(fileFormatException);
	}

	if (!isUpToDate && TraceLog::isEnable())
	{
		std::stringList stringList("BSRep::isUsable");
		stringList.append("File " + m_FileInfo.filePath() + " not Usable");
		TraceLog::addTrace(stringList);
	}
	return isUpToDate;
}

//////////////////////////////////////////////////////////////////////
// name Get Functions
//////////////////////////////////////////////////////////////////////
// Load the binary rep
3DRep BSRep::loadRep()
{
	3DRep loadedRep;

	if (open(QIODevice::ReadOnly))
	{
		if (headerIsOk())
		{
			timeStampOk(QDateTime());
			BoundingBox boundingBox;
			m_DataStream >> boundingBox;
			bool useCompression;
			m_DataStream >> useCompression;
			if (useCompression)
			{
				QByteArray CompresseBuffer;
				m_DataStream >> CompresseBuffer;
				QByteArray uncompressedBuffer= qUncompress(CompresseBuffer);
				uncompressedBuffer.squeeze();
				CompresseBuffer.clear();
				CompresseBuffer.squeeze();
				QDataStream bufferStream(uncompressedBuffer);
				bufferStream >> loadedRep;
			}
			else
			{
				m_DataStream >> loadedRep;
			}
			loadedRep.setFileName(m_FileInfo.filePath());

			if (!close())
			{
				std::string message(std::string("BSRep::loadRep An error occur when loading file ") + m_FileInfo.fileName());
				FileFormatException fileFormatException(message, m_FileInfo.fileName(), FileFormatException::WrongFileFormat);
				throw(fileFormatException);
			}
		}
		else
		{
			std::string message(std::string("BSRep::loadRep File not supported ") + m_FileInfo.fileName());
			FileFormatException fileFormatException(message, m_FileInfo.fileName(), FileFormatException::FileNotSupported);
			close();
			throw(fileFormatException);
		}
	}
	else
	{
		std::string message(std::string("BSRep::loadRep Enable to open the file ") + m_FileInfo.fileName());
		FileFormatException fileFormatException(message, m_FileInfo.fileName(), FileFormatException::FileNotFound);
		close();
		throw(fileFormatException);
	}


	return loadedRep;
}

// Return the bounding box of the binary representation
BoundingBox BSRep::boundingBox()
{
	BoundingBox boundingBox;

	if (open(QIODevice::ReadOnly))
	{
		if (headerIsOk())
		{
			timeStampOk(QDateTime());

			m_DataStream >> boundingBox;
		}
		close();
	}
	return boundingBox;
}

// Return bsrep suffix
std::string BSRep::suffix()
{
	return m_Suffix;
}

//////////////////////////////////////////////////////////////////////
//name Set Functions
//////////////////////////////////////////////////////////////////////
// Set the binary representation file name
void BSRep::setAbsoluteFileName(const std::string& fileName)
{
	m_FileInfo.setFile(fileName);
	if (m_FileInfo.suffix() != m_Suffix)
	{
		m_FileInfo.setFile(fileName + '.' + m_Suffix);
	}

}

// Save the 3DRep in serialised binary
bool BSRep::save(const 3DRep& rep)
{

	//! Check if the currentFileInfo is valid and writable
	bool saveOk= open(QIODevice::WriteOnly);
	if (saveOk)
	{
		writeHeader(rep.lastModified());

		// Representation Bounding Box
		m_DataStream << rep.boundingBox();

		// Compression usage

		if (m_UseCompression && (rep.faceCount() < 1000000))
		{
			m_DataStream << true;
			QByteArray uncompressedBuffer;
			{
				QBuffer buffer(&uncompressedBuffer);
				buffer.open(QIODevice::WriteOnly);
				QDataStream bufferStream(&buffer);
				bufferStream << rep;
			}
			m_DataStream << qCompress(uncompressedBuffer, m_CompressionLevel);
		}
		else
		{
			m_DataStream << false;
			// Binary representation geometry
			// Add the rep
			m_DataStream << rep;
		}

		// Flag the file
		qint64 offset= sizeof(QUuid);
		offset+= sizeof(unsigned int);

		m_pFile->seek(offset);
		bool writeOk= true;
		m_DataStream << writeOk;
		// Close the file
		saveOk= close();
	}
	return saveOk;
}


// Open the file
bool BSRep::open(QIODevice::OpenMode mode)
{
	bool openOk= m_FileInfo.exists();
	if (openOk || (mode == QIODevice::WriteOnly))
	{
		m_DataStream.setDevice(NULL);
		delete m_pFile;
		m_pFile= new QFile(m_FileInfo.filePath());
		openOk= m_pFile->open(mode);
		if (openOk)
		{
			m_DataStream.setDevice(m_pFile);
		}
	}
	else if (TraceLog::isEnable())
	{
		std::stringList stringList("BSRep::open");
		stringList.append("File " + m_FileInfo.filePath() + " doesn't exists");
		TraceLog::addTrace(stringList);
	}

	return openOk;
}

// Close the file
bool BSRep::close()
{
	assert(m_pFile != NULL);
	assert(m_DataStream.device() != NULL);
	bool closeOk= m_DataStream.status() == QDataStream::Ok;
	m_DataStream.setDevice(NULL);
	m_pFile->close();
	delete m_pFile;
	m_pFile= NULL;

	return closeOk;
}

// Write the header
void BSRep::writeHeader(const QDateTime& dateTime)
{
	assert(m_pFile != NULL);
	assert(m_DataStream.device() != NULL);

	// Binary representation Header
	// Add the magic number
	m_DataStream << m_Uuid;
	// Add the version
	m_DataStream << m_Version;
	bool writeFinished= false;
	m_DataStream << writeFinished;

	// Set the version of the data stream
	m_DataStream.setVersion(QDataStream::Qt_4_6);

	// Add the time stamp
	m_DataStream << dateTime;
}

// Check the header
bool BSRep::headerIsOk()
{
	assert(m_pFile != NULL);
	assert(m_DataStream.device() != NULL);
	assert(m_pFile->openMode() == QIODevice::ReadOnly);

	QUuid uuid;
	unsigned int version;
	bool writeFinished;

	m_DataStream >> uuid;
	m_DataStream >> version;
	m_DataStream >> writeFinished;

	// Set the version of the data stream
	m_DataStream.setVersion(QDataStream::Qt_4_6);

	bool headerOk= (uuid == m_Uuid) && (version == m_Version) && writeFinished;

	return headerOk;
}

// Check the time Stamp
bool BSRep::timeStampOk(const QDateTime& timeStamp)
{
	assert(m_pFile != NULL);
	assert(m_DataStream.device() != NULL);
	assert(m_pFile->openMode() == QIODevice::ReadOnly);

	QDateTime dateTime;
	m_DataStream >> dateTime;

	bool timeStampOk= !timeStamp.isValid() || (dateTime == timeStamp);
	return timeStampOk;
}

