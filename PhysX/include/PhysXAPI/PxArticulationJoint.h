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


#ifndef PX_PHYSICS_NX_ARTICULATION_JOINT
#define PX_PHYSICS_NX_ARTICULATION_JOINT
/** \addtogroup physics 
@{ */

#include "PxPhysX.h"
#include "common/PxSerialFramework.h"

class PxArticulationJoint : public PxSerializable

{
// PX_SERIALIZATION
	protected:
								PxArticulationJoint(PxRefResolver& v) : PxSerializable(v)	{}
								PX_DECLARE_SERIAL_RTTI(PxArticulationJoint, PxSerializable)

	PX_INLINE					PxArticulationJoint() {}
	virtual						~PxArticulationJoint() {}
//~PX_SERIALIZATION

	public:
	

	/**
	\brief set the joint pose in the parent frame

	\param[in] pose the joint pose in the parent frame
	<b>Default:</b> the identity matrix

	@see getParentPose()
	*/

	virtual		void			setParentPose(const PxTransform& pose) = 0;

	/**
	\brief get the joint pose in the parent frame

	\return the joint pose in the parent frame

	@see setParentPose()
	*/

	virtual		PxTransform		getParentPose() const = 0;


	/**
	\brief set the joint pose in the child frame

	\param[in] pose the joint pose in the child frame
	<b>Default:</b> the identity matrix

	@see getChildPose()
	*/

	virtual		void			setChildPose(const PxTransform& pose) = 0;

	/**
	\brief get the joint pose in the child frame

	\return the joint pose in the child frame

	@see setChildPose()
	*/
	virtual		PxTransform		getChildPose() const = 0;


	/**
	\brief set the target drive

	This is the target position for the joint drive, measured in the parent constraint frame.

	\param[in] orientation the target orientation for the joint
	<b>Range:</b> a unit quaternion
	<b>Default:</b> the identity quaternion

	@see getTargetOrientation()
	*/

	virtual		void			setTargetOrientation(const PxQuat& orientation) = 0;

	/**
	\brief get the target drive position

	\return the joint drive target position

	@see setTargetOrientation()
	*/
	virtual		PxQuat			getTargetOrientation() const = 0;

	/**
	\brief set the target drive velocity

	This is the target velocity for the joint drive, measured in the parent constraint frame

	\param[in] velocity the target velocity for the joint
	<b>Default:</b> the zero vector

	@see getTargetVelocity()
	*/
	virtual		void			setTargetVelocity(const PxVec3& velocity) = 0;

	/**
	\brief get the target drive velocity

	\return the target velocity for the joint

	@see setTargetVelocity()
	*/
	virtual		PxVec3			getTargetVelocity() const = 0;


	/**
	\brief set the drive strength of the joint acceleration spring. 

	The acceleration generated by the spring drive is proportional to
	this value and the angle between the drive target position and the
	current position.

	\param[in] spring the spring strength of the joint 
	<b>Range:</b> [0,inf)<br>
	<b>Default:</b> 0.0

	@see getSpring()
	*/
	virtual		void			setSpring(PxReal spring) = 0;

	/**
	\brief get the drive strength of the joint acceleration spring

	\return the spring strength of the joint

	@see setSpring()
	*/
	virtual		PxReal			getSpring() const = 0;


	/**
	\brief set the damping of the joint acceleration spring

	The acceleration generated by the spring drive is proportional to
	this value and the difference between the angular velocity of the
	joint and the target drive velocity.

	\param[in] damping the damping of the joint drive
	<b>Range:</b> [0,inf)<br>
	<b>Default:</b> 0.0

	@see getDamping()
	*/
	virtual		void			setDamping(PxReal damping) = 0;

	/**
	\brief get the damping of the joint acceleration spring

	@see setDamping()
	*/

	virtual		PxReal			getDamping() const = 0;

	/**
	\brief set the internal compliance

	Compliance determines the extent to which the joint resists acceleration. 
	
	There are separate values for resistance to accelerations caused by external
	forces such as gravity and contact forces, and internal forces generated from
	other joints.

	A low compliance means that forces from have little effect, a compliance of 1 means 
	the joint does not resist such forces at all.

	\param[in] compliance the compliance to internal forces
	<b> Range: (0,1]</b>
	<b> Default:</b> 0.0

	@see getInternalCompliance()
	*/

	virtual		void			setInternalCompliance(PxReal compliance) = 0;


	/**
	\brief get the internal compliance

	\return the compliance to internal forces

	@see setInternalCompliance()
	*/
	virtual		PxReal			getInternalCompliance() const = 0;

	/**
	\brief get the drive external compliance

	Compliance determines the extent to which the joint resists acceleration. 
	
	There are separate values for resistance to accelerations caused by external
	forces such as gravity and contact forces, and internal forces generated from
	other joints.

	A low compliance means that forces from have little effect, a compliance of 1 means 
	the joint does not resist such forces at all.

	\param[in] compliance the compliance to external forces
	<b> Range: (0,1]</b>
	<b> Default:</b> 0.0

	@see getExternalCompliance()
	*/

	virtual		void			setExternalCompliance(PxReal compliance) = 0;

	/**
	\brief get the drive external compliance

	\return the compliance to external forces

	@see setExternalCompliance()
	*/
	virtual		PxReal			getExternalCompliance() const = 0;



	/**
	\brief set the extents of the cone limit. The extents are measured in the frame
	of the parent.

	Note that very small or highly elliptical limit cones may result in jitter.

	\param[in] yLimit the allowed extent of rotation around the y-axis
	\param[in] zLimit the allowed extent of rotation around the z-axis
	<b> Range:</b> ([0,Pi), [0, Pi)]
	<b> Default:</b> (0.0, 0.0)
	*/

	virtual		void			setSwingLimit(PxReal yLimit, PxReal zLimit) = 0;


	/**
	\brief get the extents for the swing limit cone
	
	\param[out] yLimit the allowed extent of rotation around the y-axis
	\param[out] zLimit the allowed extent of rotation around the z-axis

	@see setSwingLimit()
	*/
	virtual		void			getSwingLimit(PxReal &yLimit, PxReal &zLimit) const = 0;


	/**
	\brief set the flag which enables the swing limit

	\param[in] enabled whether the limit is enabled
	<b>Default:</b> false

	@see getSwingLimitEnabled()
	*/
	virtual		void			setSwingLimitEnabled(bool enabled) = 0;

	/**
	\brief get the flag which enables the swing limit

	\return whether the swing limit is enabled

	@see setSwingLimitEnabled()
	*/

	virtual		bool			getSwingLimitEnabled() const = 0;


	/**
	\brief set the bounds of the twistLimit

	\param[in] lower the lower extent of the twist limit
	\param[in] upper the upper extent of the twist limit
	<b> Range: (-Pi,Pi)</b>
	<b> Default:</b> (0.0, 0.0)

	The lower limit value must be less than the upper limit if the limit is enabled

	@see getTwistLimit()
	*/
	virtual		void			setTwistLimit(PxReal lower, PxReal upper) = 0;

	/**
	\brief get the bounds of the twistLimit

	\param[out] lower the lower extent of the twist limit
	\param[out] upper the upper extent of the twist limit

	@see setTwistLimit()
	*/

	virtual		void			getTwistLimit(PxReal &lower, PxReal &upper) const = 0;


	/**
	\brief set the flag which enables the twist limit

	\param[in] enabled whether the twist limit is enabled
	<b>Default:</b> false

	@see getTwistLimitEnabled()
	*/
	virtual		void			setTwistLimitEnabled(bool enabled) = 0;

	/**
	\brief get the twistLimitEnabled flag

	\return whether the twist limit is enabled

	@see setTwistLimitEnabled()
	*/

	virtual		bool			getTwistLimitEnabled() const = 0;
};

/** @} */
#endif
