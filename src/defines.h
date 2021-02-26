
#define mBitsSet(f,m)	((f)|=(m))
#define mBitsClr(f,m)	((f)&=(~(m)))
#define mBitsTgl(f,m)	((f)^=(m))
#define mBitsMsk(f,m)	((f)&(m))
#define mIsBitSet(f,m)	(((f)&(m))==(m))
#define mIsBitClr(f,m)	(((~f)&(m)==(m))

#define WINDOW_WIDTH    ( (2*PADDING_HRZ) + (SCENE_CELL_SPACING * (SCENE_NB_COL-1) ) + (SCENE_CELL_SIZE * SCENE_NB_COL) )
#define WINDOW_HEIGHT   ( PADDING_TOP + PADDING_BOTTOM + (SCENE_CELL_SPACING * (SCENE_NB_ROW-1) ) + (SCENE_CELL_SIZE * SCENE_NB_ROW) )

#define PLAYFIELD_WIDTH     ( (WINDOW_WIDTH)-(2*PADDING_HRZ) )
#define PLAYFIELD_HEIGHT    ( (WINDOW_HEIGHT)-(PADDING_TOP)-(PADDING_BOTTOM) )

#define SCENE_NB_ROW        (8)
#define SCENE_NB_COL        (8)
#define SCENE_NB_PERCENT    (5)
#define SCENE_NB_CELLS      (SCENE_NB_ROW*SCENE_NB_COL)

#define CELL_FONT_PADDING   (4)

#define CELL_MINE           (9)
#define CELL_VOID           (0)

#define PADDING_TOP             (48)
#define PADDING_BOTTOM          (32)
#define PADDING_HRZ             (16)

#define SCENE_CELL_SPACING      (4)
#define SCENE_CELL_SIZE         (64)
#define SCENE_CELL_BLOCK        (SCENE_CELL_SIZE+SCENE_CELL_SPACING)

#define GAME_OVER_VALUE         (-5)

#define cellValueMask       (0x000000FF)
#define mCellValue(c)       mBitsMsk(c, cellValueMask)

#define playedMask          (0x80000000)
#define mIsPlayed(c)        mIsBitSet(c,playedMask)
#define mSetPlayed(c)       mBitsSet(c, playedMask)

#define flaggedMask         (0x08000000)
#define mIsFlagged(c)       mIsBitSet(c, flaggedMask)
#define mTglFlagged(c)      mBitsTgl(c,flaggedMask)
#define mSetFlagged(c)      mBitsSet(c,flaggedMask)

#define questionMask        (0x00008000)
#define mIsQuestion(c)      mIsBitSet(c, questionMask)
#define mTglQuestion(c)     mBitsTgl(c, questionMask)

#define mClrFlagged(c)      mBitsClr(c, flaggedMask)
#define mClrQuestion(c)     mBitsClr(c, questionMask)

#define TESTING_WINDOW_WIDTH	(3*RECT_SPACING+2*RECT_WIDTH)
#define TESTING_WINDOW_HEIGHT	(3*RECT_SPACING+2*RECT_HEIGHT)

#define mRGBA(r,g,b,a)          ( ((uint32_t)r<<24)|\
                                  ((uint32_t)g<<16)|\
                                  ((uint32_t)b<< 8)|\
                                  ((uint32_t)a<< 0)  )

#define mToColor(c, RGBA)       ( c.r=(RGBA>>24)&0xff,\
                                  c.g=(RGBA>>16)&0xff,\
                                  c.b=(RGBA>>8)&0xff, \
                                  c.a=(RGBA>>0)&0xff )

#define SCENE_COLOR_BACKGROUND  mRGBA(56, 60, 74, 255)
#define SCENE_COLOR_MINE        mRGBA(82, 148, 255, 255)
#define SCENE_COLOR_VOID        mRGBA(64, 69, 82, 255)
#define SCENE_COLOR_HIDDEN      mRGBA(75, 81, 98, 255)