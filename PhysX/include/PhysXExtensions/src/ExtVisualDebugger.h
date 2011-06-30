// This code contains NVIDIA Confidential Information and is disclosed to you 
// under a form of NVIDIA software license agreement provided separately to you.
//
// Notice
// NVIDIA Corporation and its licensors retain all intellectual property and
// proprietary rights in and to this software and related documentation and 
// any modifications thereto. Any use, reproduction, disclosure, or 
// distribution of this software and related documentation without an express 
// license agreement from NVIDIA Corporation is strictly prohibited.
// 
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
// Copyright (c) 2008-2011 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  


#ifndef EXT_VISUAL_DEBUGGER_H
#define EXT_VISUAL_DEBUGGER_H

#if PX_SUPPORT_VISUAL_DEBUGGER

#include "CmPhysXCommon.h"
#include "PsUserAllocated.h"
#include "PxVisualDebuggerExt.h"
#include "PxJoint.h"

namespace PVD
{
	class PvdDataStream;
	class PvdCommLayerValue;
}

class PxJoint;
class PxD6JointDrive;
class PxJointLimitParameters;
class PxJointLimit;
class PxJointLimitPair;
class PxJointLimitCone;

#define JOINT_GROUP 3

namespace Ext
{
	template<class Base, PxJointType::Enum t> class Joint;

	class VisualDebugger: public PxExtensionVisualDebugger, public Ps::UserAllocated
	{
	public:
		class PvdNameSpace
		{
		public:
			PvdNameSpace(PVD::PvdDataStream& conn, const char* name);
			~PvdNameSpace();
		private:
			PVD::PvdDataStream& mConnection;
		};

		template<class Base, PxJointType::Enum t>
		static void updatePvdProperties(PVD::PvdDataStream& pvdConnection, const PxConstraint* c, const Joint<Base, t>* joint);
		template<class Base, PxJointType::Enum t>
		static bool simUpdate(PVD::PvdDataStream& pvdConnection, const PxConstraint* c, const Joint<Base, t>* joint);
		template<class Base, PxJointType::Enum t>
		static void createPvdInstance(PVD::PvdDataStream& pvdConnection, const PxConstraint* c, const Joint<Base, t>* joint);
		
		static void releasePvdInstance(PVD::PvdDataStream& pvdConnection, const PxJoint* joint);
		static bool sendClassDescriptions(PVD::PvdDataStream& pvdConnection);
		static void updateDrive(PVD::PvdDataStream& pvdConnection, const PxD6JointDrive& drive, PxU64 id);
		static void updateJointLimitParameters(const PxJointLimitParameters& param, PVD::PvdCommLayerValue* values);
		static void updateJointLimit(PVD::PvdDataStream& pvdConnection, const PxJointLimit& limit, PxU64 id);
		static void updateJointLimitPair(PVD::PvdDataStream& pvdConnection, const PxJointLimitPair& limit, PxU64 id);
		static void updateJointLimitCone(PVD::PvdDataStream& pvdConnection, const PxJointLimitCone& limit, PxU64 id);
	};
}


#endif // PX_SUPPORT_VISUAL_DEBUGGER
#endif // EXT_VISUAL_DEBUGGER_H