#ifndef _COMMON_SERVICES_H_
#define _COMMON_SERVICES_H_

#ifndef TARGET_RT_LITTLE_ENDIAN
#define TARGET_RT_LITTLE_ENDIAN 0
#endif
#ifndef TARGET_RT_BIG_ENDIAN
#define TARGET_RT_BIG_ENDIAN 1
#endif
//===========================================================================================================================
//	Alignment safe read/write/swap macros
//===========================================================================================================================

// Big endian reading

#define	ReadBig16( PTR ) \
	( (uint16_t)( \
		( ( (uint16_t)( (uint8_t *)(PTR) )[ 0 ] ) << 8 ) | \
		  ( (uint16_t)( (uint8_t *)(PTR) )[ 1 ] ) ) )

#define	ReadBig24( PTR ) \
	( (uint32_t)( \
		( ( (uint32_t)( (uint8_t *)(PTR) )[ 0 ] ) << 16 ) | \
		( ( (uint32_t)( (uint8_t *)(PTR) )[ 1 ] ) <<  8 ) | \
		  ( (uint32_t)( (uint8_t *)(PTR) )[ 2 ] ) ) )

#define	ReadBig32( PTR ) \
	( (uint32_t)( \
		( ( (uint32_t)( (uint8_t *)(PTR) )[ 0 ] ) << 24 ) | \
		( ( (uint32_t)( (uint8_t *)(PTR) )[ 1 ] ) << 16 ) | \
		( ( (uint32_t)( (uint8_t *)(PTR) )[ 2 ] ) <<  8 ) | \
		  ( (uint32_t)( (uint8_t *)(PTR) )[ 3 ] ) ) )

#define	ReadBig48( PTR ) \
	( (uint64_t)( \
		( ( (uint64_t)( (uint8_t *)(PTR) )[ 0 ] ) << 40 ) | \
		( ( (uint64_t)( (uint8_t *)(PTR) )[ 1 ] ) << 32 ) | \
		( ( (uint64_t)( (uint8_t *)(PTR) )[ 2 ] ) << 24 ) | \
		( ( (uint64_t)( (uint8_t *)(PTR) )[ 3 ] ) << 16 ) | \
		( ( (uint64_t)( (uint8_t *)(PTR) )[ 4 ] ) <<  8 ) | \
		  ( (uint64_t)( (uint8_t *)(PTR) )[ 5 ] ) ) )

#define	ReadBig64( PTR ) \
	( (uint64_t)( \
		( ( (uint64_t)( (uint8_t *)(PTR) )[ 0 ] ) << 56 ) | \
		( ( (uint64_t)( (uint8_t *)(PTR) )[ 1 ] ) << 48 ) | \
		( ( (uint64_t)( (uint8_t *)(PTR) )[ 2 ] ) << 40 ) | \
		( ( (uint64_t)( (uint8_t *)(PTR) )[ 3 ] ) << 32 ) | \
		( ( (uint64_t)( (uint8_t *)(PTR) )[ 4 ] ) << 24 ) | \
		( ( (uint64_t)( (uint8_t *)(PTR) )[ 5 ] ) << 16 ) | \
		( ( (uint64_t)( (uint8_t *)(PTR) )[ 6 ] ) <<  8 ) | \
		  ( (uint64_t)( (uint8_t *)(PTR) )[ 7 ] ) ) )

// Big endian writing

#define	WriteBig16( PTR, X ) \
	do \
	{ \
		( (uint8_t *)(PTR) )[ 0 ] = (uint8_t)( ( (X) >>  8 ) & 0xFF ); \
		( (uint8_t *)(PTR) )[ 1 ] = (uint8_t)(   (X)         & 0xFF ); \
	\
	}	while( 0 )

#define	WriteBig24( PTR, X ) \
	do \
	{ \
		( (uint8_t *)(PTR) )[ 0 ] = (uint8_t)( ( (X) >> 16 ) & 0xFF ); \
		( (uint8_t *)(PTR) )[ 1 ] = (uint8_t)( ( (X) >>  8 ) & 0xFF ); \
		( (uint8_t *)(PTR) )[ 2 ] = (uint8_t)(   (X)         & 0xFF ); \
	\
	}	while( 0 )

#define	WriteBig32( PTR, X ) \
	do \
	{ \
		( (uint8_t *)(PTR) )[ 0 ] = (uint8_t)( ( (X) >> 24 ) & 0xFF ); \
		( (uint8_t *)(PTR) )[ 1 ] = (uint8_t)( ( (X) >> 16 ) & 0xFF ); \
		( (uint8_t *)(PTR) )[ 2 ] = (uint8_t)( ( (X) >>  8 ) & 0xFF ); \
		( (uint8_t *)(PTR) )[ 3 ] = (uint8_t)(   (X)         & 0xFF ); \
	\
	}	while( 0 )

#define	WriteBig48( PTR, X ) \
	do \
	{ \
		( (uint8_t *)(PTR) )[ 0 ] = (uint8_t)( ( (X) >> 40 ) & 0xFF ); \
		( (uint8_t *)(PTR) )[ 1 ] = (uint8_t)( ( (X) >> 32 ) & 0xFF ); \
		( (uint8_t *)(PTR) )[ 2 ] = (uint8_t)( ( (X) >> 24 ) & 0xFF ); \
		( (uint8_t *)(PTR) )[ 3 ] = (uint8_t)( ( (X) >> 16 ) & 0xFF ); \
		( (uint8_t *)(PTR) )[ 4 ] = (uint8_t)( ( (X) >>  8 ) & 0xFF ); \
		( (uint8_t *)(PTR) )[ 5 ] = (uint8_t)(   (X)         & 0xFF ); \
	\
	}	while( 0 )

#define	WriteBig64( PTR, X ) \
	do \
	{ \
		( (uint8_t *)(PTR) )[ 0 ] = (uint8_t)( ( (X) >> 56 ) & 0xFF ); \
		( (uint8_t *)(PTR) )[ 1 ] = (uint8_t)( ( (X) >> 48 ) & 0xFF ); \
		( (uint8_t *)(PTR) )[ 2 ] = (uint8_t)( ( (X) >> 40 ) & 0xFF ); \
		( (uint8_t *)(PTR) )[ 3 ] = (uint8_t)( ( (X) >> 32 ) & 0xFF ); \
		( (uint8_t *)(PTR) )[ 4 ] = (uint8_t)( ( (X) >> 24 ) & 0xFF ); \
		( (uint8_t *)(PTR) )[ 5 ] = (uint8_t)( ( (X) >> 16 ) & 0xFF ); \
		( (uint8_t *)(PTR) )[ 6 ] = (uint8_t)( ( (X) >>  8 ) & 0xFF ); \
		( (uint8_t *)(PTR) )[ 7 ] = (uint8_t)(   (X)         & 0xFF ); \
	\
	}	while( 0 )

// Little endian reading

#if( TARGET_RT_LITTLE_ENDIAN )
	#define	ReadLittle16( PTR )		( *( (uint16_t *)(PTR) ) )
#else
	#define	ReadLittle16( PTR ) \
		( (uint16_t)( \
			  ( (uint16_t)( (uint8_t *)(PTR) )[ 0 ] ) | \
			( ( (uint16_t)( (uint8_t *)(PTR) )[ 1 ] ) <<  8 ) ) )
#endif

#define	ReadLittle24( PTR ) \
	( (uint32_t)( \
		  ( (uint32_t)( (uint8_t *)(PTR) )[ 0 ] ) | \
		( ( (uint32_t)( (uint8_t *)(PTR) )[ 1 ] ) <<  8 ) | \
		( ( (uint32_t)( (uint8_t *)(PTR) )[ 2 ] ) << 16 ) ) )

#if( TARGET_RT_LITTLE_ENDIAN )
	#define	ReadLittle32( PTR )		( *( (uint32_t *)(PTR) ) )
#else
	#define	ReadLittle32( PTR ) \
		( (uint32_t)( \
			  ( (uint32_t)( (uint8_t *)(PTR) )[ 0 ] ) | \
			( ( (uint32_t)( (uint8_t *)(PTR) )[ 1 ] ) <<  8 ) | \
			( ( (uint32_t)( (uint8_t *)(PTR) )[ 2 ] ) << 16 ) | \
			( ( (uint32_t)( (uint8_t *)(PTR) )[ 3 ] ) << 24 ) ) )
#endif

#define	ReadLittle48( PTR ) \
	( (uint64_t)( \
		  ( (uint64_t)( (uint8_t *)(PTR) )[ 0 ] )			| \
		( ( (uint64_t)( (uint8_t *)(PTR) )[ 1 ] ) <<  8 )	| \
		( ( (uint64_t)( (uint8_t *)(PTR) )[ 2 ] ) << 16 )	| \
		( ( (uint64_t)( (uint8_t *)(PTR) )[ 3 ] ) << 24 )	| \
		( ( (uint64_t)( (uint8_t *)(PTR) )[ 4 ] ) << 32 )	| \
		( ( (uint64_t)( (uint8_t *)(PTR) )[ 5 ] ) << 40 ) ) )

#if( TARGET_RT_LITTLE_ENDIAN )
	#define	ReadLittle64( PTR )		( *( (uint64_t *)(PTR) ) )
#else
	#define	ReadLittle64( PTR ) \
		( (uint64_t)( \
			  ( (uint64_t)( (uint8_t *)(PTR) )[ 0 ] )			| \
			( ( (uint64_t)( (uint8_t *)(PTR) )[ 1 ] ) <<  8 )	| \
			( ( (uint64_t)( (uint8_t *)(PTR) )[ 2 ] ) << 16 )	| \
			( ( (uint64_t)( (uint8_t *)(PTR) )[ 3 ] ) << 24 )	| \
			( ( (uint64_t)( (uint8_t *)(PTR) )[ 4 ] ) << 32 )	| \
			( ( (uint64_t)( (uint8_t *)(PTR) )[ 5 ] ) << 40 )	| \
			( ( (uint64_t)( (uint8_t *)(PTR) )[ 6 ] ) << 48 )	| \
			( ( (uint64_t)( (uint8_t *)(PTR) )[ 7 ] ) << 56 ) ) )
#endif

// Little endian writing

#if( TARGET_RT_LITTLE_ENDIAN )
	#define	WriteLittle16( PTR, X )		do { *( (uint16_t *)(PTR) ) = (uint16_t)(X); } while( 0 )
#else
	#define	WriteLittle16( PTR, X ) \
		do \
		{ \
			( (uint8_t *)(PTR) )[ 0 ] = (uint8_t)(   (X)         & 0xFF ); \
			( (uint8_t *)(PTR) )[ 1 ] = (uint8_t)( ( (X) >>  8 ) & 0xFF ); \
		\
		}	while( 0 )
#endif

#define	WriteLittle24( PTR, X ) \
	do \
	{ \
		( (uint8_t *)(PTR) )[ 0 ] = (uint8_t)(   (X)         & 0xFF ); \
		( (uint8_t *)(PTR) )[ 1 ] = (uint8_t)( ( (X) >>  8 ) & 0xFF ); \
		( (uint8_t *)(PTR) )[ 2 ] = (uint8_t)( ( (X) >> 16 ) & 0xFF ); \
	\
	}	while( 0 )

#if( TARGET_RT_LITTLE_ENDIAN  )
	#define	WriteLittle32( PTR, X )		do { *( (uint32_t *)(PTR) ) = (uint32_t)(X); } while( 0 )
#else
	#define	WriteLittle32( PTR, X ) \
		do \
		{ \
			( (uint8_t *)(PTR) )[ 0 ] = (uint8_t)(   (X)         & 0xFF ); \
			( (uint8_t *)(PTR) )[ 1 ] = (uint8_t)( ( (X) >>  8 ) & 0xFF ); \
			( (uint8_t *)(PTR) )[ 2 ] = (uint8_t)( ( (X) >> 16 ) & 0xFF ); \
			( (uint8_t *)(PTR) )[ 3 ] = (uint8_t)( ( (X) >> 24 ) & 0xFF ); \
		\
		}	while( 0 )
#endif

#define	WriteLittle48( PTR, X ) \
	do \
	{ \
		( (uint8_t *)(PTR) )[ 0 ] = (uint8_t)(   (X)         & 0xFF ); \
		( (uint8_t *)(PTR) )[ 1 ] = (uint8_t)( ( (X) >>  8 ) & 0xFF ); \
		( (uint8_t *)(PTR) )[ 2 ] = (uint8_t)( ( (X) >> 16 ) & 0xFF ); \
		( (uint8_t *)(PTR) )[ 3 ] = (uint8_t)( ( (X) >> 24 ) & 0xFF ); \
		( (uint8_t *)(PTR) )[ 4 ] = (uint8_t)( ( (X) >> 32 ) & 0xFF ); \
		( (uint8_t *)(PTR) )[ 5 ] = (uint8_t)( ( (X) >> 40 ) & 0xFF ); \
	\
	}	while( 0 )

#if( TARGET_RT_LITTLE_ENDIAN )
	#define	WriteLittle64( PTR, X )		do { *( (uint64_t *)(PTR) ) = (uint64_t)(X); } while( 0 )
#else
	#define	WriteLittle64( PTR, X ) \
		do \
		{ \
			( (uint8_t *)(PTR) )[ 0 ] = (uint8_t)(   (X)         & 0xFF ); \
			( (uint8_t *)(PTR) )[ 1 ] = (uint8_t)( ( (X) >>  8 ) & 0xFF ); \
			( (uint8_t *)(PTR) )[ 2 ] = (uint8_t)( ( (X) >> 16 ) & 0xFF ); \
			( (uint8_t *)(PTR) )[ 3 ] = (uint8_t)( ( (X) >> 24 ) & 0xFF ); \
			( (uint8_t *)(PTR) )[ 4 ] = (uint8_t)( ( (X) >> 32 ) & 0xFF ); \
			( (uint8_t *)(PTR) )[ 5 ] = (uint8_t)( ( (X) >> 40 ) & 0xFF ); \
			( (uint8_t *)(PTR) )[ 6 ] = (uint8_t)( ( (X) >> 48 ) & 0xFF ); \
			( (uint8_t *)(PTR) )[ 7 ] = (uint8_t)( ( (X) >> 56 ) & 0xFF ); \
		\
		}	while( 0 )
#endif

//// Floating point
//
//STATIC_INLINE Float32	ReadBigFloat32( const void *p )				{ Value32 v32; v32.u32 = ReadBig32( p ); return( v32.f32 ); }
//STATIC_INLINE Float64	ReadBigFloat64( const void *p )				{ Value64 v64; v64.u64 = ReadBig64( p ); return( v64.f64 ); }
//STATIC_INLINE Float32	ReadLittleFloat32( const void *p )			{ Value32 v32; v32.u32 = ReadLittle32( p ); return( v32.f32 ); }
//STATIC_INLINE Float64	ReadLittleFloat64( const void *p )			{ Value64 v64; v64.u64 = ReadLittle64( p ); return( v64.f64 ); }
//
//STATIC_INLINE void		WriteBigFloat32( void *p, Float32 x )		{ Value32 v32; v32.f32 = x; WriteBig32( p, v32.u32 ); }
//STATIC_INLINE void		WriteBigFloat64( void *p, Float64 x )		{ Value64 v64; v64.f64 = x; WriteBig64( p, v64.u64 ); }
//STATIC_INLINE void		WriteLittleFloat32( void *p, Float32 x )	{ Value32 v32; v32.f32 = x; WriteLittle32( p, v32.u32 ); }
//STATIC_INLINE void		WriteLittleFloat64( void *p, Float64 x )	{ Value64 v64; v64.f64 = x; WriteLittle64( p, v64.u64 ); }

// Host order read/write

#define Read8( PTR )					( ( (const uint8_t *)(PTR) )[ 0 ] )
#define Write8( PTR, X )				( ( (uint8_t *)(PTR) )[ 0 ] = (X) )

#if( TARGET_RT_BIG_ENDIAN )
	#define ReadHost16( PTR )			ReadBig16( (PTR) )
	#define ReadHost24( PTR )			ReadBig24( (PTR) )
	#define ReadHost32( PTR )			ReadBig32( (PTR) )
	#define ReadHost48( PTR )			ReadBig48( (PTR) )
	#define ReadHost64( PTR )			ReadBig64( (PTR) )

	#define WriteHost16( PTR, X )		WriteBig16( (PTR), (X) )
	#define WriteHost24( PTR, X )		WriteBig24( (PTR), (X) )
	#define WriteHost32( PTR, X )		WriteBig32( (PTR), (X) )
	#define WriteHost48( PTR, X )		WriteBig48( (PTR), (X) )
	#define WriteHost64( PTR, X )		WriteBig64( (PTR), (X) )

	#define ReadHostFloat32( PTR )		ReadBigFloat32( (PTR) )
	#define ReadHostFloat64( PTR )		ReadBigFloat64( (PTR) )

	#define WriteHostFloat32( PTR, X )	WriteBigFloat32( (PTR), (X) )
	#define WriteHostFloat64( PTR, X )	WriteBigFloat64( (PTR), (X) )
#else
	#define ReadHost16( PTR )			ReadLittle16( (PTR) )
	#define ReadHost24( PTR )			ReadLittle24( (PTR) )
	#define ReadHost32( PTR )			ReadLittle32( (PTR) )
	#define ReadHost48( PTR )			ReadLittle48( (PTR) )
	#define ReadHost64( PTR )			ReadLittle64( (PTR) )

	#define WriteHost16( PTR, X )		WriteLittle16( (PTR), (X) )
	#define WriteHost24( PTR, X )		WriteLittle24( (PTR), (X) )
	#define WriteHost32( PTR, X )		WriteLittle32( (PTR), (X) )
	#define WriteHost48( PTR, X )		WriteLittle48( (PTR), (X) )
	#define WriteHost64( PTR, X )		WriteLittle64( (PTR), (X) )

	#define ReadHostFloat32( PTR )		ReadLittleFloat32( (PTR) )
	#define ReadHostFloat64( PTR )		ReadLittleFloat64( (PTR) )

	#define WriteHostFloat32( PTR, X )	WriteLittleFloat32( (PTR), (X) )
	#define WriteHostFloat64( PTR, X )	WriteLittleFloat64( (PTR), (X) )
#endif

// Unconditional swap read/write.

#if( TARGET_RT_BIG_ENDIAN )
	#define ReadSwap16( PTR )			ReadLittle16( (PTR) )
	#define ReadSwap24( PTR )			ReadLittle24( (PTR) )
	#define ReadSwap32( PTR )			ReadLittle32( (PTR) )
	#define ReadSwap48( PTR )			ReadLittle48( (PTR) )
	#define ReadSwap64( PTR )			ReadLittle64( (PTR) )

	#define WriteSwap16( PTR, X )		WriteLittle16( (PTR), (X) )
	#define WriteSwap24( PTR, X )		WriteLittle24( (PTR), (X) )
	#define WriteSwap32( PTR, X )		WriteLittle32( (PTR), (X) )
	#define WriteSwap48( PTR, X )		WriteLittle48( (PTR), (X) )
	#define WriteSwap64( PTR, X )		WriteLittle64( (PTR), (X) )

	#define ReadSwapFloat32( PTR )		ReadLittleFloat32( (PTR) )
	#define ReadSwapFloat64( PTR )		ReadLittleFloat64( (PTR) )

	#define WriteSwapFloat32( PTR, X )	WriteLittleFloat32( (PTR), (X) )
	#define WriteSwapFloat64( PTR, X )	WriteLittleFloat64( (PTR), (X) )
#else
	#define ReadSwap16( PTR )			ReadBig16( (PTR) )
	#define ReadSwap24( PTR )			ReadBig24( (PTR) )
	#define ReadSwap32( PTR )			ReadBig32( (PTR) )
	#define ReadSwap48( PTR )			ReadBig48( (PTR) )
	#define ReadSwap64( PTR )			ReadBig64( (PTR) )

	#define WriteSwap16( PTR, X )		WriteBig16( (PTR), (X) )
	#define WriteSwap24( PTR, X )		WriteBig24( (PTR), (X) )
	#define WriteSwap32( PTR, X )		WriteBig32( (PTR), (X) )
	#define WriteSwap48( PTR, X )		WriteBig48( (PTR), (X) )
	#define WriteSwap64( PTR, X )		WriteBig64( (PTR), (X) )

	#define ReadSwapFloat32( PTR )		ReadBigFloat32( (PTR) )
	#define ReadSwapFloat64( PTR )		ReadBigFloat64( (PTR) )

	#define WriteSwapFloat32( PTR, X )	WriteBigFloat32( (PTR), (X) )
	#define WriteSwapFloat64( PTR, X )	WriteBigFloat64( (PTR), (X) )
#endif

// Conditional swaps

#if( TARGET_RT_BIG_ENDIAN )
	#define HostToLittle16( X )						Swap16( (X) )
	#define LittleToHost16( X )						Swap16( (X) )
	#define HostToLittle32( X )						Swap32( (X) )
	#define LittleToHost32( X )						Swap32( (X) )
	#define HostToLittle64( X )						Swap64( (X) )
	#define LittleToHost64( X )						Swap64( (X) )

	#define HostToBig16( X )						(X)
	#define BigToHost16( X )						(X)
	#define HostToBig32( X )						(X)
	#define BigToHost32( X )						(X)
	#define HostToBig64( X )						(X)
	#define BigToHost64( X )						(X)

	#define HostToBig16Mem( SRC, LEN, DST )			memmove( (DST), (SRC), (LEN) )
	#define BigToHost16Mem( SRC, LEN, DST )			memmove( (DST), (SRC), (LEN) )

	#define HostToLittle16Mem( SRC, LEN, DST )		Swap16Mem( (SRC), (LEN), (DST) )
	#define LittleToHost16Mem( SRC, LEN, DST )		Swap16Mem( (SRC), (LEN), (DST) )

	#define HostToBigMem( SRC, LEN, DST )			memmove( (DST), (SRC), (LEN) )
	#define BigToHostMem( SRC, LEN, DST )			memmove( (DST), (SRC), (LEN) )

	#define HostToLittleMem( SRC, LEN, DST )		MemReverse( (SRC), (LEN), (DST) )
	#define LittleToHostMem( SRC, LEN, DST )		MemReverse( (SRC), (LEN), (DST) )
#else
	#define HostToLittle16( X )						(X)
	#define LittleToHost16( X )						(X)
	#define HostToLittle32( X )						(X)
	#define LittleToHost32( X )						(X)
	#define HostToLittle64( X )						(X)
	#define LittleToHost64( X )						(X)

	#define HostToBig16( X )						Swap16( (X) )
	#define BigToHost16( X )						Swap16( (X) )
	#define HostToBig32( X )						Swap32( (X) )
	#define BigToHost32( X )						Swap32( (X) )
	#define HostToBig64( X )						Swap64( (X) )
	#define BigToHost64( X )						Swap64( (X) )

	#define HostToBig16Mem( SRC, LEN, DST )			Swap16Mem( (SRC), (LEN), (DST) )
	#define BigToHost16Mem( SRC, LEN, DST )			Swap16Mem( (SRC), (LEN), (DST) )

	#define HostToLittle16Mem( SRC, LEN, DST )		memmove( (DST), (SRC), (LEN) )
	#define LittleToHost16Mem( SRC, LEN, DST )		memmove( (DST), (SRC), (LEN) )

	#define HostToBigMem( SRC, LEN, DST )			MemReverse( (SRC), (LEN), (DST) )
	#define BigToHostMem( SRC, LEN, DST )			MemReverse( (SRC), (LEN), (DST) )

	#define HostToLittleMem( SRC, LEN, DST )		memmove( (DST), (SRC), (LEN) )
	#define LittleToHostMem( SRC, LEN, DST )		memmove( (DST), (SRC), (LEN) )
#endif

// Unconditional endian swaps

#define	Swap16( X ) \
	( (uint16_t)( \
		( ( ( (uint16_t)(X) ) << 8 ) & UINT16_C( 0xFF00 ) ) | \
		( ( ( (uint16_t)(X) ) >> 8 ) & UINT16_C( 0x00FF ) ) ) )

#define	Swap32( X ) \
	( (uint32_t)( \
		( ( ( (uint32_t)(X) ) << 24 ) & UINT32_C( 0xFF000000 ) ) | \
		( ( ( (uint32_t)(X) ) <<  8 ) & UINT32_C( 0x00FF0000 ) ) | \
		( ( ( (uint32_t)(X) ) >>  8 ) & UINT32_C( 0x0000FF00 ) ) | \
		( ( ( (uint32_t)(X) ) >> 24 ) & UINT32_C( 0x000000FF ) ) ) )

#define Swap64( X ) \
	( (uint64_t)( \
		( ( ( (uint64_t)(X) ) << 56 ) & UINT64_C( 0xFF00000000000000 ) ) | \
		( ( ( (uint64_t)(X) ) << 40 ) & UINT64_C( 0x00FF000000000000 ) ) | \
		( ( ( (uint64_t)(X) ) << 24 ) & UINT64_C( 0x0000FF0000000000 ) ) | \
		( ( ( (uint64_t)(X) ) <<  8 ) & UINT64_C( 0x000000FF00000000 ) ) | \
		( ( ( (uint64_t)(X) ) >>  8 ) & UINT64_C( 0x00000000FF000000 ) ) | \
		( ( ( (uint64_t)(X) ) >> 24 ) & UINT64_C( 0x0000000000FF0000 ) ) | \
		( ( ( (uint64_t)(X) ) >> 40 ) & UINT64_C( 0x000000000000FF00 ) ) | \
		( ( ( (uint64_t)(X) ) >> 56 ) & UINT64_C( 0x00000000000000FF ) ) ) )

// Host<->Network/Big endian swaps

#if( TARGET_RT_BIG_ENDIAN )
	#define hton16( X )		(X)
	#define ntoh16( X )		(X)

	#define hton32( X )		(X)
	#define ntoh32( X )		(X)

	#define hton64( X )		(X)
	#define ntoh64( X )		(X)
#else
	#define hton16( X )		Swap16( X )
	#define ntoh16( X )		Swap16( X )

	#define hton32( X )		Swap32( X )
	#define ntoh32( X )		Swap32( X )

	#define hton64( X )		Swap64( X )
	#define ntoh64( X )		Swap64( X )
#endif
#endif
