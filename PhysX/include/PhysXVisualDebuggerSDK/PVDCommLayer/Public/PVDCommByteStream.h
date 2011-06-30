// This code contains NVIDIA Confidential Information and is disclosed 
// under the Mutual Non-Disclosure Agreement.
//
// Notice
// ALL NVIDIA DESIGN SPECIFICATIONS, CODE ARE PROVIDED "AS IS.". NVIDIA MAKES 
// NO WARRANTIES, EXPRESSED, IMPLIED, STATUTORY, OR OTHERWISE WITH RESPECT TO 
// THE MATERIALS, AND EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES OF NONINFRINGEMENT, 
// MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.
//
// Information and code furnished is believed to be accurate and reliable. 
// However, NVIDIA Corporation assumes no responsibility for the consequences of use of such 
// information or for any infringement of patents or other rights of third parties that may 
// result from its use. No license is granted by implication or otherwise under any patent 
// or patent rights of NVIDIA Corporation. Details are subject to change without notice. 
// This code supersedes and replaces all information previously supplied. 
// NVIDIA Corporation products are not authorized for use as critical 
// components in life support devices or systems without express written approval of 
// NVIDIA Corporation.
//
// Copyright (c) 2009 NVIDIA Corporation. All rights reserved.
#ifndef PVD_PVDCOMMBYTESTREAM_H
#define PVD_PVDCOMMBYTESTREAM_H

#include "PVDCommLayerTypes.h"

namespace PVD
{
	/**
	 *	Implementations don't need to implement a caching layer nor do they need
	 *	to worry about threadsafe implementations; that is all built on top of
	 *	this interface.
	 */
	class PvdCommOutStream
	{
	protected:
		virtual ~PvdCommOutStream(){}

	public:
		/**
		 *	write bytes to the other endpoint of the connection.  If an error occurs
		 *	this connection will assume to be dead.
		 *	
		 *	Errors -
		 *	
		 *	NetworkError - Some failure to write data either to buffer or over network.
		 */
		virtual PvdCommLayerError write( const PxU8* inBytes, PxU32 inLength ) = 0;
		
		/**
		 *	Return true if this stream is still connected.
		 */
		virtual bool isConnected() const = 0;
		/**
		 *	Close the in stream.
		 */
		virtual void disconnect() = 0;
		/**
		 *	release any resources related to this stream.
		 */
		virtual void destroy() = 0;
	};

	/**
	 *	Implementations don't need to implement a caching layer nor do they need
	 *	to worry about threadsafe implementations; that is all built on top of
	 *	this interface.
	 */
	class PvdCommInStream
	{
	protected:
		virtual ~PvdCommInStream(){}

	public:
		/**
		 *	Read the requested number of bytes from the socket.  Block until that number
		 *	of bytes is returned.
		 *	
		 *	Errors -
		 *	NetworkError - If call cannot complete.
		 */
		virtual PvdCommLayerError readBytes( PxU8* outBytes, PxU32 ioRequested ) = 0;

		/**
		 *	Return true if this stream is still connected.
		 */
		virtual bool isConnected() const = 0;
		/**
		 *	Close the in stream.
		 */
		virtual void disconnect() = 0;
		/**
		 *	release any resources related to this stream.
		 */
		virtual void destroy() = 0;
	};
	

	template<typename TStreamType>
	struct PvdCommLayerInStreamImpl : public PvdCommInStream
	{
		TStreamType* mStream;
		PvdCommLayerInStreamImpl( TStreamType* inStream )
			: mStream( inStream )
		{
		}
		virtual PvdCommLayerError readBytes( PxU8* outBytes, PxU32 ioRequested ) { return mStream->readBytes( outBytes, ioRequested ); }
		virtual bool isConnected() const { return mStream->isConnected(); }
		virtual void disconnect() { mStream->disconnect(); }
		virtual void destroy() { mStream->destroy(); }
	};
}

#endif