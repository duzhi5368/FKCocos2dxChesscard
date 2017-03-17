#include "FKVisiableRect.h"
//------------------------------------------------------------
CCRect CFKVisiableRect::s_VisibleRect;
CCSize CFKVisiableRect::s_Framesize;
//------------------------------------------------------------
void CFKVisiableRect::LazyInit()
{
	if (s_VisibleRect.size.width == 0.0f && s_VisibleRect.size.height == 0.0f)
	{
		CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
		s_VisibleRect.origin = pEGLView->getVisibleOrigin();
		s_VisibleRect.size = pEGLView->getVisibleSize();
		s_Framesize = pEGLView->getFrameSize();
	}
}
//------------------------------------------------------------
CCRect CFKVisiableRect::GetVisibleRect()
{
	LazyInit();
	return CCRectMake(s_VisibleRect.origin.x, s_VisibleRect.origin.y, 
		s_VisibleRect.size.width, s_VisibleRect.size.height);
}
//------------------------------------------------------------
CCSize CFKVisiableRect::GetVisibleSize()
{
	LazyInit();
	return s_VisibleRect.size;
}
//------------------------------------------------------------
CCSize CFKVisiableRect::GetFrameSize()
{
	LazyInit();
	return s_Framesize;
}
//------------------------------------------------------------
CCPoint CFKVisiableRect::Left()
{
	LazyInit();
	return ccp(s_VisibleRect.origin.x, 
		s_VisibleRect.origin.y+s_VisibleRect.size.height/2);
}
//------------------------------------------------------------
CCPoint CFKVisiableRect::Right()
{
	LazyInit();
	return ccp(s_VisibleRect.origin.x+s_VisibleRect.size.width, 
		s_VisibleRect.origin.y+s_VisibleRect.size.height/2);
}
//------------------------------------------------------------
CCPoint CFKVisiableRect::Top()
{
	LazyInit();
	return ccp(s_VisibleRect.origin.x+s_VisibleRect.size.width/2, 
		s_VisibleRect.origin.y+s_VisibleRect.size.height);
}
//------------------------------------------------------------
CCPoint CFKVisiableRect::Bottom()
{
	LazyInit();
	return ccp(s_VisibleRect.origin.x+s_VisibleRect.size.width/2, 
		s_VisibleRect.origin.y);
}
//------------------------------------------------------------
CCPoint CFKVisiableRect::Center()
{
	LazyInit();
	return ccp(s_VisibleRect.origin.x+s_VisibleRect.size.width/2, 
		s_VisibleRect.origin.y+s_VisibleRect.size.height/2);
}
//------------------------------------------------------------
CCPoint CFKVisiableRect::LeftTop()
{
	LazyInit();
	return ccp(s_VisibleRect.origin.x, 
		s_VisibleRect.origin.y+s_VisibleRect.size.height);
}
//------------------------------------------------------------
CCPoint CFKVisiableRect::RightTop()
{
	LazyInit();
	return ccp(s_VisibleRect.origin.x+s_VisibleRect.size.width, 
		s_VisibleRect.origin.y+s_VisibleRect.size.height);
}
//------------------------------------------------------------
CCPoint CFKVisiableRect::LeftBottom()
{
	LazyInit();
	return s_VisibleRect.origin;
}
//------------------------------------------------------------
CCPoint CFKVisiableRect::RightBottom()
{
	LazyInit();
	return ccp(s_VisibleRect.origin.x+s_VisibleRect.size.width, 
		s_VisibleRect.origin.y);
}
//------------------------------------------------------------