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

#ifndef PX_PHYSICS_NX_SHAPE
#define PX_PHYSICS_NX_SHAPE
/** \addtogroup physics
@{
*/

#include "PxPhysX.h"
#include "common/PxSerialFramework.h"
#include "geometry/PxGeometry.h"

class PxBoxGeometry;
class PxSphereGeometry;
class PxCapsuleGeometry;
class PxPlaneGeometry;
class PxConvexMeshGeometry;
class PxTriangleMeshGeometry;
class PxHeightFieldGeometry;
class PxRigidActor;
struct PxFilterData;


/**
\brief Flags which affect the behavior of PxShapes.

@see PxShape PxShapeDesc PxShape.setFlag()
*/
struct PxShapeFlag
{
	enum Enum
	{
		/**
		\brief The shape will partake in collision in the physical simulation.

		@see PxSimulationEventCallback.onContact() PxScene.setSimulationEventCallback()
		*/
		eSIMULATION_SHAPE				= (1<<0),

		/**
		\brief The shape will partake in scene queries (ray casts, overlap tests, sweeps, ...).
		*/
		eSCENE_QUERY_SHAPE				= (1<<1),

		/**
		\brief The shape is a trigger which can send reports whenever other shapes enter/leave its volume.

		\note Triangle meshes and heightfields can not be triggers. Shape creation will fail in these cases.

		\note Shapes marked as triggers do not collide with other objects. If an object should act both
		as a trigger shape and a collision shape then create a rigid body with two shapes, one being a 
		trigger shape and the other a collision shape.

		@see PxSimulationEventCallback.onTrigger() PxScene.setSimulationEventCallback()
		*/
		eTRIGGER_SHAPE					= (1<<2),

		/**
		\brief Enable debug renderer for this shape

		@see PxScene.getRenderBuffer() PxRenderBuffer PxVisualizationParameter
		*/
		eVISUALIZATION					= (1<<3),

		/**
		\brief Sets the shape to be a particle drain.
		*/
		ePARTICLE_DRAIN					= (1<<4),

		/**
		\brief Sets the shape to be a deformable drain.
		*/
		eDEFORMABLE_DRAIN				= (1<<5),

		/**
		\brief  Enables the shape's use of temporal bounds during the post integration broad phase check.
		*/
		eUSE_SWEPT_BOUNDS				= (1<<6),
	};
};

/**
\brief collection of set bits defined in PxShapeFlag.

@see PxShapeFlag
*/
typedef PxFlags<PxShapeFlag::Enum,PxU8> PxShapeFlags;
PX_FLAGS_OPERATORS(PxShapeFlag::Enum,PxU8);


/**
\brief Abstract class for collision shapes.

An instance can be created by calling the createShape() method of the PxRigidActor class,
or by adding the shape descriptors into the PxRigidActorDesc class before creating the actor.

<h3>Visualizations</h3>
\li PxVisualizationParameter::eCOLLISION_AABBS
\li PxVisualizationParameter::eCOLLISION_SHAPES
\li PxVisualizationParameter::eCOLLISION_AXES

The NVIDIA PhysX SDK users guide describes which shape geometries can collide with each other (direct link: <a href="PhysXDocumentation.chm::/Guide/collision_Interactions.html">users guide</a>)

@see PxRigidActor.createShape() PxBoxGeometry PxSphereGeometry PxCapsuleGeometry PxPlaneGeometry PxConvexMeshGeometry
PxTriangleMeshGeometry PxHeightFieldGeometry
*/
class PxShape
// PX_SERIALIZATION
	: public PxSerializable
//~PX_SERIALIZATION
{
	protected:
// PX_SERIALIZATION
										PxShape(PxRefResolver& v)	: PxSerializable(v)	{}
										PX_DECLARE_SERIAL_RTTI(PxShape, PxSerializable)
//~PX_SERIALIZATION
	PX_INLINE							PxShape() : userData(NULL)
													{}
	virtual								~PxShape()	{}

	public:

	/**
	\brief Deletes the shape.
	
	Do not keep a reference to the deleted instance.

	For static rigid actors it is not possible to release all shapes associated with the actor.
	An attempt to remove the last shape will be ignored.

	@see PxRigidActor::createShape()
	*/
	virtual		void					release() = 0;

	/**
	\brief Get the geometry type of the shape.

	\return Type of shape geometry.

	@see PxGeometryType
	*/
	virtual		PxGeometryType::Enum	getGeometryType() const = 0;

	/**
	\brief Adjust the geometry of the shape.

	\note The type of the passed in geometry must match the geometry type of the shape.
	It is not allowed to change the geometry type of a shape.

	\param[in] geometry New geometry of the shape.

	@see PxGeometry PxGeometryType getGeometryType()
	*/
	virtual		void					setGeometry(const PxGeometry& geometry) = 0;

	/**
	\brief Fetch the geometry of the shape.

	\note If the type of geometry to extract does not match the geometry type of the shape
	then the method will return false and the passed in geometry descriptor is not modified.

	\param[in] geometry The descriptor to save the shape's geometry data to.
	\return True on success else false

	@see PxGeometry PxGeometryType getGeometryType()
	*/
	virtual		bool					getBoxGeometry(PxBoxGeometry& geometry) const = 0;

	/**
	\brief Fetch the geometry of the shape.

	\note If the type of geometry to extract does not match the geometry type of the shape
	then the method will return false and the passed in geometry descriptor is not modified.

	\param[in] geometry The descriptor to save the shape's geometry data to.
	\return True on success else false

	@see PxGeometry PxGeometryType getGeometryType()
	*/
	virtual		bool					getSphereGeometry(PxSphereGeometry& geometry) const = 0;

	/**
	\brief Fetch the geometry of the shape.

	\note If the type of geometry to extract does not match the geometry type of the shape
	then the method will return false and the passed in geometry descriptor is not modified.

	\param[in] geometry The descriptor to save the shape's geometry data to.
	\return True on success else false

	@see PxGeometry PxGeometryType getGeometryType()
	*/
	virtual		bool					getCapsuleGeometry(PxCapsuleGeometry& geometry) const = 0;

	/**
	\brief Fetch the geometry of the shape.

	\note If the type of geometry to extract does not match the geometry type of the shape
	then the method will return false and the passed in geometry descriptor is not modified.

	\param[in] geometry The descriptor to save the shape's geometry data to.
	\return True on success else false

	@see PxGeometry PxGeometryType getGeometryType()
	*/
	virtual		bool					getPlaneGeometry(PxPlaneGeometry& geometry) const = 0;

	/**
	\brief Fetch the geometry of the shape.

	\note If the type of geometry to extract does not match the geometry type of the shape
	then the method will return false and the passed in geometry descriptor is not modified.

	\param[in] geometry The descriptor to save the shape's geometry data to.
	\return True on success else false

	@see PxGeometry PxGeometryType getGeometryType()
	*/
	virtual		bool					getConvexMeshGeometry(PxConvexMeshGeometry& geometry) const = 0;

	/**
	\brief Fetch the geometry of the shape.

	\note If the type of geometry to extract does not match the geometry type of the shape
	then the method will return false and the passed in geometry descriptor is not modified.

	\param[in] geometry The descriptor to save the shape's geometry data to.
	\return True on success else false

	@see PxGeometry PxGeometryType getGeometryType()
	*/
	virtual		bool					getTriangleMeshGeometry(PxTriangleMeshGeometry& geometry) const = 0;

	/**
	\brief Fetch the geometry of the shape.

	\note If the type of geometry to extract does not match the geometry type of the shape
	then the method will return false and the passed in geometry descriptor is not modified.

	\param[in] geometry The descriptor to save the shape's geometry data to.
	\return True on success else false

	@see PxGeometry PxGeometryType getGeometryType()
	*/
	virtual		bool					getHeightFieldGeometry(PxHeightFieldGeometry& geometry) const = 0;

	/**
	\brief Retrieves the actor which this shape is associated with.

	\return The actor this shape is associated with.

	@see PxRigidStatic, PxRigidDynamic, PxArticulationLink
	*/
	virtual		PxRigidActor&			getActor() const = 0;

	/**
	\brief Retrieves the axis aligned bounding box enclosing the shape.

	\return The shape's bounding box.

	@see PxBounds3
	*/
	virtual		PxBounds3				getWorldBounds() const = 0;

/************************************************************************************************/

/** @name Pose Manipulation
*/
//@{

	/**
	\brief Sets the pose of the shape in actor space, i.e. relative to the actor they are owned by.
	
	This transformation is identity by default.

	<b>Sleeping:</b> Does <b>NOT</b> wake the associated actor up automatically.

	<i>Note:</i> Does not automatically update the inertia properties of the owning actor (if applicable); use the
	PhysX extensions method #PxRigidBodyExt::updateMassAndInertia() to do this.

	\param[in] pose	The new transform from the actor frame to the shape frame. <b>Range:</b> rigid body transform

	@see getLocalPose() PxShapeDesc.localPose
	*/
	virtual		void					setLocalPose(const PxTransform& pose)		= 0;

	/**
	\brief Retrieves the pose of the shape in actor space, i.e. relative to the actor they are owned by.

	This transformation is identity by default.

	\return Pose of shape relative to the actor's frame.

	@see setLocalPose() PxShapeDesc.localPose
	*/
	virtual		PxTransform				getLocalPose()					const	= 0;

//@}
/************************************************************************************************/

/** @name Collision Filtering
*/
//@{

	/**
	\brief Sets the user definable collision filter data.
	
	<b>Sleeping:</b> Does wake up the actor if the filter data change causes a formerly suppressed
	collision pair to be enabled.

	@see getSimulationFilterData() PxShapeDesc.simulationFilterData
	*/
	virtual		void					setSimulationFilterData(const PxFilterData& data)	= 0;

	/**
	\brief Retrieves the shape's collision filter data.

	@see setSimulationFilterData() PxShapeDesc.simulationFilterData
	*/
	virtual		PxFilterData			getSimulationFilterData()					const	= 0;

	/**
	\brief Marks the object to reset interactions and re-run collision filters in the next simulation step.
	
	This call forces the object to remove all existing collision interactions, to search anew for existing contact
	pairs and to run the collision filters again for found collision pairs.

	\note All persistent state of existing interactions will be lost and can not be retrieved even if the same collison pair
	is found again in the next step. This will mean, for example, that you will not get notified about persistent contact
	for such an interaction (see #PxPairFlag::eNOTIFY_TOUCH_PERSISTS), the contact pair will be interpreted as newly found instead.

	\note Lost touch contact reports will be sent for every collision pair which includes this shape, if they have
	been requested through #PxPairFlag::eNOTIFY_TOUCH_LOST or #PxPairFlag::eNOTIFY_THRESHOLD_FORCE_LOST.

	\note This is an expensive operation, don't use it if you don't have to.

	\note Can be used to retrieve collision pairs that were killed by the collision filters (see #PxFilterFlag::eKILL)

	<b>Sleeping:</b> Does wake up the actor.

	@see PxSimulationFilterShader PxSimulationFilterCallback
	*/
	virtual		void					resetFiltering()						= 0;

	/**
	\brief Sets the user definable query filter data.
	
	@see getQueryFilterData() PxShapeDesc.QueryFilterData
	*/
	virtual		void					setQueryFilterData(const PxFilterData& data)	= 0;

	/**
	\brief Retrieves the shape's Query filter data.

	@see setQueryFilterData() 
	*/
	virtual		PxFilterData			getQueryFilterData()					const	= 0;

//@}
/************************************************************************************************/

	/**
	\brief Assigns material(s) to the shape.
	
	<b>Sleeping:</b> Does <b>NOT</b> wake the associated actor up automatically.

	\param[in] materials List of material pointers to assign to the shape. See #PxMaterial
	\param[in] materialCount The number of materials provided.

	@see PxPhysics.createMaterial() getMaterials() PxShapeDesc::materials
	*/
	virtual		void					setMaterials(PxMaterial*const* materials, PxU32 materialCount)	= 0;

	/**
	\brief Returns the number of materials assigned to the shape.

	You can use #getMaterials() to retrieve the material pointers.

	\return Number of materials associated with this shape.

	@see PxMaterial getMaterials()
	*/
	virtual		PxU32					getNbMaterials()		const	= 0;

	/**
	\brief Retrieve all the material pointers associated with the shape.

	You can retrieve the number of material pointers by calling #getNbMaterials()

	Note: Removing materials with #PxMaterial::release() will invalidate the pointer of the released material.

	\param[out] userBuffer The buffer to store the material pointers.
	\param[in] bufferSize Size of provided user buffer.
	\return Number of material pointers written to the buffer.

	@see PxMaterial getNbMaterials() PxMaterial::release()
	*/
	virtual		PxU32					getMaterials(PxMaterial** userBuffer, PxU32 bufferSize) const				= 0;
	
	/**
	\brief Retrieve material from given triangle index.

	The input index is the internal triangle index as used inside the SDK. This is the index
	returned to users by various SDK functions such as raycasts.
	
	This function is only useful for triangle meshes or heightfields, which have per-triangle
	materials. For other shapes the function returns the single material associated with the
	shape, regardless of the index.

	\param[in] faceIndex The internal triangle index whose material you want to retrieve.
	\return Material from input triangle

	@see PxMaterial getNbMaterials() PxMaterial::release()
	*/
	virtual		PxMaterial*				getMaterialFromInternalFaceIndex(PxU32 faceIndex) const = 0;

	/**
	\brief Sets the contact offset.

	<b>Sleeping:</b> Does <b>NOT</b> wake the associated actor up automatically.

	\param[in] contactOffset <b>Range:</b> (maximum(0,restOffset),inf)

	@see getContactOffset
	*/
	virtual		void					setContactOffset(PxReal contactOffset)	= 0;

	/**
	\brief Retrieves the contact offset. 

	\return The contact offset of the shape.

	@see setContactOffset()
	*/
	virtual		PxReal					getContactOffset() const	= 0;

	/**
	\brief Sets the rest offset. 

	<b>Sleeping:</b> Does <b>NOT</b> wake the associated actor up automatically.

	\param[in] restOffset	<b>Range:</b> (-inf,contactOffset)

	@see getRestOffset
	*/
	virtual		void					setRestOffset(PxReal restOffset)	= 0;

	/**
	\brief Retrieves the rest offset. 

	\return The rest offset of the shape.

	@see setRestOffset()
	*/
	virtual		PxReal					getRestOffset() const	= 0;

/************************************************************************************************/

	/**
	\brief Sets shape flags

	\note At least one of the following flags has to remain set: #PxShapeFlag::eSIMULATION_SHAPE, #PxShapeFlag::eTRIGGER_SHAPE, #PxShapeFlag::eSCENE_QUERY_SHAPE

	<b>Sleeping:</b> Does <b>NOT</b> wake the associated actor up automatically.

	\param[in] flag The shape flag to enable/disable. See #PxShapeFlag.
	\param[in] value True to set the flag. False to clear the flag specified in flag.

	@see PxShapeFlag getFlags()
	*/
	virtual		void					setFlag(PxShapeFlag::Enum flag, bool value) = 0;

	/**
	\brief Sets shape flags

	@see PxShapeFlag getFlags()
	*/
	virtual		void					setFlags(  PxShapeFlags inFlags ) = 0;

	/**
	\brief Retrieves shape flags.

	\return The values of the shape flags.

	@see PxShapeFlag setFlag()
	*/
	virtual		PxShapeFlags			getFlags() const = 0;

	/**
	\brief Sets a name string for the object that can be retrieved with #getName().
	
	This is for debugging and is not used by the SDK.
	The string is not copied by the SDK, only the pointer is stored.

	\param[in] name The name string to set the objects name to.

	@see getName()
	*/
	virtual		void					setName(const char* name)		= 0;

	/**
	\brief retrieves the name string set with setName().
	\return The name associated with the shape.

	@see setName()
	*/
	virtual		const char*				getName()			const	= 0;

/************************************************************************************************/

				void*					userData;	//!< user can assign this to whatever, usually to create a 1:1 relationship with a user object.
};

/** @} */
#endif
