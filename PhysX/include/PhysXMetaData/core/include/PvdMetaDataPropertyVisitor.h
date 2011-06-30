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

#ifndef PX_META_DATA_PROPERTY_VISITOR_H
#define PX_META_DATA_PROPERTY_VISITOR_H

#if PX_SUPPORT_VISUAL_DEBUGGER

#include "PvdMetaDataExtensions.h"

namespace Pvd
{

	
//PVD only deals with read-only properties, indexed, and properties like this by in large.
//so we have a filter that expands properties to a level where we can get to them and expands them into
//named functions that make more sense and are easier to read than operator()
template<typename TOperatorType>
struct PvdPropertyFilter
{
	TOperatorType		mOperator;
	PxU32*				mKeyOverride;
	PxU32*				mOffsetOverride;

	PvdPropertyFilter( TOperatorType& inOperator ) 
				: mOperator( inOperator )
				, mKeyOverride( 0 )
				, mOffsetOverride( 0 ) {}

	PvdPropertyFilter( TOperatorType& inOperator, PxU32* inKeyOverride, PxU32* inOffsetOverride ) 
				: mOperator( inOperator )
				, mKeyOverride( inKeyOverride )
				, mOffsetOverride( inOffsetOverride ) {}

	PvdPropertyFilter( const PvdPropertyFilter& inOther ) : mOperator( inOther.mOperator ), mKeyOverride( inOther.mKeyOverride ), mOffsetOverride( inOther.mOffsetOverride ) {}

	template<PxU32 TKey, typename TAccessorType>
	void dispatchAccessor( PxU32 inKey, const TAccessorType& inAccessor, bool, const PxUnknownClassInfo&, bool )
	{
		mOperator.simpleProperty(inKey, inAccessor );
	}

	template<PxU32 TKey, typename TAccessorType>
	void dispatchAccessor( PxU32 inKey, const TAccessorType& inAccessor, bool, const PxUnknownClassInfo&, const PxU32ToName* inConversions )
	{
		mOperator.flagsProperty( inKey, inAccessor, inConversions );
	}
	
	template<PxU32 TKey, typename TAccessorType>
	void dispatchAccessor( PxU32 inKey, const TAccessorType& inAccessor, const PxU32ToName* inConversions, const PxUnknownClassInfo&, bool )
	{
		mOperator.enumProperty( inKey, inAccessor, inConversions );
	}
	
	template<PxU32 TKey, typename TAccessorType, typename TInfoType>
	void dispatchAccessor(PxU32, const TAccessorType& inAccessor, bool, const TInfoType& inInfo, bool )
	{
		PxU32 rangeStart = PxEnumRangeMap<TKey>().Start;
		PxU32& propIdx = mKeyOverride == NULL ? rangeStart : *mKeyOverride;
		mOperator.complexProperty( &propIdx, inAccessor, inInfo );
	}

	PxU32 getKeyValue( PxU32 inPropertyKey )
	{
		PxU32 retval = inPropertyKey;
		if ( mKeyOverride )
		{
			retval = *mKeyOverride;
			(*mKeyOverride)++;
		}
		return retval;
	}

	
	void setupValueStructOffset( const ValueStructOffsetRecord&, bool, PxU32* ) {}
	void setupValueStructOffset( const ValueStructOffsetRecord& inAccessor, PxU32 inOffset, PxU32* inAdditionalOffset ) 
	{
		//This allows us to nest properties correctly.
		if ( inAdditionalOffset ) inOffset += *inAdditionalOffset;
		inAccessor.setupValueStructOffset( inOffset );
	}
	
	template<PxU32 TKey, typename TAccessorType>
	void handleAccessor( PxU32 inKey, const TAccessorType& inAccessor )
	{
		typedef typename TAccessorType::prop_type TPropertyType;
		dispatchAccessor<TKey>(	inKey
								, inAccessor
								, PxEnumTraits<TPropertyType>().NameConversion
								, PxClassInfoTraits<TPropertyType>().Info
								, IsFlagsType<TPropertyType>().FlagData );

	}


	template<PxU32 TKey, typename TAccessorType>
	void handleAccessor( const TAccessorType& inAccessor )
	{
		setupValueStructOffset( inAccessor, PxPropertyToValueStructMemberMap<TKey>().Offset, mOffsetOverride );
		handleAccessor<TKey>( getKeyValue( TKey ), inAccessor );
	}

	template<PxU32 TKey, typename TObjType, typename TPropertyType>
	void operator()( const PxReadOnlyPropertyInfo<TKey,TObjType,TPropertyType>& inProperty, PxU32 )
	{
		PxPvdReadOnlyPropertyAccessor< TKey, TObjType, TPropertyType > theAccessor( inProperty );
		mOperator.pushName( inProperty.mName );
		handleAccessor<TKey>( theAccessor );
		mOperator.popName();
	}

	//We don't handle unbounded indexed properties
	template<PxU32 TKey, typename TObjType, typename TIndexType, typename TPropertyType, typename TValueConversionType, typename TInfoType>
	void indexedProperty( PxU32, const PxIndexedPropertyInfo<TKey, TObjType, TIndexType, TPropertyType >&, bool, TValueConversionType, const TInfoType& ) {}

	template<PxU32 TKey, typename TObjType, typename TIndexType, typename TPropertyType>
	void indexedProperty( PxU32, const PxIndexedPropertyInfo<TKey, TObjType, TIndexType, TPropertyType >& inProp, const PxU32ToName* theConversions, const PxUnknownClassInfo& )
	{
		mOperator.pushName( inProp.mName );
		PxU32 rangeStart = PxEnumRangeMap<TKey>().Start;
		PxU32& propIdx = mKeyOverride == NULL ? rangeStart : *mKeyOverride;
		PxU32 theOffset = 0; 
		if ( mOffsetOverride ) theOffset = *mOffsetOverride;
		
		while( theConversions->mName != NULL )
		{
			mOperator.pushBracketedName( theConversions->mName );
			PxPvdIndexedPropertyAccessor<TKey, TObjType, TIndexType, TPropertyType> theAccessor( inProp, theConversions->mValue );
			setupValueStructOffset( theAccessor, PxPropertyToValueStructMemberMap<TKey>().Offset, &theOffset );
			handleAccessor<TKey>( propIdx, theAccessor );
			mOperator.popName();
			++propIdx;
			++theConversions;
			theOffset += sizeof( TPropertyType );
		}
		mOperator.popName();
	}

	template<PxU32 TKey, typename TObjType, typename TIndexType, typename TPropertyType, typename TInfoType>
	void indexedProperty( PxU32, const PxIndexedPropertyInfo<TKey, TObjType, TIndexType, TPropertyType >& inProp, const PxU32ToName* theConversions, const TInfoType& inInfo )
	{
		//ouch, not nice.  Indexed complex property.
		mOperator.pushName( inProp.mName );
		PxU32 propIdx = PxEnumRangeMap<TKey>().Start;
		PxU32 theOffset = 0; 
		if ( mOffsetOverride ) theOffset = *mOffsetOverride;

		while( theConversions->mName != NULL )
		{
			mOperator.pushBracketedName( theConversions->mName );
			PxPvdIndexedPropertyAccessor<TKey, TObjType, TIndexType, TPropertyType> theAccessor( inProp, theConversions->mValue );
			setupValueStructOffset( theAccessor, PxPropertyToValueStructMemberMap<TKey>().Offset, &theOffset );
			PX_ASSERT( theAccessor.mHasValidOffset );			
			mOperator.complexProperty( &propIdx, theAccessor, inInfo );
			mOperator.popName();
			++theConversions;
			theOffset += sizeof( TPropertyType );
		}
		mOperator.popName();
	}

	template<PxU32 TKey, typename TObjType, typename TIndexType, typename TPropertyType>
	void operator()( const PxIndexedPropertyInfo<TKey, TObjType, TIndexType, TPropertyType >& inProp, PxU32 inIndex ) 
	{
		indexedProperty( inIndex, inProp, IndexerToNameMap<TKey,TIndexType>().Converter.NameConversion
							, PxClassInfoTraits<TPropertyType>().Info );
	}
	
	//We don't handle unbounded indexed properties
	template<PxU32 TKey, typename TObjType, typename TIndexType, typename TIndex2Type, typename TPropertyType, typename TNameConv, typename TNameConv2 >
	void dualIndexedProperty( PxU32 idx, const PxDualIndexedPropertyInfo<TKey, TObjType, TIndexType, TIndex2Type, TPropertyType >&, TNameConv, TNameConv2 ) {}

	template<PxU32 TKey, typename TObjType, typename TIndexType, typename TIndex2Type, typename TPropertyType>
	void dualIndexedProperty( PxU32 idx, const PxDualIndexedPropertyInfo<TKey, TObjType, TIndexType, TIndex2Type, TPropertyType >& inProp, const PxU32ToName* c1, const PxU32ToName* c2 )
	{
		mOperator.pushName( inProp.mName );
		PxU32 rangeStart = PxEnumRangeMap<TKey>().Start;
		PxU32& propIdx = mKeyOverride == NULL ? rangeStart : *mKeyOverride;
		PxU32 theOffset = 0; 
		if ( mOffsetOverride ) theOffset = *mOffsetOverride;
		while( c1->mName != NULL )
		{
			mOperator.pushBracketedName( c1->mName );
			const PxU32ToName* c2Idx = c2;
			while( c2Idx->mName != NULL )
			{
				mOperator.pushBracketedName( c2Idx->mName );
				PxPvdDualIndexedPropertyAccessor<TKey, TObjType, TIndexType, TIndex2Type, TPropertyType> theAccessor( inProp, c1->mValue, c2Idx->mValue );
				setupValueStructOffset( theAccessor, PxPropertyToValueStructMemberMap<TKey>().Offset, &theOffset );
				handleAccessor<TKey>( propIdx, theAccessor );
				mOperator.popName();
				++propIdx;
				++c2Idx;
				theOffset += sizeof( TPropertyType );
			}
			mOperator.popName();
			++c1;
		}
		mOperator.popName();
	}

	template<PxU32 TKey, typename TObjType, typename TIndexType, typename TIndex2Type, typename TPropertyType>
	void operator()( const PxDualIndexedPropertyInfo<TKey, TObjType, TIndexType, TIndex2Type, TPropertyType >& inProp, PxU32 idx ) 
	{
		dualIndexedProperty( idx, inProp
								, IndexerToNameMap<TKey,TIndexType>().Converter.NameConversion
								, IndexerToNameMap<TKey,TIndex2Type>().Converter.NameConversion );
	}
	
	template<PxU32 TKey, typename TObjType, typename TPropertyType>
	void operator()( const PxRangePropertyInfo<TKey, TObjType, TPropertyType>& inProperty, PxU32 idx )
	{
		PxU32 rangeStart = PxEnumRangeMap<TKey>().Start;
		PxU32& propIdx = mKeyOverride == NULL ? rangeStart : *mKeyOverride;
		PxU32 theOffset = 0; 
		if ( mOffsetOverride ) theOffset = *mOffsetOverride;

		mOperator.pushName( inProperty.mName );
		mOperator.pushName( inProperty.mArg0Name );
		PxPvdRangePropertyAccessor<TKey, TObjType, TPropertyType> theAccessor( inProperty, true );
		setupValueStructOffset( theAccessor, PxPropertyToValueStructMemberMap<TKey>().Offset, &theOffset );
		handleAccessor<TKey>( propIdx, theAccessor );
		++propIdx;
		theOffset += sizeof( TPropertyType );
		mOperator.popName();
		mOperator.pushName( inProperty.mArg1Name );
		theAccessor.mFirstValue = false;
		setupValueStructOffset( theAccessor, PxPropertyToValueStructMemberMap<TKey>().Offset, &theOffset );
		handleAccessor<TKey>( propIdx, theAccessor );
		mOperator.popName();
		mOperator.popName();
	}

	template<PxU32 TKey, typename TObjectType, typename TPropertyType, PxU32 TEnableFlag>
	void handleBuffer( const PxBufferPropertyInfo<TKey, TObjectType, TPropertyType, TEnableFlag>& inProp )
	{
		mOperator.handleBuffer( inProp );
	}
	
	template<PxU32 TKey, typename TObjectType, typename TEnumType, typename TStorageType, PxU32 TEnableFlag>
	void handleBuffer( const PxBufferPropertyInfo<TKey, TObjectType, PxStrideIterator<const PxFlags<TEnumType, TStorageType> >, TEnableFlag>& inProp )
	{
		mOperator.handleFlagsBuffer( inProp, PxEnumTraits<TEnumType>().NameConversion );
	}

	template<PxU32 TKey, typename TObjectType, typename TPropertyType, PxU32 TEnableFlag>
	void operator()( const PxBufferPropertyInfo<TKey, TObjectType, TPropertyType, TEnableFlag>& inProp, PxU32 )
	{
		handleBuffer( inProp );
	}
	
	template<PxU32 TKey, typename TObjType, typename TPropertyType>
	void operator()( const PxWriteOnlyPropertyInfo<TKey,TObjType,TPropertyType>& inProperty, PxU32 ) {}

	template<PxU32 TKey, typename TObjType, typename TCollectionType>
	void operator()( const PxReadOnlyCollectionPropertyInfo<TKey, TObjType, TCollectionType>&, PxU32 ) {}

	template<PxU32 TKey, typename TObjType, typename TCollectionType, typename TFilterType>
	void operator()( const PxReadOnlyFilteredCollectionPropertyInfo<TKey, TObjType, TCollectionType, TFilterType >&, PxU32 ) {}


	//We don't deal with these property datatypes.
#define DEFINE_PVD_PROPERTY_NOP(datatype) \
	template<PxU32 TKey, typename TObjType> \
	void operator()( const PxReadOnlyPropertyInfo<TKey,TObjType,datatype>& inProperty, PxU32 ){}
	
	DEFINE_PVD_PROPERTY_NOP( const void* );
	DEFINE_PVD_PROPERTY_NOP( void* );
	DEFINE_PVD_PROPERTY_NOP( PxSimulationFilterCallback * );
	DEFINE_PVD_PROPERTY_NOP( physx::pxtask::TaskManager * );
	DEFINE_PVD_PROPERTY_NOP( PxSimulationFilterShader );
	DEFINE_PVD_PROPERTY_NOP( PxContactModifyCallback * );
	DEFINE_PVD_PROPERTY_NOP( PxSimulationEventCallback * );
	DEFINE_PVD_PROPERTY_NOP( physx::pxtask::GpuDispatcher* );
	DEFINE_PVD_PROPERTY_NOP( physx::pxtask::CpuDispatcher * );
	DEFINE_PVD_PROPERTY_NOP( PxRigidActor );
	DEFINE_PVD_PROPERTY_NOP( const PxRigidActor );
	DEFINE_PVD_PROPERTY_NOP( PxRigidActor& );
	DEFINE_PVD_PROPERTY_NOP( const PxRigidActor& );
	DEFINE_PVD_PROPERTY_NOP( PxScene* );
	DEFINE_PVD_PROPERTY_NOP( PxAggregate * );
	DEFINE_PVD_PROPERTY_NOP( PxArticulation& );
	DEFINE_PVD_PROPERTY_NOP( const PxArticulationLink * );
	DEFINE_PVD_PROPERTY_NOP( const PxDeformable * );
	DEFINE_PVD_PROPERTY_NOP( const PxParticleFluid * );
	DEFINE_PVD_PROPERTY_NOP( const PxParticleSystem * );
	DEFINE_PVD_PROPERTY_NOP( const PxRigidDynamic * );
	DEFINE_PVD_PROPERTY_NOP( const PxRigidStatic * );
	DEFINE_PVD_PROPERTY_NOP( const PxParticleBase * );
	DEFINE_PVD_PROPERTY_NOP( PxArticulationJoint * );
	DEFINE_PVD_PROPERTY_NOP( PxDeformableMesh * );
};

template<typename TOperator>
inline PvdPropertyFilter<TOperator> makePvdPropertyFilter( TOperator inOperator )
{
	return PvdPropertyFilter<TOperator>( inOperator );
}

template<typename TOperator>
inline PvdPropertyFilter<TOperator> makePvdPropertyFilter( TOperator inOperator, PxU32* inKey, PxU32* inOffset )
{
	return PvdPropertyFilter<TOperator>( inOperator, inKey, inOffset );
}

template<typename TOperator, typename TFuncType>
inline void visitWithPvdFilter( TOperator inOperator, TFuncType inFuncType )
{
	TFuncType( makePvdPropertyFilter( inOperator ) );
}

template<typename TObjType, typename TOperator> 
inline void visitInstancePvdProperties( TOperator inOperator )
{
	visitInstanceProperties<TObjType>( makePvdPropertyFilter( inOperator ) );
}

template<typename TOperator>
struct PvdAllPropertyVisitor
{
	TOperator mOperator;
	PvdAllPropertyVisitor( TOperator op ) : mOperator( op ) {}
	template<typename TObjectType>
	bool operator()( const TObjectType* ) { visitInstancePvdProperties<TObjectType>( mOperator ); return false; }
};


template<typename TOperator>
struct PvdAllInfoVisitor
{
	TOperator mOperator;
	PvdAllInfoVisitor( TOperator op ) : mOperator( op ) {}
	template<typename TInfoType>
	void operator()( TInfoType inInfo ) 
	{ 
		inInfo.template visitType<bool>( PvdAllPropertyVisitor<TOperator>( mOperator ) );
		inInfo.visitBases( *this );
	}
};


template<typename TObjType, typename TOperator> 
inline void visitAllPvdProperties( TOperator inOperator )
{
	visitAllProperties<TObjType>( makePvdPropertyFilter( inOperator ) );
}



template<typename TOperator>
inline void visitRigidDynamicPerFrameProperties( TOperator inOperator )
{
	PvdPropertyFilter<TOperator> theFilter( inOperator );
	PxRigidDynamicGeneratedInfo theInfo;
	theFilter( theInfo.GlobalPose, 0 );
	theFilter( theInfo.LinearVelocity, 1 );
	theFilter( theInfo.AngularVelocity, 2 );
	theFilter( theInfo.IsSleeping, 3 );
}

template<typename TOperator>
inline void visitArticulationLinkPerFrameProperties( TOperator inOperator )
{
	PvdPropertyFilter<TOperator> theFilter( inOperator );
	PxArticulationLinkGeneratedInfo theInfo;
	theFilter( theInfo.GlobalPose, 0 );
	theFilter( theInfo.LinearVelocity, 1 );
	theFilter( theInfo.AngularVelocity, 2 );
}

}

#endif
#endif