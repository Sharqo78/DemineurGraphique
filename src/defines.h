
#define mBitsSet(f,m)	((f)|=(m))
#define mBitsClr(f,m)	((f)&=(~(m)))
#define mBitsTgl(f,m)	((f)^=(m))
#define mBitsMsk(f,m)	((f)&(m))
#define mIsBitSet(f,m)	(((f)&(m))==(m))
#define mIsBitClr(f,m)	(((~f)&(m)==(m))

#define RECT_A_TOGGLE_MASK	(0x00000001)
#define RECT_B_TOGGLE_MASK	(0x00000002)
#define RECT_C_TOGGLE_MASK	(0x00000004)
#define RECT_D_TOGGLE_MASK	(0x00000008)

#define mRectATgl(f)	mBitsTgl(f, RECT_A_TOGGLE_MASK)
#define mRectBTgl(f)	mBitsTgl(f, RECT_B_TOGGLE_MASK)
#define mRectCTgl(f)	mBitsTgl(f, RECT_C_TOGGLE_MASK)
#define mRectDTgl(f)	mBitsTgl(f, RECT_D_TOGGLE_MASK)

#define mIsRectAColOn(f)	mIsBitSet(f, RECT_A_TOGGLE_MASK)
#define mIsRectBColOn(f)	mIsBitSet(f, RECT_B_TOGGLE_MASK)
#define mIsRectCColOn(f)	mIsBitSet(f, RECT_C_TOGGLE_MASK)
#define mIsRectDColOn(f)	mIsBitSet(f, RECT_D_TOGGLE_MASK)

#define RECT_WIDTH		(300)
#define	RECT_HEIGHT		(300)

#define RECT_SPACING	(30)

#define	RECT_A_X	(RECT_SPACING)
#define	RECT_A_Y	(RECT_SPACING)

#define	RECT_B_X	(RECT_A_X+RECT_WIDTH+RECT_SPACING)
#define	RECT_B_Y	(RECT_A_Y)

#define	RECT_C_X	(RECT_A_X)
#define	RECT_C_Y	(RECT_A_Y+RECT_HEIGHT+RECT_SPACING)

#define	RECT_D_X	(RECT_A_Y+RECT_WIDTH+RECT_SPACING)
#define	RECT_D_Y	(RECT_A_Y+RECT_HEIGHT+RECT_SPACING)

#define WINDOW_WIDTH	(3*RECT_SPACING+2*RECT_WIDTH)
#define WINDOW_HEIGHT	(3*RECT_SPACING+2*RECT_HEIGHT)
